/******************************************************************************
*@file  : main.c
*@brief : This file provides SPI Master Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include  "app.h"

int main(void)  
{
    
    
    HAL_Init();
    
    SystemClock_Config(SYSCLK_200M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);

    __set_PRIMASK(0);
    
    BSP_UART_Init(USART1, 115200); 

    //BSP_PB_Init();

    printfS("----------------------------------------------------------------------------------------\r\n");
    
    /*  
    TEST_MASTER_COMM,
    TEST_MASTER_DMA,
    TEST_MASTER_NOR_FLASH, 
    */
    SPI_Master_Test(TEST_MASTER_COMM);

    while(1)
    {
           
    }
}
