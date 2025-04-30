/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/

#include "acm32h5xx_it.h"
#include "app.h"

extern volatile uint32_t sp_end; 

void NMI_Handler(void)
{   
    printfS("UsageFault handler\r\n");    
    printfS("Stack end data: 0x%x\r\n", *(uint32_t *)(sp_end + 4) );   
    while(1);  
}


void Hard_Fault_Handler_c(unsigned int * hardfault_args, unsigned lr_value)
{    
    unsigned int stacked_r0; //??? r0
    unsigned int stacked_r1; //??? r1
    unsigned int stacked_r2; //??? r2
    unsigned int stacked_r3; //??? r3
    unsigned int stacked_r12; //??? r12
    unsigned int stacked_lr; //??? lr
    unsigned int stacked_pc; //??? pc
    unsigned int stacked_psr; //??? psr

    stacked_r0 = ((unsigned int) hardfault_args[0]);
    stacked_r1 = ((unsigned int) hardfault_args[1]);
    stacked_r2 = ((unsigned int) hardfault_args[2]);
    stacked_r3 = ((unsigned int) hardfault_args[3]);
    stacked_r12 = ((unsigned int)hardfault_args[4]);
    stacked_lr = ((unsigned int) hardfault_args[5]);
    stacked_pc = ((unsigned int) hardfault_args[6]);
    stacked_psr = ((unsigned int) hardfault_args[7]);
    
    {
        printf("[Hard Fault handler]\r\n");  
        printfS("Stack end data: 0x%x\r\n", *(uint32_t *)(sp_end + 4) ); 
        printfS("USG SCB->CFSR:0x%x\n", SCB->CFSR);   
        printf("R0 = 0x%08x\r\n", stacked_r0);
        printf("R1 = 0x%08x\r\n", stacked_r1);
        printf("R2 = 0x%08x\r\n", stacked_r2);
        printf("R3 = 0x%08x\r\n", stacked_r3);
        printf("R12 = 0x%08x\r\n", stacked_r12);
        printf("Stacked LR = 0x%08x\r\n", stacked_lr);
        printf("Stacked PC = 0x%08x\r\n", stacked_pc);
        printf("Stacked PSR = 0x%08x\r\n", stacked_psr);
        printf("SCB_SHCSR=0x%08x\r\n",SCB->SHCSR);
        printf("Current LR = 0x%08x\r\n", lr_value); 
    }
    while(1);
}


void MemManage_Handler_c(unsigned int * hardfault_args, unsigned lr_value)
{    
    unsigned int stacked_r0; //??? r0
    unsigned int stacked_r1; //??? r1
    unsigned int stacked_r2; //??? r2
    unsigned int stacked_r3; //??? r3
    unsigned int stacked_r12; //??? r12
    unsigned int stacked_lr; //??? lr
    unsigned int stacked_pc; //??? pc
    unsigned int stacked_psr; //??? psr
    unsigned int cfsr; 

    stacked_r0 = ((unsigned int) hardfault_args[0]);
    stacked_r1 = ((unsigned int) hardfault_args[1]);
    stacked_r2 = ((unsigned int) hardfault_args[2]);
    stacked_r3 = ((unsigned int) hardfault_args[3]);
    stacked_r12 = ((unsigned int)hardfault_args[4]);
    stacked_lr = ((unsigned int) hardfault_args[5]);
    stacked_pc = ((unsigned int) hardfault_args[6]);
    stacked_psr = ((unsigned int) hardfault_args[7]);  
    cfsr = SCB->CFSR;  
    
    {
        printf("[MemManage handler]\r\n");  
        printfS("Stack end data: 0x%x\r\n", *(uint32_t *)(sp_end + 4) ); 
        printf("R0 = 0x%08x\r\n", stacked_r0);
        printf("R1 = 0x%08x\r\n", stacked_r1);
        printf("R2 = 0x%08x\r\n", stacked_r2);
        printf("R3 = 0x%08x\r\n", stacked_r3);
        printf("R12 = 0x%08x\r\n", stacked_r12);
        printf("Stacked LR = 0x%08x\r\n", stacked_lr);
        printf("Stacked PC = 0x%08x\r\n", stacked_pc);
        printf("Stacked PSR = 0x%08x\r\n", stacked_psr);
        printf("SCB_SHCSR=0x%08x\r\n",SCB->SHCSR);
        printf("Current LR = 0x%08x\r\n", lr_value); 
        if (cfsr & 0x0080)
        {
            printfS("SCB_MMFAR=0x%08x\r\n", SCB->MMFAR);  
        }
    }
    while(1);
}



void BusFault_Handler_c(unsigned int * hardfault_args, unsigned lr_value)
{    
    unsigned int stacked_r0; //??? r0
    unsigned int stacked_r1; //??? r1
    unsigned int stacked_r2; //??? r2
    unsigned int stacked_r3; //??? r3
    unsigned int stacked_r12; //??? r12
    unsigned int stacked_lr; //??? lr
    unsigned int stacked_pc; //??? pc
    unsigned int stacked_psr; //??? psr 
    unsigned int cfsr; 

    stacked_r0 = ((unsigned int) hardfault_args[0]);
    stacked_r1 = ((unsigned int) hardfault_args[1]);
    stacked_r2 = ((unsigned int) hardfault_args[2]);
    stacked_r3 = ((unsigned int) hardfault_args[3]);
    stacked_r12 = ((unsigned int)hardfault_args[4]);
    stacked_lr = ((unsigned int) hardfault_args[5]);
    stacked_pc = ((unsigned int) hardfault_args[6]);
    stacked_psr = ((unsigned int) hardfault_args[7]);
    cfsr = SCB->CFSR;
    {
        printfS("[BusFault handler]\r\n");  
        printfS("Stack end data: 0x%x\r\n", *(uint32_t *)(sp_end + 4) ); 
        printfS("BUS SCB->CFSR:0x%x\n", cfsr);     
        printfS("R0 = 0x%08x\r\n", stacked_r0);
        printfS("R1 = 0x%08x\r\n", stacked_r1);
        printfS("R2 = 0x%08x\r\n", stacked_r2);
        printfS("R3 = 0x%08x\r\n", stacked_r3);
        printfS("R12 = 0x%08x\r\n", stacked_r12);
        printfS("Stacked LR = 0x%08x\r\n", stacked_lr);
        printfS("Stacked PC = 0x%08x\r\n", stacked_pc);
        printfS("Stacked PSR = 0x%08x\r\n", stacked_psr);
        printfS("SCB_SHCSR=0x%08x\r\n",SCB->SHCSR);
        printfS("Current LR = 0x%08x\r\n", lr_value); 
        if (cfsr & 0x8000)
        {
            printfS("SCB_BFAR=0x%08x\r\n", SCB->BFAR);    
        }
    }
    while(1);
}


void UsageFault_Handler_c(unsigned int * hardfault_args, unsigned lr_value)
{    
    unsigned int stacked_r0; //??? r0
    unsigned int stacked_r1; //??? r1
    unsigned int stacked_r2; //??? r2
    unsigned int stacked_r3; //??? r3
    unsigned int stacked_r12; //??? r12
    unsigned int stacked_lr; //??? lr
    unsigned int stacked_pc; //??? pc
    unsigned int stacked_psr; //??? psr

    stacked_r0 = ((unsigned int) hardfault_args[0]);
    stacked_r1 = ((unsigned int) hardfault_args[1]);
    stacked_r2 = ((unsigned int) hardfault_args[2]);
    stacked_r3 = ((unsigned int) hardfault_args[3]);
    stacked_r12 = ((unsigned int)hardfault_args[4]);
    stacked_lr = ((unsigned int) hardfault_args[5]);
    stacked_pc = ((unsigned int) hardfault_args[6]);
    stacked_psr = ((unsigned int) hardfault_args[7]);
    
    {
        printfS("[UsageFault handler]\r\n");  
        printfS("Stack end data: 0x%x\r\n", *(uint32_t *)(sp_end + 4) ); 
        printfS("USG SCB->CFSR:0x%x\n", SCB->CFSR); 
        printfS("R0 = 0x%08x\r\n", stacked_r0);
        printfS("R1 = 0x%08x\r\n", stacked_r1);
        printfS("R2 = 0x%08x\r\n", stacked_r2);
        printfS("R3 = 0x%08x\r\n", stacked_r3);
        printfS("R12 = 0x%08x\r\n", stacked_r12);
        printfS("Stacked LR = 0x%08x\r\n", stacked_lr);
        printfS("Stacked PC = 0x%08x\r\n", stacked_pc);
        printfS("Stacked PSR = 0x%08x\r\n", stacked_psr);
        printfS("SCB_SHCSR=0x%08x\r\n",SCB->SHCSR);
        printfS("Current LR = 0x%08x\r\n", lr_value); 
    }
    while(1);
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
	HAL_EXTI_IRQHandler(0);
}

void EXTI1_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(1);
}

void EXTI2_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(2);
}

void EXTI3_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(3);
}

void EXTI4_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(4);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(5);
	HAL_EXTI_IRQHandler(6);
	HAL_EXTI_IRQHandler(7);
	HAL_EXTI_IRQHandler(8);
	HAL_EXTI_IRQHandler(9);
}


void EXTI15_10_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(10);
	HAL_EXTI_IRQHandler(11);
	HAL_EXTI_IRQHandler(12);
	HAL_EXTI_IRQHandler(13);
	HAL_EXTI_IRQHandler(14);
	HAL_EXTI_IRQHandler(15);
}





