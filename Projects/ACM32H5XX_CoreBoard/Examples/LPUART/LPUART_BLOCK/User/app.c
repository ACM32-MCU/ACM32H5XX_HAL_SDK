
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPUART_HandleTypeDef LPUART1_Handle;

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    RCC_OscInitTypeDef      RCC_OscInitStruct = {0};
    uint8_t data[128];
    
	BSP_LED_Init();
	
	BSP_LED_On();
	HAL_Delay(500);
	BSP_LED_Off();
	HAL_Delay(500);
	BSP_LED_On();
	
	RCC_OscInitStruct.OscType = RCC_OSC_TYPE_RCL;
	RCC_OscInitStruct.RCL = ENABLE;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
    LPUART1_Handle.Instance         = LPUART1;
    LPUART1_Handle.Init.BaudRate    = 4800;
    LPUART1_Handle.Init.WordLength  = LPUART_WORDLENGTH_8B;
    LPUART1_Handle.Init.StopBits    = LPUART_STOPBITS_1B;
    LPUART1_Handle.Init.Parity      = LPUART_PARITY_NONE;
    LPUART1_Handle.Init.Mode        = LPUART_MODE_TXRX;
    LPUART1_Handle.Init.ClockSource = LPUART_CLOCKSOURCE_RCL;
    LPUART1_Handle.Init.WakeupMode  = LPUART_WAKEUPMODE_NONE;

    HAL_LPUART_Init(&LPUART1_Handle);
    
    memset((void *)data, 0, sizeof(data));
    
    while(1)
    {
        printfS("test baudrate:4800\r\n");
        
        printfS("Receive 10 characters within 10 seconds and send the received characters\r\n");
        
        if (HAL_LPUART_Receive(&LPUART1_Handle, data, 10, 100000) != HAL_OK)
        {
            printfS("receive timeout\r\n");
            break;
        }
        
        printfS("receive complete\r\n");
        
        if (HAL_LPUART_Transmit(&LPUART1_Handle, data, 10, 100000) != HAL_OK)
        {
            printfS("transmit timeout\r\n");
            break;
        }
        printfS("transmit complete\r\n");
        printfS("\r\n");
        
    }
    while(1)
    {
        BSP_LED_Toggle();
        HAL_Delay(500);
    }
}



