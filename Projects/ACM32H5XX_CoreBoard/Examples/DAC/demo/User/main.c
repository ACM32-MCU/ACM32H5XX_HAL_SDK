
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

    printfS("Firmware compiled on %s %s \n", __DATE__, __TIME__);
    printfS("----------------------------------------------------------------------------------------\r\n");
    
    /*
    DAC_Output_Noise,
    DAC_Output_SAWTOOTH,
    DAC_OutPut_Voltage,   
    DAC_OutPut_Sinx,
    DAC_DMA_Output_Sinx,
    DAC_DMADUALDATA_Output_Sinx,
    */

    APP_DAC_Demo(DAC_OutPut_Voltage);
    
	while(1)
	{
	}
}


