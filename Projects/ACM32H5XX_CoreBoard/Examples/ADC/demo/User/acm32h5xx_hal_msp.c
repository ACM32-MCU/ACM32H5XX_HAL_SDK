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


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    uint32_t i;
    GPIO_InitTypeDef GPIO_Handle = {0}; 
    uint32_t ADC1_Positive_Pin_Map[][3] =
    { 
        {ADC_CHANNEL_1_EN, (uint32_t)GPIOA, GPIO_PIN_5 },   //ADC1_INP1-PA5 
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOF, GPIO_PIN_11 },  //ADC1_INP2-PF11 
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOA, GPIO_PIN_4 },   //ADC1_INP3-PA4    
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOC, GPIO_PIN_4 },   //ADC1_INP4-PC4
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOB, GPIO_PIN_1 },   //ADC1_INP5-PB1
        {ADC_CHANNEL_6_EN, (uint32_t)GPIOF, GPIO_PIN_12 },  //ADC1_INP6-PF12
        {ADC_CHANNEL_7_EN, (uint32_t)GPIOA, GPIO_PIN_7 },   //ADC1_INP7-PA7 
        {ADC_CHANNEL_8_EN, (uint32_t)GPIOC, GPIO_PIN_5 },   //ADC1_INP8-PC5  
        {ADC_CHANNEL_9_EN, (uint32_t)GPIOA, GPIO_PIN_6 },   //ADC1_INP9-PA6
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_0 },  //ADC1_INP10-PC0
        {ADC_CHANNEL_11_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC1_INP11-PC1
        {ADC_CHANNEL_14_EN, (uint32_t)GPIOA, GPIO_PIN_2 },  //ADC1_INP14-PA2 
        {ADC_CHANNEL_15_EN, (uint32_t)GPIOA, GPIO_PIN_3 },  //ADC1_INP15-PA3
        {ADC_CHANNEL_16_EN, (uint32_t)GPIOA, GPIO_PIN_0 },  //ADC1_INP16-PA0
        {ADC_CHANNEL_17_EN, (uint32_t)GPIOA, GPIO_PIN_1 },  //ADC1_INP17-PA1
        {ADC_CHANNEL_18_EN, (uint32_t)GPIOB, GPIO_PIN_0 },  //ADC1_INP18-PB0
        
        { 0xffffffff, 0, 0 }, //结束标志
    };
    uint32_t ADC1_Negative_Pin_Map[][3] =
    {
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOF, GPIO_PIN_12 },  //ADC1_INN2-PF12
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOA, GPIO_PIN_5 },   //ADC1_INN3-PA5  
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOC, GPIO_PIN_5 },   //ADC1_INN4-PC5    
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOB, GPIO_PIN_0 },   //ADC1_INN5-PB0
        {ADC_CHANNEL_9_EN, (uint32_t)GPIOA, GPIO_PIN_7 },   //ADC1_INN9-PA7         
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC1_INN10-PC1  
        {ADC_CHANNEL_16_EN, (uint32_t)GPIOA, GPIO_PIN_1 },  //ADC1_INN16-PA1
        { 0xffffffff, 0, 0 }, //结束标志
    };

    uint32_t ADC2_Positive_Pin_Map[][3] =
    {
        {ADC_CHANNEL_1_EN, (uint32_t)GPIOA, GPIO_PIN_5 },   //ADC2_INP1-PA5 
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOF, GPIO_PIN_13 },  //ADC2_INP2-PF13  
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOA, GPIO_PIN_4 },   //ADC2_INP3-PA4     
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOC, GPIO_PIN_4 },   //ADC2_INP4-PC4
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOB, GPIO_PIN_1 },   //ADC2_INP5-PB1
        {ADC_CHANNEL_6_EN, (uint32_t)GPIOF, GPIO_PIN_14 },  //ADC2_INP6-PF14
        {ADC_CHANNEL_7_EN, (uint32_t)GPIOA, GPIO_PIN_7 },   //ADC2_INP7-PA7 
        {ADC_CHANNEL_8_EN, (uint32_t)GPIOC, GPIO_PIN_5 },   //ADC2_INP8-PC5  
        {ADC_CHANNEL_9_EN, (uint32_t)GPIOA, GPIO_PIN_6 },   //ADC2_INP9-PA6    
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_0 },  //ADC2_INP10-PC0
        {ADC_CHANNEL_11_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC2_INP11-PC1
        {ADC_CHANNEL_12_EN, (uint32_t)GPIOB, GPIO_PIN_2 },  //ADC2_INP12-PB2
        {ADC_CHANNEL_14_EN, (uint32_t)GPIOA, GPIO_PIN_2 },  //ADC2_INP14-PA2
        {ADC_CHANNEL_15_EN, (uint32_t)GPIOA, GPIO_PIN_3 },  //ADC2_INP15-PA3
        {ADC_CHANNEL_18_EN, (uint32_t)GPIOB, GPIO_PIN_0 },  //ADC2_INP18-PB0
        
        { 0xffffffff, 0, 0 }, //结束标志
    };
    uint32_t ADC2_Negative_Pin_Map[][3] =
    {
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOF, GPIO_PIN_14 },  //ADC2_INN2-PF14  
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOA, GPIO_PIN_5 },   //ADC2_INN3-PA5     
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOC, GPIO_PIN_5 },   //ADC2_INN4-PC5       
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOB, GPIO_PIN_0 },   //ADC2_INN5-PB0
        {ADC_CHANNEL_9_EN, (uint32_t)GPIOA, GPIO_PIN_7 },   //ADC2_INN9-PA7       
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC2_INN10-PC1
         
        { 0xffffffff, 0, 0 }, //结束标志
    };

    uint32_t ADC3_Positive_Pin_Map[][3] =
    {
        {ADC_CHANNEL_1_EN, (uint32_t)GPIOC, GPIO_PIN_3 },   //ADC3_INP1-PC3
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOH, GPIO_PIN_4 },   //ADC3_INP2-PH4
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOH, GPIO_PIN_5 },   //ADC3_INP3-PH5
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOF, GPIO_PIN_5 },   //ADC3_INP4-PF5
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOF, GPIO_PIN_3 },   //ADC3_INP5-PF3 
        {ADC_CHANNEL_6_EN, (uint32_t)GPIOF, GPIO_PIN_10 },  //ADC3_INP6-PF10
        {ADC_CHANNEL_7_EN, (uint32_t)GPIOF, GPIO_PIN_8 },   //ADC3_INP7-PF8 
        {ADC_CHANNEL_8_EN, (uint32_t)GPIOF, GPIO_PIN_6 },   //ADC3_INP8-PF6
        {ADC_CHANNEL_9_EN, (uint32_t)GPIOF, GPIO_PIN_4 },   //ADC3_INP9-PF4
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_0 },  //ADC3_INP10-PC0   
        {ADC_CHANNEL_11_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC3_INP11-PC1
        {ADC_CHANNEL_12_EN, (uint32_t)GPIOC, GPIO_PIN_2 },  //ADC3_INP12-PC2
        {ADC_CHANNEL_13_EN, (uint32_t)GPIOH, GPIO_PIN_3 },  //ADC3_INP13-PH2
        {ADC_CHANNEL_14_EN, (uint32_t)GPIOH, GPIO_PIN_2 },  //ADC3_INP14-PH3
        {ADC_CHANNEL_15_EN, (uint32_t)GPIOF, GPIO_PIN_9 },  //ADC3_INP15-PF9
        {ADC_CHANNEL_16_EN, (uint32_t)GPIOF, GPIO_PIN_7 },  //ADC3_INP16-PF7
        { 0xffffffff, 0, 0 }, //结束标志
    };
    uint32_t ADC3_Negative_Pin_Map[][3] =
    {
        {ADC_CHANNEL_1_EN, (uint32_t)GPIOC, GPIO_PIN_2 },   //ADC3_INN1-PC2 
        {ADC_CHANNEL_2_EN, (uint32_t)GPIOF, GPIO_PIN_10 },  //ADC3_INN2-PF10
        {ADC_CHANNEL_3_EN, (uint32_t)GPIOH, GPIO_PIN_4 },   //ADC3_INN3-PH4
        {ADC_CHANNEL_4_EN, (uint32_t)GPIOH, GPIO_PIN_5 },   //ADC3_INN4-PH5 
        {ADC_CHANNEL_5_EN, (uint32_t)GPIOF, GPIO_PIN_4 },   //ADC3_INN5-PF4
        {ADC_CHANNEL_10_EN, (uint32_t)GPIOC, GPIO_PIN_1 },  //ADC3_INN10-PC1
        {ADC_CHANNEL_13_EN, (uint32_t)GPIOH, GPIO_PIN_3 },  //ADC3_INN13-PH3  
        {ADC_CHANNEL_14_EN, (uint32_t)GPIOF, GPIO_PIN_8 },  //ADC3_INN14-PF8
        {ADC_CHANNEL_15_EN, (uint32_t)GPIOF, GPIO_PIN_6 },  //ADC3_INN15-PF6
        { 0xffffffff, 0, 0 }, //结束标志
    };
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    if(hadc->Instance == ADC1)
    {
        //Set Positive gpio to analog.
        for(i = 0; ADC1_Positive_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC1_Positive_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC1_Positive_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC1_Positive_Pin_Map[i][1], &GPIO_Handle);   
            }
        }
        //Set Positive gpio to analog.
        for(i = 0; ADC1_Negative_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC1_Negative_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC1_Negative_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC1_Negative_Pin_Map[i][1], &GPIO_Handle);   
            }
        }
    }
    else if(hadc->Instance == ADC2)
    {
        //Set Positive gpio to analog.
        for(i = 0; ADC2_Positive_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC2_Positive_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC2_Positive_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC2_Positive_Pin_Map[i][1], &GPIO_Handle);   
            }
        }
        //Set Positive gpio to analog.
        for(i = 0; ADC2_Negative_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC2_Negative_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC2_Negative_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC2_Negative_Pin_Map[i][1], &GPIO_Handle);   
            }
        }    
    }
    else if(hadc->Instance == ADC3)
    {
        //Set Positive gpio to analog.
        for(i = 0; ADC3_Positive_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC3_Positive_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC3_Positive_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC3_Positive_Pin_Map[i][1], &GPIO_Handle);   
            }
        }
        //Set Positive gpio to analog.
        for(i = 0; ADC3_Negative_Pin_Map[i][0] != 0xffffffff; i++)
        {
            if(hadc->Init.ChannelEn & ADC3_Negative_Pin_Map[i][0])
            {
                GPIO_Handle.Pin            = ADC3_Negative_Pin_Map[i][2];
                GPIO_Handle.Mode           = GPIO_MODE_ANALOG_SWITCH_ON;
                GPIO_Handle.Pull           = GPIO_NOPULL;
                HAL_GPIO_Init((GPIO_TypeDef *)ADC3_Negative_Pin_Map[i][1], &GPIO_Handle);   
            }
        }    
    }
    
    if((hadc->Instance == ADC1) || (hadc->Instance == ADC2))
    {
        /* Enable ADC Clock */
        __HAL_RCC_ADC12_CLK_ENABLE();
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(ADC12_IRQn);	
        /* Enable External Interrupt */
        NVIC_EnableIRQ(ADC12_IRQn);
    }
    else if(hadc->Instance == ADC3)
    {
        /* Enable ADC Clock */
        __HAL_RCC_ADC3_CLK_ENABLE();
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(ADC3_IRQn);	
        /* Enable External Interrupt */
        NVIC_EnableIRQ(ADC3_IRQn);
    }
}


