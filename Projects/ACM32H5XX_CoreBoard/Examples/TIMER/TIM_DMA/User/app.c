/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


#define TIM_CLOCK_FREQ            (10000)  
#define BIT_32_TIM_CLOCK_FREQ        (1000000)  

TIM_HandleTypeDef * p_TIMx_Handler = NULL;   
TIM_HandleTypeDef DMA_TIM_Handler; 


uint8_t update_dma_timer_list[26] = {1, 2, 3, 4, 5, 6, 7 , 8, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0};  
uint8_t channel_1_dma_timer_list[26] = {1, 2, 3, 4, 5, 8, 15, 16, 17, 18, 19, 20, 23, 24, 25, 0};     

uint8_t channel_2_dma_timer_list[26] = {1, 2, 3, 4, 5, 8, 15, 20, 23, 24, 25, 0};    

uint8_t channel_3_dma_timer_list[26] = {1, 2, 3, 4, 5, 8, 20, 23, 24, 0};      

uint8_t channel_4_dma_timer_list[26] = {1, 2, 3, 4, 5, 8, 20, 23, 24, 0};         

uint8_t trigger_dma_timer_list[26] = {1, 2, 3, 4, 5, 8, 15, 18, 19, 20, 23, 24, 25, 0};   

uint8_t com_dma_timer_list[26] = {1, 5, 8, 15, 18, 19, 20, 23, 24, 25, 0};  

volatile uint32_t Timer_Update_Flag;    
volatile uint32_t Timer_Update_Times;  

uint8_t Get_Next_Update_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(update_dma_timer_list); i++)
        {
            if (curr_index == update_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return update_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(update_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return update_dma_timer_list[i+1];   
    }
    
} 


uint8_t Get_Next_Channel_1_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(channel_1_dma_timer_list); i++)
        {
            if (curr_index == channel_1_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return channel_1_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(channel_1_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return channel_1_dma_timer_list[i+1];      
    }
    
} 

uint8_t Get_Next_Channel_2_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(channel_2_dma_timer_list); i++)
        {
            if (curr_index == channel_2_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return channel_2_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(channel_2_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return channel_2_dma_timer_list[i+1];       
    }
    
} 


uint8_t Get_Next_Channel_3_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(channel_3_dma_timer_list); i++)
        {
            if (curr_index == channel_3_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return channel_3_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(channel_3_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return channel_3_dma_timer_list[i+1];          
    }
    
}   


uint8_t Get_Next_Channel_4_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(channel_4_dma_timer_list); i++)
        {
            if (curr_index == channel_4_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return channel_4_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(channel_4_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return channel_4_dma_timer_list[i+1];            
    }
    
} 


uint8_t Get_Next_Trigger_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(trigger_dma_timer_list); i++)
        {
            if (curr_index == trigger_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return trigger_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(trigger_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return trigger_dma_timer_list[i+1];   
    }
    
}   


uint8_t Get_Next_COM_DMA_Timer_Index(uint8_t curr_index)
{
    uint8_t timer_index, i;   
    
    if (0 != curr_index )
    {
        for(i = 0; i < sizeof(com_dma_timer_list); i++)
        {
            if (curr_index == com_dma_timer_list[i])
            {
                break; 
            }
        }
    }
    else
    {
        return com_dma_timer_list[0];   
    }
    
    if (i >= (sizeof(com_dma_timer_list) - 1) ) 
    {
        return 0;  
    }
    else
    {
        return com_dma_timer_list[i+1];   
    }
    
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
    htim->Instance->SR = (~BIT0);   
}



void TIMx_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);     
}
void TIMx_Init(TIM_TypeDef *Instance)  
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
    
    p_TIMx_Handler = &DMA_TIM_Handler;  
    
    memset(p_TIMx_Handler, 0, sizeof(DMA_TIM_Handler) ); 
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_DISABLE;    
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP;  
	p_TIMx_Handler->Init.RepetitionCounter = 0;  
	p_TIMx_Handler->Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	p_TIMx_Handler->Init.Period = (TIM_CLOCK_FREQ/1000)*500 - 1;  // 500ms 
	
	TIMx_MSP_Pre_Init(p_TIMx_Handler);      
    
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
	HAL_TIM_ENABLE_IT(p_TIMx_Handler, TIMER_INT_EN_UPD); 
	            
}


uint32_t Period_Value[8] = {9999, 19999};     

void MX_DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();  
    __HAL_RCC_DMA2_CLK_ENABLE();  
    
    NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
    NVIC_EnableIRQ(DMA1_CH0_IRQn);   
    
    NVIC_ClearPendingIRQ(DMA1_CH1_IRQn);
    NVIC_EnableIRQ(DMA1_CH1_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA1_CH2_IRQn);
    NVIC_EnableIRQ(DMA1_CH2_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA1_CH3_IRQn);
    NVIC_EnableIRQ(DMA1_CH3_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA1_CH4_IRQn);
    NVIC_EnableIRQ(DMA1_CH4_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA1_CH5_IRQn);
    NVIC_EnableIRQ(DMA1_CH5_IRQn);   
    
    NVIC_ClearPendingIRQ(DMA1_CH6_IRQn);
    NVIC_EnableIRQ(DMA1_CH6_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA1_CH7_IRQn);
    NVIC_EnableIRQ(DMA1_CH7_IRQn);    
    
    
    NVIC_ClearPendingIRQ(DMA2_CH0_IRQn);
    NVIC_EnableIRQ(DMA2_CH0_IRQn);    
    
    NVIC_ClearPendingIRQ(DMA2_CH1_IRQn);
    NVIC_EnableIRQ(DMA2_CH1_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH2_IRQn);
    NVIC_EnableIRQ(DMA2_CH2_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH3_IRQn);
    NVIC_EnableIRQ(DMA2_CH3_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH4_IRQn);
    NVIC_EnableIRQ(DMA2_CH4_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH5_IRQn);
    NVIC_EnableIRQ(DMA2_CH5_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH6_IRQn);
    NVIC_EnableIRQ(DMA2_CH6_IRQn);  
    
    NVIC_ClearPendingIRQ(DMA2_CH7_IRQn);
    NVIC_EnableIRQ(DMA2_CH7_IRQn);  
    
}

void Timer_Update_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Update_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_UPD);    
    
    HAL_TIMER_Base_Start_DMA(p_TIMx_Handler, (uint32_t)Period_Value, 2);   
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++;
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Update_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Update DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_UPD);    
                HAL_TIMER_Base_Start_DMA(p_TIMx_Handler, (uint32_t)Period_Value, 2);     
                
                Timer_Update_Times = 0;    
            }
		}
	}
}


void Timer_Channel1_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
    printfS("Timer_Channel1_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Channel_1_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC1);    
    timer_instance->CCR1 = timer_instance->ARR/4 - 1;  
    printfS("Timer CCDS : 0x%08x \n", timer_instance->CR2); // check bit3 
    
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC1_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_1);    
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++;
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Channel_1_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Channel1 DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC1);   
                timer_instance->CCR1 = timer_instance->ARR/4 - 1;     
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC1_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }
    
                HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_1);  
                
                Timer_Update_Times = 0;    
            }
		}
	}
}  


void Timer_Channel2_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
    printfS("Timer_Channel2_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Channel_2_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC2);    
    timer_instance->CCR2 = timer_instance->ARR/4 - 1;  
    printfS("Timer CCDS : 0x%08x \n", timer_instance->CR2); // check bit3 
    
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC2_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_2);    
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++;
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Channel_2_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Channel2 DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC2);   
                timer_instance->CCR2 = timer_instance->ARR/4 - 1;     
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC2_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }
    
                HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_2);  
                
                Timer_Update_Times = 0;    
            }
		}
	}
} 


void Timer_Channel3_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
    printfS("Timer_Channel3_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Channel_3_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC3);    
    timer_instance->CCR3 = timer_instance->ARR/4 - 1;  
    printfS("Timer CCDS : 0x%08x \n", timer_instance->CR2); // check bit3 
    
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC3_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_3);    
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++;
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Channel_3_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Channel3 DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC3);   
                timer_instance->CCR3 = timer_instance->ARR/4 - 1;     
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC3_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }
    
                HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_3);  
                
                Timer_Update_Times = 0;    
            }
		}
	}
}  



void Timer_Channel4_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
    printfS("Timer_Channel4_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Channel_4_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC4);    
    timer_instance->CCR4 = timer_instance->ARR/4 - 1;  
    printfS("Timer CCDS : 0x%08x \n", timer_instance->CR2); // check bit3 
    
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC4_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_4);    
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++;
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Channel_4_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Channel4 DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_CC4);   
                timer_instance->CCR4 = timer_instance->ARR/4 - 1;     
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_CC4_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }
    
                HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, TIM_CHANNEL_4);  
                
                Timer_Update_Times = 0;    
            }
		}
	}
}



void Timer_Trigger_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;   
    
    printfS("Timer_Trigger_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_Trigger_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_TRI);         
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_TRIG_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);   
    
    p_TIMx_Handler->Instance->EGR = BIT6;   // Trigger event 
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++; 
            p_TIMx_Handler->Instance->EGR = BIT6;   // Trigger event 
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_Trigger_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer Trigger DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_TRI);    
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_TRIG_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }  
                HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);   
                p_TIMx_Handler->Instance->EGR = BIT6;   // Trigger event 
                
                Timer_Update_Times = 0;    
            }
		}
	}
}  



void Timer_COM_DMA_Test(void)
{
    TIM_TypeDef * timer_instance;   
    uint8_t timer_index;  
    
	Timer_Update_Flag = 0;  
    Timer_Update_Times = 0;   
    
    timer_index = 0;  
    
    printfS("Timer_COM_DMA_Test\n");  
    
	MX_DMA_Init();  
    
    timer_index = Get_Next_COM_DMA_Timer_Index(timer_index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
    
    TIMx_Init(timer_instance);           
    HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_COM);         
    if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_COM_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
    {
        return;    
    }
    
    HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);       
    p_TIMx_Handler->Instance->EGR = BIT5;   // COM event 
	while(1)
	{
		if(Timer_Update_Flag) 
		{		
            printfS("Timer Update Occurs, Timer Index:%d, ARR:%d\n", timer_index, p_TIMx_Handler->Instance->ARR);     
			Timer_Update_Flag = 0;  
            Timer_Update_Times++; 
            p_TIMx_Handler->Instance->EGR = BIT5;   // COM event 
            if (Timer_Update_Times >= 6)
            {
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler); 
                
                timer_index = Get_Next_COM_DMA_Timer_Index(timer_index);  
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index); 
                if ( (0 == timer_index) || (timer_index > 25) ) 
                {
                    printfS("Timer COM DMA Test End, %d\n", timer_index);       
                    return;   
                }
                TIMx_Init(timer_instance);           
                HAL_TIM_ENABLE_DMA(p_TIMx_Handler, TIMER_DMA_EN_COM);    
                if (HAL_DMA_Start_IT(p_TIMx_Handler->hdma[TIM_DMA_COM_INDEX], (uint32_t)Period_Value, (uint32_t)(&(p_TIMx_Handler->Instance->ARR)), 2))
                {
                    return;    
                }  
                HAL_TIMER_Base_Start(p_TIMx_Handler->Instance);   
                p_TIMx_Handler->Instance->EGR = BIT5;   // COM event 
                
                Timer_Update_Times = 0;    
            }
		}
	}
}





