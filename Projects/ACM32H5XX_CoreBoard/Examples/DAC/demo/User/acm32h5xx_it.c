
/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "acm32h5xx_it.h"
extern TIM_HandleTypeDef TIM6_Handler; 
#ifdef HAL_DMA_MODULE_ENABLED
extern DMA_HandleTypeDef hdma_dac1_ch1, hdma_dac1_ch2;
#endif

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
	HAL_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_EXTI_IRQHandler(GPIO_PIN_6);
	HAL_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_EXTI_IRQHandler(GPIO_PIN_9);
}


void EXTI15_10_IRQHandler(void)
{
	HAL_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_EXTI_IRQHandler(GPIO_PIN_15);
}

void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);   	
}

#ifdef HAL_DMA_MODULE_ENABLED
void DMA1_CH0_IRQHandler(void)
{
//    assert_param(0);
    HAL_DMA_IRQHandler(&hdma_dac1_ch1);
    HAL_DMA_IRQHandler(&hdma_dac1_ch2);
}
#endif

void DAC1_IRQHandler(void)
{
//    HAL_DMA_Abort(&hdma_dac1_ch1);
//    HAL_TIMER_Base_Stop(TIM6); 
    HAL_DAC_IRQHandler(&hdac1);
}

void DAC2_IRQHandler(void)
{
//    HAL_DMA_Abort(&hdma_dac1_ch1);
//    HAL_TIMER_Base_Stop(TIM6); 
    HAL_DAC_IRQHandler(&hdac1);
}
