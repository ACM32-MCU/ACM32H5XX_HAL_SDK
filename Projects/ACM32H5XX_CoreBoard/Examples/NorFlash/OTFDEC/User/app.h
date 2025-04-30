/******************************************************************************
*@file  : app.h
*@brief : header file
******************************************************************************/
#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

#define printfB8    printf_buff_byte  

#define SPI_FLASH_PAGE_SIZE  256   

#define PLAIN_TABLE1_ADDRESS   0x08000000 
#define PLAIN_TABLE2_ADDRESS   0x08001000   

#define OTFDEC_START_ADDRESS_A 0x08100000U  
#define OTFDEC_START_ADDRESS_B 0x08180000U    
#define OTFDEC_END_ADDRESS_B 0x081FFFFFU   

void printf_buff_byte(volatile uint8_t* buff, uint32_t length);  
void XIP_Plain_Region(uint32_t start_address);   
void OTFDEC_Region_A_Test(uint32_t start_address);
void OTFDEC_Region_B_Test(uint32_t start_address);  

#endif
