/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h" 
#include "SPI_API.h"   
#include "romapi.h"   


uint8_t userkey_region_a[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};   

uint8_t userkey_region_b[16] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x15};     

uint32_t spi_data_read[64]; 
uint32_t spi_data_write[64]; 

uint32_t spi_data_cipher_write[64]; 
uint8_t spi_xip_data_plain_read[256]; 

volatile uint8_t spi_flash_test_buffer[64]; 


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


void XIP_Plain_Region(uint32_t start_address)
{
    uint32_t i;  
    
    if (PLAIN_TABLE1_ADDRESS == start_address)
    {
        printfS("Table1 Read Test\n"); 
    }
    else if (PLAIN_TABLE2_ADDRESS == start_address)    
    {
        printfS("Table2 Read Test\n"); 
    }
    else
    {
        printfS("Address Error!\n");  
        return;  
    }
    
    for(i = 0; i < 256; i++) 
    {
        spi_xip_data_plain_read[i] = *(uint8_t *)(start_address + i);              
    }
    
    printfB8((uint8_t *)spi_xip_data_plain_read, 256);     
    
}  


void OTFDEC_Region_A_Test(uint32_t start_address)
{
    uint32_t i, ret;   
    uint8_t *write_ptr;  
    
    printfS("OTFDEC Region_A Test\n"); 
    
    API_Erase_SPI_Flash_Sector(start_address);   
    API_Read_SPI_Flash_1S1S1S(start_address, (uint8_t *)spi_data_read, sizeof(spi_data_read) );           
    printfS("SPI Flash 1S1S1S Read Data After Erase:\n");  
    printfB8((uint8_t *)spi_data_read, 8);   
        
    System_InvalidateDAccelerate_by_Addr( (uint32_t *)start_address, 0x1000);   // Invalidate one sector  
    for(i = 0; i < 16; i++)
    {
        spi_xip_data_plain_read[i] = *(uint8_t *)(start_address + i);              
    }
        
    printfS("SPI Flash XIP Read Data After Erase:\n");    
    printfB8((uint8_t *)spi_xip_data_plain_read, 16);  
    
    // Program and read back 
    write_ptr = (uint8_t *)spi_data_write; 
    for (i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
    {
        write_ptr[i] = i;  
    }
        
    API_Encrypt_Set_Region1_Key(userkey_region_a); 
    API_Encrypt_Program_SPI_Flash_Data(start_address, (uint32_t *)spi_data_write,  16, (uint32_t *)spi_data_cipher_write);  
    printfS("spi_data_cipher_write:\n");   
    printfB8((uint8_t *)spi_data_cipher_write, 16);   
    API_Read_SPI_Flash_1S1S1S(start_address, (uint8_t *)spi_data_read, sizeof(spi_data_read) );     
    printfS("spi_data_cipher_write saved in spi flash:\n");   
    printfB8((uint8_t *)spi_data_read, 16); 
                
    System_InvalidateDAccelerate_by_Addr( (uint32_t *)start_address, 0x1000);  // Invalidate one sector    
    for(i = 0; i < 16; i++)  
    {
        spi_xip_data_plain_read[i] = *(uint8_t *)(start_address + i);                
    }
        
    printfS("SPI Flash XIP Read Data After Program:\n");    
    printfB8((uint8_t *)spi_xip_data_plain_read, 16);   
    
    if (0 == memcmp(spi_xip_data_plain_read, spi_data_write, 16 ) )  
    {
        printfS("OTFDEC Succ:\n");  
    }
    else
    {
        printfS("OTFDEC Fail:\n");    
    }
}




void OTFDEC_Region_B_Test(uint32_t start_address)
{
    uint32_t i, ret, random1, random2;   
    uint8_t *write_ptr;  
    
    printfS("OTFDEC Region_B Test\n"); 
    
    API_Erase_SPI_Flash_Sector(start_address);   
    API_Read_SPI_Flash_1S1S1S(start_address, (uint8_t *)spi_data_read, sizeof(spi_data_read) );           
    printfS("SPI Flash 1S1S1S Read Data After Erase:\n");  
    printfB8((uint8_t *)spi_data_read, 8);   
        
    System_InvalidateDAccelerate_by_Addr( (uint32_t *)start_address, 0x1000);   // Invalidate one sector  
    for(i = 0; i < 16; i++)
    {
        spi_xip_data_plain_read[i] = *(uint8_t *)(start_address + i);              
    }
        
    printfS("SPI Flash XIP Read Data After Erase:\n");    
    printfB8((uint8_t *)spi_xip_data_plain_read, 16);  
    
    // Program and read back 
    write_ptr = (uint8_t *)spi_data_write; 
    for (i = 0; i < SPI_FLASH_PAGE_SIZE; i++)
    {
        write_ptr[i] = i;  
    }
        
    random1 = 0x1258ABF6;
    random2 = 0x356BCF18;
    API_Encrypt_Set_Region2_Key_Random_EndAdress(userkey_region_b, random1, random2, OTFDEC_END_ADDRESS_B);  
    
    API_Encrypt_Region2_Program_SPI_Flash_Data(start_address, (uint32_t *)spi_data_write,  16, (uint32_t *)spi_data_cipher_write);  
    
    printfS("spi_data_cipher_write:\n");   
    printfB8((uint8_t *)spi_data_cipher_write, 16);   
    API_Read_SPI_Flash_1S1S1S(start_address, (uint8_t *)spi_data_read, sizeof(spi_data_read) );     
    printfS("spi_data_cipher_write saved in spi flash:\n");   
    printfB8((uint8_t *)spi_data_read, 16); 
                
    System_InvalidateDAccelerate_by_Addr( (uint32_t *)start_address, 0x1000);  // Invalidate one sector    
    for(i = 0; i < 16; i++)  
    {
        spi_xip_data_plain_read[i] = *(uint8_t *)(start_address + i);                
    }
        
    printfS("SPI Flash XIP Read Data After Program:\n");    
    printfB8((uint8_t *)spi_xip_data_plain_read, 16);   
    
    if (0 == memcmp(spi_xip_data_plain_read, spi_data_write, 16 ) )  
    {
        printfS("OTFDEC Succ:\n");  
    }
    else
    {
        printfS("OTFDEC Fail:\n");    
    }
}



