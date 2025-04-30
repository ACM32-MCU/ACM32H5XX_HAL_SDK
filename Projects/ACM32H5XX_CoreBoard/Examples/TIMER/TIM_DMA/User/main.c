/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 
#include "app.h"   

/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
	HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);      
    
    BSP_UART_Init(USART1, 115200);  
	
	printfS("\r\n\r\n");
	printfS("system startup\r\n");
	
	get_reset_source();
    BSP_MCO_Init(RCC_MCO_HCLK, ENABLE, 10);
    
	printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());


	Timer_Update_DMA_Test();  
    
    Timer_Channel1_DMA_Test();     
    
    Timer_Channel2_DMA_Test(); 
    
    Timer_Channel3_DMA_Test(); 
    
    Timer_Channel4_DMA_Test();  
    
    Timer_Trigger_DMA_Test();     
    Timer_COM_DMA_Test();  

    
    while(1)
    {
        
    }
}














