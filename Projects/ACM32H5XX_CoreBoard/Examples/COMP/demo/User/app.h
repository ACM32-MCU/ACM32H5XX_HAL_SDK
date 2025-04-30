/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

#define SRAM_START_ADDR         (0x20008000)//SRAM的起始地址
#define SRAM_SIZE               (0x50000)
#define DATA_SEG_SIZE           (0x1000)
#define CODE_SEG_SIZE           (0x3000)
#define BOOT_DATA_SEG_SIZE      (0x2000)

void APP_Test(void);

#endif
