

#ifndef __RTC_H__
#define __RTC_H__

#include "hal.h"


extern volatile uint32_t gRTCIrqFlag;


void RTC_Init(void);

void RTC_SetDateTime(void);

void RTC_GetDateTime(void);

void RTC_WakeupTimer_Init(uint32_t source, uint32_t counter);

void RTC_Tamper_Init(uint32_t adge, uint32_t clearBkp);



















#endif


