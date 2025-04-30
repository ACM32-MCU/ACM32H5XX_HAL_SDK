/*
  ******************************************************************************
  * Copyright (c)  2008 - 2024, Shanghai AisinoChip Co.,Ltd .
  * @file    APP.c 
  * @version V1.0.0
  * @date    2022
  * @author  Aisinochip Firmware Team  
  * @brief   DMA demo source code.
  ******************************************************************************        
*/
 
#include  "app.h"

extern UART_HandleTypeDef  BSP_UART_Handle; 

extern DMA_LinkTypeDef DMA_List_Node[2];  

extern uint8_t gu8_Tx1_DMA_Buffer[512]; 
extern uint8_t gu8_Tx2_DMA_Buffer[512]; 
extern volatile uint32_t dma_complete_times;

volatile uint32_t gu32_DMA_Status = false;   

DMA_HandleTypeDef  DMA_CH0_Handle;  

DMA_Two_Buffer_TypeDef  dma_two_buffer_info;  
/*********************************************************************************
* Function    : DMA_IRQHandler
* Description : DMA Interrupt handler
* Input       : 
* Outpu       : 
* Author      : xwl                         Date : 2024Äê
**********************************************************************************/
void DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&DMA_CH0_Handle);
}


/*********************************************************************************
* Function    : UART1_DMA_ITC_Callback
* Description : callback function of DMA complete event 
* Input       : 
* Outpu       : 
* Author      : xwl                        Date : 2024Äê  
**********************************************************************************/
void UART1_DMA_ITC_Callback(DMA_HandleTypeDef *Handler)
{
    gu32_DMA_Status = true;
    
    ((UART_HandleTypeDef *)(Handler->Parent))->TxBusy = 0;  
    
    dma_complete_times++; 
    
#ifdef DMA_TWO_BUFFER_MODE      
    if (dma_complete_times%2)   
    {
        gu8_Tx1_DMA_Buffer[510] += 1;  // Second buffer transfer finished, plus 1 to change sending data  
    }
    else
    {
        gu8_Tx2_DMA_Buffer[510] += 1;   // Second buffer transfer finished, plus 1 to change sending data  
    }
#endif 
}

void UART_TX_DMA_Init(void)
{   
    DMA_CH0_Handle.Instance             = DMA1_Channel0;
    DMA_CH0_Handle.Init.DataFlow        = DMA_DATAFLOW_M2P; 
    DMA_CH0_Handle.Init.ReqID           = DMA1_REQ_USART1_TX; 
    DMA_CH0_Handle.Init.SrcIncDec       = DMA_SRCINCDEC_INC;
    DMA_CH0_Handle.Init.DestIncDec      = DMA_DESTINCDEC_DISABLE;  
    DMA_CH0_Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
    DMA_CH0_Handle.Init.DestWidth       = DMA_DESTWIDTH_BYTE;
            
    DMA_CH0_Handle.XferCpltCallback = UART1_DMA_ITC_Callback;
            
    HAL_DMA_Init(&DMA_CH0_Handle);
            
    __HAL_LINKDMA(&BSP_UART_Handle, HDMA_Tx, DMA_CH0_Handle);  
}

// Send UART Data by DMA 
void UART_Send_By_DMA(void)
{
#ifndef DMA_TWO_BUFFER_MODE  
    uint32_t len;
    len = strlen("Send UART DATA By DMA,Buffer111...\r\n"); 
    
    memcpy(gu8_Tx1_DMA_Buffer, "Send UART DATA By DMA,Buffer111...\r\n", len);   
    memcpy(gu8_Tx2_DMA_Buffer, "Send UART DATA By DMA,Buffer222...\r\n", len);    
    gu32_DMA_Status = false;
    HAL_UART_Transmit_DMA(&BSP_UART_Handle, gu8_Tx1_DMA_Buffer, len );     
    while(false == gu32_DMA_Status); 
    
    gu32_DMA_Status = false; 
    HAL_UART_Transmit_DMA(&BSP_UART_Handle, gu8_Tx2_DMA_Buffer, len );       
    while(false == gu32_DMA_Status); 
#else    
    dma_complete_times = 0; 
    memcpy(gu8_Tx1_DMA_Buffer, "Buffer1111111111", 16);   // Buffer1 flag 
    gu8_Tx1_DMA_Buffer[510] = 1;
    gu8_Tx1_DMA_Buffer[511] = '\n';
    dma_two_buffer_info.SrcAddr1 = (uint32_t)gu8_Tx1_DMA_Buffer;
    dma_two_buffer_info.DstAddr1 = (uint32_t)&BSP_UART_Handle.Instance->DR;  
    dma_two_buffer_info.size1 = 512; 
    
    memcpy(gu8_Tx2_DMA_Buffer, "Buffer2222222222", 16);   // Buffer2 flag   
    gu8_Tx2_DMA_Buffer[510] = 5;  
    gu8_Tx2_DMA_Buffer[511] = '\n';   
    dma_two_buffer_info.SrcAddr2 = (uint32_t)gu8_Tx2_DMA_Buffer;  
    dma_two_buffer_info.DstAddr2 = (uint32_t)&BSP_UART_Handle.Instance->DR;  
    dma_two_buffer_info.size2 = 512; 
    
    dma_two_buffer_info.running_always = 1;   
    
    BSP_UART_Handle.Instance->CR1 |= UART_CR1_TXDMAE;  
       
    HAL_DMA_TwoBuffer_Start_IT(BSP_UART_Handle.HDMA_Tx, DMA_List_Node, &dma_two_buffer_info);  
    
    gu32_DMA_Status = false;   
    
#endif
}

