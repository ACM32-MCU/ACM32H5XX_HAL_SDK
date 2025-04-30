/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/

#include "acm32h5xx_it.h"
#include "app.h"

extern TIM_HandleTypeDef TIM_ETR_Handler;  

extern void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim); 

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





void TIM1_BRK_UP_TRG_COM_IRQHandler(void) 
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}

void TIM2_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);     
}

void TIM3_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM4_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM5_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM6_IRQHandler(void)
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}

void TIM7_IRQHandler(void)
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}  

void TIM8_BRK_UP_TRG_COM_IRQHandler(void)   
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}  

void TIM9_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM10_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM11_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM12_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM13_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM14_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}  

void TIM15_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM16_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM17_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM18_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}

void TIM19_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}  

void TIM20_BRK_UP_TRG_COM_IRQHandler(void)    
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);         
}

void TIM21_IRQHandler(void)
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}

void TIM22_IRQHandler(void)  
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}

void TIM23_IRQHandler(void)
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);    
}

void TIM24_IRQHandler(void)  
{	
	HAL_TIM_IRQHandler(&TIM_ETR_Handler);      
}  

void TIM25_IRQHandler(void)  
{
    HAL_TIM_IRQHandler(&TIM_ETR_Handler);         
}  

