/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


#define TIM_CLOCK_FREQ            (10000)  
#define BIT_32_TIM_CLOCK_FREQ        (1000000)  

TIM_HandleTypeDef * p_TIMx_Handler = NULL;   
TIM_HandleTypeDef Base_TIM_Handler; 
TIM_HandleTypeDef General_16_TIM_Handler; 
TIM_HandleTypeDef General_32_TIM_Handler; 
TIM_HandleTypeDef Advanced_TIM_Handler;  

uint8_t base_timer_list[4] = {6, 7 , 21, 22};  
uint8_t advanced_timer_list[3] = {1, 8, 20}; 
uint8_t general_timer16_list[18] = {3, 4, 9, 12, 10, 11, 13, 14, 15, 25, 16, 17, 18, 19}; 
uint8_t general_timer32_list[18] = {2, 5, 23, 24};   

volatile uint32_t Timer_Update_Flag;    
volatile uint32_t Timer_Update_times;    

uint8_t Get_Timer_In_Array(uint8_t index)
{
    uint8_t array_type;
    
    for(array_type = 0; array_type < 4; array_type++)  
    {
        uint8_t i, j;
        
        switch(array_type)
        {
            case 0: 
            j = sizeof(base_timer_list);     
            for (i = 0; i < j; i++)
            {
                if(index == base_timer_list[i])
                {
                    return array_type;  
                }
            }
            break;  
            
            case 1:
            j = sizeof(advanced_timer_list);     
            for (i = 0; i < j; i++)
            {
                if(index == advanced_timer_list[i])
                {
                    return array_type;  
                }
            }
            break;  
            
            case 2:
            j = sizeof(general_timer16_list);   
            for (i = 0; i < j; i++)
            {
                if(index == general_timer16_list[i])
                {
                    return array_type;  
                }
            }
            break;  
            
            case 3: 
            j = sizeof(general_timer32_list);   
            for (i = 0; i < j; i++)
            {
                if(index == general_timer32_list[i])   
                {
                    return array_type;  
                }
            }
            break; 
            
            default:    
            return 0xFF;   
        } 

        
    }
    
    return array_type;  
}

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
    Timer_Update_times++;  
    htim->Instance->SR = (~BIT0);   
}



void Advanced_TIM_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);    
}

// Configure Advanced 16 bit Timer, period as 2000ms 
void Advanced_TIM_Init(TIM_TypeDef *Instance)   
{
    uint32_t timer_clock; 
    
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
    
    p_TIMx_Handler = &Advanced_TIM_Handler;  
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1; 
    if ( IS_TIM_REPETITION_COUNTER_INSTANCE(Instance) ) 
    {
        p_TIMx_Handler->Init.RepetitionCounter = 1;   
    }
    else
    {
        p_TIMx_Handler->Init.RepetitionCounter = 0;   
    }
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = (TIM_CLOCK_FREQ) - 1; // 1(ARR) * 2(repetition) = 2S        
	
	Advanced_TIM_MSP_Pre_Init(p_TIMx_Handler);       
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
	
	          
}



void General_16_TIM_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);    
}


// Configure Genneral 16 bit Timer, period = 500ms * 1(repetition disalbed) or 500ms * 6 (RepetitionCounter = 5)
void General_16_TIM_Init(TIM_TypeDef *Instance)   
{
    uint32_t timer_clock; 
    
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
    
    p_TIMx_Handler = &General_16_TIM_Handler;    
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP; 
    if ( IS_TIM_REPETITION_COUNTER_INSTANCE(Instance) ) 
    {
        p_TIMx_Handler->Init.RepetitionCounter = 5;  
    }
    else
    {
        p_TIMx_Handler->Init.RepetitionCounter = 0;  
    }
    
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = (TIM_CLOCK_FREQ/1000)*500 - 1;   // 500ms 
	
	General_16_TIM_MSP_Pre_Init(p_TIMx_Handler);         
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
	            
}







void General_32_TIM_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);    
}

// Configure Genneral 32 bit Timer, period as 1000ms * 1 (RepetitionCounter = 0) 
void General_32_TIM_Init(TIM_TypeDef *Instance)   
{
    uint32_t timer_clock; 
    
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
    
    p_TIMx_Handler = &General_32_TIM_Handler;    
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;     
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_DOWN;  
    if ( IS_TIM_REPETITION_COUNTER_INSTANCE(Instance) ) 
    {
        p_TIMx_Handler->Init.RepetitionCounter = 1;  
    }
    else
    {
        p_TIMx_Handler->Init.RepetitionCounter = 0; 
    }
	p_TIMx_Handler->Init.Prescaler = (timer_clock/BIT_32_TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = BIT_32_TIM_CLOCK_FREQ - 1;   
	
	General_32_TIM_MSP_Pre_Init(p_TIMx_Handler);         
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
	            
}




void Base_TIM_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);     
}
// Configure basic 16 bit Timer, period as 1000ms 
void Base_TIM_Init(TIM_TypeDef *Instance)  
{
    uint32_t timer_clock; 
    
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
    
    p_TIMx_Handler = &Base_TIM_Handler;  
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP;  
	p_TIMx_Handler->Init.RepetitionCounter = 0;  
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = TIM_CLOCK_FREQ - 1;  // 1s 
	
	Base_TIM_MSP_Pre_Init(p_TIMx_Handler);    
    
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
	            
}





void Timer_Update_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index, timer_type;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_times = 0;  
    
    timer_index = 1;   
    Advanced_TIM_Init(TIM1);   
        
//    timer_index = 2;  
//    General_32_TIM_Init(TIM2);   
     
    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);     
    
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, %d\n", timer_index);   
            if (Timer_Update_times >= 3)
            {
                p_TIMx_Handler->Instance->CR1 &= (~BIT0);    
                timer_index++;   
                if (timer_index > 25)
                {
                    printfS("Timer Test End, %d\n", timer_index);    
                    break;   
                }
                timer_type = Get_Timer_In_Array(timer_index); 
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index);                  
                
                if (0xFF != timer_type)
                {
                    switch(timer_type)
                    {
                        case 0:
                        Base_TIM_Init(timer_instance);  
                        break;
                        
                        case 1: 
                        Advanced_TIM_Init(timer_instance);  
                        break;
                        
                        case 2:  
                        General_16_TIM_Init(timer_instance);   
                        break;
                        
                        case 3:  
                        General_32_TIM_Init(timer_instance);      
                        break;  
                    }
                    
                    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);      
                }
                Timer_Update_times = 0;   
                
            }
			Timer_Update_Flag = 0;   
               
		}
	}
}

