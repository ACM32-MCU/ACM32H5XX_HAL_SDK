/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/

#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

#define BUFFER_LENGTH    (256)

#define SLAVE_ADDRESS    (0xA6)

typedef enum
{
    TEST_SLAVE_POLLING,
    TEST_SLAVE_INTERRUPT,
#ifdef HAL_DMA_MODULE_ENABLED    
    TEST_SLAVE_DMA,
#endif    
}enum_TEST_MODE_t;

extern I2C_HandleTypeDef I2C_Handle;
void I2C_Slave_Test(enum_TEST_MODE_t fe_Mode);

#endif
