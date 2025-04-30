
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPTIM_HandleTypeDef LPTIM1_Handle;

uint32_t g_Flag;

static void LPTIM1_DirectionUpCallback(LPTIM_HandleTypeDef *hlptim);
static void LPTIM1_DirectionDownCallback(LPTIM_HandleTypeDef *hlptim);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t i;
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    RCC_OscInitTypeDef      RCC_OscInitStruct = {0};
    
    printfS("LPTIM1 Encoder Demo\r\n");
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
    LPTIM1_Handle.Init.ClockPrescaler        = LPTIM_CLOCKPRESCALER_DIV1;
    LPTIM1_Handle.Init.TriggerSource         = LPTIM_TRIGSOURCE_SOFTWARE;
    LPTIM1_Handle.Init.CounterSource         = LPTIM_COUNTERSOURCE_EXTERNAL;
    LPTIM1_Handle.Init.UpdateMode            = LPTIM_UPDATE_IMMEDIATE;
    LPTIM1_Handle.Init.Input1Source          = LPTIM_INPUT1SOURCE_0;
    LPTIM1_Handle.Init.Input2Source          = LPTIM_INPUT2SOURCE_0;
    LPTIM1_Handle.Init.CounterPolarity		 = LPTIM_COUNTERPOLARITY_RISING;
    LPTIM1_Handle.DirectionUpCallback        = LPTIM1_DirectionUpCallback;
    LPTIM1_Handle.DirectionDownCallback      = LPTIM1_DirectionDownCallback;
    
    if (HAL_LPTIM_Init(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 init failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_ConfigCountValue(&LPTIM1_Handle, 0, 5000, 0) != HAL_OK)
    {
        printfS("LPTIM1 Encoder config failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_EnableIT(&LPTIM1_Handle, LPTIM_IT_UP | LPTIM_IT_DOWN) != HAL_OK)
    {
        printfS("LPTIM1 Encode enable interrupt failed\r\n");
        while (1);
    }
    
    printfS("LPTIM1 Encoder startup\r\n");
    
    if (HAL_LPTIM_Encoder_Start(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 Encoder startup failed\r\n");
        while (1);
    }
    printfS("LPTIM1 Encoder startup succeeded\r\n");
    printfS("\r\n");
    
    g_Flag = 0;
    while (1);
}

static void LPTIM1_DirectionUpCallback(LPTIM_HandleTypeDef *hlptim)
{
    BSP_LED_On();
    if (g_Flag != 0)
    {
        g_Flag = 0;
        printfS("\r\n");
    }
    printfS("+");
}

static void LPTIM1_DirectionDownCallback(LPTIM_HandleTypeDef *hlptim)
{
    BSP_LED_Off();
    if (g_Flag == 0)
    {
        g_Flag = 1;
        printfS("\r\n");
    }
    printfS("-");
}

