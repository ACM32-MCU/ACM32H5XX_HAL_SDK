
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"


LPUART_HandleTypeDef LPUART1_Handle;

volatile uint32_t g_ReceiveCompleteFlag;
volatile uint32_t g_TransmitCompleteFlag;

static void Receive_Complete_Callback(LPUART_HandleTypeDef *hlpuart);
static void Transmit_Complete_Callback(LPUART_HandleTypeDef *hlpuart);

static void EnterStopMode(void);

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
    /* Enable RCL */
    RCC_OscInitStruct.OscType = RCC_OSC_TYPE_RCL;
    RCC_OscInitStruct.RCL = ENABLE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        printfS("Osc init failed\r\n");
        while (1);
    }

    __HAL_RCC_EXTI_CLK_ENABLE();
    if (HAL_EXTI_SetConfigLine(NULL, EXTI_LINE_LPUART1, EXTI_MODE_IT_RISING_FALLING) != HAL_OK)
    {
        printfS("EXTI config failed\r\n");
        while (1);
    }
    LPUART1_Handle.Instance         = LPUART1;
    LPUART1_Handle.Init.BaudRate    = 4800;
    LPUART1_Handle.Init.WordLength  = LPUART_WORDLENGTH_8B;
    LPUART1_Handle.Init.StopBits    = LPUART_STOPBITS_1B;
    LPUART1_Handle.Init.Parity      = LPUART_PARITY_NONE;
    LPUART1_Handle.Init.Mode        = LPUART_MODE_TXRX;
    LPUART1_Handle.Init.ClockSource = LPUART_CLOCKSOURCE_RCL;
    LPUART1_Handle.Init.WakeupMode  = LPUART_WAKEUPMODE_STARTBIT;

    HAL_LPUART_Init(&LPUART1_Handle);

    HAL_LPUART_RegisterCallback(&LPUART1_Handle, LPUART_CALLBACKID_RXCPLT, Receive_Complete_Callback);

    HAL_LPUART_RegisterCallback(&LPUART1_Handle, LPUART_CALLBACKID_TXCPLT, Transmit_Complete_Callback);

    __HAL_LPUART_ENABLE_IT(&LPUART1_Handle, LPUART_IT_START);

    memset((void *)data, 0, sizeof(data));

    while (1)
    {
		
        printfS("test baudrate:4800\r\n");
        printfS("receive 10 characters and send the received characters\r\n");
        g_ReceiveCompleteFlag = 0;
        if (HAL_LPUART_Receive_IT(&LPUART1_Handle, (uint8_t *)data, 10) != HAL_OK)
        {
            printfS("receive failed\r\n");
        }

        BSP_LED_Off();
        printfS("enter stop\r\n");

		USART1->ISR = 0XFFFFFFFF;
		NVIC_DisableIRQ(USART1_IRQn);
		__set_PRIMASK(1);    // disable interrupt 
		SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick  
		SCB->ICSR = BIT25;   // clear systick pending bit 
		
        HAL_PMU_EnterStop(WAIT_FOR_INT);
		
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick              
		__set_PRIMASK(0);    // enable interrupt 

		NVIC_EnableIRQ(USART1_IRQn);
		
        BSP_LED_On();


        printfS("exit stop\r\n");

        while (g_ReceiveCompleteFlag == 0);

        printfS("receive complete\r\n");

        g_TransmitCompleteFlag = 0;
        if (HAL_LPUART_Transmit_IT(&LPUART1_Handle, (uint8_t *)data, 10) != HAL_OK)
        {
            printfS("transmit failed\r\n");
        }
        while (g_TransmitCompleteFlag == 0);

        printfS("transmit complete\r\n");
        printfS("\r\n");
		HAL_Delay(3000);
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

static void EnterStopMode(void)
{

    __HAL_RCC_PMU_CLK_ENABLE();
    SysTick->CTRL = 0;   // disable systick
    __set_PRIMASK(1);    // disable interrupt
    SCB->ICSR = BIT25;   // clear systick pending bit
    HAL_EXTI_ClearAllPending();
    SET_BIT(PMU->CTRL0,PMU_CTRL0_RTCWE);

    HAL_PMU_EnterStop(WAIT_FOR_INT);

    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
    __set_PRIMASK(0);    // enable interrupt

}
