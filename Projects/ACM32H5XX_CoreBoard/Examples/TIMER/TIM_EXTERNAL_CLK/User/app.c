/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


TIM_HandleTypeDef TIM_ETR_Handler;   

volatile uint32_t Timer_Update_Flag;    


void HAL_TIM_Updeate_Event_Callback(TIM_HandleTypeDef *htim)
{
    Timer_Update_Flag = 1;   
}

void TIMx_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	HAL_TIMER_MSP_Init(&TIM_ETR_Handler);   
}

void COMP1_Init(void)
{
    GPIO_InitTypeDef    GPIO_COMP_IN = {0};
    GPIO_InitTypeDef    GPIO_COMP_OUT = {0};
    
    __HAL_RCC_CMP1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();  
        
    GPIO_COMP_IN.Pin       = GPIO_PIN_1|GPIO_PIN_2; // PB1:INM, PB2:INP
    GPIO_COMP_IN.Mode      = GPIO_MODE_ANALOG_SWITCH_OFF;
    GPIO_COMP_IN.Pull      = GPIO_NOPULL;
    GPIO_COMP_IN.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_COMP_IN.Alternate = GPIO_FUNCTION_0;  
        
    HAL_GPIO_Init(GPIOB, &GPIO_COMP_IN);   
    
    
    GPIO_COMP_OUT.Pin       = GPIO_PIN_8; // PB8: COMP1_OUT    
    GPIO_COMP_OUT.Mode      = GPIO_MODE_AF_PP;
    GPIO_COMP_OUT.Pull      = GPIO_NOPULL;
    GPIO_COMP_OUT.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_COMP_OUT.Alternate = GPIO_FUNCTION_1;  
        
    HAL_GPIO_Init(GPIOB, &GPIO_COMP_OUT);  

    COMP1->CR = (COMP1->CR & ~0x0FFF) | 0x09 | (1 << 4) | (1 << 8);  // HYS = 10mv, enable COMP1     
    
}


void TIMx_MSP_Post_Init(TIM_TypeDef *Instance, uint32_t etr_input)
{
    uint32_t tim_base_addr;  
    
    tim_base_addr = (uint32_t)Instance;  
    
    GPIO_InitTypeDef    GPIO_Timer = {0}; 
    
    RCC->AHB2CKENR |= 0x1FFFF; // GPIOA to GPIOQ   
    
    GPIO_Timer.Mode      = GPIO_MODE_AF_PP;  
    GPIO_Timer.Pull      = GPIO_PULLUP;
    GPIO_Timer.Drive     = GPIO_DRIVE_LEVEL3;
      
    
    switch(tim_base_addr)
    {
        case TIM1_BASE_ADDR: 
                
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM1-PE7\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_7;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;         
            HAL_GPIO_Init(GPIOE, &GPIO_Timer);   // PE7  
        }
        else if (ETR_SELECT_IN1 == etr_input)  
        {
            printfS("TIM1-COMP1\n");   
            COMP1_Init();  
        }
        break;  
        
        case TIM2_BASE_ADDR: 
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM2-PA5\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_5;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;    
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);      
        } 
        else if (ETR_SELECT_IN11 == etr_input)
        {
            printfS("TIM3-PD2\n");  
            GPIO_Timer.Pin       = GPIO_PIN_2;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;   
            HAL_GPIO_Init(GPIOD, &GPIO_Timer);   
        }
        else if (ETR_SELECT_IN12 == etr_input)
        {
            printfS("TIM4-PE0\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;   
            HAL_GPIO_Init(GPIOE, &GPIO_Timer);  
        }
        else if (ETR_SELECT_IN13 == etr_input)  
        {
            printfS("TIM5-PA4\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_4;
            GPIO_Timer.Alternate = GPIO_FUNCTION_4;   
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);    
        }
        else if (ETR_SELECT_IN3 == etr_input)  
        {          
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~0xFFU) | 0x19;  
            HAL_Delay(500);  
            printfS("LSE:0x%08x\n", RCC->STDBYCTRL);    
        }
        else if (ETR_SELECT_IN1 == etr_input)  
        {
            printfS("TIM2-COMP1\n");   
            COMP1_Init();  
        }
        break; 
        
        case TIM3_BASE_ADDR:  
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM3-PD2\n");  
            GPIO_Timer.Pin       = GPIO_PIN_2;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;   
            HAL_GPIO_Init(GPIOD, &GPIO_Timer);    
        } 
        else if(ETR_SELECT_IN8 == etr_input) 
        {
            printfS("TIM2-PA5\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_5;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;    
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);   
        }
        else if(ETR_SELECT_IN9 == etr_input) 
        {
            printfS("TIM4-PE0\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;   
            HAL_GPIO_Init(GPIOE, &GPIO_Timer);  
        }
        else if (ETR_SELECT_IN1 == etr_input)  
        {
            printfS("TIM3-COMP1\n");   
            COMP1_Init();  
        }
        break;  
        
        case TIM4_BASE_ADDR:  
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM4-PE0\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;   
            HAL_GPIO_Init(GPIOE, &GPIO_Timer);  
        }
        else if(ETR_SELECT_IN8 == etr_input) 
        {
            printfS("TIM3-PD2\n");  
            GPIO_Timer.Pin       = GPIO_PIN_2;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;   
            HAL_GPIO_Init(GPIOD, &GPIO_Timer);  
        }
        else if(ETR_SELECT_IN9 == etr_input) 
        {
            printfS("TIM5-PA4\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_4;
            GPIO_Timer.Alternate = GPIO_FUNCTION_4;   
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);  
        }
        break; 
        
        case TIM5_BASE_ADDR: 
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM5-PA4\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_4;
            GPIO_Timer.Alternate = GPIO_FUNCTION_4;   
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);  
        }
        else if(ETR_SELECT_IN8 == etr_input)  
        {
            printfS("TIM2-PA5\n"); 
            GPIO_Timer.Pin       = GPIO_PIN_5;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;    
            HAL_GPIO_Init(GPIOA, &GPIO_Timer); 
        }
        else if(ETR_SELECT_IN9 == etr_input)  
        {
            printfS("TIM3-PD2\n");  
            GPIO_Timer.Pin       = GPIO_PIN_2;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;   
            HAL_GPIO_Init(GPIOD, &GPIO_Timer);  
        }
        break;  
        
        case TIM8_BASE_ADDR:   
        printfS("TIM8-PG8\n"); 
        GPIO_Timer.Pin       = GPIO_PIN_8;
        GPIO_Timer.Alternate = GPIO_FUNCTION_2;    
        HAL_GPIO_Init(GPIOG, &GPIO_Timer);  
        break;   
        
        case TIM6_BASE_ADDR:  // not support    
        case TIM7_BASE_ADDR: 
        case TIM9_BASE_ADDR:    
        case TIM10_BASE_ADDR:   
        case TIM11_BASE_ADDR:   
        case TIM12_BASE_ADDR:  
        case TIM13_BASE_ADDR:   
        case TIM14_BASE_ADDR: 
        case TIM15_BASE_ADDR: 
        case TIM16_BASE_ADDR: 
        case TIM17_BASE_ADDR:  
        case TIM18_BASE_ADDR: 
        case TIM19_BASE_ADDR:  
        case TIM21_BASE_ADDR:   
        case TIM22_BASE_ADDR:    
        case TIM25_BASE_ADDR:     
        printfS("Invalid\n"); 
        break;  
        
        case TIM20_BASE_ADDR: 
        printfS("TIM20-PE0\n"); 
        GPIO_Timer.Pin       = GPIO_PIN_0;
        GPIO_Timer.Alternate = GPIO_FUNCTION_7;      
        HAL_GPIO_Init(GPIOE, &GPIO_Timer);  
        break;   
        
        case TIM23_BASE_ADDR: 
        if (ETR_SELECT_GPIO == etr_input)
        {
            printfS("TIM23-PB2\n");   
            GPIO_Timer.Pin       = GPIO_PIN_2;
            GPIO_Timer.Alternate = GPIO_FUNCTION_8;        
            HAL_GPIO_Init(GPIOB, &GPIO_Timer);  
        }
        else if (ETR_SELECT_IN3 == etr_input)  
        {          
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~0xFFU) | 0x19;  
            HAL_Delay(500);  
            printfS("LSE:0x%08x\n", RCC->STDBYCTRL);    
        }
        break;  
        
        case TIM24_BASE_ADDR:   
        printfS("TIM24-PG2\n"); 
        
        GPIO_Timer.Pin       = GPIO_PIN_2;
        GPIO_Timer.Alternate = GPIO_FUNCTION_10;        
        HAL_GPIO_Init(GPIOG, &GPIO_Timer);   
        break;  
        
        default: 
        break;   
              
    }
       
}


uint32_t Get_Timer_Bus_Clock(TIM_TypeDef *Instance)
{
    uint32_t clk_freq, tim_base_addr; 
    
    tim_base_addr = (uint32_t)Instance;    
    
    switch(tim_base_addr) 
    {
        case TIM2_BASE_ADDR: 
        case TIM3_BASE_ADDR:
        case TIM4_BASE_ADDR:
        case TIM5_BASE_ADDR:
        case TIM6_BASE_ADDR:
        case TIM7_BASE_ADDR:
        case TIM12_BASE_ADDR:
        case TIM13_BASE_ADDR: 
        case TIM14_BASE_ADDR:
        case TIM25_BASE_ADDR:  
        clk_freq = HAL_RCC_GetPCLK1Freq();      
        break; 

        case TIM1_BASE_ADDR:
        case TIM8_BASE_ADDR: 
        case TIM9_BASE_ADDR: 
        case TIM10_BASE_ADDR: 
        case TIM11_BASE_ADDR: 
        case TIM15_BASE_ADDR: 
        case TIM16_BASE_ADDR:  
        case TIM17_BASE_ADDR: 
        case TIM18_BASE_ADDR: 
        case TIM19_BASE_ADDR:  
        case TIM20_BASE_ADDR:   
        case TIM21_BASE_ADDR: 
        case TIM22_BASE_ADDR:  
        case TIM23_BASE_ADDR: 
        case TIM24_BASE_ADDR:  
        clk_freq = HAL_RCC_GetPCLK2Freq();   
        break; 

        default:
        clk_freq = 0; 
        break;  
    }
    
    return clk_freq; 
} 


void TIMx_ETR_Config_And_Start(TIM_TypeDef * instance, uint32_t etr_input)  
{
    uint32_t timer_clock; 
    
    TIM_ClockConfigTypeDef sClockSourceConfig;
    
    timer_clock = Get_Timer_Bus_Clock(instance);     
    
    if (HAL_RCC_GetHCLKFreq() != timer_clock)  
    {
       timer_clock =  timer_clock << 1;    
    }
    
	TIM_ETR_Handler.Instance = instance;  
	TIM_ETR_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	TIM_ETR_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM_ETR_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM_ETR_Handler.Init.RepetitionCounter = 0;  
	TIM_ETR_Handler.Init.Prescaler = 0;  
    if (IS_TIM_32_BITS_INSTANCE(instance) ) 
    {
        TIM_ETR_Handler.Init.Period = 0x1FFFFFFF;       
    }
    else
    {
        TIM_ETR_Handler.Init.Period = 0xFFFF;    
    }
	 
	
	TIMx_MSP_Pre_Init(&TIM_ETR_Handler);       
	HAL_TIMER_Base_Init(&TIM_ETR_Handler);    
	HAL_TIM_ENABLE_IT(&TIM_ETR_Handler, TIMER_INT_EN_UPD); 
    
    sClockSourceConfig.ClockFilter = INPUT_FILTER_LEVEL_0;       
    sClockSourceConfig.ClockPolarity = INPUT_ACTIVE_RISING;  
    sClockSourceConfig.ClockPrescaler = TIM_ETR_PRESCALER_1;  
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETR;  
    HAL_TIMER_SelectClockSource(&TIM_ETR_Handler,  &sClockSourceConfig);   
    
    HAL_TIMEx_ETRSelection(&TIM_ETR_Handler, etr_input);   
    
    TIMx_MSP_Post_Init(instance, etr_input);    
    	       
	HAL_TIMER_Base_Start(TIM_ETR_Handler.Instance);            
}


void ETR_Signal_Generator_Init(void)
{
    GPIO_InitTypeDef    GPIO_ETR = {0};
    
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_ETR.Pin       = GPIO_PIN_0;
    GPIO_ETR.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_ETR.Pull      = GPIO_NOPULL;
    GPIO_ETR.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_ETR.Alternate = GPIO_FUNCTION_0;
        
    HAL_GPIO_Init(GPIOC, &GPIO_ETR); 
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); 
}

void ETR_Signal_Generator(void)
{
    HAL_Delay(5); 
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);  
    HAL_Delay(5); 
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);    
}

void Timer_Update_Test(void)
{
    printfS("Timer ETR Update_Test\n"); 
    
    ETR_Signal_Generator_Init();
    
	Timer_Update_Flag = 0;  
    
	TIMx_ETR_Config_And_Start(TIM1, ETR_SELECT_GPIO);    
//    TIMx_ETR_Config_And_Start(TIM1, ETR_SELECT_IN1);   
    
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_GPIO);    
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_IN11);    
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_IN12);   
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_IN13);   
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_IN3);     
//    TIMx_ETR_Config_And_Start(TIM2, ETR_SELECT_IN1);  
    
//    TIMx_ETR_Config_And_Start(TIM3, ETR_SELECT_GPIO);     
//    TIMx_ETR_Config_And_Start(TIM3, ETR_SELECT_IN8);      
//    TIMx_ETR_Config_And_Start(TIM3, ETR_SELECT_IN9);  
//    TIMx_ETR_Config_And_Start(TIM3, ETR_SELECT_IN1);  
      
//    TIMx_ETR_Config_And_Start(TIM4, ETR_SELECT_GPIO);    
//    TIMx_ETR_Config_And_Start(TIM4, ETR_SELECT_IN8); 
//    TIMx_ETR_Config_And_Start(TIM4, ETR_SELECT_IN9); 
    
//    TIMx_ETR_Config_And_Start(TIM5, ETR_SELECT_GPIO); 
//    TIMx_ETR_Config_And_Start(TIM5, ETR_SELECT_IN8); 
//    TIMx_ETR_Config_And_Start(TIM5, ETR_SELECT_IN9); 

//    TIMx_ETR_Config_And_Start(TIM8, ETR_SELECT_GPIO);    

//    TIMx_ETR_Config_And_Start(TIM20, ETR_SELECT_GPIO); 
    
//    TIMx_ETR_Config_And_Start(TIM23, ETR_SELECT_GPIO);  
//    TIMx_ETR_Config_And_Start(TIM23, ETR_SELECT_IN3);   

//    TIMx_ETR_Config_And_Start(TIM24, ETR_SELECT_GPIO); 
    
	while(1)
	{
        printfS("CNT:%d\n", TIM_ETR_Handler.Instance->CNT);   
        ETR_Signal_Generator();  
        HAL_Delay(500);         
	}
}

