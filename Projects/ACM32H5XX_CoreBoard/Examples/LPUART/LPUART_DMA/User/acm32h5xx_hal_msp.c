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
//tx:PA9_6//PB6_10
//rx:PA10_6//PB7_10
void HAL_LPUART_MspInit(LPUART_HandleTypeDef *hlpuart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_LPUART1_CLK_ENABLE();
	
#if 0
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_6;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
#else
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_10;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* Configure the DMA handler for Transmission process */
    DMA1CH0_Tx.Instance          = DMA1_Channel0;
    DMA1CH0_Tx.Init.Mode         = DMA_MODE_NORMAL;//DMA_MODE_NORMAL;
    DMA1CH0_Tx.Init.ReqID        = DMA1_REQ_LPUART_TX;
    DMA1CH0_Tx.Init.DataFlow     = DMA_DATAFLOW_M2P;
    DMA1CH0_Tx.Init.SrcIncDec       = DMA_SRCINCDEC_INC;
    DMA1CH0_Tx.Init.DestIncDec      = DMA_DESTINCDEC_DISABLE;
    DMA1CH0_Tx.Init.SrcWidth     = DMA_SRCWIDTH_BYTE;
    DMA1CH0_Tx.Init.DestWidth    = DMA_DESTWIDTH_BYTE;
    DMA1CH0_Tx.Init.SrcBurst     = DMA_SRCBURST_1;
    DMA1CH0_Tx.Init.DestBurst    = DMA_DESTBURST_1;

    HAL_DMA_Init(&DMA1CH0_Tx);

    /* Associate the initialized DMA handle to the UART handle */
    __HAL_LINKDMA(hlpuart, hdmatx, DMA1CH0_Tx);
	
    __HAL_RCC_DMA2_CLK_ENABLE();
    /* Configure the DMA handler for reception process */
    DMA2CH0_Rx.Instance          = DMA2_Channel0;
    DMA2CH0_Rx.Init.Mode         = DMA_MODE_CIRCULAR;
    DMA2CH0_Rx.Init.ReqID        = DMA2_REQ_LPUART_RX;
    DMA2CH0_Rx.Init.DataFlow     = DMA_DATAFLOW_P2M;
    DMA2CH0_Rx.Init.SrcIncDec       = DMA_SRCINCDEC_DISABLE;
    DMA2CH0_Rx.Init.DestIncDec      = DMA_DESTINCDEC_INC;
    DMA2CH0_Rx.Init.SrcWidth     = DMA_SRCWIDTH_BYTE;
    DMA2CH0_Rx.Init.DestWidth    = DMA_DESTWIDTH_BYTE;
    DMA2CH0_Rx.Init.SrcBurst     = DMA_SRCBURST_1;
    DMA2CH0_Rx.Init.DestBurst    = DMA_DESTBURST_1;

    HAL_DMA_Init(&DMA2CH0_Rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(hlpuart, hdmarx, DMA2CH0_Rx);

  
    NVIC_ClearPendingIRQ(LPUART1_IRQn);
    NVIC_SetPriority(LPUART1_IRQn, 0x00);
    NVIC_EnableIRQ(LPUART1_IRQn);
    
    NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
    NVIC_SetPriority(DMA1_CH0_IRQn, 0x00);
    NVIC_EnableIRQ(DMA1_CH0_IRQn);
    
    NVIC_ClearPendingIRQ(DMA2_CH0_IRQn);
    NVIC_SetPriority(DMA2_CH0_IRQn, 0x00);
    NVIC_EnableIRQ(DMA2_CH0_IRQn);
}
