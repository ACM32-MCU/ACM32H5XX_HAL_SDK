/******************************************************************************
*@file  : main.c
*@brief : This file provides UART Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"  

int main(void)
{
    HAL_Init();  
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    BSP_UART_Init(USART1, 115200); 
    
    printfS("UART1 TXD:PA9 , RXD:PA10\r\n"); 
    
    printfS("HCLK: %u\r\n", HAL_RCC_GetHCLKFreq());
	printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
	printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
	printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
	printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
       
    APP_Test();
    
    while(1)
    {
        
    }

}
