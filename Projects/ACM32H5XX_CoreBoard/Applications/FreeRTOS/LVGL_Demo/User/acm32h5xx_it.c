/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and
*           peripherals interrupt service routine.
******************************************************************************/

#include "acm32h5xx_it.h"
#include "FreeRTOS.h"


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

void DebugMon_Handler(void)
{
}

void EXTI0_IRQHandler(void)
{
    HAL_EXTI_IRQHandler(EXTI_LINE_0);
}

void SysTick_Handler(void)
{
    extern void vSysTickHandler( void );
    HAL_IncTick();
	if(0!=uxTaskGetNumberOfTasks())
	{
		vSysTickHandler();
	}
}

