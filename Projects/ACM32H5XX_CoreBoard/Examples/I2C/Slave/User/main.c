/******************************************************************************
*@file  : main.c
*@brief : This file provides I2C Slave Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/
#include "app.h"  
/*********************************************************************************
* Function    : main
* Description : The application entry point.
* Input       : None
* Output      : None
**********************************************************************************/
int main(void)  
{

	HAL_Init();
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, 4, 1, 1, 1);
    
    BSP_UART_Init(USART1, 115200);
    
    //BSP_PB_Init();
	
	printfS("----------------------------------------------------------------------------------------\r\n");	

	/*  
        TEST_SLAVE_POLLING 
        TEST_SLAVE_INTERRUPT 
        TEST_SLAVE_DMA 
    */
    
    I2C_Slave_Test(TEST_SLAVE_POLLING);
 
    
	while(1)
	{
        
	}	
}


