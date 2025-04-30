
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


LPUART_HandleTypeDef LPUART1_Handle;
DMA_HandleTypeDef DMA1CH0_Tx;
DMA_HandleTypeDef DMA2CH0_Rx;

volatile uint32_t g_ReceiveCompleteFlag;
volatile uint32_t g_TransmitCompleteFlag;

static void Receive_Complete_Callback(LPUART_HandleTypeDef *hlpuart);
static void Transmit_Complete_Callback(LPUART_HandleTypeDef *hlpuart);

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
    
    memset((void *)data, 0, sizeof(data));
    
    LPUART1_Handle.Instance         = LPUART1;
    LPUART1_Handle.Init.BaudRate    = 4800;
    LPUART1_Handle.Init.WordLength  = LPUART_WORDLENGTH_8B;
    LPUART1_Handle.Init.StopBits    = LPUART_STOPBITS_1B;
    LPUART1_Handle.Init.Parity      = LPUART_PARITY_NONE;
    LPUART1_Handle.Init.Mode        = LPUART_MODE_TXRX;
    LPUART1_Handle.Init.ClockSource = LPUART_CLOCKSOURCE_RCL;
    LPUART1_Handle.Init.WakeupMode  = LPUART_WAKEUPMODE_NONE;

    HAL_LPUART_Init(&LPUART1_Handle);
    
    HAL_LPUART_RegisterCallback(&LPUART1_Handle, LPUART_CALLBACKID_RXCPLT, Receive_Complete_Callback);
    
    HAL_LPUART_RegisterCallback(&LPUART1_Handle, LPUART_CALLBACKID_TXCPLT, Transmit_Complete_Callback);
    
    g_ReceiveCompleteFlag = 0;
    if (HAL_LPUART_Receive_DMA(&LPUART1_Handle, (uint8_t *)data, 10) != HAL_OK)
    {
        printfS("receive failed\r\n");
        while (1);
    }
        
    while (1)
    {
        
        printfS("test baudrate:4800\r\n");
        
        printfS("Receive 10 characters and send the received characters\r\n");
        
        while (g_ReceiveCompleteFlag == 0);
        g_ReceiveCompleteFlag = 0;
        
        printfS("receive complete\r\n");
        
        g_TransmitCompleteFlag = 0;
        if (HAL_LPUART_Transmit_DMA(&LPUART1_Handle, (uint8_t *)data, 10) != HAL_OK)
        {
            printfS("transmit failed\r\n");
            break;
        }
        while (g_TransmitCompleteFlag == 0);
        
        printfS("transmit complete\r\n");
        printfS("\r\n");
        
    }
    
    while(1)
    {
        BSP_LED_Toggle();
        HAL_Delay(500);
    }
}


static void Receive_Complete_Callback(LPUART_HandleTypeDef *hlpuart)
{
    g_ReceiveCompleteFlag = 1;
}

static void Transmit_Complete_Callback(LPUART_HandleTypeDef *hlpuart)
{
    g_TransmitCompleteFlag = 1;
}

