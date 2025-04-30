/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

/*********************************************************************************************************
* function   : Timer_Update_Test
* Description: timer base function. When timer overflow or underflow, an update event will be generated.     
* input : none 
* return: none 
************************************************************************************************************/ 
extern void Timer_Update_Test(void);     
void COMP1_Init(void);


#define TIM_INSTANCE(x) TIM##x

#endif /* __APP_H */

