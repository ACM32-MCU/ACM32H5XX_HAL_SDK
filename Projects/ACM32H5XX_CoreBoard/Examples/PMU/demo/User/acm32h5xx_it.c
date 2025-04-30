/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/

#include "acm32h5xx_it.h"


extern volatile uint32_t gRTCIrqFlag;
extern void LVD_IRQCallback(void);


void NMI_Handler(void)
{   
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}


void MemManage_Handler(void)
{
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    while (1)
    {
    }
}


void UsageFault_Handler(void)
{
    while (1)
    {
    }
}


void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}


void PendSV_Handler(void)
{
}


/******************************************************************************
*@brief : System tick handler
*@param : none
*@return: none
******************************************************************************/
void SysTick_Handler(void)
{
	HAL_IncTick();
}
void EXTI0_IRQHandler(void)
{
    HAL_EXTI_IRQHandler(EXTI_LINE_0);
}

void EXTI15_10_IRQHandler(void)
{
    HAL_EXTI_IRQHandler(EXTI_LINE_11);
    HAL_EXTI_IRQHandler(EXTI_LINE_13);
}


void LVD_IRQHandler(void)
{
    LVD_IRQCallback();
}
