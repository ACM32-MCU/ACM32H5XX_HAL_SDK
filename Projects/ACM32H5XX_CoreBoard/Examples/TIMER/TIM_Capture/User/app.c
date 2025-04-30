/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 

TIM_HandleTypeDef * p_TIMx_Handler = NULL;   
TIM_HandleTypeDef TIM_Capture_Handler;   
 

TIMER_Config  g_timer_config;  

TIMER_Config  *p_timer_config;  

volatile uint32_t Timer_Capture_Flag;       
volatile uint32_t Timer_Capture_Channel_Index;  
volatile uint32_t Timer_Capture_Channel_Curr_Index; 
volatile uint32_t Timer_Capture_Times;    

extern void Capture_GPIO_Config(GPIO_Config *p_gpio_config);  
extern Channels_GPIO_Config timer_gpio_config[25];  

// return channel count of timer 
GPIO_Config * Get_Timer_Channel_GPIO_Config(uint8_t index, uint8_t channel_index) 
{
    GPIO_Config * p_config;  
    
    switch(channel_index) 
    {
        case 1:
        p_config = &(timer_gpio_config[index].ch1);            
        break;
        
        case 2: 
        p_config = &(timer_gpio_config[index].ch2);   
        break; 
        
        case 3: 
        p_config = &(timer_gpio_config[index].ch3);    
        break; 
        
        case 4:  
        p_config = &(timer_gpio_config[index].ch4);    
        break; 

        default: 
        p_config = NULL;   
        break;  
    }
    
    if (NULL != p_config)  
    {
        if (0 == p_config->Port) 
        {
            p_config = NULL;  
        }
    }  
    
    return p_config;  
}



TIMER_Config * Get_Next_Timer_Channel(uint32_t init_timer_index, uint32_t init_timer_channel)
{
    uint32_t timer_index, timer_channel; 
    
    GPIO_Config * p_gpio_config;   
    
    timer_index = init_timer_index; 
    timer_channel = init_timer_channel + 1; 
    
    while(timer_index <= TIMER_INDEX_MAX)  
    {
        while(timer_channel <= TIMER_CHANNEL_INDEX_MAX) 
        {
            p_gpio_config = Get_Timer_Channel_GPIO_Config(timer_index - 1, timer_channel);  
            if (NULL !=  p_gpio_config)  
            {
                goto END_PROCESS;  
            }
            else
            {
                timer_channel++;  
            }
        }
        timer_channel = 1;  
        timer_index++;           
    }
    
    
    return NULL;
        
    END_PROCESS:
    g_timer_config.p_config = p_gpio_config;
    g_timer_config.timer_channel = timer_channel; 
    g_timer_config.timer_index = timer_index; 
    
    return &g_timer_config;       
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




uint32_t timer_capture_value[TIM_CAPTURE_CHANNEL_MAX]; 

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    switch(htim->activeChannel)
    {
        case HAL_TIM_ACTIVE_CHANNEL_1:
        timer_capture_value[0] = htim->Instance->CCR1; 
        Timer_Capture_Flag = 1; 
        Timer_Capture_Channel_Index = 1; 
        break;   
        
        case HAL_TIM_ACTIVE_CHANNEL_2:
        timer_capture_value[1] = htim->Instance->CCR2; 
        Timer_Capture_Flag = 1; 
        Timer_Capture_Channel_Index = 2; 
        break;
        
        case HAL_TIM_ACTIVE_CHANNEL_3: 
        timer_capture_value[2] = htim->Instance->CCR3;  
        Timer_Capture_Flag = 1; 
        Timer_Capture_Channel_Index = 3; 
        break;
        
        case HAL_TIM_ACTIVE_CHANNEL_4: 
        timer_capture_value[3] = htim->Instance->CCR4;    
        Timer_Capture_Flag = 1;  
        Timer_Capture_Channel_Index = 4;  
        break;  
        
        default:
        Timer_Capture_Channel_Index = 0;   
        break;   
    }
}

void Timer_Channels_MSP_Post_Init(GPIO_Config *p_gpio_config) 
{
	 Capture_GPIO_Config(p_gpio_config);   
}


void Timer_Channels_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);    
}



void Timer_Channels_Capture_Init(TIM_TypeDef *Instance, uint32_t capture_channel, GPIO_Config *p_gpio_config)   
{
    uint32_t timer_clock; 
    
    TIM_IC_InitTypeDef Tim_IC_Init_Para;       
    
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
    
    p_TIMx_Handler = &TIM_Capture_Handler;      
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;     
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP;  

    p_TIMx_Handler->Init.RepetitionCounter = 0; 
    
	p_TIMx_Handler->Init.Prescaler = 0;  
    
    if (IS_TIM_32_BITS_INSTANCE(Instance) )
    {
        p_TIMx_Handler->Init.Period = 0xFFFFFFFF;  
    }
    else
    {
        p_TIMx_Handler->Init.Period = 0xFFFF;     
    }  
	
	Timer_Channels_MSP_Pre_Init(p_TIMx_Handler);         
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
    
    
    Tim_IC_Init_Para.TIFilter = 0;    // no filter    
	Tim_IC_Init_Para.ICPolarity = TIM_SLAVE_CAPTURE_ACTIVE_FALLING;         
	Tim_IC_Init_Para.ICPrescaler = 0;  
	Tim_IC_Init_Para.ICSelection = TIM_ICSELECTION_DIRECTTI;     // TI1FP1/TI2FP2  	 
	HAL_TIMER_Capture_Config(p_TIMx_Handler->Instance, &Tim_IC_Init_Para,  capture_channel);   
         
    if (NULL != p_gpio_config)
    {
        Timer_Channels_MSP_Post_Init(p_gpio_config);     
    }
}

uint32_t capture_data[8], capture_delta;   
uint16_t capture16_data[8], capture16_delta;   

void Timer_Capture_Test(void)
{
    TIM_TypeDef * timer_instance;  
         
    uint8_t timer_index, timer_type; 
    uint8_t uart_received_byte;  
    
	Timer_Capture_Flag  = 0;  
    Timer_Capture_Times = 0;  
    
    Timer_Capture_Channel_Index = 0; 
    Timer_Capture_Channel_Curr_Index = 0;    
    timer_index = 1;   
    
    p_timer_config = Get_Next_Timer_Channel(timer_index, Timer_Capture_Channel_Curr_Index);   
    
    if (NULL != p_timer_config)   
    {
        timer_index = p_timer_config->timer_index;                   
        Timer_Capture_Channel_Curr_Index = p_timer_config->timer_channel;  
    }
    else
    {
        printfS("Timer Test End, %d\n", timer_index);      
        return ; 
    }
    printfS("Timer index = %d, Channel index = %d\n", timer_index, Timer_Capture_Channel_Curr_Index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index);              
    Timer_Channels_Capture_Init(timer_instance, Timer_Capture_Channel_Curr_Index - 1, p_timer_config->p_config);      
    HAL_TIM_ENABLE_IT(p_TIMx_Handler, 1 << (Timer_Capture_Channel_Curr_Index) );        
    HAL_TIM_Capture_Start(p_TIMx_Handler->Instance, Timer_Capture_Channel_Curr_Index - 1);         
    
	while(1)
	{
		if(Timer_Capture_Flag) 
		{	
            
            capture_data[Timer_Capture_Times] = timer_capture_value[Timer_Capture_Channel_Index - 1];   
            Timer_Capture_Times++;             
            
            if (Timer_Capture_Times >= 2)
            {
                p_TIMx_Handler->Instance->CR1 &= (~BIT0); 
                HAL_TIM_DISABLE_IT(p_TIMx_Handler, 1 << (Timer_Capture_Channel_Curr_Index) );  
                
                if (IS_TIM_32_BITS_INSTANCE(p_TIMx_Handler->Instance) ) 
                {
                    capture_delta = capture_data[1] - capture_data[0];  
                    printfS("capture_delta, %d, capture_0:%d, capture_1:%d\n", capture_delta, capture_data[0], capture_data[1]); 
                }
                else
                {
                    capture16_data[0] = capture_data[0]; 
                    capture16_data[1] = capture_data[1]; 
                    capture16_delta = capture16_data[1] - capture16_data[0];   
                    printfS("capture_delta, %d, capture_0:%d, capture_1:%d\n", capture16_delta, capture_data[0], capture_data[1]); 
                }
                
                                       
                Timer_Capture_Times = 0;  
                
                p_timer_config = Get_Next_Timer_Channel(timer_index, Timer_Capture_Channel_Curr_Index);    
                
                if (NULL != p_timer_config)   
                {
                    timer_index = p_timer_config->timer_index;                   
                    Timer_Capture_Channel_Curr_Index = p_timer_config->timer_channel;  
                    printfS("Timer index = %d, Channel index = %d\n", timer_index, Timer_Capture_Channel_Curr_Index);    
                }
                else
                {
                    printfS("Timer Test End, %d\n", timer_index);    
                    return;  
                }
                  
                while ( UART_FR_RXFE == (USART1->FR & UART_FR_RXFE) )
                {
                     
                }
                
                uart_received_byte = USART1->DR;  
                printfS("Start Test Next Channel, 0x%02x\n", uart_received_byte);     
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index);                                

                Timer_Channels_Capture_Init(timer_instance, Timer_Capture_Channel_Curr_Index - 1, p_timer_config->p_config);                   
                HAL_TIM_ENABLE_IT(p_TIMx_Handler, 1 << (Timer_Capture_Channel_Curr_Index) );        
                HAL_TIM_Capture_Start(p_TIMx_Handler->Instance, Timer_Capture_Channel_Curr_Index - 1);      
            }
                
			Timer_Capture_Flag = 0;   
               
		}
	}
}  



void Timer_Capture_Inter_Connect_Test(TIM_TypeDef *Instance, uint32_t channel, uint32_t ti_select)
{
    TIM_TypeDef * timer_instance;  
    
	Timer_Capture_Flag  = 0;  
    Timer_Capture_Times = 0;  
                
    Timer_Channels_Capture_Init(Instance, channel, NULL);     
    Instance->TISEL = ti_select; 
    
    if (TIM15 == Instance)
    {
        GPIO_InitTypeDef    GPIO_Timer = {0}; 
    
        RCC->AHB2CKENR |= 0x1FFFF; // GPIOA to GPIOQ   
    
        GPIO_Timer.Mode      = GPIO_MODE_AF_PP;  
        GPIO_Timer.Pull      = GPIO_PULLUP;
        GPIO_Timer.Drive     = GPIO_DRIVE_LEVEL3;
            
        switch(ti_select)
        {
            case 1:      
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;         
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);     
            printfS("TIM2-CH1-PA0\n");   
            break; 
            
            case 2:  
            GPIO_Timer.Pin       = GPIO_PIN_6;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;         
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);  
            printfS("TIM3-CH1-PA6\n");  
            break; 
            
            case 3:  
            GPIO_Timer.Pin       = GPIO_PIN_6;
            GPIO_Timer.Alternate = GPIO_FUNCTION_8;         
            HAL_GPIO_Init(GPIOB, &GPIO_Timer);  
            printfS("TIM4-CH1-PB6\n");    
            break;
            
            case 4: 
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~0xFFU) | 0x19;  
            HAL_Delay(500);  
            printfS("LSE\n");   
            break; 
            
            case 6:
            RCC->CLKOCR |= BIT31;  
            HAL_Delay(500);    
            printfS("MCO2\n");   
            break;
            
            case (1 << 8): // TIM15_CH2_TI2_1   
            GPIO_Timer.Pin       = GPIO_PIN_1;
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;         
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);     
            printfS("TIM2-CH2-PA1\n");  
            break;  
            
            case (2 << 8): // TIM15_CH2_TI2_2  
            GPIO_Timer.Pin       = GPIO_PIN_5;
            GPIO_Timer.Alternate = GPIO_FUNCTION_1;         
            HAL_GPIO_Init(GPIOB, &GPIO_Timer);     
            printfS("TIM3-CH2-PB5\n");    
            break;  
            
            case (3 << 8): // TIM15_CH2_TI2_3   
            GPIO_Timer.Pin       = GPIO_PIN_13;  
            GPIO_Timer.Alternate = GPIO_FUNCTION_2;         
            HAL_GPIO_Init(GPIOD, &GPIO_Timer);     
            printfS("TIM4-CH2-PD13\n");  
            break;  
            
            default:  
            printfS("Invalid\n"); 
            break; 
        }
    }
    
    if (TIM25 == Instance)
    {
        GPIO_InitTypeDef    GPIO_Timer = {0}; 
    
        RCC->AHB2CKENR |= 0x1FFFF; // GPIOA to GPIOQ   
    
        GPIO_Timer.Mode      = GPIO_MODE_AF_PP;  
        GPIO_Timer.Pull      = GPIO_PULLUP;
        GPIO_Timer.Drive     = GPIO_DRIVE_LEVEL3;
            
        switch(ti_select)
        {
            case 1:      
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_4;         
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);     
            printfS("TIM5-CH1-PA0\n");   
            break; 
            
            case 2:  
            GPIO_Timer.Pin       = GPIO_PIN_6;
            GPIO_Timer.Alternate = GPIO_FUNCTION_8;         
            HAL_GPIO_Init(GPIOB, &GPIO_Timer);  
            printfS("TIM4-CH1-PB6\n");  
            break; 
            
            case 3:  
            GPIO_Timer.Pin       = GPIO_PIN_0;
            GPIO_Timer.Alternate = GPIO_FUNCTION_8;         
            HAL_GPIO_Init(GPIOF, &GPIO_Timer);  
            printfS("TIM23-CH1-PF0\n");    
            break;
            
            case 4: 
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~0xFFU) | 0x19;  
            HAL_Delay(500);  
            printfS("LSE\n");   
            break; 
                       
            case (1 << 8): // TIM25_CH2_TI2_1   
            GPIO_Timer.Pin       = GPIO_PIN_1;
            GPIO_Timer.Alternate = GPIO_FUNCTION_4;         
            HAL_GPIO_Init(GPIOA, &GPIO_Timer);     
            printfS("TIM5-CH2-PA1\n");  
            break;  
            
            case (2 << 8): // TIM25_CH2_TI2_2  
            GPIO_Timer.Pin       = GPIO_PIN_1;
            GPIO_Timer.Alternate = GPIO_FUNCTION_8;         
            HAL_GPIO_Init(GPIOF, &GPIO_Timer);     
            printfS("TIM23-CH2-PF1\n");    
            break;  
            
            case (3 << 8): // TIM25_CH2_TI2_3   
            GPIO_Timer.Pin       = GPIO_PIN_12;  
            GPIO_Timer.Alternate = GPIO_FUNCTION_10;         
            HAL_GPIO_Init(GPIOF, &GPIO_Timer);     
            printfS("TIM24-CH2-PF12\n");  
            break;  
            
            default:  
            printfS("Invalid\n"); 
            break; 
        }
    }    
    
    
    if (TIM16 == Instance)
    {
        GPIO_InitTypeDef    GPIO_Timer = {0}; 
    
        RCC->AHB2CKENR |= 0x1FFFF; // GPIOA to GPIOQ   
    
        GPIO_Timer.Mode      = GPIO_MODE_AF_PP;  
        GPIO_Timer.Pull      = GPIO_PULLUP;
        GPIO_Timer.Drive     = GPIO_DRIVE_LEVEL3;
            
        switch(ti_select)
        {
            case 1:         
            printfS("LSI\n");   
            break; 
            
            case 2:  
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~0xFFU) | 0x19;  
            HAL_Delay(500);  
            printfS("LSE\n"); 
            break; 
            
            case 3:  
            printfS("RTC-Wakeup\n");      
            break;  
            
            case (6 << 0):    
            RCC->XTHCR |= BIT0;   
            HAL_Delay(500); 
            printfS("HSE-Div32, 0x%08x\n", RCC->XTHCR);    
            break;  
            
            default:  
            printfS("Invalid\n"); 
            break; 
        }
    }   
    
    if (TIM_CHANNEL_1 == channel)
    {
        HAL_TIM_ENABLE_IT(p_TIMx_Handler, BIT1 );   
    } 
    else if (TIM_CHANNEL_2 == channel)
    {
        HAL_TIM_ENABLE_IT(p_TIMx_Handler, BIT2 );   
    }
    else if (TIM_CHANNEL_3 == channel)
    {
        HAL_TIM_ENABLE_IT(p_TIMx_Handler, BIT3 );   
    }
    else if (TIM_CHANNEL_4 == channel)
    {
        HAL_TIM_ENABLE_IT(p_TIMx_Handler, BIT4 );   
    }
    
    HAL_TIM_Capture_Start(Instance, channel);          
    
	while(1)
	{
		if(Timer_Capture_Flag) 
		{	          
            capture_data[Timer_Capture_Times] = timer_capture_value[Timer_Capture_Channel_Index - 1];   
            Timer_Capture_Times++;             
            
            if (Timer_Capture_Times >= 3)
            {
                p_TIMx_Handler->Instance->CR1 &= (~BIT0); 
                HAL_TIM_DISABLE_IT(p_TIMx_Handler, 1 << (Timer_Capture_Channel_Curr_Index) );  
                
                if (IS_TIM_32_BITS_INSTANCE(p_TIMx_Handler->Instance) ) 
                {
                    capture_delta = capture_data[2] - capture_data[1];  
                    printfS("capture_delta, %d, capture_0:%d, capture_1:%d\n", capture_delta, capture_data[1], capture_data[2]); 
                }
                else
                {
                    capture16_data[1] = capture_data[1]; 
                    capture16_data[2] = capture_data[2]; 
                    capture16_delta = capture16_data[2] - capture16_data[1];   
                    printfS("capture_delta, %d, capture_0:%d, capture_1:%d\n", capture16_delta, capture_data[1], capture_data[2]); 
                }
                                                     
                Timer_Capture_Times = 0;                                  
            }
                
			Timer_Capture_Flag = 0;   
               
		}
	}
} 





