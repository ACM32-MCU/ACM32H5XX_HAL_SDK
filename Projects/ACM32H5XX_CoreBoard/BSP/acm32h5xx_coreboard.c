/******************************************************************************
*@file  : acm32h5xx_coreboard.h
*@brief : This file provides set of firmware functions to manage Leds and 
*         push-button available on ACM32H5XX Core Board
******************************************************************************/

#include "acm32h5xx_coreboard.h"

UART_HandleTypeDef BSP_UART_Handle;

/******************************************************************************
*@brief : LED init
*@param : none
*@return: none
******************************************************************************/
void BSP_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    BSP_LED_CLK_ENABLE();

    GPIO_InitStruct.Pin       = BSP_LED_PIN;
    GPIO_InitStruct.Mode      = BSP_LED_MODE;
    GPIO_InitStruct.Pull      = BSP_LED_PULL;
    GPIO_InitStruct.Drive     = BSP_LED_DRIVE;
    GPIO_InitStruct.Alternate = BSP_LED_ALTERNATE;

    HAL_GPIO_Init(BSP_LED_GPIO, &GPIO_InitStruct);

    HAL_GPIO_WritePin(BSP_LED_GPIO, BSP_LED_PIN, GPIO_PIN_SET);
}

/******************************************************************************
*@brief : LED on
*@param : none
*@return: none
******************************************************************************/
void BSP_LED_On(void)
{
    HAL_GPIO_WritePin(BSP_LED_GPIO, BSP_LED_PIN, GPIO_PIN_RESET);
}

/******************************************************************************
*@brief : LED off
*@param : none
*@return: none
******************************************************************************/
void BSP_LED_Off(void)
{
    HAL_GPIO_WritePin(BSP_LED_GPIO, BSP_LED_PIN, GPIO_PIN_SET);
}

/******************************************************************************
*@brief : LED toggle
*@param : none
*@return: none
******************************************************************************/
void BSP_LED_Toggle(void)
{
    HAL_GPIO_TogglePin(BSP_LED_GPIO, BSP_LED_PIN);
}

/******************************************************************************
*@brief : PB init
*@param : none
*@return: none
******************************************************************************/
void BSP_PB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    BSP_PB_CLK_ENABLE();

    GPIO_InitStruct.Pin       = BSP_PB_PIN;
    GPIO_InitStruct.Mode      = BSP_PB_MODE;
    GPIO_InitStruct.Pull      = BSP_PB_PULL;
    GPIO_InitStruct.Drive     = BSP_PB_DRIVE;
    GPIO_InitStruct.Alternate = BSP_PB_ALTERNATE;

    HAL_GPIO_Init(BSP_PB_GPIO, &GPIO_InitStruct);

	HAL_EXTI_ClearPending(BSP_PB_EXTI_LINE);
    NVIC_ClearPendingIRQ(BSP_PB_IRQ);
    NVIC_SetPriority(BSP_PB_IRQ, 0x00);
    NVIC_EnableIRQ(BSP_PB_IRQ);
}

/******************************************************************************
*@brief : get PB state
*@param : none
*@return: none
******************************************************************************/
uint32_t BSP_PB_GetState(void)
{
    return (HAL_GPIO_ReadPin(BSP_PB_GPIO, BSP_PB_PIN));
}

/******************************************************************************
*@brief : uart1 init
*@param : none
*@return: none
******************************************************************************/
void BSP_UART_Init(UART_TypeDef *uartx, uint32_t baudrate)
{
    BSP_UART_Handle.Instance        = uartx;
    BSP_UART_Handle.Init.BaudRate   = baudrate;
    BSP_UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    BSP_UART_Handle.Init.StopBits   = UART_STOPBITS_1;
    BSP_UART_Handle.Init.Parity     = UART_PARITY_NONE;
    BSP_UART_Handle.Init.Mode       = UART_MODE_TX_RX;
    BSP_UART_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    HAL_UART_Init(&BSP_UART_Handle);
    
    HAL_UART_SetDebugUart(BSP_UART_Handle.Instance);
}

/******************************************************************************
*@brief : uart1 reinit
*@param : none
*@return: none
******************************************************************************/
void BSP_UART_DeInit(void)
{
    HAL_UART_DeInit(&BSP_UART_Handle);
}


/******************************************************************************
*@brief : MCO init
*@param : none
*@return: none
******************************************************************************/
void BSP_MCO_Init(uint32_t mco, uint32_t state, uint32_t mco_div)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_MCOInitTypeDef MCO_InitStruct = {0};

    assert_param(IS_RCC_MCO(mco));
    assert_param(IS_FUNCTIONAL_STATE(state));
    assert_param(IS_RCC_MCO1_DIV(mco_div));
    
    BSP_MCO1_CLK_ENABLE();

    GPIO_InitStruct.Pin       = BSP_MCO1_PIN;
    GPIO_InitStruct.Mode      = BSP_MCO1_MODE;
    GPIO_InitStruct.Pull      = BSP_MCO1_PULL;
    GPIO_InitStruct.Drive     = BSP_MCO1_DRIVE;
    GPIO_InitStruct.Alternate = BSP_MCO1_ALTERNATE;

    HAL_GPIO_Init(BSP_MCO1_GPIO, &GPIO_InitStruct);
    
    MCO_InitStruct.MCO = mco;
    MCO_InitStruct.MCO1 = state;
    MCO_InitStruct.MCO1Div = mco_div;
    MCO_InitStruct.MCO1RevPol = DISABLE;
    MCO_InitStruct.MCO2 = DISABLE;
    MCO_InitStruct.MCO2Div = 0;
    MCO_InitStruct.MCO2RevPol = DISABLE;
    
    HAL_RCC_MCOConfig(&MCO_InitStruct);
}

/******************************************************************************
*@brief : get reset source
*@param : none
*@return: none
******************************************************************************/
void get_reset_source(void)
{
	uint32_t reset;
	uint32_t flag;
	
	reset = HAL_RCC_GetResetSource();
    HAL_RCC_ClearAllResetSource();
    
	if (reset == 0)
	{
		printfS("reset source£ºunknown\r\n");
        return;
	}

    flag = 0;
	if (reset & RCC_RESET_SOURCE_PWR)
	{
        flag = 1;
		printfS("reset source: pwr.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_POR12)
	{
        flag = 1;
		printfS("reset source: por12.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_SRST)
	{
        flag = 1;
		printfS("reset source: soft.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_RSTN)
	{
        flag = 1;
		printfS("reset source: rstn.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_SYSREQ)
	{
        flag = 1;
		printfS("reset source: sysreq.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_LOCKUP)
	{
        flag = 1;
		printfS("reset source: lockup.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_IWDT)
	{
        flag = 1;
		printfS("reset source: iwdt.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_WDT)
	{
        flag = 1;
		printfS("reset source: wdt.\r\n");
	}
	if (reset & RCC_RESET_SOURCE_LVD)
	{
        flag = 1;
		printfS("reset source: lvd.\r\n");
	}
    
    if (flag == 0)
    {
		printfS("reset source: unkown\r\n");
    }
}

extern UART_TypeDef *g_DebugUart;
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    
    UNUSED(f);
    
    if(g_DebugUart)
    {
        HAL_UART_Receive(&BSP_UART_Handle, &ch, 1, 0xffffffff);
    }
    return ((int)ch);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	printfS("failed£ºfile:%s, line:%u\r\n", file, (int)line);
	while (1);
}
#endif
