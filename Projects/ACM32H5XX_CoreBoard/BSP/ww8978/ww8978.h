/******************************************************************************
*@file  : ww8978.h
*@brief : WW8978 driver. 
******************************************************************************/

#ifndef __WW8978_H
#define __WW8978_H
                                            
#include "hal.h" 									
 
#define	WM8978_ADDRESS				( 0x1AU )
#define	WM8978_WIRTE_ADDRESS		( WM8978_ADDRESS << 1)
 
//如果AD0脚(4脚)接地,IIC地址为0X4A(不包含最低位).
//如果接V3.3,则IIC地址为0X4B(不包含最低位).
#define WM8978_ADDR				0x1A	//WM8978的器件地址,固定为0x1A 
 
#define EQ1_80Hz		0X00
#define EQ1_105Hz		0X01
#define EQ1_135Hz		0X02
#define EQ1_175Hz		0X03

#define EQ2_230Hz		0X00
#define EQ2_300Hz		0X01
#define EQ2_385Hz		0X02
#define EQ2_500Hz		0X03

#define EQ3_650Hz		0X00
#define EQ3_850Hz		0X01
#define EQ3_1100Hz		0X02
#define EQ3_14000Hz		0X03

#define EQ4_1800Hz		0X00
#define EQ4_2400Hz		0X01
#define EQ4_3200Hz		0X02
#define EQ4_4100Hz		0X03

#define EQ5_5300Hz		0X00
#define EQ5_6900Hz		0X01
#define EQ5_9000Hz		0X02
#define EQ5_11700Hz		0X03

typedef uint8_t  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
  
  
extern I2S_HandleTypeDef I2S1_Handle;
extern I2C_HandleTypeDef I2C4_Handle;


uint32_t BSP_WM8978_Init(uint32_t mode); 
void BSP_WM8978_ADDA_Cfg(uint32_t dacen, uint32_t adcen);
void BSP_WM8978_Input_Cfg(uint32_t micen, uint32_t lineinen, uint32_t auxen);
void BSP_WM8978_Output_Cfg(uint32_t dacen, uint32_t bpsen);
void BSP_WM8978_MIC_Gain(uint32_t gain);
void BSP_WM8978_LINEIN_Gain(uint32_t gain);
void BSP_WM8978_AUX_Gain(uint32_t gain);
uint32_t BSP_WM8978_Write_Reg(uint32_t reg, uint32_t value); 
uint32_t BSP_WM8978_Read_Reg(uint32_t reg);
void BSP_WM8978_HPvol_Set(uint32_t voll, uint32_t volr);
void BSP_WM8978_SPKvol_Set(uint32_t volx);
void BSP_WM8978_I2S_Cfg(uint32_t fmt, uint32_t len);
void BSP_WM8978_3D_Set(uint32_t depth);
void BSP_WM8978_EQ_3D_Dir(uint32_t dir); 
void BSP_WM8978_EQ1_Set(uint32_t cfreq, uint32_t gain); 
void BSP_WM8978_EQ2_Set(uint32_t cfreq, uint32_t gain);
void BSP_WM8978_EQ3_Set(uint32_t cfreq, uint32_t gain);
void BSP_WM8978_EQ4_Set(uint32_t cfreq, uint32_t gain);
void BSP_WM8978_EQ5_Set(uint32_t cfreq, uint32_t gain);


#endif



