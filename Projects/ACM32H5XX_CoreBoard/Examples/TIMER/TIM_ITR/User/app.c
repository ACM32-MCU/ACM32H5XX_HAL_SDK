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


typedef struct _slave_master_info
{
    uint32_t slave_timer; 
    uint32_t slave_input; 
    uint32_t master_timer;     
}slave_master_info;  

slave_master_info slave_timer2_master_periphral[8] = {{(uint32_t)TIM2, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM2, TIM_TS_ITR1, (uint32_t)TIM8},  {(uint32_t)TIM2, TIM_TS_ITR2, (uint32_t)TIM3}, {(uint32_t)TIM2, TIM_TS_ITR3, (uint32_t)TIM4},  
                                                     {0, 0},  };  

slave_master_info slave_timer23_master_periphral[8] = {{(uint32_t)TIM23, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM23, TIM_TS_ITR1, (uint32_t)TIM8},  {(uint32_t)TIM23, TIM_TS_ITR2, (uint32_t)TIM3}, {(uint32_t)TIM23, TIM_TS_ITR3, (uint32_t)TIM24},  
                                                     {0, 0},  };  

slave_master_info slave_timer3_master_periphral[8] = {{(uint32_t)TIM3, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM3, TIM_TS_ITR1, (uint32_t)TIM2},  {(uint32_t)TIM3, TIM_TS_ITR2, (uint32_t)TIM15}, {(uint32_t)TIM3, TIM_TS_ITR3, (uint32_t)TIM4},  
                                                     {0, 0},  };  
slave_master_info slave_timer4_master_periphral[8] = {{(uint32_t)TIM4, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM4, TIM_TS_ITR1, (uint32_t)TIM2},  {(uint32_t)TIM4, TIM_TS_ITR2, (uint32_t)TIM3}, {(uint32_t)TIM4, TIM_TS_ITR3, (uint32_t)TIM8},  
                                                     {0, 0},  };    
slave_master_info slave_timer5_master_periphral[8] = {{(uint32_t)TIM5, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM5, TIM_TS_ITR1, (uint32_t)TIM8},  {(uint32_t)TIM5, TIM_TS_ITR2, (uint32_t)TIM3}, {(uint32_t)TIM5, TIM_TS_ITR3, (uint32_t)TIM4},  
                                                     {0, 0},  };  

slave_master_info slave_timer24_master_periphral[8] = {{(uint32_t)TIM24, TIM_TS_ITR0, (uint32_t)TIM20}, {(uint32_t)TIM24, TIM_TS_ITR1, (uint32_t)TIM8},  {(uint32_t)TIM24, TIM_TS_ITR2, (uint32_t)TIM23}, {(uint32_t)TIM24, TIM_TS_ITR3, (uint32_t)TIM4},  
                                                     {0, 0},  };  

slave_master_info slave_timer12_master_periphral[8] = {{(uint32_t)TIM12, TIM_TS_ITR0, (uint32_t)TIM4}, {(uint32_t)TIM12, TIM_TS_ITR1, (uint32_t)TIM5},  {0, 0}, {0, 0},  
                                                     {0, 0},  };  

slave_master_info slave_timer9_master_periphral[8] = {{(uint32_t)TIM9, TIM_TS_ITR0, (uint32_t)TIM2}, {(uint32_t)TIM9, TIM_TS_ITR1, (uint32_t)TIM3},  {0, 0}, {0, 0},  
                                                     {0, 0},  };  
slave_master_info slave_timer1_master_periphral[8] = {{(uint32_t)TIM1, TIM_TS_ITR0, (uint32_t)TIM15}, {(uint32_t)TIM1, TIM_TS_ITR1, (uint32_t)TIM2},  {(uint32_t)TIM1, TIM_TS_ITR2, (uint32_t)TIM3}, {(uint32_t)TIM1, TIM_TS_ITR3, (uint32_t)TIM4},  
                                                     {0, 0},  };  
slave_master_info slave_timer8_master_periphral[8] = {{(uint32_t)TIM8, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM8, TIM_TS_ITR1, (uint32_t)TIM2},  {(uint32_t)TIM8, TIM_TS_ITR2, (uint32_t)TIM4}, {(uint32_t)TIM8, TIM_TS_ITR3, (uint32_t)TIM5},  
                                                     {0, 0},  };  
slave_master_info slave_timer20_master_periphral[8] = {{(uint32_t)TIM20, TIM_TS_ITR0, (uint32_t)TIM8}, {(uint32_t)TIM20, TIM_TS_ITR1, (uint32_t)TIM23},  {(uint32_t)TIM20, TIM_TS_ITR2, (uint32_t)TIM24}, {(uint32_t)TIM20, TIM_TS_ITR3, (uint32_t)TIM25},  
                                                     {0, 0},  }; 
slave_master_info slave_timer15_master_periphral[8] = {{(uint32_t)TIM15, TIM_TS_ITR0, (uint32_t)TIM1}, {(uint32_t)TIM15, TIM_TS_ITR1, (uint32_t)TIM3},  {0, 0}, {0, 0},  
                                                     {0, 0},  }; 
slave_master_info slave_timer25_master_periphral[8] = {{(uint32_t)TIM25, TIM_TS_ITR0, (uint32_t)TIM8}, {(uint32_t)TIM25, TIM_TS_ITR1, (uint32_t)TIM4},  {0, 0}, {0, 0},  
                                                     {0, 0},  }; 

uint32_t timer_list_array[25] = {(uint32_t)slave_timer2_master_periphral, (uint32_t)slave_timer23_master_periphral, (uint32_t)slave_timer3_master_periphral, (uint32_t)slave_timer4_master_periphral,
(uint32_t)slave_timer5_master_periphral, (uint32_t)slave_timer24_master_periphral, (uint32_t)slave_timer12_master_periphral, (uint32_t)slave_timer9_master_periphral, (uint32_t)slave_timer1_master_periphral, 
(uint32_t)slave_timer8_master_periphral, (uint32_t)slave_timer20_master_periphral, (uint32_t)slave_timer15_master_periphral, (uint32_t)slave_timer25_master_periphral, 0};                                               
                                                     

//eg. TIM_Slave_ITR_Config_And_Start(TIM3, TIM_TS_ITR1);    
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
    
    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);        
}



//eg.  TIM_Master_TRGO_Config_And_Start(TIM2, TIM_TRGO_UPDATE);     
void TIM_Master_TRGO_Config_And_Start(TIM_TypeDef *Instance, uint32_t master_sel)    
{
    uint32_t timer_clock; 
    TIM_MasterConfigTypeDef Master_Config;  
    
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
    
    Master_Config.MasterOutputTrigger = master_sel;
    Master_Config.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;  
    Master_Config.MasterOutputTrigger2 = 0;  
    HAL_TIMER_Master_Mode_Config(p_TIMx_Handler->Instance, &Master_Config); 
    p_TIMx_Handler->Instance->CR1 |= BIT0;         
}


void Timer_ITR_Update_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_list_index, i, timer_itr_index;  
    
	Timer_Update_Flag = 0;  
    
    timer_list_index = 0;   
    timer_itr_index = 0;   
    
    TIM_Slave_ITR_Config_And_Start((TIM_TypeDef *)( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer , ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_input);       
    TIM_Master_TRGO_Config_And_Start((TIM_TypeDef *)(( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].master_timer), TIM_TRGO_UPDATE);  
    printfS("Timer_ITR_Update_Test, %d\n", timer_list_index);  
    
//    TIM_Slave_ITR_Config_And_Start(TIM3, TIM_TS_ITR1);       
//    TIM_Master_TRGO_Config_And_Start(TIM2, TIM_TRGO_UPDATE);     
       
    i = 0;      
	while(1)
	{
        if (Master_TIM_Handler.Instance->SR & BIT0)
        {      
            Master_TIM_Handler.Instance->SR &= ~BIT0;        
            printfS("Master Update: %d\n", i);                      
        }
        
		if(Timer_Update_Flag) 
		{		
            i++;
            printfS("Slave Update, TimeList:%d, TimeITR:%d\n", timer_list_index, timer_itr_index);     
			Timer_Update_Flag = 0;       
                       
            if (i >= 3)  
            {
                i = 0;   
                Master_TIM_Handler.Instance->CR1 &= ~BIT0;  
                
                ((TIM_TypeDef *) ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer)->DIER = 0; 
                ((TIM_TypeDef *) ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].master_timer)->DIER = 0;   
                ((TIM_TypeDef *) ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer)->SR = 0; 
                ((TIM_TypeDef *) ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].master_timer)->SR = 0;    
                
                printfS("Timer Slave Test End\n");  
                timer_itr_index++;
                if (0 != ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer )
                {
                    TIM_Slave_ITR_Config_And_Start((TIM_TypeDef *)( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer , ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_input);       
                    TIM_Master_TRGO_Config_And_Start((TIM_TypeDef *)(( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].master_timer), TIM_TRGO_UPDATE);   
                }
                else
                {
                    timer_list_index++; 
                    if (0 == timer_list_array[timer_list_index])
                    {
                        printfS("Timer ITR Test End\n"); 
                        return;  
                    }
                    else
                    {
                        timer_itr_index = 0;   
                        TIM_Slave_ITR_Config_And_Start((TIM_TypeDef *)( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_timer , ( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].slave_input);       
                        TIM_Master_TRGO_Config_And_Start((TIM_TypeDef *)(( (slave_master_info *)timer_list_array[timer_list_index])[timer_itr_index].master_timer), TIM_TRGO_UPDATE);  
                    }
                }
            }
		}
	}
}

