/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "romapi.h"   
#include "SPI_API.h"   



// read mid and device id 
uint8_t API_Read_SPI_Flash_ID(uint8_t * p_out)  
{
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    
    s_Flash_Parameter.Command = SPI_READ_REGISTER_1S0S1S;   
    
    s_Flash_Parameter.Delay = 0xFFU;   
    s_Flash_Parameter.Operation_length = 3;          
    s_Flash_Parameter.Output_data_Addr = (uint32_t)p_register_value;         
    s_Flash_Parameter.Dummy_clks = 0; 
    ret = ptr_api[3](&s_Flash_Parameter);   

    return ret;   
}


uint8_t API_Read_SPI_Flash_1S1S1S(uint32_t read_addr, uint8_t *data_out, uint32_t len)   
{
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api; 
    uint32_t offset;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
     
    offset = addr & 0x00FFFFFFU; 

    if (offset < 64 * 1024)
    {
        return 1;   
    }
    
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api;  
    uint32_t offset;   
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
    
    offset = addr & 0x00FFFFFFU; 

    if (offset < 64 * 1024)
    {
        return 1;   
    }
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func *ptr_api;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;     
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
    SPI_API_Func ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func )0x1FF07C01;     
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPI7;  
    s_Flash_Parameter.Command = SPI_PROG_DATA_1S1S1S_24;    // prgoram encrypt data to spi flash  
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



void otfdec_key_reverse(uint8_t *data_in, uint32_t *data_out)
{
    uint32_t i;
    
    i = 0; 
    
    for(i = 0; i < 4; i++)  
    {
        data_out[i] =  (data_in[15-4*i] << 24) + (data_in[14-i*4] << 16) + (data_in[13-i*4] << 8) + (data_in[12-i*4] << 0);  
    }
}

// input value: pointer to 16 bytes key 
// return value: 0 means valid, other value means invalid  
uint8_t API_Encrypt_Set_Region1_Key(uint8_t * key_input)  
{
    uint32_t key_reverse[8];  
    SPI_API_Func *ptr_api;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    otfdec_key_reverse( (uint8_t *)key_input, key_reverse);   
    
    key_reverse[7] = 0xFE01;   
    
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;        
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



// input value: pointer to 16 bytes key, 8 bytes random, 4 bytes end address  
// return value: 0 means valid, other value means invalid  
uint8_t API_Encrypt_Set_Region2_Key_Random_EndAdress(uint8_t * key_input, uint32_t random_1, uint32_t random_2, uint32_t end_address)  
{
    uint32_t encrypt_info[8];   
    SPI_API_Func *ptr_api;  
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
      
    if (key_input)
    {
        memcpy(encrypt_info, key_input, 16); 
    }
    else
    {
        return 1; 
    }
    encrypt_info[4] = random_1;
    encrypt_info[5] = random_2; 
    encrypt_info[6] = end_address; 
    encrypt_info[7] = 0xFD02 | (0xFE01U << 16);   
    
    ptr_api = (SPI_API_Func *)ROM_API_TABLE_ENTRY_ADDR;        
    s_Flash_Parameter.SPI_Instance = (uint32_t)SPIX_FLASH_INSTANCE; 
    s_Flash_Parameter.Command = SPI_CMD_ID_NOT_CARE;     
    s_Flash_Parameter.Cont_MID = 0; 
    s_Flash_Parameter.Addr = 0;  
    s_Flash_Parameter.Delay = 0xFFU;      
    s_Flash_Parameter.Operation_length = 0;  
    s_Flash_Parameter.Input_data_Addr = (uint32_t)encrypt_info;       
    
    ret = ptr_api[11](&s_Flash_Parameter);     
    
    return ret;  
}


uint8_t API_Encrypt_Region2_Program_SPI_Flash_Data(uint32_t addr, uint32_t *data_in, uint32_t len, uint32_t *data_out)    
{
    SPI_API_Func ptr_api; 
    SPI_Interface_Parameter  s_Flash_Parameter;    
    uint8_t ret; 
       
    ptr_api = (SPI_API_Func )0x1FF07D01;     
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
