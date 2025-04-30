/******************************************************************************
*@file  : main.c
*@brief : This file provides UART Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"  

int main(void)
{
    int tick=0;
    
    HAL_Init();      
    
    SystemClock_Config(SYSCLK_220M_SRC_RCH, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);  
    
    BSP_UART_Init(USART1, 115200);
    
    printfS("SPI7 DIV:%x, SSHIFT:0x%x\r\n",SPI7->BAUD & 0xFFFF, (unsigned int)( (SPI7->RX_CTL& (BIT8|BIT9|BIT24) ) ) );  
    
    HAL_DWT_Init();
   

    //BSP_PB_Init();

    //HAL_NORFLASH_Init();
    APP_Test();
    
    while(1)
    {
        HAL_Delay(1000);
        APP_Test();
        printfS("tick %d\r\n",tick++);
        if(tick % 5 == 0)
        {
            uint32_t reg_addr;
            uint32_t spi_data_write[1] = {0};
            HAL_NORFLASH_EraseBootFlag();
            printfS("erase boot flag\r\n");
            HAL_NORFLASH_ResetMcu();
            #if 0
            spi_data_write[0] = 4;  
            reg_addr = (uint32_t)(&(SPI7->BAUD)); 
            HAL_NORFLASH_ModifySPIControllerParam(reg_addr, spi_data_write, 1);   
            printfS("Read SPI DIV 3nd:%d\n", SPI7->BAUD);
            #endif
        }
    }
}
