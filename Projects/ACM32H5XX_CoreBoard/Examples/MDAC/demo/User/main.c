
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
    bool debug=false;
      
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);
    HAL_Init();
    if(debug)
    {
        HAL_PMU_Init();
        HAL_PMU_EnterStandbyMode(WAIT_FOR_INT);
    }

    BSP_UART_Init(USART1, 115200); 

    printfS("Firmware compiled on %s %s \n", __DATE__, __TIME__);
    printfS("----------------------------------------------------------------------------------------\r\n");    
    
    MDAC_test();
    
	while(1)
	{
	}
}


