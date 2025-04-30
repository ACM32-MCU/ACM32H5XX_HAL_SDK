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
    TEST_SLAVE_COMM,
    TEST_SLAVE_IT,
    TEST_SLAVE_DMA,
}enum_TEST_MODE_t;

extern SPI_HandleTypeDef SPI_Handle;

#ifdef HAL_DMA_MODULE_ENABLED
extern DMA_HandleTypeDef DMA_SPIR_Handle;
extern DMA_HandleTypeDef DMA_SPIT_Handle;
#endif

/* Function : SPI_Slave_Test */
void SPI_Slave_Test(enum_TEST_MODE_t fe_Mode);

#endif
