
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"

//#define DMA_TWO_BUFFER_MODE   


void UART_TX_DMA_Init(void); 
void UART_Send_By_DMA(void);    

#endif /* __APP_H */

