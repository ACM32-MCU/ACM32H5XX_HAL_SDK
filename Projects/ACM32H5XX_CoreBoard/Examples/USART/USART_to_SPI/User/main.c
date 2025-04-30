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
    
    BSP_UART_Init(USART3, 115200); 

    //BSP_PB_Init();

    printfS("----------------------------------------------------------------------------------------\r\n");

    APP_Test();  

    while(1)
    {

    }
}
