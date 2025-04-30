/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/

#include "hal.h"  

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* 
      NOTE: This function should be modified, when the callback is needed,
      the HAL_UART_MspInit can be implemented in the user file.
    */

    /* For Example */
    GPIO_InitTypeDef    GPIO_Uart = {0};
    
    if (huart->Instance == USART1) 
    {
        /* Enable Clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* A9:Tx  A10:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            /* A11:CTS */
            GPIO_Uart.Pin = GPIO_PIN_11;
            
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            /* A12:RTS */
            GPIO_Uart.Pin = GPIO_PIN_12;
            
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, 1);
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (huart->Instance == USART2) 
    {
        /* Enable Clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* A2:Tx  A3:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
}


void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
    
    GPIO_InitTypeDef GPIO_Handle; 
    
    //Enable COMP clock
    __HAL_RCC_CMP1_CLK_ENABLE();
   
    //VINP0:PB0, VINM2:PC4, VOUT:PE12(AF15)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_Handle.Pin     = GPIO_PIN_0;
    GPIO_Handle.Mode    = GPIO_MODE_ANALOG_SWITCH_ON;
    GPIO_Handle.Pull    = GPIO_NOPULL;
    GPIO_Handle.Drive   = GPIO_DRIVE_LEVEL3;        
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_Handle.Pin     = GPIO_PIN_4;
    GPIO_Handle.Mode    = GPIO_MODE_ANALOG_SWITCH_ON;
    GPIO_Handle.Pull    = GPIO_NOPULL;
    GPIO_Handle.Drive   = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(GPIOC, &GPIO_Handle);
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_Handle.Pin         = GPIO_PIN_12;
    GPIO_Handle.Mode        = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull        = GPIO_NOPULL;        
    GPIO_Handle.Drive       = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate   = GPIO_FUNCTION_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);

}
/******************************************************************************
*@brief : COMP MSP De-Initialization.This function freeze the hardware resources used in this example. 
*@param : hcomp: COMP handle
*@return: None
******************************************************************************/
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
    //VINP0:PB0, VINM2:PC4, VOUT:PE12(AF15)
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_12);
}