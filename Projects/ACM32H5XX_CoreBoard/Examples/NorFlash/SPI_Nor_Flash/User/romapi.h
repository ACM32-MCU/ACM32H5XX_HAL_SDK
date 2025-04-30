/******************************************************************************
*@file  : romapi.h
*@brief : header file
******************************************************************************/
#ifndef __ROMAPI_H__
#define __ROMAPI_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

#define SPIX_FLASH_INSTANCE    SPI7    

#define ROM_API_TABLE_ENTRY_ADDR  0x1FF07FD0     


uint8_t API_Read_SPI_Flash_ID(uint8_t * p_out);
uint8_t API_Read_SPI_Flash_Unique_ID(uint8_t * p_out); 
uint8_t API_Write_SPI_Flash_Register(uint8_t register_index, uint8_t register_value); 
uint8_t API_Read_SPI_Flash_Register(uint8_t *p_register_value);  
uint8_t API_Read_SPI_Flash_1S1S1S(uint32_t read_addr, uint8_t *data_out, uint32_t len);  
uint8_t API_Read_SPI_Flash_1S1S4S(uint32_t read_addr, uint8_t *data_out, uint32_t len); 
uint8_t API_Read_SPI_Flash_1S4S4S(uint32_t read_addr, uint8_t *data_out, uint32_t len); 
uint8_t API_Program_SPI_Flash_Data_1S1S1S(uint32_t addr, uint8_t *data_in, uint32_t len);  
uint8_t API_Program_SPI_Flash_Data_1S1S4S(uint32_t addr, uint8_t *data_in, uint32_t len);  
uint8_t API_Erase_SPI_Flash_Sector(uint32_t addr);   
uint8_t API_Erase_SPI_Flash_Small_Block(uint32_t addr);  
uint8_t API_Erase_SPI_Flash_Large_Block(uint32_t addr);  
uint8_t API_Modify_SPI_Value(uint32_t write_addr, uint32_t * p_value, uint32_t len);   
uint8_t API_Encrypt_Program_SPI_Flash_Data(uint32_t addr, uint32_t *data_in, uint32_t len, uint32_t *data_out);  
uint8_t API_Encrypt_Set_Region1_Key(uint8_t * key_input);    

#endif





