/*
  ******************************************************************************
  * @file    APP.h
  * @author  Chris_Kyle
  * @version V1.0.0
  * @date    2020
  * @brief   UART demo Header file.
  ******************************************************************************
*/
#ifndef __SPI_API_H__
#define __SPI_API_H__      

#include "hal.h"   


typedef enum
{
    SPI_CMD_ID_NOT_CARE = 0x00,  
    
    SPI_READ_REGISTER_1S0S1S = 0x01, 
    
    SPI_WRITE_REGISTER1_1S1S1S, 
    
    SPI_WRITE_REGISTER2_1S1S1S, 
   
    SPI_WRITE_REGISTER3_1S1S1S, 
   
    SPI_READ_ID_1S0S1S,  
    
    SPI_READ_DATA_1S1S1S_24, // 0x03, STR, xSPI 
    SPI_READ_DATA_1S1S1S_32, // 0x13, STR, xSPI 
    
    SPI_READ_DATA_1S1S4S_24, // 0x6B, STR, xSPI 
    SPI_READ_DATA_1S4S4S_24, // 0xEB, STR, xSPI 

    SPI_ERASE_SECTOR_1S1S_24, // 0x20, STR, xSPI  
    SPI_ERASE_SECTOR_1S1S_32, // 0x21, STR, xSPI

    
    SPI_ERASE_BLOCK1_1S1S_24, // 0x52, STR, xSPI 
    SPI_ERASE_BLOCK1_1S1S_32, // 0x5C, STR, xSPI 
    
    SPI_ERASE_BLOCK2_1S1S_24, // 0xD8, STR, xSPI  
    SPI_ERASE_BLOCK2_1S1S_32, // 0xDC, STR, xSPI  
 
    
    SPI_PROG_DATA_1S1S1S_24,  // 0x02, STR, xSPI 
    SPI_PROG_DATA_1S1S1S_32,  // 0x12, STR, xSPI 
    SPI_PROG_DATA_1S1S4S_24,  // 0x32, STR, xSPI   
    
    SPI_ENTER_ADDR_32_1S,     // 0xB7, STR, xSPI
    
    SPI_EXIT_ADDR_32_1S,      // 0xE9, STR, xSPI   
    
    SPI_READ_UNIQUE_ID = 0x4B,

    
    
}SPI_READ_CMD;   



typedef struct
{
    uint32_t SPI_Instance;     
    
    uint8_t  Command; //must 
    uint8_t  Delay;   //must  
    uint8_t  Cont_MID;  //must   
    uint8_t  Dummy_clks; //must     
    
    uint32_t Addr;   //must
    uint32_t Operation_length; //must 
    uint32_t Input_data_Addr; //must
    uint32_t Output_data_Addr;  //must  
            
}SPI_Interface_Parameter;  

typedef uint8_t (*SPI_API_Func)(SPI_Interface_Parameter *);  

 



#endif
