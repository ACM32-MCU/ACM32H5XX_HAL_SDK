#ifndef __DAC7311_H__
#define __DAC7311_H__

#include "hal.h"





#define DAC1_SYNC_PIN       (GPIO_PIN_0)
#define DAC1_SYNC_PORT      (GPIOC)
     
#define DAC2_SYNC_PIN       (GPIO_PIN_2)
#define DAC2_SYNC_PORT      (GPIOC) 

#define DAC_SCK_PIN         (GPIO_PIN_1)
#define DAC_SCK_PORT        (GPIOC) 

#define DAC_DIN_PIN         (GPIO_PIN_3)
#define DAC_DIN_PORT        (GPIOC) 


#define AO1_DIN_H()		HAL_GPIO_WritePin(DAC_DIN_PORT, DAC_DIN_PIN, GPIO_PIN_SET)
#define AO1_DIN_L()   	HAL_GPIO_WritePin(DAC_DIN_PORT, DAC_DIN_PIN, GPIO_PIN_RESET)
#define AO1_SCLK_H()	HAL_GPIO_WritePin(DAC_SCK_PORT, DAC_SCK_PIN, GPIO_PIN_SET)
#define AO1_SCLK_L()   	HAL_GPIO_WritePin(DAC_SCK_PORT, DAC_SCK_PIN, GPIO_PIN_RESET)
#define AO1_SYNC_H()	HAL_GPIO_WritePin(DAC1_SYNC_PORT, DAC1_SYNC_PIN, GPIO_PIN_SET)
#define AO1_SYNC_L()   	HAL_GPIO_WritePin(DAC1_SYNC_PORT, DAC1_SYNC_PIN, GPIO_PIN_RESET)

#define AO2_DIN_H()		HAL_GPIO_WritePin(DAC_DIN_PORT, DAC_DIN_PIN, GPIO_PIN_SET)
#define AO2_DIN_L()   	HAL_GPIO_WritePin(DAC_DIN_PORT, DAC_DIN_PIN, GPIO_PIN_RESET)
#define AO2_SCLK_H()	HAL_GPIO_WritePin(DAC_SCK_PORT, DAC_SCK_PIN, GPIO_PIN_SET)
#define AO2_SCLK_L()   	HAL_GPIO_WritePin(DAC_SCK_PORT, DAC_SCK_PIN, GPIO_PIN_RESET)
#define AO2_SYNC_H()	HAL_GPIO_WritePin(DAC2_SYNC_PORT, DAC2_SYNC_PIN, GPIO_PIN_SET)
#define AO2_SYNC_L()   	HAL_GPIO_WritePin(DAC2_SYNC_PORT, DAC2_SYNC_PIN, GPIO_PIN_RESET)

void delay(volatile uint8_t us);
void DAC7311_Init(void);
void AO1_Write(uint16_t Data);
void AO2_Write(uint16_t Data);
void AO1_Set_Voltage(float voltage);
void AO2_Set_Voltage(float voltage);
                        




#endif 