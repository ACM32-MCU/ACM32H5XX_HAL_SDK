/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

typedef enum
{
    PMU_SLEEP_INT_MODE = 0,
    PMU_SLEEP_EVENT_MODE,
    PMU_STOP_INT_MODE,
    PMU_STOP_EVENT_MODE,
    PMU_STANDBY_INT_MODE,
    PMU_STANDBY_EVENT_MODE,
}LowPowerMode_TypeDef;

void APP_Test(void);
void KEY_Init(void);

#endif /* __APP_H */

