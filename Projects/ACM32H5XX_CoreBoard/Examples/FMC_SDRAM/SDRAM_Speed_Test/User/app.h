
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
	Test_8_16_32Bit,
	Test_Attribute,
    Test_Memcpy,
    Test_DMA,
}Test_mode_t;   


void APP_Test(void);




#endif /* __APP_H */

