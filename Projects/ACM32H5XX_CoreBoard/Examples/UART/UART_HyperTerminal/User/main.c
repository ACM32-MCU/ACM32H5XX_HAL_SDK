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

    //BSP_PB_Init();

    printfS("--------------------------------------------------------------------\r\n");
    
    UART1_Init();
    UART2_Init();
    
    printfS("UART PINs selected:\r\n");
    printfS("UART1 TXD:PA9 , RXD:PA10\r\n");
    printfS("UART2 TXD:PD5 , RXD:PD6\r\n");

    printfS("Modify HAL_UART_MspInit() function in user code if you select other UART PINs\r\n\r\n");
    
    printfS("UART1 baudrate cfg: %d\r\n", UART1_Handle.Init.BaudRate);
    printfS("UART2 baudrate cfg: %d\r\n\r\n", UART2_Handle.Init.BaudRate);
    /* 
    Select Mode: 
    TEST_UART1_LOOP, 
    TEST_UART1_IT, 
    TEST_UART1_DMA, 
    TEST_UART2_IT, 
    TEST_UART2_LOOP, 
    */  
    APP_Test(TEST_UART1_LOOP);  

    while(1)
    {

    }
}
