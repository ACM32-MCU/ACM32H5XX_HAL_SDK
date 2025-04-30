/******************************************************************************
*@file  : main.c
*@brief : This file provides UART Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"  
#include "romapi.h" 
#include "SPI_API.h"     

volatile uint8_t spi_flash_id_buffer[16]; 

int main(void)
{
    uint32_t i, ret, addr, test_times;   
    uint8_t *write_ptr;  
    uint8_t temp; 

      
    HAL_Init();  
    
    if (0 == (SCB->CCR & (uint32_t)SCB_CCR_DC_Msk) )  
    {
        System_EnableDAccelerate();
    }
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    BSP_UART_Init(USART1, 115200); 

    printfS("App runs in SPI Flash!\n");  
    printfS("Read SPI DIV:%d\n", SPI7->BAUD);  
    printfS("Read SPI RX CTRL:0x%08x\n", SPI7->RX_CTL);  
    
    __set_PRIMASK(1);  
    
    test_times = 0; 
    
    while(test_times++ < 3)
    {
        printfS("***********************************\n");         
        printfS("Test Times:%d\n", test_times);           
        printfS("***********************************\n");   
        
        // Read Flash ID: Manufacture ID, Capacity ID, Device ID 
        API_Read_SPI_Flash_ID((uint8_t *)spi_flash_id_buffer);     
        printfS("SPI Flash ID:\n");  
        temp = spi_flash_id_buffer[0];    
        spi_flash_id_buffer[0] = spi_flash_id_buffer[2];   
        spi_flash_id_buffer[2] = temp;   
        printfB8(spi_flash_id_buffer, 3); // manufacture id first 
    
        // Read Unique ID 
        API_Read_SPI_Flash_Unique_ID((uint8_t *)spi_flash_id_buffer);      
        printfS("SPI Flash Unique ID:\n");   
        printfB8(spi_flash_id_buffer, 16);  
        
        XIP_Plain_Region(PLAIN_TABLE1_ADDRESS);   
        XIP_Plain_Region(PLAIN_TABLE2_ADDRESS);   
        OTFDEC_Region_A_Test(OTFDEC_START_ADDRESS_A);       
        OTFDEC_Region_B_Test(OTFDEC_START_ADDRESS_B);     
        
    }
    __set_PRIMASK(0);  
    
    i = 2000;      
    while(1)
    {
        HAL_Delay(i++);  
        
        __set_PRIMASK(1); 
        API_Read_SPI_Flash_ID((uint8_t *)spi_flash_id_buffer);                 
        __set_PRIMASK(0);    
        
        temp = spi_flash_id_buffer[0];    
        spi_flash_id_buffer[0] = spi_flash_id_buffer[2];   
        spi_flash_id_buffer[2] = temp; 
        printfS("SPI Flash ID:\n");    
        printfB8(spi_flash_id_buffer, 3);   // manufacture id first     
        printfS("Read SPI DIV:%d\n", SPI7->BAUD);  
        printfS("Read SPI RX CTRL:0x%08x\n", SPI7->RX_CTL);  
    }
}
