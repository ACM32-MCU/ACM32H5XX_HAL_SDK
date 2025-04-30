/******************************************************************************
*@file  : main.c
*@brief : This file provides SPI Slave Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include  "app.h"

int main(void)  
{
    HAL_Init();
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);

    BSP_UART_Init(USART1, 115200);  
    
    //BSP_PB_Init();

    printfS("\r\n\r\n====== Dragon MCU is runing ======\r\n");
    printfS("SysCoreClk: %dHz\r\n", HAL_RCC_GetFCLKFreq());
    printfS("PCLK1     : %dHz\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2     : %dHz\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3     : %dHz\r\n", HAL_RCC_GetPCLK3Freq());
    
    printfS("----------------------------------------------------------------------------------------\r\n");
    
    /*
        TEST_SLAVE_COMM,
        TEST_SLAVE_IT,
        TEST_SLAVE_DMA,
    */

    SPI_Slave_Test(TEST_SLAVE_COMM);

    while(1)
    {

    }
}
