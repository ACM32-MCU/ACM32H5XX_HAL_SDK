/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 


#define ITCM_BASE_ADDRESS  0  
#define DTCM_BASE_ADDRESS  0x20000000     

void DTCM_Access_Test(void);  
void Copy_Vector_Table_Remap(void);

#endif
