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
  
    printfS("\r\n\r\n******** ACM32H5XX MCU is runing ********\r\n");
    printfS("Build date£º%s,%s\r\n",__DATE__, __TIME__);    
    printfS("HCK: %u, PCLK1: %u\r\n", (int)HAL_RCC_GetHCLKFreq(),(int)HAL_RCC_GetPCLK1Freq()); 
    get_reset_source();
    printfS("\r\n");  
    
    BSP_MCO_Init(RCC_MCO_RCH, ENABLE, 1);


   APP_Test();
   
   while(1)
   {
       
   }



}

