/******************************************************************************
*@file  : main.c
*@brief : This file provides spi nor flash erase/read/write Demo
*@ver   : 1.0.0
*@date  : 2024.07.29   
******************************************************************************/

#include "app.h"  
#include "romapi.h"    


#define printfB8    printf_buff_byte


uint32_t spi_data_read[64]; 
uint32_t spi_data_write[64]; 


volatile uint8_t spi_flash_test_buffer[64]; 

#define TEST_START_ADDRESS 0x08100000U  
#define SPI_FLASH_PAGE_SIZE  256  


void spi_flash_power_down(uint32_t boot_instance); 
void spi_flash_exit_power_down(uint32_t boot_instance);

/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(volatile uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n"); 
}

/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(uint32_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.8x ",buff[i]);	
	}
	printf("\n");
}



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
        API_Read_SPI_Flash_ID((uint8_t *)spi_flash_test_buffer);     
        printfS("SPI Flash ID:\n");  
        temp = spi_flash_test_buffer[0];    
        spi_flash_test_buffer[0] = spi_flash_test_buffer[2];   
        spi_flash_test_buffer[2] = temp;   
        printfB8(spi_flash_test_buffer, 3); // manufacture id first 
    
        // Read Unique ID 
        API_Read_SPI_Flash_Unique_ID((uint8_t *)spi_flash_test_buffer);      
        printfS("SPI Flash Unique ID:\n");   
        printfB8(spi_flash_test_buffer, 16);  
 
#if 1  // write and read registers test          
        // Read 3 status registers 
        spi_flash_test_buffer[0] = 0xFF;  
        spi_flash_test_buffer[0] = 0xFF; 
        spi_flash_test_buffer[0] = 0xFF; 
        ret = API_Read_SPI_Flash_Register((uint8_t *)spi_flash_test_buffer);  
        if (0 == ret)
        {
            printfS("Read SPI Flash Status Registers-1-2-3 1st:\n");   
            printfB8(spi_flash_test_buffer, 3);  
        }
        else
        {
            printfS("Read SPI Flash Status Registers Fail\n");   
        }
    
        // Register 1 test 
        spi_flash_test_buffer[0] = BIT2;  // BP0     
        API_Write_SPI_Flash_Register(1, spi_flash_test_buffer[0]);   
        API_Read_SPI_Flash_Register((uint8_t *)spi_flash_test_buffer); 
        printfS("Read SPI Flash Status Registers-1-2-3 2nd:\n");   
        printfB8(spi_flash_test_buffer, 3);  
    
        spi_flash_test_buffer[0] = 0;   // BP0    
        API_Write_SPI_Flash_Register(1, spi_flash_test_buffer[0]);   
        API_Read_SPI_Flash_Register((uint8_t *)spi_flash_test_buffer); 
        printfS("Read SPI Flash Status Registers-1-2-3 3rd:\n");   
        printfB8(spi_flash_test_buffer, 3);   
    
        // Register 2: QE should not be changed   
    
        // Register 3 test 
        spi_flash_test_buffer[2] = 0;    
        API_Write_SPI_Flash_Register(3, spi_flash_test_buffer[2]);       
        API_Read_SPI_Flash_Register((uint8_t *)spi_flash_test_buffer); 
        printfS("SPI Flash Status Registers-1-2-3 4th:\n");   
        printfB8(spi_flash_test_buffer, 3);  
    
        spi_flash_test_buffer[2] = 0x20;    
        API_Write_SPI_Flash_Register(3, spi_flash_test_buffer[2]);        
        API_Read_SPI_Flash_Register((uint8_t *)spi_flash_test_buffer);   
        printfS("SPI Flash Status Registers-1-2-3 5th:\n");   
        printfB8(spi_flash_test_buffer, 3);   
 #endif     
        
        // Read Flash data first 
        memset(spi_data_read, 0, sizeof(spi_data_read) ); 
        API_Read_SPI_Flash_1S4S4S(TEST_START_ADDRESS, (uint8_t *)spi_data_read, sizeof(spi_data_read) );           
        printfS("SPI Flash 1S4S4S Read Data:\n");    
        printfB8((uint8_t *)spi_data_read, 16);   
        
        // Erase SPI Flash 
        API_Erase_SPI_Flash_Sector(TEST_START_ADDRESS);   
        
 /*  Return to Boot       
        API_Erase_SPI_Flash_Sector(0x08001000);   
*/        
        API_Read_SPI_Flash_1S4S4S(TEST_START_ADDRESS, (uint8_t *)spi_data_read, sizeof(spi_data_read) );           
        printfS("SPI Flash 1S4S4S Read Data After Erase:\n");  
        printfB8((uint8_t *)spi_data_read, 16);   
        
    
        // Program and read back 
        write_ptr = (uint8_t *)spi_data_write; 
        for (i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
        {
            write_ptr[i] = i+1;  
        }
    
        API_Program_SPI_Flash_Data_1S1S1S(TEST_START_ADDRESS, (uint8_t *)spi_data_write, sizeof(spi_data_write) );  
        API_Read_SPI_Flash_1S4S4S(TEST_START_ADDRESS, (uint8_t *)spi_data_read, sizeof(spi_data_read) );           
        printfS("SPI Flash 1S4S4S Read Data After Program:\n");  
        printfB8((uint8_t *)spi_data_read, 16);  
    
        if (0 == memcmp(spi_data_read, spi_data_write, sizeof(spi_data_write) ) )
        {
            printfS("SPI Flash Program and Read 1S1S1S Succ:\n");  
        }
        else
        {
            printfS("SPI Flash Program and Read 1S1S1S Fail:\n");  
        }
        
     
        // Change SPI DIV to 8
        printfS("Read SPI DIV 1st:%d\n", SPI7->BAUD);  
        spi_data_write[0] = 8;  
        addr = (uint32_t)(&(SPI7->BAUD)); 
        API_Modify_SPI_Value(addr, spi_data_write, 1);   
        printfS("Read SPI DIV 2nd:%d\n", SPI7->BAUD);   
    
        // Change SPI RX shift  
        printfS("Read SPI RX CTRL 1st:0x%08x\n", SPI7->RX_CTL);    
        spi_data_write[0] = (SPI7->RX_CTL & ~(BIT8|BIT9) ) | BIT9;     
        addr = (uint32_t)(&(SPI7->RX_CTL));  
        API_Modify_SPI_Value(addr, spi_data_write, 1);   
        printfS("Read SPI RX CTRL 2nd:0x%08x\n", SPI7->RX_CTL);   
    
        // Change SPI RX shift  
        spi_data_write[0] = (SPI7->RX_CTL & ~(BIT8|BIT9) ) | BIT8;     
        addr = (uint32_t)(&(SPI7->RX_CTL));  
        API_Modify_SPI_Value(addr, spi_data_write, 1);   
        printfS("Read SPI RX CTRL 3rd:0x%08x\n", SPI7->RX_CTL);  
    
        // Change SPI DIV to 4   
        spi_data_write[0] = 4;  
        addr = (uint32_t)(&(SPI7->BAUD)); 
        API_Modify_SPI_Value(addr, spi_data_write, 1);   
        printfS("Read SPI DIV 3nd:%d\n", SPI7->BAUD);   
        
        spi_flash_powerdown_exitpowerdown((uint32_t)SPIX_FLASH_INSTANCE); 
    }
    __set_PRIMASK(0);  
    
    
    i = 2000;      
    while(1)
    {
        HAL_Delay(2000);  
        
        __set_PRIMASK(1); 
        API_Read_SPI_Flash_ID((uint8_t *)spi_flash_test_buffer);                 
        __set_PRIMASK(0);    
        
        temp = spi_flash_test_buffer[0];    
        spi_flash_test_buffer[0] = spi_flash_test_buffer[2];   
        spi_flash_test_buffer[2] = temp; 
        printfS("SPI Flash ID:\n");    
        printfB8(spi_flash_test_buffer, 3);   // manufacture id first     
        printfS("Read SPI DIV:%d\n", SPI7->BAUD);  
        printfS("Read SPI RX CTRL:0x%08x\n", SPI7->RX_CTL);  
    }
}



