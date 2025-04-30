
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPTIM_HandleTypeDef LPTIM1_Handle;

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    RCC_OscInitTypeDef      RCC_OscInitStruct = {0};
    
    printfS("LPTIM1 Timeout Demo\r\n");
    printfS("\r\n");
    
	BSP_LED_Init();
	
	BSP_LED_On();
	HAL_Delay(500);
	BSP_LED_Off();
	HAL_Delay(500);
	BSP_LED_On();
    
    
    /* Enable RCL */
    RCC_OscInitStruct.OscType = RCC_OSC_TYPE_RCL;

    RCC_OscInitStruct.RCL = ENABLE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        printfS("Osc init failed\r\n");
        while (1);
    }
    
    /* Init LPTIM1 */
	memset((void *)&LPTIM1_Handle, 0, sizeof(LPTIM_HandleTypeDef));
    
    LPTIM1_Handle.Instance                   = LPTIM1;
    LPTIM1_Handle.Init.ClockSource           = LPTIM_CLOCKSOURCE_INTERNAL_RCL;
    LPTIM1_Handle.Init.ClockPrescaler        = LPTIM_CLOCKPRESCALER_DIV1;
    LPTIM1_Handle.Init.TriggerSource         = LPTIM_TRIGSOURCE_SOFTWARE;
    LPTIM1_Handle.Init.CounterSource         = LPTIM_COUNTERSOURCE_INTERNAL;
    LPTIM1_Handle.Init.UpdateMode            = LPTIM_UPDATE_IMMEDIATE;
    
    if (HAL_LPTIM_Init(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 init failed\r\n");
        while (1);
    }
    
    __HAL_RCC_EXTI_CLK_ENABLE();
    if (HAL_EXTI_SetConfigLine(NULL, EXTI_LINE_LPTIM1, EXTI_MODE_IT_RISING) != HAL_OK)
    {
        printfS("EXTI config failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_ConfigCountValue(&LPTIM1_Handle, 0, 65535U, 32767U) != HAL_OK)
    {
        printfS("LPTIM1 config count value failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_EnableIT(&LPTIM1_Handle, LPTIM_IT_CMPM) != HAL_OK)
    {
        printfS("LPTIM1 enable interrupt failed\r\n");
        while (1);
    }
//	while(1);
    
    while(1)
    {
        printfS("LPTIM1 Timeout startup\r\n");
        
        if (HAL_LPTIM_Timeout_Start(&LPTIM1_Handle) != HAL_OK)
        {
            printfS("LPTIM1 Timeout startup failed\r\n");
            while (1);
        }
        
        printfS("LPTIM1 Timeout startup succeeded\r\n");
        printfS("\r\n");
    
        printfS("Enter stop\r\n");
        
		USART1->ISR = 0XFFFFFFFF;
		NVIC_DisableIRQ(USART1_IRQn);
		__set_PRIMASK(1);    // disable interrupt 
		SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick  
		SCB->ICSR = BIT25;   // clear systick pending bit 
		
        HAL_PMU_EnterStop(WAIT_FOR_INT);
        
        BSP_LED_Toggle();
        
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick              
		__set_PRIMASK(0);    // enable interrupt 

		NVIC_EnableIRQ(USART1_IRQn);
        
        printfS("Exit stop\r\n");
        printfS("\r\n");
        
        printfS("LPTIM1 Timeout stop\r\n");
        
        if (HAL_LPTIM_Timeout_Stop(&LPTIM1_Handle) != HAL_OK)
        {
            printfS("LPTIM1 Timeout stop failed\r\n");
            while (1);
        }
        
        printfS("LPTIM1 Timeout stop succeeded\r\n");
        printfS("\r\n");
		HAL_DelayMs(3000);
    
    }
}




