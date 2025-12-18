/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "S_UART.h"
#include "LSM9DS0.h"
#include "health_cal.h"
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define PACKET_SIZE 14 // 数据包大�????
#define line0 30       // LCD显示行位置定�????
#define line1 60
#define line2 90
#define line3 120
#define line4 150
#define line5 180
#define line6 210
#define line7 240
#define line8 270
#define line9 300
#define line10 330
#define line11 360
#define line12 390
// WiFi配置参数
#define WIFI_SSID "Magic4P"          // WiFi名称
#define WIFI_PASSWORD "ttjjwwjj"     // WiFi密码
#define TCP_SERVER_IP "192.168.63.5" // TCP服务器IP地址
#define TCP_SERVER_PORT "5001"       // TCP服务器端口号
// 加�?�度计配置参�????
#define ACC_SENSITIVITY_2G 0.000061f // 加�?�度计灵敏度 (2g量程): g/�????低有效位
#define ACC_X_OFFSET (-0.40f)        // X轴加速度偏移校准�????
#define ACC_Y_OFFSET (-0.55f)        // Y轴加速度偏移校准�????
#define ACC_Z_OFFSET (-0.28f)        // Z轴加速度偏移校准�????
#define PI 3.1415926                 // 圆周率定�????
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// 读取按键GPIO电平状�??
#define KEY0_STATE ~HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) // 高电平有�????
#define KEY1_STATE HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)  // 读取KEY1状�??
#define KEY2_STATE HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)  // 读取KEY2状�??
#define KEY3_STATE HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)  // 读取KEY3状�??

// 按键按下标志位定�????
#define NO_KEY_PRES 0                   // 无按键按�????
#define KEY0_PRES 1                     // KEY0按下
#define KEY1_PRES 2                     // KEY1按下
#define KEY2_PRES 3                     // KEY2按下
#define KEY3_PRES 4                     // KEY3按下
static uint8_t KeySta[] = {1, 1, 1, 1}; // 按键状�?�数组，初始化为未按�????
void Key_Scan(void);                    // 按键扫描函数声明
uint8_t KeyPress(void);                 // 按键按下�????测函数声�????
uint8_t KeyDrive(uint8_t KeyValue);     // 按键驱动函数声明
void KeyActivity(void);                 // 按键活动处理函数声明

// 1. 定义发送缓冲区
#define SEND_QUEUE_SIZE 5
unsigned char send_queue[SEND_QUEUE_SIZE][20];
uint8_t send_head = 0, send_tail = 0;
uint8_t sending_busy = 0;

// 定时发送数据包
static uint8_t  curr_pkt[20];     // 当前最新包
static volatile uint8_t pkt_ready = 0; // 有新包标记
static uint32_t last_send_tick = 0;
#define SEND_PERIOD 4000          // 4 s

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define RX_BUFFER_SIZE 64

uint8_t UART1_RX_Buffer[RX_BUFFER_SIZE]; // UART1接收缓冲�????
uint16_t uart1_rx_index = 0;             // 接收缓冲区索�????
uint8_t uart1_reception_complete = 0;    // 接收完成标志

uint8_t num = 0;                                                      // 通用计数器变�????
uint8_t I2c_Buf_Read[100];                                            // I2C读取缓冲�????
uint8_t uch_dummy;                                                    // 临时变量，用于读取寄存器
uint32_t aun_red_buffer[10];                                          // 红光数据缓冲�????
uint32_t aun_ir_buffer[10];                                           // 红外光数据缓冲区
uint32_t signal_data = 1;                                             // 信号数据，默认为1
char string1[20], string2[20], string3[20], string4[20], string5[20]; // 显示字符串数�????

uint8_t wifi_connected = 0;                                  // WiFi连接状�?�标�????
uint8_t tcp_connected = 0;                                   // TCP连接状�?�标�????
unsigned char packet[PACKET_SIZE];                           // 数据包缓冲区
int ESP8266_Init(void);                                      // ESP8266初始化函数声�????
int ESP8266_SendCmd(char *cmd, char *ack, uint16_t timeout); // 发�?�AT命令函数声明
int ESP8266_Connect_WiFi(void);                              // 连接WiFi函数声明
int ESP8266_Connect_TCP(void);                               // 连接TCP服务器函数声�????
void ESP8266_Show_Status(void);                              // 显示状�?�函数声�????
int ESP8266_TCP_Send(unsigned char *data, int data_len);     // TCP发�?�数据函数声�????
void Display_Raw_Sensor_Data(int16_t rawX, int16_t rawY, int16_t rawZ,
                             float gX, float gY, float gZ);

pedometer_params_t params = {0}; // 计步器参数结构体，初始化�????0
axis_info_t Accel, Accel_sample; // 加�?�度数据结构体：原始数据和采样数�????
filter_avg_t mean_filter_0;      // 均�?�滤波器结构�????
peak_value_t peak_val_0;         // 动�?�峰值检测结构体
slid_reg_t slid_reg_0;           // 滑动窗口寄存器结构体
uint8_t lcd_gx = 0;              // LCD更新标志�????0-不更新，1-�????要更�????
int last_step = 0;               // 上一次的步数记录，用于检测步数变�????

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// void create_packet(uint32_t data1, uint32_t data2, unsigned char *packet);
void create_packet_float(uint32_t user_id, uint32_t step_count, float calories, unsigned char *packet);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  params.step_max = 50;
  params.start_flag = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART3_UART_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart1, &UART1_RX_Buffer[0], 1); // 启动UART1接收中断
  HAL_TIM_Base_Start_IT(&htim10);                       // 启动TIM10定时器中�????
  lcd_init();                                           // 初始化LCD显示�????

  HAL_Delay(100);
  LSM9DS0_SN74LS157_GPIO_Init(); // 初始化传感器选择GPIO
  LSM9DS0_Start();               // 启动LSM9DS0传感�????
  // 显示传感器初始化信息
  HAL_Delay(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  if (ESP8266_Init() == 0) // 初始化ESP8266模块
  {
    ESP8266_Connect_TCP(); // 连接TCP服务�????
  }

  lcd_clear(WHITE);
  while (1)
  {
    KeyActivity();

    if (params.start_flag == 1) // 如果正在运动状�??
    {

      // 读取三轴加�?�度原始数据�???16位有符号整数�???
      int16_t X = ((u16)ReadSingleFromLSM9DS0_X(OUT_X_H_A) << 8) | ReadSingleFromLSM9DS0_X(OUT_X_L_A);
      int16_t Y = ((u16)ReadSingleFromLSM9DS0_X(OUT_Y_H_A) << 8) | ReadSingleFromLSM9DS0_X(OUT_Y_L_A);
      int16_t Z = ((u16)ReadSingleFromLSM9DS0_X(OUT_Z_H_A) << 8) | ReadSingleFromLSM9DS0_X(OUT_Z_L_A);

      // 将原始数据转换为重力加�?�度�???(g)，并应用校准偏移
      Accel.x = X * ACC_SENSITIVITY_2G + ACC_X_OFFSET;
      Accel.y = Y * ACC_SENSITIVITY_2G + ACC_Y_OFFSET;
      Accel.z = Z * ACC_SENSITIVITY_2G + ACC_Z_OFFSET;
      // 在LCD上显示原始传感器数据

      char sensor_display[60];
      sprintf(sensor_display, "X:%6d Y:%6d Z:%6d", X, Y, Z);
      lcd_show_string(0, line3, 400, 210, 24, sensor_display, RED);

      filter_update(&mean_filter, &Accel);                                  // 更新均�?�滤波器FIFO缓冲�???
      Mean_filtering(&mean_filter, &Accel_sample);                          // 执行均�?�滤波处�???
      peak_update(&peak_val, &Accel_sample);                                // 更新动�?�峰值检�???
      slid_update(&slid_reg, &Accel_sample);                                // 更新滑动窗口，计算动态阈�???
      last_step = params.step_count;                                        // 保存上一次步�???
      params.step_count = detect_step(&peak_val, &slid_reg, &Accel_sample); // �???测步�???
      params.calories = 0.01 * params.step_count;                           // 计算消�?�卡路里（简化公式）
      if (params.static_state == 1 && time_r != 0)                          // 如果处于静止状�?�且有有效时�???
      {
        params.speed = 0.415f * 1.75 / ((float)time_r / 1000); // 计算速度（m/s�???
      }
      else params.speed = 0; // 否则速度设为0

      if (params.step_count >= params.step_max) params.start_flag = 2; // 设置运动完成标志


      // TCP传输打包入队
      if (tcp_connected == 1 && !sending_busy)
      {
    	  create_packet_float(5, params.step_count, params.calories, curr_pkt);
    	  pkt_ready = 1;        // 标记“有新包”
      }
    }
    // 主循环末尾：非阻塞发送，只试一次，失败就放弃
    static uint32_t last_tcp_try = 0;
    if (tcp_connected && (HAL_GetTick() - last_send_tick >= SEND_PERIOD))
    {
        last_send_tick = HAL_GetTick();
        if (pkt_ready)                 // 只要有新包就发
        {
            ESP8266_TCP_Send(curr_pkt, 20);   // 失败也不管
            pkt_ready = 0;                    // 无论成败，原地被下次新包覆盖
        }
    }
    // 传感器处理完后直接刷新
    static uint32_t last_ui_update = 0;
    if (HAL_GetTick() - last_ui_update > 50)
    { // 每 200 ms 刷新一次
      PedometerUI_Update(&params);
      last_ui_update = HAL_GetTick();
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int ESP8266_Init(void) // ESP8266初始化函�????
{
  lcd_show_string(0, line0, 240, 210, 24, "ESP8266 Init...", RED); // 显示初始化信�????

  // 启动UART3接收中断
  //    HAL_UART_Receive_IT(&huart1, &UART1_RX_Buffer[0], 1);
  // 1. 复位ESP8266
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET); // 拉低PF6引脚（RST�????
  HAL_Delay(100);                                       // 延时100ms
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);   // 拉高PF6引脚
  HAL_Delay(100);                                       // 等待模块启动

  //   1. 测试AT指令
  if (ESP8266_SendCmd("AT\r\n", "OK", 2000) != 0) // 发�?�AT指令测试
  {
    lcd_show_string(0, line1, 240, 210, 24, "AT Test Failed", RED); // 显示测试失败
    return 1;                                                       // 返回错误代码
  }

  // 2. 关闭回显
  if (ESP8266_SendCmd("ATE0\r\n", "OK", 1000) != 0) // 关闭命令回显
  {
    lcd_show_string(0, line1, 240, 210, 24, "Echo Off Failed", RED); // 显示关闭回显失败
    return 2;                                                        // 返回错误代码
  }

  // 3. 设置为STA模式
  if (ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK", 3000) != 0) // 设置WiFi模式为STA
  {
    lcd_show_string(0, line1, 240, 210, 24, "Set Mode Failed", RED); // 显示设置模式失败
    return 3;                                                        // 返回错误代码
  }

  // 4. 连接WiFi
  if (ESP8266_Connect_WiFi() != 0) // 连接WiFi网络
    return 0;                      // 连接失败返回0

  wifi_connected = 1;                                              // 设置WiFi连接标志
  lcd_show_string(0, line1, 240, 210, 24, "ESP8266 Ready", GREEN); // 显示ESP8266就绪
  return 0;                                                        // 初始化成�????
}

// 在ESP8266_SendCmd函数中添加超时退出
int ESP8266_SendCmd(char *cmd, char *ack, uint16_t timeout)
{
  // 清空接收缓冲区
  uart1_rx_index = 0;
  uart1_reception_complete = 0;
  memset(UART1_RX_Buffer, 0, RX_BUFFER_SIZE);

  // 发送命令
  HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 100);

  uint32_t start_time = HAL_GetTick();

  while ((HAL_GetTick() - start_time) < timeout)
  {
    if (uart1_reception_complete)
    {
      // 调试信息：显示接收到的数据
      char debug_msg[50];
      sprintf(debug_msg, "RCV:%.20s", UART1_RX_Buffer);
      lcd_show_string(0, line11, 240, 210, 24, debug_msg, RED);

      if (strstr((char *)UART1_RX_Buffer, ack) != NULL)
      {
        return 0; // 成功
      }
      if (strstr((char *)UART1_RX_Buffer, "ERROR") != NULL)
      {
        return 1; // 错误
      }

      uart1_reception_complete = 0;
    }

    // 关键修改：添加按键检查，可以随时退出等待
    Key_Scan();
    uint8_t key = KeyPress();
    if (key == KEY0_PRES) // 按KEY0强制退出
    {
      lcd_show_string(0, line12, 240, 210, 16, "CMD Timeout!     ", RED);
      return 2; // 用户取消
    }

    HAL_Delay(10);
  }

  return 3; // 超时
}

int ESP8266_Connect_WiFi(void) // 连接WiFi函数
{
  char cmd[64];                                                         // 命令缓冲�????
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD); // 格式化连接WiFi命令

  lcd_show_string(0, line4, 240, 210, 24, "Connecting WiFi...", RED); // 显示连接WiFi信息
  if (ESP8266_SendCmd(cmd, "WIFI GOT IP", 20000) == 0)                // 发�?�连接WiFi命令
  {
    lcd_show_string(0, line4, 240, 210, 24, "WiFi Connected    ", RED); // 显示WiFi连接成功
    return 0;                                                           // 返回成功
  }
  else
  {
    lcd_show_string(0, line4, 240, 210, 24, "WiFi Failed    ", RED); // 显示WiFi连接失败
    return 1;                                                        // 返回失败
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // UART接收完成回调函数
{
  if (huart->Instance == USART1) // 如果是USART1
  {
    // �????查是否收到结束符或缓冲区�????
    if (UART1_RX_Buffer[uart1_rx_index] == '\n' || // 换行符结�????
        uart1_rx_index >= RX_BUFFER_SIZE - 1)      // 缓冲区满
    {
      // 接收完成
      UART1_RX_Buffer[uart1_rx_index + 1] = '\0'; // 添加字符串结束符
      uart1_reception_complete = 1;               // 设置接收完成标志
                                                  //            UART_printf(&huart1, "Received: %s\r\n", UART1_RX_Buffer);

      uart1_rx_index = 0; // 重置接收索引
    }
    else
    {
      // 继续接收下一个字�????
      uart1_rx_index++; // 递增接收索引
    }

    // 重新启动接收中断，指向下�????个位�????
    HAL_UART_Receive_IT(&huart1, &UART1_RX_Buffer[uart1_rx_index], 1); // 重新使能接收中断
  }
}

// 连接TCP服务�????
int ESP8266_Connect_TCP(void) // 连接TCP服务器函�????
{
  char cmd[64];                     // 命令缓冲�????
  sprintf(cmd, "AT+CIPMUX=0\r\n");  // 关闭多连�????
  ESP8266_SendCmd(cmd, "OK", 5000); // 发�?�命�????

  //    sprintf(cmd, "AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD, ""); // 设置AP信息
  //    if(ESP8266_SendCmd(cmd, "OK", 5000) != 0) return 1;

  sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", TCP_SERVER_IP, TCP_SERVER_PORT); // 格式化TCP连接命令
  lcd_show_string(0, line5, 240, 210, 24, "Connecting TCP...", RED);                 // 显示连接TCP信息

  if (ESP8266_SendCmd(cmd, "OK", 5000) == 0) // 发�?�TCP连接命令
  {
    tcp_connected = 1;                                                     // 设置TCP连接标志
    lcd_show_string(0, line5, 240, 210, 24, "TCP Connected        ", RED); // 显示TCP连接成功
    return 0;                                                              // 返回成功
  }
  else
  {
    tcp_connected = 0;                                                  // 清除TCP连接标志
    lcd_show_string(0, line5, 240, 210, 24, "TCP Failed        ", RED); // 显示TCP连接失败
    return 1;                                                           // 返回失败
  }
}

// 显示WiFi状�??
void ESP8266_Show_Status(void) // 显示状�?�函�????
{
  if (wifi_connected)                                                  // 如果WiFi已连�????
    lcd_show_string(0, line2, 240, 210, 24, "WiFi:Connected", RED);    // 显示已连�????
  else                                                                 // WiFi未连�????
    lcd_show_string(0, line2, 240, 210, 24, "WiFi:Disconnected", RED); // 显示未连�????
}

int ESP8266_TCP_Send(unsigned char *data, int data_len) // TCP发�?�数据函�????
{
  char cmd1[32], cmd2[32];   // 命令缓冲�????
  sprintf(cmd2, "%s", data); // 格式化数据（调试用）

  sprintf(cmd1, "AT+CIPSEND=%d\r\n", data_len); // 格式化发送长度命�????

  if (ESP8266_SendCmd(cmd1, ">", 500) != 0) // 发�?�数据长�????
  {
    lcd_show_string(0, line10, 240, 210, 24, "Set Length Failed", RED); // 显示设置长度失败
    return 1;                                                           // 返回错误
  }
  HAL_UART_Transmit(&huart1, data, data_len, 100); // 直接发�?�数�????

  if (ESP8266_SendCmd("", "SEND OK", 500) == 0) // �????查发送结�????
  {
    return 0; // 成功
  }
  else
  {
    return 2; // 返回错误
  }
}
void create_packet_float(uint32_t user_id, uint32_t step_count, float calories, unsigned char *packet)
{
  // 将浮点数转换为字符串（保�?2位小数）
  char calories_str[16];
  snprintf(calories_str, sizeof(calories_str), "%.2f", calories);

  // 包头
  packet[0] = 0x55;
  packet[1] = 0xAA;

  // 用户ID
  packet[2] = (user_id >> 24) & 0xFF;
  packet[3] = (user_id >> 16) & 0xFF;
  packet[4] = (user_id >> 8) & 0xFF;
  packet[5] = user_id & 0xFF;

  // 步数
  packet[6] = (step_count >> 24) & 0xFF;
  packet[7] = (step_count >> 16) & 0xFF;
  packet[8] = (step_count >> 8) & 0xFF;
  packet[9] = step_count & 0xFF;

  // 卡路里字符串（ASCII编码�?
  int i;
  for (i = 0; i < 8 && calories_str[i] != '\0'; i++)
  {
    packet[10 + i] = calories_str[i];
  }
  // 剩余位置�?0
  for (; i < 8; i++)
  {
    packet[10 + i] = 0x00;
  }

  // 包尾
  packet[18] = 0xAA;
  packet[19] = 0x55;
}

void Key_Scan(void) // 按键扫描函数
{
  uint8_t i;                                           // 循环计数�????
  static uint8_t KeyBuff[] = {0xff, 0xff, 0xff, 0xff}; // 按键缓冲区，初始化为未按�????

  // 读取按键值并进行消抖处理
  KeyBuff[0] = (KeyBuff[0] << 1 | (KEY0_STATE & 0x01)); // KEY0状�?�移�????
  KeyBuff[1] = (KeyBuff[1] << 1 | (KEY1_STATE & 0x01)); // KEY1状�?�移�????
  KeyBuff[2] = (KeyBuff[2] << 1 | (KEY2_STATE & 0x01)); // KEY2状�?�移�????
  KeyBuff[3] = (KeyBuff[3] << 1 | (KEY3_STATE & 0x01)); // KEY3状�?�移�????

  for (i = 0; i < 4; i++) // 遍历�????有按�????
  {
    // 连续扫描8次都�????1�????8ms内都是高电平状�?�，判断为释�????
    if (KeyBuff[i] == 0xff) // �????有位都为1
    {
      KeySta[i] = 1; // 设置按键状�?�为释放
    }
    // 连续扫描8次都�????0�????8ms内都是低电平状�?�，判断为按�????
    else if (KeyBuff[i] == 0x00) // �????有位都为0
    {
      KeySta[i] = 0; // 设置按键状�?�为按下
    }
    else // 状�?�不稳定，保持原状�??
    {
    }
  }
}
uint8_t KeyPress(void) // 按键按下�????测函�????
{
  static uint8_t KeyBackup[] = {1, 1, 1, 1}; // 按键状�?�备�????
  uint8_t i, Key_Value = 0;                  // 循环计数器和按键�????

  for (i = 0; i < 4; i++) // 遍历�????有按�????
  {
    // �????测到按键状�?�发生变�????
    if (KeySta[i] != KeyBackup[i]) // 状�?�发生变�????
    {
      // 如果是按下事�????
      if (KeySta[i] == 0)       // 当前状�?�为按下
                                //				Key_Value = KeyDrive(i);  // 获取按键�????
        Key_Value = i + 1;      // 计算按键值（1-4�????
      KeyBackup[i] = KeySta[i]; // 更新状�?�备�????
    }
  }

  return Key_Value; // 返回按键�????
}

uint8_t KeyDrive(uint8_t KeyValue) // 按键驱动函数
{
  uint8_t Key_Temp = 0; // 临时按键�????

  switch (KeyValue) // 根据按键值判�????
  {
  case 0:
    Key_Temp = KEY0_PRES;
    break; // KEY0按下
  case 1:
    Key_Temp = KEY1_PRES;
    break; // KEY1按下
  case 2:
    Key_Temp = KEY2_PRES;
    break; // KEY2按下
  case 3:
    Key_Temp = KEY3_PRES;
    break; // KEY3按下
  default:
    Key_Temp = NO_KEY_PRES;
    break; // 无按键按�????
  }

  return Key_Temp; // 返回按键标志
}

void KeyActivity(void) // 按键活动处理函数
{
  uint8_t key; // 按键值
  static uint32_t last_key_time = 0;
  uint32_t current_time = HAL_GetTick();

  // 防抖处理
  if (current_time - last_key_time < 300)
  {
    return;
  }

  key = KeyPress(); // 获取按键值

  if (key == KEY2_PRES)
  {
    last_key_time = current_time;

    /* 第一次按键只做“开始”，不做“停止” */
    static uint8_t first_key_done = 0;
    if (!first_key_done) // 上电后第一次 KEY2 只负责“开始”
    {
      first_key_done = 1;
      params.start_flag = 1;
      HAL_TIM_Base_Start_IT(&htim11);
    }
    else // 之后再按才是“开始/暂停”切换
    {
      params.start_flag = !params.start_flag;
      if (params.start_flag)
        HAL_TIM_Base_Start_IT(&htim11);
      else
        HAL_TIM_Base_Stop_IT(&htim11);
    }

    lcd_gx = 1; // 触发一次 UI 刷新（可保留）
  }
  else if (key == KEY0_PRES)
  {
    last_key_time = current_time;
    signal_data = 0x06;
  }
  else if (key == KEY1_PRES)
  {
    last_key_time = current_time;
    signal_data = 0x05;
  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ // 定时器周期中断回调函�????
  if (htim->Instance == TIM10)
  {             // 如果是TIM10
    Key_Scan(); // 执行按键扫描
    if (tcp_connected && !sending_busy && send_head != send_tail)
    {
      sending_busy = 1;
      // 启动DMA发送或标记发送状态
    }
  }
  //    if (htim->Instance == TIM7) {

  //		}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq(); // 禁用中断
  while (1)        // 死循�????
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
