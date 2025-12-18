#ifndef __LSM9DS0_H
#define __LSM9DS0_H
 
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "main.h"
		
#define SPI_CS_G_H     HAL_GPIO_WritePin(CS_G_GPIO_Port,CS_G_Pin,1)
#define SPI_CS_G_L     HAL_GPIO_WritePin(CS_G_GPIO_Port,CS_G_Pin,0)

#define SPI_CS_XM_H     HAL_GPIO_WritePin(CS_XM_GPIO_Port,CS_XM_Pin,1)   
#define SPI_CS_XM_L     HAL_GPIO_WritePin(CS_XM_GPIO_Port,CS_XM_Pin,0)  
//
//
//#define SPI_SDO_XM   HAL_GPIO_ReadPin(SDO_XM_GPIO_Port,SDO_XM_Pin)  //

#define DEN_G_H 	 HAL_GPIO_WritePin(DEN_GPIO_Port,DEN_Pin,1)
#define DEN_G_L 	 HAL_GPIO_WritePin(DEN_GPIO_Port,DEN_Pin,0)

#define INT_G      HAL_GPIO_ReadPin(INT_GPIO_Port,INT_Pin)

#define DRDY_G     HAL_GPIO_ReadPin(DRDY_GPIO_Port,DRDY_Pin)

#define INT1       HAL_GPIO_ReadPin(INT1M_GPIO_Port,INT1M_Pin)

#define INT2       HAL_GPIO_ReadPin(INT2M_GPIO_Port,INTM_Pin)
//#define S 	 PBout(11)//74LS157????D?o?(*#* ��?��???3����?3?:???��D��䨪��?)
//#define G 	 PBout(10)//74LS157????D?o?(*#* ��?��???3����?3?)
//??��??�¦�??��?����?
// LSM9DS0 Gyro Registers 
#define WHO_AM_I_G			0x0F
#define CTRL_REG1_G			0x20
#define CTRL_REG2_G			0x21
#define CTRL_REG3_G			0x22
#define CTRL_REG4_G			0x23
#define CTRL_REG5_G			0x24
#define REFERENCE_G			0x25
#define STATUS_REG_G		0x27
#define OUT_X_L_G			0x28
#define OUT_X_H_G			0x29
#define OUT_Y_L_G			0x2A
#define OUT_Y_H_G			0x2B
#define OUT_Z_L_G			0x2C
#define OUT_Z_H_G			0x2D/
#define FIFO_CTRL_REG_G		0x2E
#define FIFO_SRC_REG_G		0x2F
#define INT1_CFG_G			0x30
#define INT1_SRC_G			0x31
#define INT1_THS_XH_G		0x32
#define INT1_THS_XL_G		0x33
#define INT1_THS_YH_G		0x34
#define INT1_THS_YL_G		0x35
#define INT1_THS_ZH_G		0x36
#define INT1_THS_ZL_G		0x37
#define INT1_DURATION_G		0x38

// LSM9DS0 Accel/Magneto (XM) Registers //

#define OUT_TEMP_L_XM		0x05
#define OUT_TEMP_H_XM		0x06
#define STATUS_REG_M		0x07
#define OUT_X_L_M				0x08
#define OUT_X_H_M				0x09
#define OUT_Y_L_M				0x0A
#define OUT_Y_H_M				0x0B
#define OUT_Z_L_M				0x0C
#define OUT_Z_H_M				0x0D
#define WHO_AM_I_XM			0x0F
#define INT_CTRL_REG_M	0x12
#define INT_SRC_REG_M		0x13
#define INT_THS_L_M			0x14
#define INT_THS_H_M			0x15
#define OFFSET_X_L_M		0x16
#define OFFSET_X_H_M		0x17
#define OFFSET_Y_L_M		0x18
#define OFFSET_Y_H_M		0x19
#define OFFSET_Z_L_M		0x1A
#define OFFSET_Z_H_M		0x1B
#define REFERENCE_X			0x1C
#define REFERENCE_Y			0x1D
#define REFERENCE_Z			0x1E
#define CTRL_REG0_XM		0x1F
#define CTRL_REG1_XM		0x20
#define CTRL_REG2_XM		0x21
#define CTRL_REG3_XM		0x22
#define CTRL_REG4_XM		0x23
#define CTRL_REG5_XM		0x24
#define CTRL_REG6_XM		0x25
#define CTRL_REG7_XM		0x26
#define STATUS_REG_A		0x27
#define OUT_X_L_A				0x28
#define OUT_X_H_A				0x29
#define OUT_Y_L_A				0x2A
#define OUT_Y_H_A				0x2B
#define OUT_Z_L_A				0x2C
#define OUT_Z_H_A				0x2D
#define FIFO_CTRL_REG		0x2E
#define FIFO_SRC_REG		0x2F
#define INT_GEN_1_REG		0x30
#define INT_GEN_1_SRC		0x31
#define INT_GEN_1_THS		0x32
#define INT_GEN_1_DURATION	0x33
#define INT_GEN_2_REG		0x34
#define INT_GEN_2_SRC		0x35
#define INT_GEN_2_THS		0x36
#define INT_GEN_2_DURATION	0x37
#define CLICK_CFG				0x38
#define CLICK_SRC				0x39
#define CLICK_THS				0x3A
#define TIME_LIMIT			0x3B
#define TIME_LATENCY		0x3C
#define TIME_WINDOW			0x3D
#define ACT_THS					0x3E
#define ACT_DUR					0x3F
//??��??��????��y?Y
///**????CTRL_REG_1**/
//#define FAST_ODR_HIGH 	0x02//???����?3?��1?��(LowPower����1000Hz/Medium-performance560Hz/High-performance300HZ)
//#define FAST_ODR_LOW  	0x00//???����?3?����?��(LowPower����1000Hz)(default)
//#define TEMP_EN_HIGH  	0x80//???����?3?��1?��
//#define TEMP_EN_LOW   	0x00//???����?3?����?��(default)
//#define ST_HIGH       	0x01//self-test endisabled
//#define ST_LOW        	0x00//self-test disabled(default)
//#define OM_LP        	  0x00//Low-power mode(default)
//#define OM_MP         	0x20//Medium-performance mode
//#define DO_7         	  0x1c//Medium-performance mode
//#define OM_HP         	0x40//High-performance mode
//#define OM_UP         	0x60//Ultra-high-performance mode

///**????CTRL_REG_2**/
//#define FS_4          	0x00//Full-scale ?��4 gauss(default)
//#define FS_8          	0x20//Full-scale ?��8 gauss
//#define FS_12         	0x40//Full-scale ?��12 gauss
//#define FS_16         	0x60//Full-scale ?��16 gauss
//#define REBOOT_HIGH     0x08//reboot memory content
//#define REBOOT_LOW      0x00//normal mode(default)
//#define SOFT_RST_HIGH   0x04// Reset operation
//#define SOFT_RST_LOW    0x00//(default)

///**????CTRL_REG_3**/
//#define LP_HIGH        	0x20//
//#define LP_LOW        	0x00//(default)
//#define SIM_HIGH        0x04//SPI 3-wire interface
//#define SIM_LOW         0x00//SPI 4-wire interface(default)
//#define MD_00           0x00//Continuous-conversion mode
//#define MD_01           0x01//Single-conversion mode
//#define MD_10         	0x02//Power-down mode
//#define MD_11         	0x03//Power-down mode(default)

///**????CTRL_REG_4**/
//#define BLE_HIGH        0x02// data MSb at lower address
//#define BLE_LOW        	0x00// data LSb at lower address(default)
//#define OMZ_LP          0x00//Low-power mode(default)
//#define OMZ_MP          0x04//Medium-performance mode
//#define OMZ_HP         	0x08//High-performance mode
//#define OMZ_UP         	0x0c//Ultra-high-performance mode

///**????CTRL_REG_5**/
//#define FAST_READ_HIGH  0x80//FAST_READ enabled
//#define FAST_READ_LOW   0x00// FAST_READ disabled(default)
//#define BDU_HIGH 			  0x40//output registers not updated until MSb and LSb have been read
//#define BDU_LOW         0x00// continuous update(default)

///**????INT_CFG**/
//#define INT_CFG_EN          0x00//?D??1?��?(??��?)

//?????��D��
#define WRITE_single      0x00
#define READ_single       0x80
#define WRITE_multiple    0x40
#define READ_multiple     0xc0
//o����y����?��
void LSM9DS0_SN74LS157_GPIO_Init(void);
//void WriteSingleToLSM9DS0_G(u8 addr, u8 data);
void WriteSingleToLSM9DS0_XM(u8 addr, u8 data);
//void WriteMultipleToLSM9DS0_G(u8 addr,u8 len,u8 *databuff);
void WriteMultipleToLSM9DS0_XM(u8 addr,u8 len,u8 *databuff);
//u8 ReadSingleFromLSM9DS0_G(u8 addr);
u8 ReadSingleFromLSM9DS0_X(u8 addr);
u8 ReadSingleFromLSM9DS0_M(u8 addr);
u8 ReadSingleFromLSM9DS0_T(u8 addr);
//void ReadMultipleFromLSM9DS0_G(u8 addr,u8 len, u8 *databuff);
void ReadMultipleFromLSM9DS0_X(u8 addr,u8 len, u8 *databuff);
void ReadMultipleFromLSM9DS0_M(u8 addr,u8 len, u8 *databuff);
void ReadMultipleFromLSM9DS0_T(u8 addr,u8 len, u8 *databuff);
void LSM9DS0_Start(void);    
#endif 


