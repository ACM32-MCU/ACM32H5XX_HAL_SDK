/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/
#include "hal.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* 
      NOTE: This function can be modified by user.
    */

    GPIO_InitTypeDef    GPIO_Uart = {0};
    
    if (huart->Instance == USART1) 
    {
        /* Enable Clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        GPIO_Uart.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_11;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_12;   
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, 5);
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (huart->Instance == USART2) 
    {
        /* Enable Clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        
        GPIO_Uart.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_3;
        
        HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_3;
            GPIO_Uart.Alternate = GPIO_FUNCTION_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_4;
            GPIO_Uart.Alternate = GPIO_FUNCTION_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
#if 1    
    else if (huart->Instance == USART3) 
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
#else
    else if (huart->Instance == USART3) 
    {
        /* Enable Clock */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_Uart.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOB, &GPIO_Uart);
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART3_IRQn);
        NVIC_SetPriority(USART3_IRQn, 5);
        NVIC_EnableIRQ(USART3_IRQn);
    }
#endif
    
}
    
//void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
//{
//    GPIO_InitTypeDef    gpio = {0};
//    
//    if(hfdcan->Instance == FDCAN1)
//    {
//        __HAL_RCC_FDCAN1_RESET();        
//        
//        __HAL_RCC_FDCAN1_CLK_ENABLE();
//        __HAL_RCC_GPIOD_CLK_ENABLE();
//        __HAL_RCC_GPIOB_CLK_ENABLE();
//        
//        //1 tx, 0 rx
//        gpio.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
//        gpio.Mode      = GPIO_MODE_AF_PP;
//        gpio.Pull      = GPIO_PULLUP;
//        gpio.Drive     = GPIO_DRIVE_LEVEL3;
//        gpio.Alternate = GPIO_FUNCTION_4;
//        
//        HAL_GPIO_Init(GPIOD, &gpio);
//        
//        
//        gpio.Pin       = GPIO_PIN_10;
//        gpio.Mode      = GPIO_MODE_AF_PP;
//        gpio.Pull      = GPIO_PULLUP;
//        gpio.Drive     = GPIO_DRIVE_LEVEL3;
//        gpio.Alternate = GPIO_FUNCTION_6;        
//        HAL_GPIO_Init(GPIOB, &gpio);
//        
//        NVIC_ClearPendingIRQ(FDCAN1_IRQn);
//        NVIC_SetPriority(FDCAN1_IRQn, 5);
//        NVIC_EnableIRQ(FDCAN1_IRQn);
//    }
//}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
    GPIO_InitTypeDef    gpio = {0};
    
    if(hfdcan->Instance == FDCAN1)
    {
        __HAL_RCC_FDCAN1_RESET();        
        
        __HAL_RCC_FDCAN1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        //TX:PA12, RX:PA11
        gpio.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
        gpio.Mode      = GPIO_MODE_AF_PP;
        gpio.Pull      = GPIO_PULLUP;
        gpio.Drive     = GPIO_DRIVE_LEVEL3;
        gpio.Alternate = GPIO_FUNCTION_4;
        
        HAL_GPIO_Init(GPIOA, &gpio);        
        
        NVIC_ClearPendingIRQ(FDCAN1_IRQn);
        NVIC_SetPriority(FDCAN1_IRQn, 5);
        NVIC_EnableIRQ(FDCAN1_IRQn);
    }
    else if(hfdcan->Instance == FDCAN2)
    {
        __HAL_RCC_FDCAN2_RESET();        
        
        __HAL_RCC_FDCAN2_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();
        
        
        //7 tx, 6 rx
        gpio.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        gpio.Mode      = GPIO_MODE_AF_PP;
        gpio.Pull      = GPIO_PULLUP;
        gpio.Drive     = GPIO_DRIVE_LEVEL3;
        gpio.Alternate = GPIO_FUNCTION_7;
        
        HAL_GPIO_Init(GPIOI, &gpio);
        
        NVIC_ClearPendingIRQ(FDCAN2_IRQn);
        NVIC_SetPriority(FDCAN2_IRQn, 5);
        NVIC_EnableIRQ(FDCAN2_IRQn);
    }
    else if(hfdcan->Instance == FDCAN3)
    {
        __HAL_RCC_FDCAN3_RESET();        
        
        __HAL_RCC_FDCAN3_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        
        
        //TX:PI3, RX:PI2
        gpio.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
        gpio.Mode      = GPIO_MODE_AF_PP;
        gpio.Pull      = GPIO_PULLUP;
        gpio.Drive     = GPIO_DRIVE_LEVEL3;
        gpio.Alternate = GPIO_FUNCTION_5;
        
        HAL_GPIO_Init(GPIOF, &gpio);
        
        NVIC_ClearPendingIRQ(FDCAN3_IRQn);
        NVIC_SetPriority(FDCAN3_IRQn, 5);
        NVIC_EnableIRQ(FDCAN3_IRQn);
    }
}
