/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

extern I2S_HandleTypeDef I2S1_Handle;
extern I2C_HandleTypeDef I2C4_Handle;
extern DMA_HandleTypeDef DMA1CH0_Handle;
extern DMA_HandleTypeDef DMA2CH0_Handle;


void APP_Test(void);

#endif /* __APP_H */

