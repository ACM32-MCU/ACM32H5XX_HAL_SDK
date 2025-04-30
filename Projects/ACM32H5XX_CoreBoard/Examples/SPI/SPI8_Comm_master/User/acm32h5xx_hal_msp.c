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
            GPIO_Uart.Pin = GPIO_PIN_6;
            GPIO_Uart.Alternate = GPIO_FUNCTION_2;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
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

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    /* 
      NOTE : This function should be modified by user.
    */
    
    GPIO_InitTypeDef GPIO_Handle = {0}; 
    
    /* SPI1 */
    if (hspi->Instance == SPI1)
    {
        /* Enable Clock */
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        
        GPIO_Handle.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_0;
        HAL_GPIO_Init(GPIOA, &GPIO_Handle);
        
                
        /* NVIC Config */
        NVIC_ClearPendingIRQ(SPI1_IRQn);
        NVIC_SetPriority(SPI1_IRQn, 5);
        NVIC_EnableIRQ(SPI1_IRQn);
    }
    /* SPI2 */
    else if (hspi->Instance == SPI2) 
    {
       /* Enable Clock */
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_Handle.Pin = GPIO_PIN_15;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_3;
        HAL_GPIO_Init(GPIOA, &GPIO_Handle);
        
        GPIO_Handle.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_3;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);

        if (hspi->Init.X_Mode == SPI_4X_MODE) 
        {
            GPIO_Handle.Pin = GPIO_PIN_6 | GPIO_PIN_7;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_3;
            HAL_GPIO_Init(GPIOB, &GPIO_Handle);

        }
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(SPI2_IRQn);
        NVIC_SetPriority(SPI2_IRQn, 5);
        NVIC_EnableIRQ(SPI2_IRQn);
    }
    /* SPI3 */
    else if (hspi->Instance == SPI3)
    {
        __HAL_RCC_SPI3_RESET();

        /* Enable Clock */
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();
   
        GPIO_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_3;
        HAL_GPIO_Init(GPIOI, &GPIO_Handle);
        
        GPIO_Handle.Pin = /*GPIO_PIN_10 | GPIO_PIN_11 |*/ GPIO_PIN_12;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_2;
        HAL_GPIO_Init(GPIOC, &GPIO_Handle);

        if (hspi->Init.X_Mode == SPI_4X_MODE) 
        {
            GPIO_Handle.Pin = GPIO_PIN_15;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_5;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);
            
            GPIO_Handle.Pin = GPIO_PIN_4;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_5;
            HAL_GPIO_Init(GPIOE, &GPIO_Handle);
        }
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI3_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI3_IRQn);
    }
    /* SPI4 */
    else if (hspi->Instance == SPI4)
    {
        __HAL_RCC_SPI4_RESET();
        
        __HAL_RCC_SPI4_CLK_ENABLE();
        __HAL_RCC_GPIOK_CLK_ENABLE();
        
        GPIO_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_4;
        HAL_GPIO_Init(GPIOK, &GPIO_Handle);
        
        if (hspi->Init.X_Mode == SPI_4X_MODE) 
        {
            GPIO_Handle.Pin = GPIO_PIN_12 | GPIO_PIN_13;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_4;
            HAL_GPIO_Init(GPIOK, &GPIO_Handle);
        }
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI4_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI4_IRQn);
    }
    else if (hspi->Instance == SPI7)
    {
        /*
        CS  PM1
        CLK PL2
        IO0 PL1
        IO1 PM2
        
        IO2 PM3
        IO3 PL3
        */
        __HAL_RCC_SPI7_RESET();
        
        __HAL_RCC_SPI7_CLK_ENABLE();
        __HAL_RCC_GPIOM_CLK_ENABLE();
        __HAL_RCC_GPIOL_CLK_ENABLE();
        
        GPIO_Handle.Pin = GPIO_PIN_1 | GPIO_PIN_2;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_15;
        HAL_GPIO_Init(GPIOM, &GPIO_Handle);
        
        GPIO_Handle.Pin = GPIO_PIN_1 | GPIO_PIN_2;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_15;
        HAL_GPIO_Init(GPIOL, &GPIO_Handle);
        
        
        
        if (hspi->Init.X_Mode == SPI_4X_MODE) 
        {
            GPIO_Handle.Pin = GPIO_PIN_3;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_15;
            HAL_GPIO_Init(GPIOM, &GPIO_Handle);
            
            GPIO_Handle.Pin = GPIO_PIN_3;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_15;
            HAL_GPIO_Init(GPIOL, &GPIO_Handle);

        }
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI4_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI4_IRQn);
    }
    else if (hspi->Instance == SPI8)
    {
        /*
        CS  PM1
        CLK PL2
        IO0 PL1
        IO1 PM2
        
        IO2 PM3
        IO3 PL3
        */
        __HAL_RCC_SPI8_RESET();
        
        __HAL_RCC_SPI8_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        
        GPIO_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_11;
        HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        
        GPIO_Handle.Pin = GPIO_PIN_0;
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_11;
        HAL_GPIO_Init(GPIOE, &GPIO_Handle);

        if (hspi->Init.X_Mode == SPI_4X_MODE) 
        {
            GPIO_Handle.Pin = GPIO_PIN_14 | GPIO_PIN_15;
            GPIO_Handle.Mode = GPIO_MODE_AF_PP;
            GPIO_Handle.Pull = GPIO_PULLUP;
            GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
            GPIO_Handle.Alternate = GPIO_FUNCTION_11;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);

        }
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(SPI8_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(SPI8_IRQn);
    }
    
    
    printfS("Modify HAL_SPI_MspInit() if you select other SPI PINs\r\n\r\n");
}
