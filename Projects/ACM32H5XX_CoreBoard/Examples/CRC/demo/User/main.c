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
    
    SystemClock_Config(SYSCLK_12M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    BSP_UART_Init(USART1, 115200); 

    //BSP_PB_Init();

    printfS("--------------------------------------------------------------------\r\n");
    
    printfS("UART PINs selected:\r\n");
    printfS("UART1 TXD:PA9 , RXD:PA10\r\n");


    printfS("Modify HAL_UART_MspInit() function in user code if you select other UART PINs\r\n\r\n");
    
    printfS("UART configuration:\r\n");
    printfS("%d, 8-N-1\r\n\r\n", 115200);
        
    APP_CRC16_Test();
    APP_CRC32_Test();
    
    while(1)
    {
        
    }

}
