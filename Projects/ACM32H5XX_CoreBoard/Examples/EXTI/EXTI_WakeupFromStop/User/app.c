/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 


static void Enter_LowPowerMode(uint8_t mode);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    printfS("EXTI WakeupFromStop Demo\r\n\r\n");
    
	BSP_LED_Init();
    BSP_PB_Init();
	
	BSP_LED_On();
    HAL_Delay(500);
    
    printfS("Press the key to exit stop mode!\r\n"); 
        
    while(1)
    {
        printfS("\r\n");
        
        Enter_LowPowerMode(PMU_STOP_INT_MODE);
        
        HAL_Delay(500);
    };
}
/******************************************************************************
*@brief : 
*@param : none
*@return: none
******************************************************************************/
void HAL_EXTI_LineCallback(uint32_t Line)
{
	if (Line == EXTI_LINE_0)
	{
		BSP_LED_Toggle();
	}
}

/******************************************************************************
* @brief : Low power test.
* @param : mode: Low power consumption mode selection
* @return: none
******************************************************************************/
static void Enter_LowPowerMode(uint8_t mode)
{
    switch(mode)
    {
        /* sleep?? */
        case PMU_SLEEP_INT_MODE:
            
            printfS("Enter SLEEP WFI mode.\r\n");
        
            __set_PRIMASK(1);
            HAL_SuspendTick();
        
            HAL_PMU_EnterSleep(WAIT_FOR_INT);
        
            HAL_ResumeTick();
            __set_PRIMASK(0);
        
            printfS("Exit SLEEP WFI mode.\r\n"); 
            break;
        
        case PMU_SLEEP_EVENT_MODE:
            
            printfS("Enter SLEEP WFE mode.\r\n");
        
            __set_PRIMASK(1);
            HAL_SuspendTick();
        
            HAL_PMU_EnterSleep(WAIT_FOR_EVENT);
        
            HAL_ResumeTick();
            __set_PRIMASK(0);
        
            printfS("Exit SLEEP WFI mode.\r\n"); 
            break;
        
        /* stop?? */
        case PMU_STOP_INT_MODE:
            
            printfS("Enter STOP WFI mode.\r\n");
        
            HAL_RCC_RCLConfig(ENABLE);
        
            __set_PRIMASK(1);
            HAL_SuspendTick();
        
            HAL_PMU_EnterStop(WAIT_FOR_INT);
        
            HAL_ResumeTick();
            __set_PRIMASK(0);
        
            SystemClock_Config(SYSCLK_SELECT, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT); 
            BSP_UART_Init(USART1, 115200);
        
            printfS("Exit STOP WFI mode.\r\n");
        
            break;
        
        case PMU_STOP_EVENT_MODE:
            
            printfS("Enter STOP WFE mode.\r\n");
        
            HAL_RCC_RCLConfig(ENABLE);
        
            __set_PRIMASK(1);
            HAL_SuspendTick();
        
            HAL_PMU_EnterStop(WAIT_FOR_EVENT);
        
            HAL_ResumeTick();
            __set_PRIMASK(0);
        
            SystemClock_Config(SYSCLK_SELECT, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT); 
            BSP_UART_Init(USART1, 115200);
        
            printfS("Exit STOP WFE mode.\r\n");
            break;

        default : 
            break;
    }
}
