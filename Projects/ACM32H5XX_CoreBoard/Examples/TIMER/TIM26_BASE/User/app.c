/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


#define TIM_CLOCK_FREQ            (11000000)   

TIM_64_HandleTypeDef TIM26_Handler;   
volatile uint32_t Timer_Update_Flag;    

/************************************************************************
 * function   : TIM6_IRQHandler
 * Description: TIM6 Interrupt Handler 
 ************************************************************************/ 
void TIM26_IRQHandler(void)
{
	if (TIM26->SR & BIT8)
	{
		Timer_Update_Flag = 1;  
	}
	
	TIM26->SR = BIT8;   //write 1 to clear hardware flag  
}

void TIM26_MSP_Pre_Init(TIM_64_HandleTypeDef * htim)   
{
    __HAL_RCC_TIM26_CLK_ENABLE(); 
    NVIC_ClearPendingIRQ(TIM26_IRQn);   
    NVIC_EnableIRQ(TIM26_IRQn); 
}




void TIM26_Init(void)  
{ 
    uint32_t timer_clock; 
    timer_clock = HAL_RCC_GetPCLK1Freq();   // TIM26 is on APB1 Bus  
	TIM26_Handler.Instance = TIM26;
	TIM26_Handler.Init.Cnt_Mode = TIM_64_ARR_MODE;    
    
    if (timer_clock%TIM_CLOCK_FREQ)
    {
        printfS("Prescaler is not int value\n");   
        while(1); 
    }
	TIM26_Handler.Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	TIM26_Handler.Init.Period_Low = TIM_CLOCK_FREQ*2 -1;  // Period = 2S   
    TIM26_Handler.Init.Period_High = 0;   
    
    TIM26_Handler.Init.Count_Low = 0;
    TIM26_Handler.Init.Count_High = 0;  
    
    TIM26_Handler.Init.ARR_Preload_En = 1;  // ARR preload enable  
    
    if (TIM26_Handler.Init.Prescaler > 63)
    {
        printfS("Prescaler should <= 63:0x%08x\n", TIM26_Handler.Init.Prescaler);     
        while(1);   
    }    
	
	TIM26_MSP_Pre_Init(&TIM26_Handler);       
	HAL_TIMER_64_Base_Init(&TIM26_Handler);      
    
	HAL_TIMER_64_Enable_Interrupt(TIM26_Handler.Instance);  	
    
    printfS("Timer64 CTRL:0x%08x\n", TIM26->CTRL);    
	HAL_TIMER_64_Base_Start(TIM26_Handler.Instance);  
    
}


void TIM26_Init_FreeRun(void)  
{
    uint32_t timer_clock; 
    
    timer_clock = HAL_RCC_GetPCLK1Freq();   // TIM26 is on APB1 Bus  
	TIM26_Handler.Instance = TIM26;
	TIM26_Handler.Init.Cnt_Mode = TIM_64_FREE_RUNNING_MODE;    
	TIM26_Handler.Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
    
    if (timer_clock%TIM_CLOCK_FREQ)
    {
        printfS("Prescaler is not int value\n");   
        while(1); 
    }
    
	TIM26_Handler.Init.Period_Low =  0;  
    TIM26_Handler.Init.Period_High = 0;   
    
    
    TIM26_Handler.Init.Count_Low = 0; 
    TIM26_Handler.Init.Count_High = 0;  
    
    if (TIM26_Handler.Init.Prescaler > 63)
    {
        printfS("Prescaler should <= 63:0x%08x\n", TIM26_Handler.Init.Prescaler);     
        while(1);   
    }
    
    TIM26_Handler.Init.ARR_Preload_En = 1;  // ARR preload enable  
	
	TIM26_MSP_Pre_Init(&TIM26_Handler);       
	HAL_TIMER_64_Base_Init(&TIM26_Handler);      
    
	HAL_TIMER_64_Enable_Interrupt(TIM26_Handler.Instance);  	
    
    printfS("Timer64 CTRL:0x%08x\n", TIM26->CTRL);    
	HAL_TIMER_64_Base_Start(TIM26_Handler.Instance);  
    
}



void Timer_Update_Test(void)
{
	Timer_Update_Flag = 0;  
	TIM26_Init(); 
    
	while(1)
	{
		if(Timer_Update_Flag) 
		{
			printfS("Timer Update Occurs, ARR: %d\n", TIM26->ARR_LOW);             
			Timer_Update_Flag = 0;   
		}
	}
}


void Timer_ReadCnt_Test(void)
{
    long long cnt; 
    
    TIM26_Init_FreeRun();   
    
	while(1)
	{
		cnt = HAL_TIMER_64_Base_Read_Count(TIM26_Handler.Instance); 
        printfS("Timer Cnt:%lld, ", cnt);    
        HAL_Delay(1000);
	}
}


