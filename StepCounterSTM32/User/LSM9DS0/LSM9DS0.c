#include "LSM9DS0.h"
#include "main.h"


extern SPI_HandleTypeDef hspi1;

/****LSM9DS0_SN74LS157_GPIO_Init*****/ 
void LSM9DS0_SN74LS157_GPIO_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
//	 /*< Configure SPI_CS_XM ?��SPI_CS_G ?��SPI_CLK   pins >*/ 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	/*< Configure  G?��S pins >*/ 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	 /*< Configure  SPI_MOSI pins >*/ 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	/*< Configure SPI_MISO ?��INT1>*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	/*< Configure  INT2>*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//	/*< Configure  INT2>*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//SYNC_ADC_HIGH();
	SPI_CS_G_H;//1?SPI
	SPI_CS_XM_H;//1?SPI
//	S=0;  //S=0:??����A(?��?��?��o����?3?)
//	G=1;	//1?��???����
}  
/****WriteSingleToLSM9DS0_G*****/ 
//void WriteSingleToLSM9DS0_G(u8 addr, u8 data)
//{
//    uint8_t tx_data[2] = {addr, data};
//
//    SPI_CS_G_L;  // 选择陀螺仪
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
//    SPI_CS_G_H;  // 取消选择
//}
/****WriteSingleToLSM9DS0_XM*****/ 
void WriteSingleToLSM9DS0_XM(u8 addr, u8 data)
{
    uint8_t tx_data[2] = {addr, data};

    SPI_CS_XM_L;  // 选择加速度计/磁力计
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
    SPI_CS_XM_H;  // 取消选择
}
/****WriteMultipleToLSM9DS0_G*****/ 
//void WriteMultipleToLSM9DS0_G(u8 addr, u8 len, u8 *databuff)
//{
//    uint8_t tx_data[16];  // 最大16字节
//    tx_data[0] = WRITE_multiple | addr;
//    memcpy(&tx_data[1], databuff, len);
//
//    SPI_CS_G_L;
//    HAL_SPI_Transmit(&hspi1, tx_data, len + 1, 100);
//    SPI_CS_G_H;
//}
/****WriteMultipleToLSM9DS0_XM*****/ 
void WriteMultipleToLSM9DS0_XM(u8 addr, u8 len, u8 *databuff)
{
    uint8_t tx_data[16];  // 最大16字节
    tx_data[0] = WRITE_multiple | addr;
    memcpy(&tx_data[1], databuff, len);

    SPI_CS_XM_L;
    HAL_SPI_Transmit(&hspi1, tx_data, len + 1, 100);
    SPI_CS_XM_H;
}
/****ReadSingleFromLSM9DS0_G*****/
//u8 ReadSingleFromLSM9DS0_G(u8 addr)
//{
//    uint8_t tx_data[2] = {READ_single | addr, 0x00};
//    uint8_t rx_data[2] = {0};
//
//    SPI_CS_G_L;
//    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 100);
//    SPI_CS_G_H;
//
//    return rx_data[1];
//}
/****ReadSingleFromLSM9DS0_X*****/ 
u8 ReadSingleFromLSM9DS0_X(u8 addr)
{
    uint8_t tx_data[2] = {READ_single | addr, 0x00};
    uint8_t rx_data[2] = {0};

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 100);
    SPI_CS_XM_H;

    return rx_data[1];
}

/****ReadSingleFromLSM9DS0_M*****/ 
u8 ReadSingleFromLSM9DS0_M(u8 addr)
{
    uint8_t tx_data[2] = {READ_single | addr, 0x00};
    uint8_t rx_data[2] = {0};

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 100);
    SPI_CS_XM_H;

    return rx_data[1];
}
/****ReadSingleFromLSM9DS0_T*****/
u8 ReadSingleFromLSM9DS0_T(u8 addr)
{
    uint8_t tx_data[2] = {READ_single | addr, 0x00};
    uint8_t rx_data[2] = {0};

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 100);
    SPI_CS_XM_H;

    return rx_data[1];
}

/****ReadMultipleFromLSM9DS0_G*****/ 
//void ReadMultipleFromLSM9DS0_G(u8 addr, u8 len, u8 *databuff)
//{
//    uint8_t tx_data[16] = {0};
//    uint8_t rx_data[16] = {0};
//
//    tx_data[0] = READ_multiple | addr;
//
//    SPI_CS_G_L;
//    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, len + 1, 100);
//    SPI_CS_G_H;
//
//    memcpy(databuff, &rx_data[1], len);
//}

/****ReadMultipleFromLSM9DS0_X*****/ 
void ReadMultipleFromLSM9DS0_X(u8 addr, u8 len, u8 *databuff)
{
    uint8_t tx_data[16] = {0};
    uint8_t rx_data[16] = {0};

    tx_data[0] = READ_multiple | addr;

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, len + 1, 100);
    SPI_CS_XM_H;

    memcpy(databuff, &rx_data[1], len);
}

/****ReadMultipleFromLSM9DS0_M*****/ 
void ReadMultipleFromLSM9DS0_M(u8 addr, u8 len, u8 *databuff)
{
    uint8_t tx_data[16] = {0};
    uint8_t rx_data[16] = {0};

    tx_data[0] = READ_multiple | addr;

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, len + 1, 100);
    SPI_CS_XM_H;

    memcpy(databuff, &rx_data[1], len);
}

/****ReadMultipleFromLSM9DS0_T*****/ 
void ReadMultipleFromLSM9DS0_T(u8 addr, u8 len, u8 *databuff)
{
    uint8_t tx_data[16] = {0};
    uint8_t rx_data[16] = {0};

    tx_data[0] = READ_multiple | addr;

    SPI_CS_XM_L;
    HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, len + 1, 100);
    SPI_CS_XM_H;

    memcpy(databuff, &rx_data[1], len);
}
/****LIS3MDL_Start*****/ 
void LSM9DS0_Start(void)         
{
//	WriteSingleToLSM9DS0_G(CTRL_REG1_G,0xdf);
//	//����??����?Y��?��?3??��?��760Hz��?��1?��XYZ?����?����??����?Y��?1����¨���������??�꨺?
//	WriteSingleToLSM9DS0_G(CTRL_REG4_G,0x02);
//	//��y?Y3?D??��D?��?����??����?Y��?��?3��?a+?a245dps��?�̨�??��y?Y��?��?�̨�??��?��1?������?Y��??��1|?��?�꨺?1��?X positive sign, Y and Z negative sign��?4??spi?��?��
//	WriteSingleToLSM9DS0_G(CTRL_REG5_G,0x40);
//	//?y3��???����?��1?������?Y��?FIFO1|?��
	WriteSingleToLSM9DS0_XM(CTRL_REG0_XM,0x40);
	//?y3��???����?��1?��?��?��?��??��?????FIFO1|?��,����?��??����??2��
	WriteSingleToLSM9DS0_XM(CTRL_REG1_XM,0x9f);
	//����???��?��?��??��?3??��?��800Hz��?��y?Y?����??����??��D?��?��1?��?��?��?��??XYZ?��
	WriteSingleToLSM9DS0_XM(CTRL_REG2_XM,0x02);
	//����???��?��?��??��?3��+-2g��?��??��773Hz��?4??spi?��?����?��1?��?��?��?��??��??��1|?��Positive sign self-test
	WriteSingleToLSM9DS0_XM(CTRL_REG5_XM,0xf4);
	//???����??D?�¨�1?����?����??��?��|??��y?Y?��D??��?��?a100Hz��?1����¨���??��?��??�����䨬?��??D???��??��?
	WriteSingleToLSM9DS0_XM(CTRL_REG6_XM,0x00);
	//����??��?��|??��?3��+-2gs��?��1?��?��?��?��??��??��1|?��
	WriteSingleToLSM9DS0_XM(CTRL_REG7_XM,0x80);//?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
	//??����??2��?��?y3��?�꨺?��?����???��?��?��??1����¨���������??�꨺?��?��?��??D?�¨�?D?��a???�꨺?
	//WriteSingleToLSM9DS0_XM(FIFO_CTRL_REG,0x40);//?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
	//FIFO?�꨺?00000000:Bypass mode;00100000:FIFO mode;01000000:Stream mode......
}




