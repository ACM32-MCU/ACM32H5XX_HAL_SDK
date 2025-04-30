/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 

TIM_HandleTypeDef * p_TIMx_Handler = NULL;   
TIM_HandleTypeDef TIM_Output_Handler;   
 

TIMER_Config  g_timer_config;  

TIMER_Config  *p_timer_config;  

#define TIM_CLOCK_FREQ            (10000000U)  // TIMER INTERNAL CLK = 10MHz 
#define TIM16_PWM_OUTPUT_FREQ     (10000U)   // PWM output frequency = 10KHz 
#define TIM32_PWM_OUTPUT_FREQ     (100U)   // PWM output frequency = 100Hz   

volatile uint32_t Timer_Output_Compare_Flag;       
volatile uint32_t Timer_Output_Channel_Index;  
volatile uint32_t Timer_Output_Channel_Curr_Index; 
  

extern void PWM_GPIO_Config(GPIO_Config *p_gpio_config);  
extern Channels_GPIO_Config timer_gpio_config[25];  
extern Channels_GPIO_Config timer_pwm_complement_gpio_config[10];    
extern GPIO_Config  timer_break_gpio_config[10];  

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



void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) 
{
    switch(htim->activeChannel)
    {
        case HAL_TIM_ACTIVE_CHANNEL_1:
        Timer_Output_Compare_Flag = 1; 
        Timer_Output_Channel_Index = 1; 
        break;   
        
        case HAL_TIM_ACTIVE_CHANNEL_2:
        Timer_Output_Compare_Flag = 1; 
        Timer_Output_Channel_Index = 2; 
        break;
        
        case HAL_TIM_ACTIVE_CHANNEL_3: 
        Timer_Output_Compare_Flag = 1; 
        Timer_Output_Channel_Index = 3; 
        break;
        
        case HAL_TIM_ACTIVE_CHANNEL_4: 
        Timer_Output_Compare_Flag = 1;  
        Timer_Output_Channel_Index = 4;  
        break;  
        
        default:
        Timer_Output_Channel_Index = 0;   
        break;   
    }
}

GPIO_Config * Timer_Get_Channel_Complementary_GPIO_Config(TIM_TypeDef *Instance, uint32_t output_channel)   
{
    GPIO_Config * p_gpio_configuration;  
    uint32_t tim_base_addr;   
    
    tim_base_addr = (uint32_t)Instance;  
    p_gpio_configuration = NULL;  
    
    switch(tim_base_addr)  
    {
        case TIM1_BASE_ADDR: 
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[0].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[0].ch2); 
            break; 
            
            case TIM_CHANNEL_3: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[0].ch3); 
            break; 
            
            case TIM_CHANNEL_4:
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[0].ch4);   
            break;
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;   
        
        case TIM8_BASE_ADDR:
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[1].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[1].ch2); 
            break; 
            
            case TIM_CHANNEL_3: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[1].ch3); 
            break; 
            
            case TIM_CHANNEL_4:
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[1].ch4);   
            break;
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        case TIM15_BASE_ADDR:
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[2].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[2].ch2); 
            break; 
            
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM16_BASE_ADDR:
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[3].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[3].ch2); 
            break;  
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        case TIM17_BASE_ADDR:
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[4].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[4].ch2); 
            break;  
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;
        
        case TIM18_BASE_ADDR: 
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[5].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[5].ch2); 
            break;  
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM19_BASE_ADDR: 
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[6].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[6].ch2); 
            break;  
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;
        
        case TIM20_BASE_ADDR: 
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[7].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[7].ch2); 
            break; 
            
            case TIM_CHANNEL_3: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[7].ch3); 
            break; 
            
            case TIM_CHANNEL_4:
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[7].ch4);   
            break;
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM25_BASE_ADDR:
        switch(output_channel)
        {
            case TIM_CHANNEL_1: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[8].ch1); 
            break;  
            
            case TIM_CHANNEL_2: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[8].ch2); 
            break; 
            
            case TIM_CHANNEL_3: 
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[8].ch3); 
            break; 
            
            case TIM_CHANNEL_4:
            p_gpio_configuration = &(timer_pwm_complement_gpio_config[8].ch4);   
            break;
            
            default:
            return NULL;  
        }
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        default:  
        return NULL;   
        break;  
        
    }
    
    return p_gpio_configuration;  
}




GPIO_Config * Timer_Get_Channel_Break_GPIO_Config(TIM_TypeDef *Instance)   
{
    GPIO_Config * p_gpio_configuration;  
    uint32_t tim_base_addr;   
    
    tim_base_addr = (uint32_t)Instance;  
    p_gpio_configuration = NULL;  
    
    switch(tim_base_addr)  
    {
        case TIM1_BASE_ADDR: 
        p_gpio_configuration = &timer_break_gpio_config[0];  
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;   
        
        case TIM8_BASE_ADDR: 
        p_gpio_configuration = &timer_break_gpio_config[1];   
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        case TIM15_BASE_ADDR:  
        p_gpio_configuration = &timer_break_gpio_config[2];    
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM16_BASE_ADDR:  
        p_gpio_configuration = &timer_break_gpio_config[3];  
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        case TIM17_BASE_ADDR:  
        p_gpio_configuration = &timer_break_gpio_config[4];    
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;
        
        case TIM18_BASE_ADDR:  
        p_gpio_configuration = &timer_break_gpio_config[5];     
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM19_BASE_ADDR:    
        p_gpio_configuration = &timer_break_gpio_config[6];    
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;
        
        case TIM20_BASE_ADDR:   
        p_gpio_configuration = &timer_break_gpio_config[7];      
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break; 
        
        case TIM25_BASE_ADDR:   
        p_gpio_configuration = &timer_break_gpio_config[8];      
        if (0 == p_gpio_configuration->Port)  
        {
            return NULL; 
        } 
        break;  
        
        default:  
        return NULL;   
        break;  
        
    }
    
    return p_gpio_configuration;  
}



void Timer_Channels_MSP_Post_Init(GPIO_Config *p_gpio_config) 
{
	 PWM_GPIO_Config(p_gpio_config);   
}

void Timer_Channels_Complement_MSP_Post_Init(GPIO_Config *p_gpio_config) 
{
	 PWM_GPIO_Config(p_gpio_config);   
}

void Timer_Channels_Break_MSP_Post_Init(GPIO_Config *p_gpio_config) 
{
	 PWM_GPIO_Config(p_gpio_config);   
}



void Timer_Channels_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	 HAL_TIMER_MSP_Init(htim);    
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

void Generate_System_Break(uint32_t type)
{
    uint32_t addr; 
    
    printfS("Generate_System_Break:%d\n", type);  
    
    switch(type)
    {
        case 0 :  
        SYSCFG->SYSCR |= BIT0;  
        *((uint32_t *)0xffffffff) = 0;    //CPU Lockup 
        break;  
        
        case 1:  // LVD under threhold               
        __HAL_RCC_PMU_CLK_ENABLE();    
        RCC->CCR2 &= (~BIT20); // Filter clock selection PCLK1/32
    
        PMU->CTRL1 = 0x090A;   // enable filter, 64 PCLK1/32, 2.73V     
        HAL_SimpleDelay(100); 
        PMU->CTRL1 |= (BIT0);  // enable lvd
        HAL_Delay(2); // delay 
        
        SYSCFG->SYSCR |= BIT2;         
        break;  
                
        case 2:  // SRAM1 Check Error 
        SYSCFG->SYSCR |= BIT9;  
        addr = 0x20037000;  
        while(addr < 0x20037800)
        {  
            printfS("SRAM1,addr=0x%08x, data=0x%08x\n", addr, *(uint32_t *)addr);    
            addr = addr + 4; 
             
            if (SYSCFG->RAMECCSR & BIT1)
            {
                printfS("SRAM1 ECCSR:0x%08x\n", SYSCFG->RAMECCSR);   
                break;  
            }
        }       
        
        break;  
        
        case 3:  // SRAM3 Check Error   
        SYSCFG->SYSCR |= BIT10;   
        addr = 0x20058000;  
        while(addr < 0x20058800)
        {  
            printfS("SRAM3,addr=0x%08x, data=0x%08x\n", addr, *(uint32_t *)addr);  
            addr = addr + 4; 
             
            if (SYSCFG->RAMECCSR & BIT5)  
            {
                printfS("SRAM3 ECCSR:0x%08x\n", SYSCFG->RAMECCSR);   
                break;  
            }
        }   
        break;    
        
        case 4:  // Backup SRAM Check Error   
        __HAL_RCC_BKPSRAM_CLK_ENABLE();   
        
        SYSCFG->SYSCR |= BIT11;  
        
        addr = 0x40036400;  
        while(addr < 0x40037200)
        {  
            printfS("Backup SRAM,addr=0x%08x, data=0x%08x\n", addr, *(uint32_t *)addr);  
            addr = addr + 4; 
             
            if (SYSCFG->RAMECCSR & BIT9)  
            {
                printfS("Backup SRAM ECCSR:0x%08x\n", SYSCFG->RAMECCSR);      
                break;  
            }
        }  
        break;  
        
        default: 
        break;   
 
    }
    
}

void Timer_Channels_PWM_Init(TIM_TypeDef *Instance, uint32_t output_channel, GPIO_Config *p_gpio_config)   
{
    uint32_t timer_clock; 
    
    GPIO_Config * p_complementary_gpio_config;  
    GPIO_Config * p_break_gpio_config;
    
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
    
    p_TIMx_Handler = &TIM_Output_Handler;      
    
	p_TIMx_Handler->Instance = Instance;    
	p_TIMx_Handler->Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;     
	p_TIMx_Handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	p_TIMx_Handler->Init.CounterMode = TIM_COUNTERMODE_UP;  

    p_TIMx_Handler->Init.RepetitionCounter = 0; 
       
    if (IS_TIM_32_BITS_INSTANCE(Instance) )
    {
        p_TIMx_Handler->Init.Prescaler = timer_clock/TIM_CLOCK_FREQ - 1; 
        p_TIMx_Handler->Init.Period = TIM_CLOCK_FREQ/TIM32_PWM_OUTPUT_FREQ - 1;  // 100Hz 
    }
    else
    {
        p_TIMx_Handler->Init.Prescaler = timer_clock/TIM_CLOCK_FREQ - 1;       
        p_TIMx_Handler->Init.Period = TIM_CLOCK_FREQ/TIM16_PWM_OUTPUT_FREQ - 1; // 10KHz       
    }  
	
	Timer_Channels_MSP_Pre_Init(p_TIMx_Handler);         
	HAL_TIMER_Base_Init(p_TIMx_Handler);    
      
	Tim_OC_Init_Para.OCMode = OUTPUT_MODE_PWM1;  
	Tim_OC_Init_Para.OCIdleState = OUTPUT_IDLE_STATE_0;
	Tim_OC_Init_Para.OCNIdleState = OUTPUT_IDLE_STATE_0;      
	Tim_OC_Init_Para.OCPolarity = OUTPUT_POL_ACTIVE_HIGH;  
	Tim_OC_Init_Para.OCNPolarity = OUTPUT_POL_ACTIVE_HIGH;    
	Tim_OC_Init_Para.OCFastMode =  OUTPUT_FAST_MODE_DISABLE;  
	Tim_OC_Init_Para.Pulse = (p_TIMx_Handler->Init.Period + 1)/2;// 50% duty cycle 
    
//    Tim_OC_Init_Para.Pulse = 0;// 0% duty cycle , PWM1+UP
//    Tim_OC_Init_Para.Pulse = p_TIMx_Handler->Init.Period + 1;// 100% duty cycle, PWM1+UP
//    
//    Tim_OC_Init_Para.Pulse = 0;// 0% duty cycle , PWM1+DOWN
//    Tim_OC_Init_Para.Pulse = p_TIMx_Handler->Init.Period;// 100% duty cycle, PWM1+DOWN   
    

	HAL_TIMER_Output_Config(p_TIMx_Handler->Instance, &Tim_OC_Init_Para, output_channel);  
    
	Timer_Channels_MSP_Post_Init(p_gpio_config);   
    
    if (IS_TIM_BREAK_INSTANCE(p_TIMx_Handler->Instance) ) 
    {
        TIM_BreakDeadTimeConfigTypeDef  sBreakDeadTimeConfig; 
        TIMEx_BreakInputConfigTypeDef sBreakInputConfig; 
        
        p_complementary_gpio_config = Timer_Get_Channel_Complementary_GPIO_Config(p_TIMx_Handler->Instance, output_channel);  
        
        p_break_gpio_config = Timer_Get_Channel_Break_GPIO_Config(p_TIMx_Handler->Instance);   
        
        if (NULL == p_complementary_gpio_config) 
        {
            return;   
        }
        
        #ifdef TIM_BREAK_FEATURE_ENABLE          
        sBreakInputConfig.Enable = TIM_BREAK_ENABLE; 
        sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;  //Break signal is valid when input high level 
        #ifdef BREAK_SYSTEM_EVENT  
        sBreakInputConfig.Source = TIM_BREAKINPUTSOURCE_SYSTEM;   
        #else        
        sBreakInputConfig.Source = TIM_BREAKINPUTSOURCE_BKIN;  
        #endif  
        HAL_TIMEx_ConfigBreakInput(p_TIMx_Handler, &sBreakInputConfig);   
        
        if (TIM_BREAKINPUTSOURCE_BKIN == sBreakInputConfig.Source)
        {
            Timer_Channels_Break_MSP_Post_Init(p_break_gpio_config); 
        }
        else if (TIM_BREAKINPUTSOURCE_COMP1 == sBreakInputConfig.Source)
        {
            COMP1_Init();  
            printfS("COMP1->CR: 0x%x\n", COMP1->CR);    
        }
        else
        {                  
            printfS("System Event, Please Config SYSCFG->SYSCR\n");            
        }
        
        sBreakDeadTimeConfig.AutomaticOutput = TIM_BDTR_AOE; 
        sBreakDeadTimeConfig.BreakFilter = BREAK_INPUT_FILTER_LEVEL(0); 
        sBreakDeadTimeConfig.BreakPolarity = TIM_BDTR_ACTIVE_LOW;  
        sBreakDeadTimeConfig.BreakState = TIM_BDTR_BKE;     
        sBreakDeadTimeConfig.DeadTime = TIM_BDTR_DTG_1 | TIM_BDTR_DTG_0;  
        sBreakDeadTimeConfig.LockLevel = TIM_BDTR_LOCK_0;  
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_BDTR_OSSI_OFF_STATE;  
        sBreakDeadTimeConfig.OffStateRunMode = TIM_BDTR_OSSR_OFF_STATE;       
        HAL_TIMEx_ConfigBreakDeadTime(p_TIMx_Handler, &sBreakDeadTimeConfig);     
        Timer_Channels_Complement_MSP_Post_Init(p_complementary_gpio_config);   \
        printfS("BDTR:0x%x\n", p_TIMx_Handler->Instance->BDTR);     
        #else 
        sBreakDeadTimeConfig.AutomaticOutput = TIM_BDTR_AOE; 
        sBreakDeadTimeConfig.BreakFilter = BREAK_INPUT_FILTER_LEVEL(0); 
        sBreakDeadTimeConfig.BreakPolarity = TIM_BDTR_ACTIVE_LOW;
        sBreakDeadTimeConfig.BreakState = TIM_BDTR_BREAK_DISABLE;       
        sBreakDeadTimeConfig.DeadTime = TIM_BDTR_DTG_7 | TIM_BDTR_DTG_6 | TIM_BDTR_DTG_5;  
        sBreakDeadTimeConfig.LockLevel = TIM_BDTR_LOCK_0;  
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_BDTR_OSSI_OFF_STATE;  
        sBreakDeadTimeConfig.OffStateRunMode = TIM_BDTR_OSSR_OFF_STATE;       
        HAL_TIMEx_ConfigBreakDeadTime(p_TIMx_Handler, &sBreakDeadTimeConfig);     
        Timer_Channels_Complement_MSP_Post_Init(p_complementary_gpio_config);   
        #endif 
            
    }
      
}


void Timer_Output_Test(void)  
{
    TIM_TypeDef * timer_instance;  
         
    uint8_t timer_index, timer_type; 
    uint8_t uart_received_byte;  
    
	Timer_Output_Compare_Flag  = 0;  
    
    Timer_Output_Channel_Index = 0; 
    Timer_Output_Channel_Curr_Index = 0;     
    timer_index = 1;     
    
    p_timer_config = Get_Next_Timer_Channel(timer_index, Timer_Output_Channel_Curr_Index);   
    
    if (NULL != p_timer_config)   
    {
        timer_index = p_timer_config->timer_index;                   
        Timer_Output_Channel_Curr_Index = p_timer_config->timer_channel;  
    }
    else
    {
        printfS("Timer Test End, %d\n", timer_index);      
        return ; 
    }
    printfS("Timer index = %d, Channel index = %d\n", timer_index, Timer_Output_Channel_Curr_Index);  
    timer_instance = Convert_Timer_Index_Timer_Instance(timer_index);              
    Timer_Channels_PWM_Init(timer_instance, Timer_Output_Channel_Curr_Index - 1, p_timer_config->p_config);      
    HAL_TIM_ENABLE_IT(p_TIMx_Handler, 1 << (Timer_Output_Channel_Curr_Index) );               
    HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, Timer_Output_Channel_Curr_Index - 1);    
    
    #ifdef BREAK_SYSTEM_EVENT  
    Generate_System_Break(4);   
    #endif  
    
	while(1)
	{
		if(Timer_Output_Compare_Flag) 
		{	                         
                printfS("Timer Output Interrupt, timer_index:%d, channel:%d\n", timer_index, Timer_Output_Channel_Index);    
                
                Timer_Output_Channel_Curr_Index = Timer_Output_Channel_Index;
                             
                while ( UART_FR_RXFE == (USART1->FR & UART_FR_RXFE) )
                {
                     
                }
                // check output waveform 
                p_TIMx_Handler->Instance->CR1 &= (~BIT0); 
                p_TIMx_Handler->Instance->CCER &= (~(BIT0|BIT2|BIT4|BIT6|BIT8|BIT10|BIT12|BIT16|BIT20));  
                HAL_TIM_DISABLE_IT(p_TIMx_Handler, 1 << (Timer_Output_Channel_Curr_Index) );  
                HAL_TIMER_Base_MspDeInit(p_TIMx_Handler);    
                
                p_timer_config = Get_Next_Timer_Channel(timer_index, Timer_Output_Channel_Curr_Index);    
                
                if (NULL != p_timer_config)   
                {
                    timer_index = p_timer_config->timer_index;                   
                    Timer_Output_Channel_Curr_Index = p_timer_config->timer_channel;  
                    printfS("Timer index = %d, Channel index = %d\n", timer_index, Timer_Output_Channel_Curr_Index);    
                }
                else
                {
                    printfS("Timer Test End, %d\n", timer_index);    
                    return;  
                }
                uart_received_byte = USART1->DR;  
                printfS("Start Test Next Channel, 0x%02x\n", uart_received_byte);     
                timer_instance = Convert_Timer_Index_Timer_Instance(timer_index);                                

                Timer_Channels_PWM_Init(timer_instance, Timer_Output_Channel_Curr_Index - 1, p_timer_config->p_config);                   
                HAL_TIM_ENABLE_IT(p_TIMx_Handler, 1 << (Timer_Output_Channel_Curr_Index) );           
                HAL_TIM_PWM_Output_Start(p_TIMx_Handler->Instance, Timer_Output_Channel_Curr_Index - 1);    
                
                Timer_Output_Compare_Flag = 0;                 
		}
	}
}  





