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

    SystemClock_Config(SYSCLK_220M_SRC_RCH, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);
//    RCC->AHB1CKENR = 0xffffffff;
//    RCC->AHB2CKENR = 0xffffffff;
//    RCC->AHB3CKENR = 0xffffffff;
//    RCC->APB1CKENR1 = 0xffffffff;
//    RCC->APB1CKENR2 = 0xffffffff;
//    RCC->APB2CKENR = 0xffffffff;
//    RCC->APB3CKENR = 0xffffffff;
//    RCC->APB4CKENR = 0xffffffff;
    
    HAL_DWT_Init();
    BSP_UART_Init(USART1, 115200);    
  
    printfS("\r\n\r\n******** ACM32H5XX MCU is runing ********\r\n");
    printfS("HCK: %u, PCLK1: %u\r\n", HAL_RCC_GetHCLKFreq(),HAL_RCC_GetPCLK1Freq()); 
    get_reset_source();
    printfS("\r\n");
    
    BSP_MCO_Init(RCC_MCO_RCH, ENABLE, 1);

   APP_Test();
   

   while(1);



}

