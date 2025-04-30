
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPTIM_HandleTypeDef LPTIM1_Handle;

static void LPTIM1_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim);
static void LPTIM1_RepetitionCounterCallback(LPTIM_HandleTypeDef *hlptim);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    RCC_OscInitTypeDef      RCC_OscInitStruct = {0};
    
    printfS("LPTIM1 Counter Demo\r\n");
    printfS("\r\n");
    
	BSP_LED_Init();
	
	BSP_LED_On();
	HAL_Delay(500);
	BSP_LED_Off();
	HAL_Delay(500);
	BSP_LED_On();

    
    /* Init LPTIM1 */
	memset((void *)&LPTIM1_Handle, 0, sizeof(LPTIM_HandleTypeDef));
    
    LPTIM1_Handle.Instance                   = LPTIM1;
    LPTIM1_Handle.Init.ClockSource           = LPTIM_CLOCKSOURCE_INTERNAL_PCLK;
    LPTIM1_Handle.Init.ClockPrescaler        = LPTIM_CLOCKPRESCALER_DIV128;
    LPTIM1_Handle.Init.TriggerSource         = LPTIM_TRIGSOURCE_SOFTWARE;
    LPTIM1_Handle.Init.CounterSource         = LPTIM_COUNTERSOURCE_INTERNAL;
    LPTIM1_Handle.Init.Input1Source          = LPTIM_INPUT1SOURCE_0;
    LPTIM1_Handle.Init.UpdateMode            = LPTIM_UPDATE_IMMEDIATE;
    
    LPTIM1_Handle.AutoReloadMatchCallback    = LPTIM1_AutoReloadMatchCallback;
    LPTIM1_Handle.RepetitionUpdateCallback   = LPTIM1_RepetitionCounterCallback;
    
    if (HAL_LPTIM_Init(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 init failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_ConfigCountValue(&LPTIM1_Handle, 4U, 0XFFFFU, 0) != HAL_OK)
    {
        printfS("LPTIM1 Counter config failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_EnableIT(&LPTIM1_Handle, LPTIM_IT_REPUE | LPTIM_IT_ARRM) != HAL_OK)
    {
        printfS("LPTIM1 Counter enable interrupt failed\r\n");
        while (1);
    }
    
    printfS("LPTIM1 Counter startup\r\n");
    
    if (HAL_LPTIM_Counter_Start(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 Counter startup failed\r\n");
        while (1);
    }
    
    printfS("LPTIM1 Counter startup succeeded\r\n");
    printfS("\r\n");
    
    while(1);
}

static void LPTIM1_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    BSP_LED_Toggle();
    printfS("LPTIM1 AutoReloadMatch\r\n");
}

static void LPTIM1_RepetitionCounterCallback(LPTIM_HandleTypeDef *hlptim)
{
    BSP_LED_Toggle();
    printfS("LPTIM1 RepetitionCounter\r\n");
}

