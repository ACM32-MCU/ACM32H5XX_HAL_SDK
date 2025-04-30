/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


#define TIM_CLOCK_FREQ            (10000)  
#define BIT_32_TIM_CLOCK_FREQ        (1000000)  

TIM_HandleTypeDef * p_TIMx_Handler = NULL;   
  
TIM_HandleTypeDef Slave_TIM_Handler;  
TIM_HandleTypeDef Master_TIM_Handler;  

volatile uint32_t Timer_Update_Flag;    
volatile uint32_t Timer_Trig_Flag;  

TIM_TypeDef * Convert_Timer_Index_Timer_Instance(uint8_t index)
{
    switch(index)
    {
        case 1:
        return TIM1; 
        
        case 2:
        return TIM2;  
        
        case 3:
        return TIM3;
        
        case 4:
        return TIM4; 
        
        case 5:
        return TIM5;  
        
        case 6:
        return TIM6; 
        
        case 7:
        return TIM7; 
        
        case 8:
        return TIM8;  
        
        case 9:
        return TIM9;
        
        case 10:
        return TIM10; 
        
        case 11:
        return TIM11;  
        
        case 12:
        return TIM12; 
        
        case 13:
        return TIM13; 
        
        case 14:
        return TIM14;  
        
        case 15:
        return TIM15;
        
        case 16:
        return TIM16; 
        
        case 17:
        return TIM17;  
        
        case 18:
        return TIM18; 
        
        case 19:
        return TIM19; 
        
        case 20:
        return TIM20;  
        
        case 21:
        return TIM21;
        
        case 22:
        return TIM22; 
        
        case 23:
        return TIM23;  
        
        case 24:
        return TIM24;  
        
        case 25:
        return TIM25;   
    }
    
    return NULL;  
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


void HAL_TIM_Update_Event_Callback(TIM_HandleTypeDef *htim)
{
    Timer_Update_Flag = 1;   
    htim->Instance->SR = (~BIT0);   
}

void HAL_TIM_Trig_Event_Callback(TIM_HandleTypeDef *htim)
{
    Timer_Trig_Flag = 1;   
}
                                             
                                                     

 
void TIM_Slave_ITR_Config_And_Start(TIM_TypeDef *Instance, uint32_t trig_sel)    
{
    uint32_t timer_clock; 
    TIM_SlaveConfigTypeDef Slave_Config;  
    
    timer_clock = Get_Timer_Bus_Clock(Instance);   
    
    if (0 == timer_clock)
    {
        printfS("Get Timer Clock Error\n");   
        return;  
    }
    
    if (HAL_RCC_GetHCLKFreq() != timer_clock)  
    {
       timer_clock =  timer_clock << 1;    
    }
    
    p_TIMx_Handler = &Slave_TIM_Handler;   
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_DISABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP; 
   
    p_TIMx_Handler->Init.RepetitionCounter = 0;   
    
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;    
	p_TIMx_Handler->Init.Period = (TIM_CLOCK_FREQ/2) - 1; // 0.5S           
	
	HAL_TIMER_MSP_Init(p_TIMx_Handler);        
    HAL_TIMER_OnePulse_Init(p_TIMx_Handler, 1); 
    
    Slave_Config.SlaveMode = TIM_SLAVE_MODE_TRIG;
    Slave_Config.InputTrigger = trig_sel;  
    HAL_TIMER_Slave_Mode_Config(p_TIMx_Handler, &Slave_Config);  
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
    HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_TRI); 
    
    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);    

    p_TIMx_Handler->Instance->SR = 0;  
}


void TIM13_PWM_GPIO_Init(void)
{
    GPIO_InitTypeDef    GPIO_PWM = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();   

    GPIO_PWM.Pin       = GPIO_PIN_6; 
    GPIO_PWM.Mode      = GPIO_MODE_AF_PP;
    GPIO_PWM.Pull      = GPIO_NOPULL;
    GPIO_PWM.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_PWM.Alternate = GPIO_FUNCTION_4;  
    
    HAL_GPIO_Init(GPIOA, &GPIO_PWM);  
}



void TIM_Master_CCRx_OCx_Config_And_Start(TIM_TypeDef *Instance, uint32_t output_channel)    
{
    uint32_t timer_clock; 
    TIM_OC_InitTypeDef Tim_OC_Init_Para;   
    
    timer_clock = Get_Timer_Bus_Clock(Instance);   
    
    if (0 == timer_clock)
    {
        printfS("Get Timer Clock Error\n");   
        return;  
    }
    
    
    if (HAL_RCC_GetHCLKFreq() != timer_clock)  
    {
       timer_clock =  timer_clock << 1;    
    }
    
    p_TIMx_Handler = &Master_TIM_Handler;   
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_DISABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP; 
   
    p_TIMx_Handler->Init.RepetitionCounter = 0;   
    
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = (TIM_CLOCK_FREQ*2) - 1; // 1 * 2 = 2S        
	
	HAL_TIMER_MSP_Init(p_TIMx_Handler);        
    HAL_TIMER_Base_Init(p_TIMx_Handler);   
    
    Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM2;  
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;      
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_HIGH;    
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = (p_TIMx_Handler->Init.Period + 1)/2;// 50% duty cycle 
         

	HAL_TIMER_Output_Config(p_TIMx_Handler->Instance, &Tim_OC_Init_Para, output_channel);  
    
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, output_channel);    
         
}


void Timer_OC_Update_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_list_index, i, timer_itr_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Trig_Flag = 0; 
    
    timer_list_index = 0;   
    timer_itr_index = 0;   
    
    TIM_Slave_ITR_Config_And_Start(TIM12, TIM_TS_ITR2);    
    TIM_Master_CCRx_OCx_Config_And_Start(TIM13, TIM_CHANNEL_1);    
    
//    TIM_Slave_ITR_Config_And_Start(TIM12, TIM_TS_ITR3);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM14, TIM_CHANNEL_1); 

//    TIM_Slave_ITR_Config_And_Start(TIM9, TIM_TS_ITR2);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM10, TIM_CHANNEL_1);     

//    TIM_Slave_ITR_Config_And_Start(TIM9, TIM_TS_ITR3);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM11, TIM_CHANNEL_1);    
    
//    TIM_Slave_ITR_Config_And_Start(TIM15, TIM_TS_ITR2);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM16, TIM_CHANNEL_1);    
    
//    TIM_Slave_ITR_Config_And_Start(TIM15, TIM_TS_ITR3);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM17, TIM_CHANNEL_1);   
    
//    TIM_Slave_ITR_Config_And_Start(TIM25, TIM_TS_ITR2);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM18, TIM_CHANNEL_1);  
    
//    TIM_Slave_ITR_Config_And_Start(TIM25, TIM_TS_ITR3);       
//    TIM_Master_CCRx_OCx_Config_And_Start(TIM19, TIM_CHANNEL_1);     
       
    i = 0;      
	while(1)
	{
        if (Master_TIM_Handler.Instance->SR & BIT1)
        {      
            Master_TIM_Handler.Instance->SR &= ~BIT1;        
            printfS("Master Compare\n");                      
        }
        
        if(Timer_Trig_Flag) 
        {
            printfS("Trig\n");   
            Timer_Trig_Flag = 0;   
        }
        
		if(Timer_Update_Flag) 
		{		
            i++;
            printfS("Slave Update:%d\n", i);     
			Timer_Update_Flag = 0;                            
		}
	}
}

