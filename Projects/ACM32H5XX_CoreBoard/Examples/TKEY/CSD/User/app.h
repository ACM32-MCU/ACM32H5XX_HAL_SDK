/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

void TKEY_CSD_Init(void);
/* APP_Test */
void APP_Test(void);
void TKEY_IP_test(void);
//void RTC_IRQCallback(void);
void Cp_Autotest(void);

#endif
