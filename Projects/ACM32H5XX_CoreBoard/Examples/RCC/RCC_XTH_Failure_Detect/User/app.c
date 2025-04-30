/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


volatile  uint8_t xth_css = 0;  


/******************************************************************************
*@brief : Deinit XTH OSC_IN and OSC_OUT  
*@param : none
*@return: none
******************************************************************************/
void XTH_GPIO_DeInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOH_CLK_ENABLE();  

    GPIO_InitStruct.Pin       = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;  

    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}


/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    printfS("APP_Test\r\n");  
   
    __HAL_RCC_XTH_STOP_ENABLE();     
    RCC->CIR |= RCC_CIR_XTHSDIE;   // Enable Clock failure to generate NMI    
    
    HAL_Delay(2000);
    
    printfS("XTH:0x%08x, RCL:0x%08x, PLL1:0x%08x\r\n", RCC->XTHCR, RCC->STDBYCTRL, RCC->PLL1CR);   
    
    HAL_SimpleDelay(1000);
    
    XTH_GPIO_DeInit(); // Deinit XTH OSC_IN and OSC_OUT to stop XTH 
    
    while(1)
    {
        if (0x1A == xth_css)  
        {
            SystemClock_Config(SYSCLK_220M_SRC_RCH, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);  
            HAL_RCC_GetSysCoreClockFreq();  
            HAL_InitTick(TICK_INT_PRIORITY, TICK_PERIOD_MS);  
            BSP_UART_Init(USART1, 115200);   
            printfS("XTH Failure is Detected!!!\r\n");    
            xth_css = 0;  
            printfS("XTH:0x%08x, RCH:0x%08x, PLL1:0x%08x\r\n", RCC->XTHCR, RCC->RCHCR, RCC->PLL1CR);        
        }
    }
}
