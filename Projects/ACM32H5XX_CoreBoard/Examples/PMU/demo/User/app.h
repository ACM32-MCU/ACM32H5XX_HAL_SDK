
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h"


extern volatile uint32_t g_extiLine;


typedef enum
{
    Test_PMU_Sleep,
    Test_PMU_Stop,
    Test_PMU_Standby,
    Test_PMU_BOR,
    Test_PMU_LVD,
    Test_PMU_VBAT,
    Test_PMU_ANAOUT,
}Test_mode_t;   


void APP_Test(void);


#endif /* __APP_H */

