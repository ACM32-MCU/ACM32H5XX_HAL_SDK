
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
	Test_IWDT_Reset,
	Test_IWDT_Wakeup,
    Test_IWDT_Wakeup_Standby,
}Test_mode_t;   


void APP_Test(void);




#endif /* __APP_H */

