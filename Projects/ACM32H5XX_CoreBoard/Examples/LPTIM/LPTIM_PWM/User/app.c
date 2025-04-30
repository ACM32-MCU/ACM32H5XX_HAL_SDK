
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPTIM_HandleTypeDef LPTIM1_Handle;

volatile uint32_t g_cmp_complete;
volatile uint32_t g_arr_complete;

volatile uint32_t g_Inc;
volatile uint32_t g_Pulse;
volatile uint32_t g_MaxPulse;
volatile uint32_t g_Period;

static void LPTIM1_CompareWriteCallback(LPTIM_HandleTypeDef *hlptim);
static void LPTIM1_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim);
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
    
    printfS("LPTIM1 PWM Demo\r\n");
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
    LPTIM1_Handle.Init.CounterSource         = LPTIM_COUNTERSOURCE_INTERNAL;
    LPTIM1_Handle.Init.UpdateMode            = LPTIM_UPDATE_ENDOFPERIOD;
    
    LPTIM1_Handle.CompareWriteCallback       = LPTIM1_CompareWriteCallback;
    LPTIM1_Handle.CompareMatchCallback       = LPTIM1_CompareMatchCallback;
    LPTIM1_Handle.AutoReloadMatchCallback    = LPTIM1_AutoReloadMatchCallback;
    
    if (HAL_LPTIM_Init(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 init failed\r\n");
        while (1);
    }
    

    g_Period = HAL_RCC_GetPCLK1Freq() / 2000;
    g_Inc = g_Period / 10;
    g_MaxPulse = g_Period - 5;
    g_Pulse = g_MaxPulse - g_Inc;
    
    if (HAL_LPTIM_ConfigCountValue(&LPTIM1_Handle, 0, g_Period, g_MaxPulse) != HAL_OK)
    {
        printfS("LPTIM1 PWM config failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_EnableIT(&LPTIM1_Handle, LPTIM_IT_ARRM) != HAL_OK)
    {
        printfS("LPTIM1 PWM enable interrupt failed\r\n");
        while (1);
    }
    
    printfS("LPTIM1 PWM startup\r\n");
    
    if (HAL_LPTIM_PWM_Start(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 PWM startup failed\r\n");
        while (1);
    }
    printfS("LPTIM1 PWM startup succeeded\r\n");
    printfS("LPTIM1 PWM Output\r\n");
    
    while(1)
    {
        BSP_LED_Toggle();
        HAL_Delay(500);
    }
}


static void LPTIM1_CompareWriteCallback(LPTIM_HandleTypeDef *hlptim)
{
}

static void LPTIM1_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
}

static void LPTIM1_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    if (g_Pulse < g_Inc)
        g_Pulse = 5;
    
    __HAL_LPTIM_SET_COMPARE(hlptim, g_Pulse);
    
    if (g_Pulse < g_Inc)
        g_Pulse = g_MaxPulse;
    else
        g_Pulse -= g_Inc;
}

