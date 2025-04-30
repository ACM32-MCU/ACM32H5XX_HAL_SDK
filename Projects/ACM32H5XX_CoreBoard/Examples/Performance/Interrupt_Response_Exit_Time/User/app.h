/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h"   



typedef struct 
{
    uint32_t loader_addr;
    uint32_t exec_addr;
    uint32_t image_lengh; 
}itcm_image_type; 

#define ITCM_SIZE_IN_BYTES   0x8000  
#define DTCM_SIZE_IN_BYTES   0x8000 
#define DTCM_START_ADDR      0x20000000U 
#define ITCM_START_ADDR      0x00000000U 

#define TCM_TEST_DMA_FUNCTION   

uint32_t APP_Test(void); 

#endif
