/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "romapi.h"    



// read mid and device id 
uint8_t API_Read_SPI_Flash_ID(uint8_t * p_out)  
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_READ_ID_1S0S1S;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = 3;     
    s_Flash_Parameter.Output_data_Addr = (uint32_t)p_out;        
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[0](&s_Flash_Parameter);   

    return ret;   
}


uint8_t API_Read_SPI_Flash_Unique_ID(uint8_t * p_out) 
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_READ_UNIQUE_ID;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = 16;     
    s_Flash_Parameter.Output_data_Addr = (uint32_t)p_out;       
    s_Flash_Parameter.Dummy_clks = 8; 
    ret = ptr_api[1](&s_Flash_Parameter);   

    return ret;   
}

uint8_t API_Write_SPI_Flash_Register(uint8_t register_index, uint8_t register_value) 
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    if(1 == register_index)
    {
        s_Flash_Parameter.Command = SPI_WRITE_REGISTER1_1S1S1S;   
    }
    else if (2 == register_index)
    {
        s_Flash_Parameter.Command = SPI_WRITE_REGISTER2_1S1S1S;  
    }
    else if (3 == register_index)
    {
        s_Flash_Parameter.Command = SPI_WRITE_REGISTER3_1S1S1S;  
    }  
    else
    {
        return 1;   
    }
    
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = 1;     
    s_Flash_Parameter.Input_data_Addr = (uint32_t)&register_value;       
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[2](&s_Flash_Parameter);   

    return ret;   
}


uint8_t API_Read_SPI_Flash_Register(uint8_t *p_register_value) 
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    
    s_Flash_Parameter.Command = SPI_READ_REGISTER_1S0S1S;   
    
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = 3;          
    s_Flash_Parameter.Output_data_Addr = (uint32_t)p_register_value;         
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[3](&s_Flash_Parameter);   

    return ret;   
}

// This API Can only be called when SPI SCK's frequency is <= 55MHz 
uint8_t API_Read_SPI_Flash_1S1S1S(uint32_t read_addr, uint8_t *data_out, uint32_t len)   
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_READ_DATA_1S1S1S_24;   
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = read_addr;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;     
    s_Flash_Parameter.Output_data_Addr = (uint32_t)data_out;       
    s_Flash_Parameter.Dummy_clks = 0; // no dummy, max frequency is 50-55MHz   
    ret = ptr_api[4](&s_Flash_Parameter);   

    return ret;   
}

uint8_t API_Read_SPI_Flash_1S1S4S(uint32_t read_addr, uint8_t *data_out, uint32_t len)   
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_READ_DATA_1S1S4S_24;   
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = read_addr;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;     
    s_Flash_Parameter.Output_data_Addr = (uint32_t)data_out;       
    s_Flash_Parameter.Dummy_clks = 8; 
    ret = ptr_api[4](&s_Flash_Parameter);   

    return ret;   
}

uint8_t API_Read_SPI_Flash_1S4S4S(uint32_t read_addr, uint8_t *data_out, uint32_t len)   
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_READ_DATA_1S4S4S_24;   
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = read_addr;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;     
    s_Flash_Parameter.Output_data_Addr = (uint32_t)data_out;       
    s_Flash_Parameter.Dummy_clks = 6; 
    ret = ptr_api[4](&s_Flash_Parameter);   

    return ret;   
}


uint8_t API_Program_SPI_Flash_Data_1S1S1S(uint32_t addr, uint8_t *data_in, uint32_t len)    
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE;  
    s_Flash_Parameter.Command = SPI_PROG_DATA_1S1S1S_24;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;     
    s_Flash_Parameter.Input_data_Addr = (uint32_t)data_in;       
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[5](&s_Flash_Parameter);     

    return ret;   
}


uint8_t API_Program_SPI_Flash_Data_1S1S4S(uint32_t addr, uint8_t *data_in, uint32_t len)    
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE;  
    s_Flash_Parameter.Command = SPI_PROG_DATA_1S1S4S_24;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;     
    s_Flash_Parameter.Input_data_Addr = (uint32_t)data_in;       
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[5](&s_Flash_Parameter);     

    return ret;   
}



uint8_t API_Erase_SPI_Flash_Sector(uint32_t addr)   
{
    SPI_Nor_Func *ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_ERASE_SECTOR_1S1S_24;    
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[6](&s_Flash_Parameter);   

    return ret;   
}



uint8_t API_Erase_SPI_Flash_Small_Block(uint32_t addr)   
{
    SPI_Nor_Func *ptr_api; 
    uint32_t offset;  
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
     
    offset = addr & 0x00FFFFFFU; 

    if (offset < 64 * 1024)
    {
        return 1;   
    }
    
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_ERASE_BLOCK1_1S1S_24;    
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[6](&s_Flash_Parameter);   

    return ret;   
} 


uint8_t API_Erase_SPI_Flash_Large_Block(uint32_t addr)   
{
    SPI_Nor_Func *ptr_api;  
    uint32_t offset;   
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
    
    offset = addr & 0x00FFFFFFU; 

    if (offset < 64 * 1024)
    {
        return 1;   
    }
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_ERASE_BLOCK2_1S1S_24;    
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  
    s_Flash_Parameter.Delay = 0xFFU;      

    ret = ptr_api[6](&s_Flash_Parameter);   

    return ret;   
}   






uint8_t API_Modify_SPI_Value(uint32_t write_addr, uint32_t * p_value, uint32_t len)   
{
    SPI_Nor_Func *ptr_api;  
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = write_addr;  
    s_Flash_Parameter.Delay = 0xFFU;      
    s_Flash_Parameter.Operation_length = len;  
    s_Flash_Parameter.Input_data_Addr = (uint32_t)p_value;    
    

    ret = ptr_api[10](&s_Flash_Parameter);   

    return ret;   
}  
 


uint8_t API_Encrypt_Program_SPI_Flash_Data(uint32_t addr, uint32_t *data_in, uint32_t len, uint32_t *data_out)    
{
    SPI_Nor_Func ptr_api; 
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_Nor_Func )0x1FF07C01;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPI7;  
    s_Flash_Parameter.Command = SPI_PROG_DATA_1S1S4S_24;    // prgoram encrypt data to spi flash  
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = addr;  // should >= 0x08000000 
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = len;  // should be multiple of 16 bytes, the rom api will divide 16      
    s_Flash_Parameter.Input_data_Addr = (uint32_t)data_in;       
    s_Flash_Parameter.Dummy_clks = 0; 
    s_Flash_Parameter.Output_data_Addr = (uint32_t)data_out;    
    ret = ptr_api(&s_Flash_Parameter);   

    return ret;   
}




// input value: pointer to 16 bytes key 
// return value: 1 means valid, other value means invalid  
uint8_t API_Encrypt_Set_Region1_Key(uint8_t * key_input)  
{
    uint32_t key_reverse[8];  
    SPI_Nor_Func *ptr_api;  
    SPI_Flash_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    otfdec_key_reverse( (uint8_t *)key_input, key_reverse);   
    
    key_reverse[7] = 0xFE01;   
    
    ptr_api = (SPI_Nor_Func *)ROM_API_TABLE_ENTRY_ADDR;        
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      
    s_Flash_Parameter.Operation_length = 0;  
    s_Flash_Parameter.Input_data_Addr = (uint32_t)key_reverse;      
    

    ret = ptr_api[11](&s_Flash_Parameter);     
    
    return ret;  
}

