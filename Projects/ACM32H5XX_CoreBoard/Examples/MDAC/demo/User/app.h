
/******************************************************************************
*@file  : mdac_test.h
*@brief : header file
******************************************************************************/

#ifndef __MDAC_TEST_H
#define __MDAC_TEST_H

#include "hal.h"

#define __FIXHERE() printfS("fixhere %s, %d\r\n", __FILE__, __LINE__);


typedef enum
{
    DAC_Output_Noise,
    DAC_Output_SAWTOOTH,
    DAC_OutPut_Voltage,   
    DAC_OutPut_Sinx,
#ifdef HAL_DMA_MODULE_ENABLED    
    DAC_DMA_Output_Sinx,
    DAC_DMADUALDATA_Output_Sinx
#endif    
}DAC_TEST_MODE_t;

/* APP_DAC_Demo */
void MDAC_test(void);
void MDAC_Reset(void);
bool mdac_test_register(void);



#endif /* __MDAC_TEST_H */

