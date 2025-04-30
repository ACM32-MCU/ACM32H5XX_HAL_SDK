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
    TEST_MASTER_COMM,
    TEST_MASTER_IT,
    TEST_MASTER_EEPROM,
}enum_TEST_MODE_t;

extern I2C_HandleTypeDef I2C_Handle;
/* Function : I2C_Master_Test */
void I2C_Master_Test(enum_TEST_MODE_t fe_Mode);


#endif
