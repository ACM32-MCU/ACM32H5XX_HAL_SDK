/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

/*********************************************************************************************************
* function   : Timer_Update_DMA_Test
* Description: timer base function. When timer overflow or underflow, an update event will be generated.     
* input : none 
* return: none 
************************************************************************************************************/ 
 

extern void Timer_Update_DMA_Test(void);     

extern void Timer_Channel1_DMA_Test(void);  

extern void Timer_Channel2_DMA_Test(void); 

extern void Timer_Channel3_DMA_Test(void);     

extern void Timer_Channel4_DMA_Test(void);     

extern void Timer_Trigger_DMA_Test(void);  

extern void Timer_COM_DMA_Test(void);  

#endif /* __APP_H */

