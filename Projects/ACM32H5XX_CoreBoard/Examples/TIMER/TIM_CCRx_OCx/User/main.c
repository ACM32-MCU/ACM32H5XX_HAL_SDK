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
    
    BSP_UART_Init(USART1, 115200);  // Use Normal BIT file, PA9 = JP15-3, PA10 = JP15-5
	

	printfS("system startup\r\n");
	
	get_reset_source();
    BSP_MCO_Init(RCC_MCO_HCLK, ENABLE, 10);   
    
	printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
	printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
	printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
	printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
	printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());

    Timer_OC_Update_Test();  
    
    while(1)
    {
        
    }
}














