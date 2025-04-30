
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h"


extern ADC_HandleTypeDef ADC_RegularHandle;



typedef enum
{
	Test_ADC_Regular,
	Test_ADC_Inject,
    Test_ADC_Interrupt,
    Test_ADC_External_Trigger,
    Test_ADC_DMA,
    Test_ADC_AnalogWatchdog,   
    Test_ADC_Dual,
    Test_ADC_Differential,
    Test_ADC_TemperatureSensor,
    Test_ADC_Regular_ContinuousMode,
}Test_mode_t;   


void APP_Test(void);




#endif /* __APP_H */

