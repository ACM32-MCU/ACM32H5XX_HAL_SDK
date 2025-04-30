/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 


typedef struct 
{
    GPIO_TypeDef * Port;
    uint32_t  pin; 
    uint32_t  function;
    uint32_t  pull; 
}GPIO_Config; 

typedef struct 
{
    GPIO_Config * p_config;
    uint32_t timer_index;
    uint32_t timer_channel;  
}TIMER_Config; 

typedef struct 
{
    GPIO_Config ch1;   
    GPIO_Config ch2; 
    GPIO_Config ch3; 
    GPIO_Config ch4; 
}Channels_GPIO_Config;  

#define TIMER_INDEX_MAX   25   
#define TIMER_CHANNEL_INDEX_MAX   4    

/*********************************************************************************************************
* function   : Timer_Update_Test
* Description: timer base function. When timer overflow or underflow, an update event will be generated.     
* input : none 
* return: none 
************************************************************************************************************/ 
extern void Timer_Capture_Test(void);     

extern void Timer_Capture_Inter_Connect_Test(TIM_TypeDef *Instance, uint32_t channel, uint32_t ti_select); 

#define TIM_CAPTURE_CHANNEL_MAX  8     

#endif /* __APP_H */

