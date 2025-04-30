/******************************************************************************
*@file  : main.c
*@brief : This file provides I2C Master Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/


#include "app.h"  

int main(void)  
{
	HAL_Init();
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, 4, 1, 1, 1);
    
    BSP_UART_Init(USART1, 115200);
    
    //BSP_PB_Init();
	printfS("----------------------------------------------------------------------------------------\r\n");	
	 /* 
        TEST_MASTER_COMM,
        TEST_MASTER_IT,
        TEST_MASTER_EEPROM 
    */ 
    I2C_Master_Test(TEST_MASTER_EEPROM);
	
	while(1)
	{
        
	}	
}


