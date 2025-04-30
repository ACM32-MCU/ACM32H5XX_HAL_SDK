/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   UART demo Header file.
  ******************************************************************************
*/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h"  


#define UART_RX_FIFO_EMPTY  BIT4   
#define UART_IDLE_STATE     BIT13    
#define UART_RX_INT         BIT4 

#define FRAME_INFO_LENGTH  6  
#define FRAME_HEADER_LENGTH  4  

#define FRAME_HEAD                  0xAAU
#define FRAME_TAIL                  0x55U  





#endif
