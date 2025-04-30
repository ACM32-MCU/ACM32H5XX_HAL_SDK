/******************************************************************************
*@file  : acm32h5x_hal_msp.c
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
        NVIC_SetPriority(USART1_IRQn, 5);
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (huart->Instance == USART2) 
    {
        /* Enable Clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /* B1:Tx  B0:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOB, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            /* A6:CTS */
            GPIO_Uart.Pin = GPIO_PIN_6;
            GPIO_Uart.Alternate = GPIO_FUNCTION_2;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            /* A7:RTS */
            GPIO_Uart.Pin = GPIO_PIN_7;
            GPIO_Uart.Alternate = GPIO_FUNCTION_2;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (huart->Instance == USART3) 
    {
        /* Enable Clock */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /* B1:Tx  B0:Rx */
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

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{    
    /* For Example */
    GPIO_InitTypeDef GPIO_Handle; 
    
    /* I2C1 */
    if (hi2c->Instance == I2C1)
    {
        /* Enable Clock */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();


        GPIO_Handle.Pin            = GPIO_PIN_6;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_8;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOG, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C1_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C1_IRQn);   
    }
    /* I2C2 */
    else if (hi2c->Instance == I2C2) 
    {
        /* Enable Clock */
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();

        /* I2C2 SDA  PF0 */
        /* I2C2 SCL  PF1 */
        GPIO_Handle.Pin            = GPIO_PIN_0;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOF, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_1;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL7;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOF, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C2_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C2_IRQn);                
    }
    else if (hi2c->Instance == I2C3) 
    {
        assert_param(0);        
    }
    else if (hi2c->Instance == I2C4) 
    {
        /* Enable Clock */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /* I2C1 SDA  PB7 */
        /* I2C1 SCL  PB6 */
        GPIO_Handle.Pin            = GPIO_PIN_8;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_13;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C4_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C4_IRQn);           
    }
}
