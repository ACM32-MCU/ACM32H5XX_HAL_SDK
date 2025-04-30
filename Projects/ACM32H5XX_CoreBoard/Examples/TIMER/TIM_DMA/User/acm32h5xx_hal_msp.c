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
} 
  
DMA_HandleTypeDef  Timer_DMA_Handle0;   
DMA_HandleTypeDef  Timer_DMA_Handle1;
DMA_HandleTypeDef  Timer_DMA_Handle2;
DMA_HandleTypeDef  Timer_DMA_Handle3;
DMA_HandleTypeDef  Timer_DMA_Handle4;
DMA_HandleTypeDef  Timer_DMA_Handle5; 
DMA_HandleTypeDef  Timer_DMA_Handle6; 
DMA_HandleTypeDef  Timer_DMA_Handle7; 


static void Timer_DMA_Config(TIM_HandleTypeDef * htim, uint32_t request_id, uint32_t dma_index, DMA_TypeDef *dmac)  
{   
    DMA_HandleTypeDef  * p_Timer_DMA_Handle; 
    
    if (DMA2 == dmac)
    {
        switch(dma_index)
        {
            case TIM_DMA_UPDATE_INDEX:  
            Timer_DMA_Handle0.Instance             = DMA2_Channel0;    
            p_Timer_DMA_Handle = &Timer_DMA_Handle0;  
            break;  
            
            case TIM_DMA_CC1_INDEX:  
            Timer_DMA_Handle1.Instance             = DMA2_Channel1;    
            p_Timer_DMA_Handle = &Timer_DMA_Handle1;  
            break;  
                        
            case TIM_DMA_CC2_INDEX:  
            Timer_DMA_Handle2.Instance             = DMA2_Channel2;    
            p_Timer_DMA_Handle = &Timer_DMA_Handle2;   
            break;  
            
            case TIM_DMA_CC3_INDEX:  
            Timer_DMA_Handle3.Instance             = DMA2_Channel3;   
            p_Timer_DMA_Handle = &Timer_DMA_Handle3;   
            break;  
                     
            case TIM_DMA_CC4_INDEX:  
            Timer_DMA_Handle4.Instance             = DMA2_Channel4;   
            p_Timer_DMA_Handle = &Timer_DMA_Handle4;    
            break;   
            
            case TIM_DMA_COM_INDEX:  
            Timer_DMA_Handle5.Instance             = DMA2_Channel5;  
            p_Timer_DMA_Handle = &Timer_DMA_Handle5;    
            break;  
                        
            case TIM_DMA_TRIG_INDEX:  
            Timer_DMA_Handle6.Instance             = DMA2_Channel6;   
            p_Timer_DMA_Handle = &Timer_DMA_Handle6;    
            break; 
            
            default:  
            Timer_DMA_Handle0.Instance             = DMA2_Channel7;   
            p_Timer_DMA_Handle = &Timer_DMA_Handle7;               
            break;  

        }
        
    }
    else
    {
        switch(dma_index)
        {
            case TIM_DMA_UPDATE_INDEX:  
            Timer_DMA_Handle0.Instance             = DMA1_Channel0;    
            p_Timer_DMA_Handle = &Timer_DMA_Handle0;   
            break;  
            
            case TIM_DMA_CC1_INDEX:  
            Timer_DMA_Handle1.Instance             = DMA1_Channel1;   
            p_Timer_DMA_Handle = &Timer_DMA_Handle1;  
            break;  
                        
            case TIM_DMA_CC2_INDEX:  
            Timer_DMA_Handle2.Instance             = DMA1_Channel2;      
            p_Timer_DMA_Handle = &Timer_DMA_Handle2;   
            break;  
            
            case TIM_DMA_CC3_INDEX:  
            Timer_DMA_Handle3.Instance             = DMA1_Channel3;      
            p_Timer_DMA_Handle = &Timer_DMA_Handle3;   
            break;  
                         
            case TIM_DMA_CC4_INDEX:  
            Timer_DMA_Handle4.Instance             = DMA1_Channel4; 
            p_Timer_DMA_Handle = &Timer_DMA_Handle4;     
            break;   
            
            case TIM_DMA_COM_INDEX:  
            Timer_DMA_Handle5.Instance             = DMA1_Channel5;    
            p_Timer_DMA_Handle = &Timer_DMA_Handle5;     
            break;  
                        
            case TIM_DMA_TRIG_INDEX:  
            Timer_DMA_Handle6.Instance             = DMA1_Channel6;      
            p_Timer_DMA_Handle = &Timer_DMA_Handle6;    
            break; 
            
            default:  
            Timer_DMA_Handle0.Instance             = DMA1_Channel7;     
            p_Timer_DMA_Handle = &Timer_DMA_Handle7;   
            break;  
        }
    }
    p_Timer_DMA_Handle->Init.DataFlow        = DMA_DATAFLOW_M2P;
    p_Timer_DMA_Handle->Init.ReqID           = request_id;   
    p_Timer_DMA_Handle->Init.SrcIncDec       = DMA_SRCINCDEC_INC;   
    p_Timer_DMA_Handle->Init.DestIncDec      = DMA_DESTINCDEC_DISABLE;   
    p_Timer_DMA_Handle->Init.SrcWidth        = DMA_SRCWIDTH_WORD;
    p_Timer_DMA_Handle->Init.DestWidth       = DMA_DESTWIDTH_WORD;   
    p_Timer_DMA_Handle->Init.SrcBurst        = DMA_SRCBURST_1;
    p_Timer_DMA_Handle->Init.DestBurst       = DMA_DESTBURST_1;  
    p_Timer_DMA_Handle->XferCpltCallback     = NULL;   
    
    HAL_DMA_Init(p_Timer_DMA_Handle);     
       
}  



/*********************************************************************************
* Function    : HAL_TIMER_MSP_Init
* Description : MSP init, mainly about clock, nvic   
* Input       : timer handler 
* Output      : 0: success; else:error    
* Author      : xwl                       
**********************************************************************************/
uint32_t HAL_TIMER_MSP_Init(TIM_HandleTypeDef * htim)  
{
    uint32_t Timer_Instance;
    
    
    Timer_Instance = (uint32_t)(htim->Instance); 
    
    switch(Timer_Instance) 
    {
        case TIM1_BASE_ADDR: 
					
		__HAL_RCC_TIM1_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM1_BRK_UP_TRG_COM_IRQn);     
        NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);  
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM1_UP, TIM_DMA_UPDATE_INDEX, DMA1);                    
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);    
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM1_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);     

        Timer_DMA_Config(htim, DMA1_REQ_TIM1_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);  

        Timer_DMA_Config(htim, DMA1_REQ_TIM1_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3);  

        Timer_DMA_Config(htim, DMA1_REQ_TIM1_CH4, TIM_DMA_CC4_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);     


        Timer_DMA_Config(htim, DMA1_REQ_TIM1_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM1_TRIG, TIM_DMA_TRIG_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6); 
        break; 
        
        case TIM2_BASE_ADDR: 
        
        __HAL_RCC_TIM2_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM2_IRQn);     
        NVIC_EnableIRQ(TIM2_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM2_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM2_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);       

        Timer_DMA_Config(htim, DMA1_REQ_TIM2_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);    

        Timer_DMA_Config(htim, DMA1_REQ_TIM2_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA1_REQ_TIM2_CH4, TIM_DMA_CC4_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);  

        
        Timer_DMA_Config(htim, DMA1_REQ_TIM2_TRIG, TIM_DMA_TRIG_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);           
        break; 

        case TIM3_BASE_ADDR: 
        
        __HAL_RCC_TIM3_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM3_IRQn);     
        NVIC_EnableIRQ(TIM3_IRQn);    
        Timer_DMA_Config(htim, DMA2_REQ_TIM3_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);  

        Timer_DMA_Config(htim, DMA2_REQ_TIM3_CH1, TIM_DMA_CC1_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);       

        Timer_DMA_Config(htim, DMA2_REQ_TIM3_CH2, TIM_DMA_CC2_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);    

        Timer_DMA_Config(htim, DMA2_REQ_TIM3_CH3, TIM_DMA_CC3_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA2_REQ_TIM3_CH4, TIM_DMA_CC4_INDEX, DMA2);           
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4); 

        Timer_DMA_Config(htim, DMA2_REQ_TIM3_TRIG, TIM_DMA_TRIG_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);          
        
        break;  
        
        case TIM4_BASE_ADDR: 
        __HAL_RCC_TIM4_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM4_IRQn);     
        NVIC_EnableIRQ(TIM4_IRQn);    
        Timer_DMA_Config(htim, DMA1_REQ_TIM4_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0); 

        Timer_DMA_Config(htim, DMA1_REQ_TIM4_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);       

        Timer_DMA_Config(htim, DMA1_REQ_TIM4_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);    

        Timer_DMA_Config(htim, DMA1_REQ_TIM4_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA1_REQ_TIM4_CH4, TIM_DMA_CC4_INDEX, DMA1);             
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);   
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM4_TRIG, TIM_DMA_TRIG_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);         
        break;
        
        case TIM5_BASE_ADDR: 
        __HAL_RCC_TIM5_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM5_IRQn);     
        NVIC_EnableIRQ(TIM5_IRQn);    
        Timer_DMA_Config(htim, DMA2_REQ_TIM5_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);  
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM5_CH1, TIM_DMA_CC1_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);       

        Timer_DMA_Config(htim, DMA2_REQ_TIM5_CH2, TIM_DMA_CC2_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);    

        Timer_DMA_Config(htim, DMA2_REQ_TIM5_CH3, TIM_DMA_CC3_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA2_REQ_TIM5_CH4, TIM_DMA_CC4_INDEX, DMA2);               
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);    

        Timer_DMA_Config(htim, DMA2_REQ_TIM5_COM, TIM_DMA_COM_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM5_TRIG, TIM_DMA_TRIG_INDEX, DMA2);             
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6); 
        
        break;
                
        case TIM6_BASE_ADDR:          
        __HAL_RCC_TIM6_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM6_IRQn);   
        NVIC_EnableIRQ(TIM6_IRQn);   
        Timer_DMA_Config(htim, DMA2_REQ_TIM6_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);     
        break; 
        
        case TIM7_BASE_ADDR:             
        __HAL_RCC_TIM7_CLK_ENABLE();  
        NVIC_ClearPendingIRQ(TIM7_IRQn);   
        NVIC_EnableIRQ(TIM7_IRQn);   
        Timer_DMA_Config(htim, DMA2_REQ_TIM7_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   
        break; 

        case TIM8_BASE_ADDR: 
        __HAL_RCC_TIM8_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM8_BRK_UP_TRG_COM_IRQn);   
        NVIC_EnableIRQ(TIM8_BRK_UP_TRG_COM_IRQn);  
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_CH1, TIM_DMA_CC1_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1); 
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_CH2, TIM_DMA_CC2_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2); 
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_CH3, TIM_DMA_CC3_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3);  
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_CH4, TIM_DMA_CC4_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4); 
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_COM, TIM_DMA_COM_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA2_REQ_TIM8_TRIG, TIM_DMA_TRIG_INDEX, DMA2);          
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);   
        break; 
        
        case TIM9_BASE_ADDR:   
        __HAL_RCC_TIM9_CLK_ENABLE();  
        NVIC_ClearPendingIRQ(TIM9_IRQn);   
        NVIC_EnableIRQ(TIM9_IRQn);   
        break; 
        
        case TIM10_BASE_ADDR:   
        __HAL_RCC_TIM10_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM10_IRQn);   
        NVIC_EnableIRQ(TIM10_IRQn);    
        break;  
        
        case TIM11_BASE_ADDR:   
        __HAL_RCC_TIM11_CLK_ENABLE();  
        NVIC_ClearPendingIRQ(TIM11_IRQn);   
        NVIC_EnableIRQ(TIM11_IRQn);     
        break; 
        
        case TIM12_BASE_ADDR:   
        __HAL_RCC_TIM12_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM12_IRQn);   
        NVIC_EnableIRQ(TIM12_IRQn);     
        break; 
        
        case TIM13_BASE_ADDR:   
        __HAL_RCC_TIM13_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM13_IRQn);   
        NVIC_EnableIRQ(TIM13_IRQn);     
        break; 
        
        case TIM14_BASE_ADDR:   
        __HAL_RCC_TIM14_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM14_IRQn);   
        NVIC_EnableIRQ(TIM14_IRQn);     
        break; 
        
        case TIM15_BASE_ADDR:                   
        __HAL_RCC_TIM15_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM15_IRQn);   
        NVIC_EnableIRQ(TIM15_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM15_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM15_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM15_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);  
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM15_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM15_TRIG, TIM_DMA_TRIG_INDEX, DMA1);                 
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);    
        break;
        
        case TIM16_BASE_ADDR:        
        __HAL_RCC_TIM16_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM16_IRQn);   
        NVIC_EnableIRQ(TIM16_IRQn); 
        Timer_DMA_Config(htim, DMA2_REQ_TIM16_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);  

        Timer_DMA_Config(htim, DMA2_REQ_TIM16_CH1, TIM_DMA_CC1_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);         
        break;
        
        case TIM17_BASE_ADDR:          
        __HAL_RCC_TIM17_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM17_IRQn);   
        NVIC_EnableIRQ(TIM17_IRQn);  
        Timer_DMA_Config(htim, DMA2_REQ_TIM17_UP, TIM_DMA_UPDATE_INDEX, DMA2);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);    
        
         Timer_DMA_Config(htim, DMA2_REQ_TIM17_CH1, TIM_DMA_CC1_INDEX, DMA2);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);  
        break;  
        
        case TIM18_BASE_ADDR:        
        __HAL_RCC_TIM18_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM18_IRQn);   
        NVIC_EnableIRQ(TIM18_IRQn); 
        Timer_DMA_Config(htim, DMA1_REQ_TIM18_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);      
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM18_CH1, TIM_DMA_CC1_INDEX, DMA1);            
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);   
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM18_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM18_TRIG, TIM_DMA_TRIG_INDEX, DMA1);              
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);   
        
        break;
        
        case TIM19_BASE_ADDR:          
        __HAL_RCC_TIM19_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM19_IRQn);   
        NVIC_EnableIRQ(TIM19_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM19_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM19_CH1, TIM_DMA_CC1_INDEX, DMA1);            
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);      

        Timer_DMA_Config(htim, DMA1_REQ_TIM19_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM19_TRIG, TIM_DMA_TRIG_INDEX, DMA1);              
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);           
        break;  
        
        case TIM20_BASE_ADDR:          
        __HAL_RCC_TIM20_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM20_BRK_UP_TRG_COM_IRQn);   
        NVIC_EnableIRQ(TIM20_BRK_UP_TRG_COM_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM20_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);    

        Timer_DMA_Config(htim, DMA1_REQ_TIM20_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);     

        Timer_DMA_Config(htim, DMA1_REQ_TIM20_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);  

        Timer_DMA_Config(htim, DMA1_REQ_TIM20_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3);  

        Timer_DMA_Config(htim, DMA1_REQ_TIM20_CH4, TIM_DMA_CC4_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM20_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM20_TRIG, TIM_DMA_TRIG_INDEX, DMA1);                 
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);           
        break; 
                
        case TIM21_BASE_ADDR:          
        __HAL_RCC_TIM21_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM21_IRQn);   
        NVIC_EnableIRQ(TIM21_IRQn); 
        Timer_DMA_Config(htim, DMA1_REQ_TIM21_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);                 
        break;  
                
        case TIM22_BASE_ADDR:          
        __HAL_RCC_TIM22_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM22_IRQn);   
        NVIC_EnableIRQ(TIM22_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM22_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);  
        break;  
        
        case TIM23_BASE_ADDR:          
        __HAL_RCC_TIM23_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM23_IRQn);   
        NVIC_EnableIRQ(TIM23_IRQn); 
        Timer_DMA_Config(htim, DMA1_REQ_TIM23_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);  
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM23_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM23_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM23_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA1_REQ_TIM23_CH4, TIM_DMA_CC4_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);     

        Timer_DMA_Config(htim, DMA1_REQ_TIM23_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM23_TRIG, TIM_DMA_TRIG_INDEX, DMA1);                 
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);               
        break;  
                
        case TIM24_BASE_ADDR:          
        __HAL_RCC_TIM24_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM24_IRQn);   
        NVIC_EnableIRQ(TIM24_IRQn);  
        Timer_DMA_Config(htim, DMA1_REQ_TIM24_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);     

        Timer_DMA_Config(htim, DMA1_REQ_TIM24_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM24_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM24_CH3, TIM_DMA_CC3_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC3_INDEX], Timer_DMA_Handle3); 

        Timer_DMA_Config(htim, DMA1_REQ_TIM24_CH4, TIM_DMA_CC4_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC4_INDEX], Timer_DMA_Handle4);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM24_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM24_TRIG, TIM_DMA_TRIG_INDEX, DMA1);              
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);              
        break;  
        
        case TIM25_BASE_ADDR:          
        __HAL_RCC_TIM25_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM25_IRQn);   
        NVIC_EnableIRQ(TIM25_IRQn); 
        Timer_DMA_Config(htim, DMA1_REQ_TIM25_UP, TIM_DMA_UPDATE_INDEX, DMA1);        
        __HAL_LINKDMA(htim, hdma[TIM_DMA_UPDATE_INDEX], Timer_DMA_Handle0);   
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM25_CH1, TIM_DMA_CC1_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC1_INDEX], Timer_DMA_Handle1);   

        Timer_DMA_Config(htim, DMA1_REQ_TIM25_CH2, TIM_DMA_CC2_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_CC2_INDEX], Timer_DMA_Handle2);   
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM25_COM, TIM_DMA_COM_INDEX, DMA1);         
        __HAL_LINKDMA(htim, hdma[TIM_DMA_COM_INDEX], Timer_DMA_Handle5); 
        
        Timer_DMA_Config(htim, DMA1_REQ_TIM25_TRIG, TIM_DMA_TRIG_INDEX, DMA1);              
        __HAL_LINKDMA(htim, hdma[TIM_DMA_TRIG_INDEX], Timer_DMA_Handle6);   
        break;  
                        
        default:
        return HAL_ERROR;  
    }
    
    return HAL_OK;    
}  


uint32_t HAL_TIMER_Base_MspDeInit(TIM_HandleTypeDef * htim)  
{
    uint32_t Timer_Instance;
    
    
    Timer_Instance = (uint32_t)(htim->Instance); 
    
    switch(Timer_Instance) 
    {
        case TIM1_BASE_ADDR: 
					
		__HAL_RCC_TIM1_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM1_BRK_UP_TRG_COM_IRQn);     
        NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);  
        break; 
        
        case TIM2_BASE_ADDR: 
        
        __HAL_RCC_TIM2_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM2_IRQn);     
        NVIC_DisableIRQ(TIM2_IRQn);    
        break; 

        case TIM3_BASE_ADDR: 
        
        __HAL_RCC_TIM3_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM3_IRQn);     
        NVIC_DisableIRQ(TIM3_IRQn);    
        break;  
        
        case TIM4_BASE_ADDR: 
        __HAL_RCC_TIM4_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM4_IRQn);     
        NVIC_DisableIRQ(TIM4_IRQn);    
        break;
        
        case TIM5_BASE_ADDR: 
        __HAL_RCC_TIM5_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM5_IRQn);     
        NVIC_DisableIRQ(TIM5_IRQn);     
        break;
                
        case TIM6_BASE_ADDR:          
        __HAL_RCC_TIM6_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM6_IRQn);   
        NVIC_DisableIRQ(TIM6_IRQn);   
        break; 
        
        case TIM7_BASE_ADDR:             
        __HAL_RCC_TIM7_CLK_DISABLE();  
        NVIC_ClearPendingIRQ(TIM7_IRQn);   
        NVIC_DisableIRQ(TIM7_IRQn);   
        break; 

        case TIM8_BASE_ADDR: 
        __HAL_RCC_TIM8_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM8_BRK_UP_TRG_COM_IRQn);   
        NVIC_DisableIRQ(TIM8_BRK_UP_TRG_COM_IRQn); 
        break; 
        
        case TIM9_BASE_ADDR:   
        __HAL_RCC_TIM9_CLK_DISABLE();  
        NVIC_ClearPendingIRQ(TIM9_IRQn);   
        NVIC_DisableIRQ(TIM9_IRQn);   
        break; 
        
        case TIM10_BASE_ADDR:   
        __HAL_RCC_TIM10_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM10_IRQn);   
        NVIC_DisableIRQ(TIM10_IRQn);    
        break;  
        
        case TIM11_BASE_ADDR:   
        __HAL_RCC_TIM11_CLK_DISABLE();  
        NVIC_ClearPendingIRQ(TIM11_IRQn);   
        NVIC_DisableIRQ(TIM11_IRQn);     
        break; 
        
        case TIM12_BASE_ADDR:   
        __HAL_RCC_TIM12_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM12_IRQn);   
        NVIC_DisableIRQ(TIM12_IRQn);     
        break; 
        
        case TIM13_BASE_ADDR:   
        __HAL_RCC_TIM13_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM13_IRQn);   
        NVIC_DisableIRQ(TIM13_IRQn);     
        break; 
        
        case TIM14_BASE_ADDR:   
        __HAL_RCC_TIM14_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM14_IRQn);   
        NVIC_DisableIRQ(TIM14_IRQn);     
        break; 
        
        case TIM15_BASE_ADDR:                   
        __HAL_RCC_TIM15_CLK_DISABLE();   
        NVIC_ClearPendingIRQ(TIM15_IRQn);   
        NVIC_DisableIRQ(TIM15_IRQn);  
        break;
        
        case TIM16_BASE_ADDR:        
        __HAL_RCC_TIM16_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM16_IRQn);   
        NVIC_DisableIRQ(TIM16_IRQn); 
        break;
        
        case TIM17_BASE_ADDR:          
        __HAL_RCC_TIM17_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM17_IRQn);   
        NVIC_DisableIRQ(TIM17_IRQn); 
        break;  
        
        case TIM18_BASE_ADDR:        
        __HAL_RCC_TIM18_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM18_IRQn);   
        NVIC_DisableIRQ(TIM18_IRQn); 
        break;
        
        case TIM19_BASE_ADDR:          
        __HAL_RCC_TIM19_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM19_IRQn);   
        NVIC_DisableIRQ(TIM19_IRQn);   
        break;  
        
        case TIM20_BASE_ADDR:          
        __HAL_RCC_TIM20_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM20_BRK_UP_TRG_COM_IRQn);   
        NVIC_DisableIRQ(TIM20_BRK_UP_TRG_COM_IRQn);    
        break; 
                
        case TIM21_BASE_ADDR:          
        __HAL_RCC_TIM21_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM21_IRQn);   
        NVIC_DisableIRQ(TIM21_IRQn); 
        break;  
                
        case TIM22_BASE_ADDR:          
        __HAL_RCC_TIM22_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM22_IRQn);   
        NVIC_DisableIRQ(TIM22_IRQn);  
        break;  
        
        case TIM23_BASE_ADDR:          
        __HAL_RCC_TIM23_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM23_IRQn);   
        NVIC_DisableIRQ(TIM23_IRQn); 
        break;  
                
        case TIM24_BASE_ADDR:          
        __HAL_RCC_TIM24_CLK_DISABLE();    
        NVIC_ClearPendingIRQ(TIM24_IRQn);   
        NVIC_DisableIRQ(TIM24_IRQn);  
        break;  
        
        case TIM25_BASE_ADDR:          
        __HAL_RCC_TIM25_CLK_DISABLE();        
        NVIC_ClearPendingIRQ(TIM25_IRQn);   
        NVIC_DisableIRQ(TIM25_IRQn); 
        break;                  
                        
        default:
        return HAL_ERROR;  
    }
    
    return HAL_OK;    
}  

