/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/
#include "app.h"
#include "acm32h5xx_it.h"

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


void LPUART1_IRQHandler(void)
{
    HAL_LPUART_IRQHander(&LPUART1_Handle);
}
