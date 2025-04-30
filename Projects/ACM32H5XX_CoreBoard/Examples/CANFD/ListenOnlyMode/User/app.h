/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal_fdcan.h"
#include "acm32h5xx_coreboard.h" 


typedef enum
{
    TEST_LOM,
    TEST_LOM_LBME,   
}TEST_MODE_enum;


extern FDCAN_HandleTypeDef fdcan_handler;

void app_test(TEST_MODE_enum mode);
#endif
