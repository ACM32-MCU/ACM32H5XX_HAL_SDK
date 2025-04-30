/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h" 

typedef void (*ROM_Func)(uint32_t v1); 

extern uint8_t API_Switch_To_FIFO_Mode(void);  
extern uint8_t API_Switch_To_XIP_Mode(void);  
#ifndef ROM_API_TABLE_ENTRY_ADDR
#define ROM_API_TABLE_ENTRY_ADDR  0x1FF07FD0      
#endif 

#ifndef SPIX_FLASH_INSTANCE
#define SPIX_FLASH_INSTANCE    SPI7    
#endif 

uint8_t API_Switch_To_FIFO_Mode(void)   
{
    SPI_Nor_Func *ptr_api;  
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
          
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[8](&s_Flash_Parameter);   

    return ret;   
}  

void HAL_SimpleDelay_Ex(volatile uint32_t delay)  
{
    while(delay--)
    {
        ;
    }
}

uint8_t API_Switch_To_XIP_Mode(void)   
{
    SPI_Nor_Func *ptr_api;  
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[9](&s_Flash_Parameter);   

    return ret;   
}  

void spi_flash_power_down(uint32_t boot_instance)
{
    ROM_Func low_power_func; 
    
    API_Switch_To_FIFO_Mode(); 
    
    low_power_func = (ROM_Func)0x1ff02eb5;   
    
    low_power_func(boot_instance);   
}

void spi_flash_exit_power_down(uint32_t boot_instance)
{
    ROM_Func low_power_exit_func; 
    
    low_power_exit_func = (ROM_Func)0x1ff02f49;   
    
    low_power_exit_func(boot_instance);   
    HAL_SimpleDelay_Ex(3500); // at least 25us 
    API_Switch_To_XIP_Mode(); 
}


void spi_flash_powerdown_exitpowerdown(uint32_t boot_instance)
{
    spi_flash_power_down(boot_instance);  
    spi_flash_exit_power_down(boot_instance);   
}



