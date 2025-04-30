
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
	Test_WDT_Reset,
	Test_WDT_IRQ,
}Test_mode_t;   


void APP_Test(void);




#endif /* __APP_H */

