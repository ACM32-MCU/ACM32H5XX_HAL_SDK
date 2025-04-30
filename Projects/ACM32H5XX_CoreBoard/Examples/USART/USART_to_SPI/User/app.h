/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

extern USART_HandleTypeDef  USART1_Handle;
extern UART_HandleTypeDef   UART2_Handle;

void APP_Test(void);

#endif
