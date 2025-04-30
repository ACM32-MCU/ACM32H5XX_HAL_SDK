
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_TEST_H
#define __APP_TEST_H

#include "hal.h"
#include "acm32g103_core.h" 

typedef struct
{
    GPIO_TypeDef    *gpio;
    uint32_t        pin;
} app_test1_1;


void APP_Test1(void);

#endif /* __APP_H */

