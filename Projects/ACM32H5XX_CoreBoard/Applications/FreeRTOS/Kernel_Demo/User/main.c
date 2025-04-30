/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h"

void vApplicationTickHook(void)
{
}


void vApplicationStackOverflowHook(TaskHandle_t xTask,  char *pcTaskName)
{
    printfS("Task %s Stack Overflow\n", pcTaskName);
}

static void vPrintTask(void *pvParameters)
{
    for (;;)
    {
        printfS("Current Tick %d\n", xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void vLedTask(void *pvParameters)
{
    BSP_LED_Init();
    for (;;)
    {
        BSP_LED_Toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void vIdleTask(void *pvParameters)
{
    for (;;)
    {
        printfS("Idle\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    HAL_Init();

    SystemClock_Config(SYSCLK_SELECT, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);

    BSP_UART_Init(USART1, 115200);
    xTaskCreate(vPrintTask, "Print", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vLedTask,   "Led",   configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vIdleTask,  "Idle",   configMINIMAL_STACK_SIZE, NULL, 0, NULL);

    printfS("\r\n\r\n");
    printfS("************************************************************\r\n\r\n");
    printfS("system startup\r\n");

    get_reset_source();

    printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
    printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
    printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
    printfS("************************************************************\r\n\r\n");


    /* Start the scheduler. */
    vTaskStartScheduler();

    while (1);
}

