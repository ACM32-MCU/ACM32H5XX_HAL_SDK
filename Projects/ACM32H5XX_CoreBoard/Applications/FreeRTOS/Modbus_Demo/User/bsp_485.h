#ifndef	__BSP_485_H__
#define	__BSP_485_H__

#include "hal.h"

#define	RS485_DATA_BUFFER_SIZE		256

#define	RS485_DE_IDLE_POL			1	/**	This macro define the idle state of DE pin polarity.	*/

//#define	SOFT_CTRL_DE_PIN				/** It means that 485 DE pin is controlled by software when this macro	\
											defined. Otherwise, the DE pin is controlled by hardware.	*/

#ifndef	SOFT_CTRL_DE_PIN

#define	RS485_DE_DEAT				10		
#define	RS485_DE_DEDT				10

#endif		/*	SOFT_CTRL_DE_PIN	*/

typedef	struct{
	uint32_t read;
	uint32_t write;
	uint32_t capacity;
	uint8_t buf[RS485_DATA_BUFFER_SIZE];
}RS485_CommDataTypeDef;


void bsp_485_init(UART_TypeDef	*usartx, uint32_t baud);

uint8_t fifo_out(RS485_CommDataTypeDef*	rs485_dat, uint8_t* dat);

#endif		/*	__BSP_485_H__	*/






