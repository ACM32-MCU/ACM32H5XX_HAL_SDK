/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 


extern FDCAN_HandleTypeDef fdcan_handler;

void APP_Test(void);
#endif
