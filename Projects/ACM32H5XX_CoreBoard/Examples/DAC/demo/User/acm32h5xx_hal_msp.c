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

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    GPIO_InitTypeDef GPIO_InitStructure;   	
    
    //printfS("DAC MspInit not implemented\r\n");
    if(hdac->Instance== DAC1)
    { 
        /* Enable DAC clock */
        __HAL_RCC_DAC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        
        /* Initialization GPIO */
        /**DAC1 GPIO Configuration    
        PA4  ------> DAC_OUT1 
        PA5  ------> DAC_OUT2 
        */
        
        GPIO_InitStructure.Pin = GPIO_PIN_4|GPIO_PIN_5;	
        GPIO_InitStructure.Pull=GPIO_NOPULL;
        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG_SWITCH_OFF;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIOA->AIEN &= ~((1 << 4) | (1 << 5));  //关闭模拟开关
        
        printfS("DAC Channel-1:PA4\r\n");
        printfS("DAC Channel-2:PA5\r\n");
        
        /* Enable the DAC DMA underrun interrupt */
        hdac->Instance->CR |= DAC_CR_DMAUDIE1 | DAC_CR_DMAUDIE2;
        NVIC_ClearPendingIRQ(DAC1_IRQn);
        NVIC_SetPriority(DAC1_IRQn, 5);
        NVIC_EnableIRQ(DAC1_IRQn);
    }
    else if(hdac->Instance== DAC2)
    { 
        /* Enable DAC clock */
        __HAL_RCC_DAC2_CLK_ENABLE();
        __HAL_RCC_GPIOH_CLK_ENABLE();
        
        GPIO_InitStructure.Pin = GPIO_PIN_4|GPIO_PIN_5;	
        GPIO_InitStructure.Pull=GPIO_NOPULL;
        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG_SWITCH_OFF;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
        
        GPIOH->AIEN &= ~((1 << 4) | (1 << 5));  //关闭模拟开关
        
        printfS("DAC Channel-1:PH4\r\n");
        printfS("DAC Channel-2:PH5\r\n");

        /* Enable the DAC DMA underrun interrupt */
        hdac->Instance->CR |= DAC_CR_DMAUDIE1 | DAC_CR_DMAUDIE2;
        NVIC_ClearPendingIRQ(DAC2_IRQn);
        NVIC_SetPriority(DAC2_IRQn, 5);
        NVIC_EnableIRQ(DAC2_IRQn);
    }
}

