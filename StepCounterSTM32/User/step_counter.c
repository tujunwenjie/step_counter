#include "step_counter.h"

/*=============================*/
/*       ȫ�ֱ�������          */
/*=============================*/
uint8_t step_running = 0;          // �Ƿ����ڼƲ�
uint32_t step_count   = 0;         // �Ʋ���
uint32_t step_max_count = 100;     // Ĭ�������
uint32_t calorie = 0;              // ��·��
uint32_t sport_time_sec = 0;       // ����ʱ��

uint32_t last_tick_ms = 0;         // ʱ�������

extern peak_value_t peak_val;
extern slid_reg_t slid_reg;
extern filter_avg_t mean_filter;

/*=============================*/
/*           UI����             */
/*=============================*/

void StepUI_Init(void)
{
    lcd_clear(WHITE);
    g_point_color = BLACK;

    lcd_show_string(20,20,200,16,16,(uint8_t*)"Step Counter",g_point_color);
    lcd_show_string(20,60,200,16,16,(uint8_t*)"State: STOP",g_point_color);
    lcd_show_string(20,90,200,16,16,(uint8_t*)"Steps: 0",g_point_color);
    lcd_show_string(20,120,200,16,16,(uint8_t*)"Time: 0s",g_point_color);
    lcd_show_string(20,150,200,16,16,(uint8_t*)"Calorie: 0 kcal",g_point_color);
    lcd_show_string(20,180,200,16,16,(uint8_t*)"Goal: 100",g_point_color);
}

void StepUI_Update(void)
{
    g_point_color = BLACK;

    if(!step_running)
        lcd_show_string(20,60,200,16,16,(uint8_t*)"State: STOP   ",g_point_color);
    else
        lcd_show_string(20,60,200,16,16,(uint8_t*)"State: RUNNING",g_point_color);

    lcd_show_num(80, 90, step_count, 5, 16,g_point_color);
    lcd_show_num(80,120, sport_time_sec, 6, 16,g_point_color);
    lcd_show_num(90,150, calorie, 5, 16,g_point_color);

    if(step_count >= step_max_count)
    {
        g_point_color = RED;
        lcd_show_string(20, 210, 200,16,16,(uint8_t*)"FINISH!",g_point_color);
    }
    else
    {
        lcd_show_string(20, 210, 200,16,16,(uint8_t*)"                ",g_back_color);
    }
}

/*=============================*/
/*         ���ƺ���             */
/*=============================*/

void Step_Start(void)
{
    step_running = 1;
    step_count = 0;
    sport_time_sec = 0;
    calorie = 0;
    last_tick_ms = HAL_GetTick();

    lcd_show_string(20,210,200,16,16,(uint8_t*)"                ",g_back_color);
    StepUI_Update();
}

void Step_Stop(void)
{
    step_running = 0;
    StepUI_Update();
}

/*=============================*/
/*       ���Ʋ��߼�����         */
/*=============================*/

void Step_Process(void)
{
    if(!step_running) return;

    /*������ ���������еĶ�̬��ֵ������⺯�� ������*/
    uint32_t res = detect_step(&peak_val, &slid_reg, &Accel_sample);

    if(res == 1)
    {
        step_count++;
        calorie = step_count / 100;  // ÿ100�� 1��

        // �ﵽ���ֵ
        if(step_count >= step_max_count)
        {
            step_running = 0;
            StepUI_Update();
            return;
        }
    }

    //������ ʱ���ۼ� ������
    uint32_t now = HAL_GetTick();
    if(now - last_tick_ms >= 1000)
    {
        last_tick_ms = now;
        sport_time_sec++;
    }

    //������ UI ˢ�� ������
    StepUI_Update();
}
