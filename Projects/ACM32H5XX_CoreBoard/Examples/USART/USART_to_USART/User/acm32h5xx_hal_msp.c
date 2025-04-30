/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/
#include "hal.h"



void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
    GPIO_InitTypeDef    GPIO_Usart = {0};
    /* 
      NOTE: This function can be modified by user.
    */
    if(husart->Instance == USART1)
    {
        /* Enable Clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* A9:Tx  A10:Rx */
        GPIO_Usart.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Usart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Usart.Pull      = GPIO_NOPULL;
        GPIO_Usart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Usart.Alternate = GPIO_FUNCTION_1;

        HAL_GPIO_Init(GPIOA, &GPIO_Usart);
        
        /* PA8 USART1 CK */
        GPIO_Usart.Pin       = GPIO_PIN_8;
        GPIO_Usart.Alternate = GPIO_FUNCTION_6;
        HAL_GPIO_Init(GPIOA, &GPIO_Usart);

        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, 5);
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if(husart->Instance == USART2)
    {
        /* Enable Clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
       
        GPIO_Usart.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_Usart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Usart.Pull      = GPIO_NOPULL;
        GPIO_Usart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Usart.Alternate = GPIO_FUNCTION_3;
        HAL_GPIO_Init(GPIOD, &GPIO_Usart);
        
        /* CK */
        GPIO_Usart.Pin       = GPIO_PIN_7;
        GPIO_Usart.Alternate = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOD, &GPIO_Usart);

        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* 
      NOTE: This function can be modified by user.
    */

    GPIO_InitTypeDef    GPIO_Uart = {0};
    
    {
        /* Enable Clock */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        GPIO_Uart.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_3;
        
        HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            assert_param(0);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            assert_param(0);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART3_IRQn);
        NVIC_SetPriority(USART3_IRQn, 5);
        NVIC_EnableIRQ(USART3_IRQn);
    }
}

