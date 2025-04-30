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
//        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_DisableIRQ(USART1_IRQn);    
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


uint32_t HAL_TIMER_MSP_Init(TIM_HandleTypeDef * htim)  
{
    uint32_t Timer_Instance;
    
    
    Timer_Instance = (uint32_t)(htim->Instance); 
    
    switch(Timer_Instance) 
    {
        case TIM1_BASE_ADDR: 
					
		__HAL_RCC_TIM1_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM1_CC_IRQn);     
        NVIC_EnableIRQ(TIM1_CC_IRQn);  
        break; 
        
        case TIM2_BASE_ADDR: 
        
        __HAL_RCC_TIM2_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM2_IRQn);     
        NVIC_EnableIRQ(TIM2_IRQn);    
        break; 

        case TIM3_BASE_ADDR: 
        
        __HAL_RCC_TIM3_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM3_IRQn);     
        NVIC_EnableIRQ(TIM3_IRQn);    
        break;  
        
        case TIM4_BASE_ADDR: 
        __HAL_RCC_TIM4_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM4_IRQn);     
        NVIC_EnableIRQ(TIM4_IRQn);    
        break;
        
        case TIM5_BASE_ADDR: 
        __HAL_RCC_TIM5_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM5_IRQn);     
        NVIC_EnableIRQ(TIM5_IRQn);     
        break;
                
        case TIM6_BASE_ADDR:          
        __HAL_RCC_TIM6_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM6_IRQn);   
        NVIC_EnableIRQ(TIM6_IRQn);   
        break; 
        
        case TIM7_BASE_ADDR:             
        __HAL_RCC_TIM7_CLK_ENABLE();  
        NVIC_ClearPendingIRQ(TIM7_IRQn);   
        NVIC_EnableIRQ(TIM7_IRQn);   
        break; 

        case TIM8_BASE_ADDR: 
        __HAL_RCC_TIM8_CLK_ENABLE();   
        NVIC_ClearPendingIRQ(TIM8_CC_IRQn);   
        NVIC_EnableIRQ(TIM8_CC_IRQn); 
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
        break;
        
        case TIM16_BASE_ADDR:        
        __HAL_RCC_TIM16_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM16_IRQn);   
        NVIC_EnableIRQ(TIM16_IRQn); 
        break;
        
        case TIM17_BASE_ADDR:          
        __HAL_RCC_TIM17_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM17_IRQn);   
        NVIC_EnableIRQ(TIM17_IRQn); 
        break;  
        
        case TIM18_BASE_ADDR:        
        __HAL_RCC_TIM18_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM18_IRQn);   
        NVIC_EnableIRQ(TIM18_IRQn); 
        break;
        
        case TIM19_BASE_ADDR:          
        __HAL_RCC_TIM19_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM19_IRQn);   
        NVIC_EnableIRQ(TIM19_IRQn);   
        break;  
        
        case TIM20_BASE_ADDR:          
        __HAL_RCC_TIM20_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM20_CC_IRQn);   
        NVIC_EnableIRQ(TIM20_CC_IRQn);    
        break; 
                
        case TIM21_BASE_ADDR:          
        __HAL_RCC_TIM21_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM21_IRQn);   
        NVIC_EnableIRQ(TIM21_IRQn); 
        break;  
                
        case TIM22_BASE_ADDR:          
        __HAL_RCC_TIM22_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM22_IRQn);   
        NVIC_EnableIRQ(TIM22_IRQn);  
        break;  
        
        case TIM23_BASE_ADDR:          
        __HAL_RCC_TIM23_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM23_IRQn);   
        NVIC_EnableIRQ(TIM23_IRQn); 
        break;  
                
        case TIM24_BASE_ADDR:          
        __HAL_RCC_TIM24_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM24_IRQn);   
        NVIC_EnableIRQ(TIM24_IRQn);  
        break;  
        
        case TIM25_BASE_ADDR:          
        __HAL_RCC_TIM25_CLK_ENABLE();    
        NVIC_ClearPendingIRQ(TIM25_IRQn);   
        NVIC_EnableIRQ(TIM25_IRQn); 
        break;                  
                        
        default:
        return HAL_ERROR;  
    }
    
    return HAL_OK;    
}  


Channels_GPIO_Config timer_gpio_config[25] = 
{
    // TIMER1 
    { {GPIOE, GPIO_PIN_9, GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOE, GPIO_PIN_11,GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOE, GPIO_PIN_13, GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOE, GPIO_PIN_14, GPIO_FUNCTION_1, GPIO_PULLUP} },   
    #if 0 
    // TIMER2     
    { {GPIOA, GPIO_PIN_0, GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOA, GPIO_PIN_1,GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOA, GPIO_PIN_3, GPIO_FUNCTION_2, GPIO_PULLUP} },  
    #else
    // TIMER2     
    { {GPIOA, GPIO_PIN_0, GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOA, GPIO_PIN_1,GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_2, GPIO_PULLUP}, {0, 0, 0, 0} },  
    #endif 
    #if 0
    // TIMER3     
    { {GPIOA, GPIO_PIN_6, GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOA, GPIO_PIN_7,GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOB, GPIO_PIN_0, GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOB, GPIO_PIN_1, GPIO_FUNCTION_1, GPIO_PULLUP} },  
    #else
    // TIMER3     
    { {GPIOA, GPIO_PIN_6, GPIO_FUNCTION_1, GPIO_PULLUP}, {GPIOA, GPIO_PIN_7,GPIO_FUNCTION_1, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    #endif 
    // TIMER4     
    { {GPIOB, GPIO_PIN_6, GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOB, GPIO_PIN_7,GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOB, GPIO_PIN_8, GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOB, GPIO_PIN_9, GPIO_FUNCTION_8, GPIO_PULLUP} },  
    #if 0 
    // TIMER5       
    { {GPIOA, GPIO_PIN_0, GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOA, GPIO_PIN_1,GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOA, GPIO_PIN_3, GPIO_FUNCTION_4, GPIO_PULLUP} }, 
    #else
    { {GPIOA, GPIO_PIN_0, GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOA, GPIO_PIN_1,GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_4, GPIO_PULLUP}, {0, 0, 0, 0} }, 
    #endif 
    // TIMER6       
    { {0, 0, 0, 0}, {0, 0,0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    // TIMER7       
    { {0, 0, 0, 0}, {0, 0,0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    // TIMER8      
    { {GPIOC, GPIO_PIN_6, GPIO_FUNCTION_9, GPIO_PULLUP}, {GPIOC, GPIO_PIN_7,GPIO_FUNCTION_9, GPIO_PULLUP}, {GPIOC, GPIO_PIN_8, GPIO_FUNCTION_9, GPIO_PULLUP}, {GPIOC, GPIO_PIN_9, GPIO_FUNCTION_9, GPIO_PULLUP} }, 
    #if 0 
    // TIMER9      
    { {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_9, GPIO_PULLUP}, {GPIOA, GPIO_PIN_3,GPIO_FUNCTION_9, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    #else 
    { {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_9, GPIO_PULLUP}, {0, 0,0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    #endif 
    // TIMER10       
    { {GPIOB, GPIO_PIN_8, GPIO_FUNCTION_9, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }, 
    // TIMER11      
    { {GPIOB, GPIO_PIN_9, GPIO_FUNCTION_9, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },    
    // TIMER12      
    { {GPIOB, GPIO_PIN_14, GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOB, GPIO_PIN_15, GPIO_FUNCTION_4, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    // TIMER13        
    { {GPIOA, GPIO_PIN_6, GPIO_FUNCTION_4, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    // TIMER14  
    { {GPIOA, GPIO_PIN_7, GPIO_FUNCTION_4, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    #if 0 
    // TIMER15  
    { {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_5, GPIO_PULLUP}, {GPIOA, GPIO_PIN_3, GPIO_FUNCTION_5, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    #else 
    { {GPIOA, GPIO_PIN_2, GPIO_FUNCTION_5, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    #endif         
    // TIMER16    
    { {GPIOB, GPIO_PIN_8, GPIO_FUNCTION_2, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    // TIMER17  
    { {GPIOB, GPIO_PIN_9, GPIO_FUNCTION_2, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   
    // TIMER18   
    { {GPIOI, GPIO_PIN_12, GPIO_FUNCTION_1, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    // TIMER19  
    { {GPIOI, GPIO_PIN_13, GPIO_FUNCTION_1, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
    // TIMER20  
    { {GPIOB, GPIO_PIN_2, GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOC, GPIO_PIN_2, GPIO_FUNCTION_2, GPIO_PULLUP}, {GPIOC, GPIO_PIN_8, GPIO_FUNCTION_4, GPIO_PULLUP}, {GPIOE, GPIO_PIN_1, GPIO_FUNCTION_7, GPIO_PULLUP} },  
    // TIMER21 
    { {0, 0, 0, 0}, {0, 0,0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },    
    // TIMER22  
    { {0, 0, 0, 0}, {0, 0,0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }, 
    // TIMER23  
    { {GPIOF, GPIO_PIN_0, GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOF, GPIO_PIN_1, GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOF, GPIO_PIN_2, GPIO_FUNCTION_8, GPIO_PULLUP}, {GPIOF, GPIO_PIN_3, GPIO_FUNCTION_8, GPIO_PULLUP} },  
    // TIMER24 
    { {GPIOF, GPIO_PIN_11, GPIO_FUNCTION_10, GPIO_PULLUP}, {GPIOF, GPIO_PIN_12, GPIO_FUNCTION_10, GPIO_PULLUP}, {GPIOF, GPIO_PIN_13, GPIO_FUNCTION_10, GPIO_PULLUP}, {GPIOF, GPIO_PIN_14, GPIO_FUNCTION_10, GPIO_PULLUP} },   
    // TIMER25    
    { {GPIOI, GPIO_PIN_10, GPIO_FUNCTION_6, GPIO_PULLUP}, {GPIOI, GPIO_PIN_11, GPIO_FUNCTION_6, GPIO_PULLUP}, {0, 0, 0, 0}, {0, 0, 0, 0} },  
};




void Capture_GPIO_Config(GPIO_Config *p_gpio_config)  
{
    GPIO_InitTypeDef    GPIO_Timer = {0};
    
    RCC->AHB2CKENR |= 0x1FFFF; // GPIOA to GPIOQ 
    
    GPIO_Timer.Pin       = p_gpio_config->pin;  
    GPIO_Timer.Mode      = GPIO_MODE_AF_PP;
    GPIO_Timer.Pull      = p_gpio_config->pull;
    GPIO_Timer.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_Timer.Alternate = p_gpio_config->function;  
        
    HAL_GPIO_Init(p_gpio_config->Port, &GPIO_Timer);     
}


