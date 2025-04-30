/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/

#include "app.h"


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
        NVIC_SetPriority(USART1_IRQn, 5);
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


void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{  
    GPIO_InitTypeDef GPIO_Handle = {0}; 
    
    /* OSPI1 */
    if (hospi->Instance == OSPI1)
    {
        /* Enable Clock */
        __HAL_RCC_OSPI1_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
       
        /****************************
        OSPI1_CS   : PC11-AF13
        OSPI1_NCLK : PC12-AF13
        OSPI1_CLK  : PC10-AF11 
        OSPI1_DQS  : PG9 -AF9 
        OSPI1_IO0  : PD0 -AF13
        OSPI1_IO1  : PD1 -AF13
        OSPI1_IO2  : PD2 -AF13
        OSPI1_IO3  : PD3 -AF13
        OSPI1_IO4  : PD4 -AF13
        OSPI1_IO5  : PD5 -AF13
        OSPI1_IO6  : PD6 -AF13
        OSPI1_IO7  : PD7 -AF13
        ****************************/
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;

        /* OSPI1_DQS-PG9-AF9 */ 
        GPIO_Handle.Alternate = GPIO_FUNCTION_9;
        GPIO_Handle.Pin = GPIO_PIN_9;        
        HAL_GPIO_Init(GPIOG, &GPIO_Handle);
        
        /* OSPI1_CLK-PC10-AF11 */
        GPIO_Handle.Alternate = GPIO_FUNCTION_11;
        GPIO_Handle.Pin = GPIO_PIN_10;      
        HAL_GPIO_Init(GPIOC, &GPIO_Handle);
        
        /* OSPI1_CS-PC11-AF13, OSPI1_NCLK-PC12-AF13 */        
        GPIO_Handle.Alternate = GPIO_FUNCTION_13;
        GPIO_Handle.Pin = GPIO_PIN_11;// | GPIO_PIN_12 ;
        HAL_GPIO_Init(GPIOC, &GPIO_Handle);
        
        /* OSPI1_IO0-PD0-AF13, OSPI1_IO1-PD1-AF13 */
        GPIO_Handle.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        
        if((hospi->Init.XMode == OSPI_4X_MODE) || (hospi->Init.XMode == OSPI_8X_MODE)) 
        {
            /* OSPI1_IO2-PD2-AF13, OSPI1_IO3-PD3-AF13 */
            GPIO_Handle.Pin = GPIO_PIN_2 | GPIO_PIN_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        }
        if(hospi->Init.XMode == OSPI_8X_MODE)
        {   
            /* OSPI1_IO4-PD4-AF13, OSPI1_IO5-PD5-AF13, OSPI1_IO6-PD6-AF13, OSPI1_IO7-PD7-AF13 */            
            GPIO_Handle.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        }       

        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI1_IRQn);
        NVIC_SetPriority(OSPI1_IRQn, 5);
        NVIC_EnableIRQ(OSPI1_IRQn);
    }
    /* OSPI2 */
    else if (hospi->Instance == OSPI2) 
    {
       /* Enable Clock */
        __HAL_RCC_OSPI2_CLK_ENABLE();
        __HAL_RCC_GPIOM_CLK_ENABLE();
        __HAL_RCC_GPIOP_CLK_ENABLE();
        __HAL_RCC_GPIOQ_CLK_ENABLE();
        
        /****************************
        OSPI2_CS   : PQ9 -AF12
        OSPI2_NCLK : PM15-AF12
        OSPI2_CLK  : PM14-AF12
        OSPI2_DQS  : PQ15-AF12
        OSPI2_IO0  : PP6 -AF12
        OSPI2_IO1  : PP7 -AF12
        OSPI2_IO2  : PP8 -AF12
        OSPI2_IO3  : PP9 -AF12
        OSPI2_IO4  : PQ10-AF12
        OSPI2_IO5  : PQ11-AF12
        OSPI2_IO6  : PQ12-AF12
        OSPI2_IO7  : PQ13-AF12
        ****************************/
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_12;
        
        /* OSPI2_NCLK-PM15-AF12, OSPI2_CLK-PM14-AF12 */
        GPIO_Handle.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOM, &GPIO_Handle);
        
        /* OSPI2_CS-PQ9-AF12, OSPI2_DQS-PQ15-AF12 */        
        GPIO_Handle.Pin = GPIO_PIN_9 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOQ, &GPIO_Handle);
        
        /* OSPI2_IO0-PP6-AF12, OSPI2_IO1-PP7-AF12 */        
        GPIO_Handle.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOP, &GPIO_Handle);

        if((hospi->Init.XMode == OSPI_4X_MODE) || (hospi->Init.XMode == OSPI_8X_MODE))
        {
            /* OSPI2_IO2-PP8-AF12, OSPI2_IO3-PP9-AF12 */
            GPIO_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9;
            HAL_GPIO_Init(GPIOP, &GPIO_Handle);
        }
        if(hospi->Init.XMode == OSPI_8X_MODE)
        { 
            /* OSPI2_IO4-PQ10-AF12, OSPI2_IO5-PQ11-AF12, OSPI2_IO6-PQ12-AF12, OSPI2_IO7-PQ13-AF12 */      
            GPIO_Handle.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
            HAL_GPIO_Init(GPIOQ, &GPIO_Handle);        
        }
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI2_IRQn);
        NVIC_SetPriority(OSPI2_IRQn, 5);
        NVIC_EnableIRQ(OSPI2_IRQn);
    }
}