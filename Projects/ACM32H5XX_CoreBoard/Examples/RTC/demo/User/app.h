
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h"


extern volatile uint32_t gRTCIrqFlag;



typedef enum
{
    Test_RTC_Date_Time,
    Test_RTC_Alarm,
    Test_RTC_Tamper,
    Test_RTC_WakeupTimer,
}Test_mode_t;   

extern void RTC_SetDateTime(void);
extern void RTC_GetDateTime(void);

void APP_Test(void);

void RTC_IRQCallback(void);


#endif /* __APP_H */

