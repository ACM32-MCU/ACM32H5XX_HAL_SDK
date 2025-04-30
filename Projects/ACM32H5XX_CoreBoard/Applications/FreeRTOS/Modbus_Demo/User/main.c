/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h"
#include "modbus.h"
#include "bsp_485.h"

extern int errno;

void vApplicationTickHook(void)
{
}


void vApplicationStackOverflowHook(TaskHandle_t xTask,  char *pcTaskName)
{
    printfS("Task %s Stack Overflow\r\n", pcTaskName);
}

static void vPrintTask(void *pvParameters)
{
    for (;;)
    {
        printfS("Current Tick %d\r\n", xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void xRS485Task(void *pvParameters)
{
	modbus_t *ctx;
	char *erro = NULL;
	int rc;
	uint16_t vals[10];
	
	ctx = modbus_new_st_rtu("uart4", 115200, 'N', 8, 1);
	modbus_set_slave(ctx, 3);
	
	rc = modbus_connect(ctx);
	if (rc == -1) {
		//fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
		modbus_free(ctx);
	}

//	for(uint8_t i = 0; i < 100; i++)
//	{
//		modbus_write_bit(ctx,i + 50, i%2);
//		if (rc != 1)
//		{
//			erro = modbus_strerror(errno);
//			printf("%s	 \r\n", erro);
//		}
//		vTaskDelay(pdMS_TO_TICKS(500));   /* 延时10个tick */
//	}
	for (;;) {

		rc = modbus_read_input_registers(ctx, 0, 2, vals);
		if (rc == 2)
		{
		    printf("TEM/HUM Sensor : temp %d.%d, humi %d.%d          \r\n", vals[0]/10, vals[0]%10, vals[1]/10, vals[1]%10);
		}else
		{
			erro = modbus_strerror(errno);
		    printf("%s   \r\n", erro);
		}

		vTaskDelay(pdMS_TO_TICKS(500));	/* 延时500个tick */
	}

	/* For RTU */
	modbus_close(ctx);
	modbus_free(ctx);
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
        printfS("Idle\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
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
	bsp_485_init(USART4, 115200);
    xTaskCreate(vPrintTask, "Print", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	
	xTaskCreate(xRS485Task, "RS485", configMINIMAL_STACK_SIZE*4, NULL, 1, NULL);
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

