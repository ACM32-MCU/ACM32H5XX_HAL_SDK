
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"



extern DAC_HandleTypeDef hdac1;

typedef enum
{
    DAC_Output_Noise,
    DAC_Output_SAWTOOTH,
    DAC_OutPut_Voltage,
    DAC_OutPut_Sinx,
#ifdef HAL_DMA_MODULE_ENABLED    
    DAC_DMA_Output_Sinx,    
    DAC_DMADUALDATA_Output_Sinx,
#endif    
}DAC_TEST_MODE_t;

/* APP_DAC_Demo */
void APP_DAC_Demo(DAC_TEST_MODE_t fe_Mode);
void EXTI_GPIO_Config(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t u32_EXTILine);


#endif /* __APP_H */

