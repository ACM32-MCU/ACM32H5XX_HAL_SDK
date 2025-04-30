/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/

#include "hal.h"
#include "acm32h5xx_it.h"
#include "app.h"

volatile uint32_t systick_value_test1, systick_value_test2, systick_value_test3, svc_flag; 

void NMI_Handler(void)
{   
}


void HardFault_Handler(void)
{
    printfS("hardfault\r\n");
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
    systick_value_test2 = SysTick->VAL;  
    svc_flag = 1;  
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



void Generate_SVC_Int(void)
{
    SysTick->CTRL = 0;
    __NOP();
    __NOP();
    SysTick->LOAD  = 0xFFFFFF;                         /* set reload register */
    SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */                                                /* Function successful */

    systick_value_test1 = SysTick->VAL;  
    __asm volatile ("SVC #1"); 
    systick_value_test3 = SysTick->VAL;  
}

