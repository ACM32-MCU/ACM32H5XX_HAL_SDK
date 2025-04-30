/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

typedef enum
{
    TEST_UART1_LOOP,
    TEST_UART1_IT,
    TEST_UART1_DMA,
    TEST_UART2_LOOP,
    TEST_UART2_IT,
}enum_TEST_MODE_t;

/*--------------------extern vars-----------------*/

extern UART_HandleTypeDef  UART1_Handle;
extern UART_HandleTypeDef  UART2_Handle;

#ifdef HAL_DMA_MODULE_ENABLED
extern DMA_HandleTypeDef  DMA_TX_Handle;
extern DMA_HandleTypeDef  DMA_RX_Handle;
#endif
/*---------------------functions------------------*/

void UART1_Init(void);
void UART2_Init(void);

void APP_Test(enum_TEST_MODE_t fe_Mode);

#endif
