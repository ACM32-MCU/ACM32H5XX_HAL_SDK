/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_evb.h" 

extern uint8_t  LOCAL_ADDR[4];
extern uint16_t LOCAL_PORT;

extern uint8_t  DEST_ADDR[4];
extern uint16_t DEST_PORT;

void APP_Test(void);

#endif

