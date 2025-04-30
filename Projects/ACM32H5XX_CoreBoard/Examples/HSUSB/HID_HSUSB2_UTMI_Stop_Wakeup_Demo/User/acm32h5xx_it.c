/******************************************************************************
*@file  : acm32h5xx_it.c
*@brief : Main Interrupt Service Routines
*           This file provides template for all exceptions handler and 
*           peripherals interrupt service routine.  
******************************************************************************/

#include "acm32h5xx_it.h"
#include "app.h"
#include "usbd_def.h"   


extern USBD_HandleTypeDef USBD_Device;   

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

void USBOTG1_IRQHandler(void)  
{
  HAL_PCD_IRQHandler(USBD_Device.pData);
}

void USBOTG2_IRQHandler(void)    
{
  HAL_PCD_IRQHandler(USBD_Device.pData);   
}   

/**
  * @brief  This function handles USB OTG FS/HS Wakeup IRQ Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS  
void OTG_FS_WKUP_IRQHandler(void)
#else
void OTG_HS_WKUP_IRQHandler(void)
#endif
{

}







 



