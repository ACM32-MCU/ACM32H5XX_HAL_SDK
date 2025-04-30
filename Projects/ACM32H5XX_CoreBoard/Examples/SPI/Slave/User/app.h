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
#ifdef HAL_DMA_MODULE_ENABLED    
    TEST_SLAVE_DMA,
#endif    
}enum_TEST_MODE_t;

typedef enum
{
    TEST_1X_MODE = 0,
    TEST_2X_MODE,
    TEST_4X_MODE,
}Test_XMode_enum;


extern SPI_HandleTypeDef SPI_Handle;
#ifdef HAL_DMA_MODULE_ENABLED
extern DMA_HandleTypeDef DMA_SPIR_Handle;
extern DMA_HandleTypeDef DMA_SPIT_Handle;
#endif

void SPI_Slave_Test(enum_TEST_MODE_t fe_Mode);
void spi_slave_none_batch_commucation_test(void);
#endif
