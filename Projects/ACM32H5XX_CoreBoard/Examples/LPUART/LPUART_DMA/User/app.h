
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

extern LPUART_HandleTypeDef LPUART1_Handle;
extern DMA_HandleTypeDef DMA1CH0_Tx;
extern DMA_HandleTypeDef DMA2CH0_Rx;

void APP_Test(void);

#endif /* __APP_H */

