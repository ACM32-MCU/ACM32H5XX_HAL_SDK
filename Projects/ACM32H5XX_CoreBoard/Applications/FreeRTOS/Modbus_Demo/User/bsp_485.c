#include "bsp_485.h"

UART_HandleTypeDef	USART_RS485;
//RS485_CommDataTypeDef	RS485_REC;

//uint8_t fifo_init(RS485_CommDataTypeDef*	rs485_dat, uint32_t capacity)
//{
//	rs485_dat->read = rs485_dat->write = 0;
//	rs485_dat->capacity = capacity;
//	return 0;
//}

//uint32_t fifo_space(RS485_CommDataTypeDef*	rs485_dat)
//{
//	if(rs485_dat->write < rs485_dat->read)
//	{
//		return (rs485_dat->read - rs485_dat->write);
//	}
//	else{
//		return (rs485_dat->capacity - rs485_dat->write + rs485_dat->read);
//	}
//}

//uint8_t fifo_in(RS485_CommDataTypeDef*	rs485_dat, uint8_t dat)
//{
//	if(fifo_space(rs485_dat) > 0)
//	{
//		rs485_dat->write = (rs485_dat->write >= rs485_dat->capacity) ? 0 : rs485_dat->write;
//		rs485_dat->buf[rs485_dat->write++] = dat;
//		return 0;
//	}
//	else
//		return -1;
//}

//uint8_t fifo_out(RS485_CommDataTypeDef*	rs485_dat, uint8_t* dat)
//{
//	if(fifo_space(rs485_dat) < rs485_dat->capacity)
//	{
//		rs485_dat->read = (rs485_dat->read >= rs485_dat->capacity) ? 0 : rs485_dat->read;
//		*dat = rs485_dat->buf[rs485_dat->read++];
//		return 0;
//	}
//	else{
//		return -1;
//	}
//}



/**
**@name	void HAL_UART_MspInit(UART_HandleTypeDef *huart)
**@brief 485 module should call a function named 'HAL_UART_MspInit' to initialize the port of 485.
**		In current project, this function is declared in 'acm32h5xx_hal_msp.c".
**/

//	void HAL_UART_MspInit(UART_HandleTypeDef *huart)


/**
**@name	void HAL_UART_RS485Msp_Init(UART_HandleTypeDef *huart)
**@brief Use to initialize the DE port of RS485
**/
void HAL_UART_RS485Msp_Init(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef    GPIO_Uart = {0};
	
	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
        /* A12: DE */
		
		GPIO_Uart.Pin       = GPIO_PIN_12;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Pull      = GPIO_PULLUP;
		
#ifdef	SOFT_CTRL_DE_PIN
		GPIO_Uart.Mode      = GPIO_MODE_OUTPUT_PP;
        GPIO_Uart.Alternate = GPIO_FUNCTION_0;
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, RS485_DE_IDLE_POL);
#else
		GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
#endif
	}
	else if(huart->Instance == USART4){
		__HAL_RCC_GPIOE_CLK_ENABLE();
        /* A12: DE */
		
		GPIO_Uart.Pin       = GPIO_PIN_8;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Pull      = GPIO_PULLUP;
		
#ifdef	SOFT_CTRL_DE_PIN
		GPIO_Uart.Mode      = GPIO_MODE_OUTPUT_PP;
        GPIO_Uart.Alternate = GPIO_FUNCTION_0;
        HAL_GPIO_Init(GPIOE, &GPIO_Uart);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, RS485_DE_IDLE_POL);
#else
		GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Alternate = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOE, &GPIO_Uart);
#endif
	}
}

/**
**@name	  void bsp_485_init(UART_TypeDef	*usartx, uint32_t baud)
**@brief  This function use to initialize the RS485 module
**@usartx The label of usart which want to be RS485 port
**@baud	  The target baudrate
**/
void bsp_485_init(UART_TypeDef	*usartx, uint32_t baud)
{
	USART_RS485.Instance        = usartx;
    USART_RS485.Init.BaudRate   = baud;
    USART_RS485.Init.WordLength = UART_WORDLENGTH_8B;
    USART_RS485.Init.StopBits   = UART_STOPBITS_1;
    USART_RS485.Init.Parity     = UART_PARITY_NONE;
    USART_RS485.Init.Mode       = UART_MODE_TX_RX;
    USART_RS485.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//	fifo_init(&RS485_REC, RS485_DATA_BUFFER_SIZE);
#ifdef	SOFT_CTRL_DE_PIN
	HAL_UART_Init(&USART_RS485);
	HAL_UART_RS485Msp_Init(&USART_RS485);
#else
	HAL_UART_RS485_Init(&USART_RS485, RS485_DE_IDLE_POL, RS485_DE_DEAT, RS485_DE_DEDT);
	
#endif
	// test
//	HAL_UART_SetDebugUart(USART_RS485.Instance);
	
	// enable RXI and IDLEI
//	SET_BIT(USART_RS485.Instance->IE, UART_IE_RXI | UART_IE_IDLEI); 
}

//void USART1_IRQHandler(void)
//{
//	uint8_t ch;
//	if((USART_RS485.Instance->ISR & USART_ISR_RXI) == USART_ISR_RXI)
//	{
//		while((USART_RS485.Instance->FR & USART_FR_RXFE) != USART_FR_RXFE)
//		{
//			ch = USART_RS485.Instance->DR;
//			fifo_in(&RS485_REC, ch);
//		}
//	}
//	
//	if((USART_RS485.Instance->ISR & USART_ISR_IDLEI) == USART_ISR_IDLEI)
//	{
//		USART_RS485.Instance->ISR |= USART_ISR_IDLEI;
//		
//		while((USART_RS485.Instance->FR & USART_FR_RXFE) != USART_FR_RXFE)
//		{
//			ch = USART_RS485.Instance->DR;
//			fifo_in(&RS485_REC, ch);
//		}
//	}
//}










