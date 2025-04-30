
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h"



#define TEST_DATA_LEN    16  

extern uint32_t PSRAM_Data_Test[TEST_DATA_LEN];  

uint8_t App_in_PSRAM(uint32_t * p_datain);  

#endif /* __APP_H */

