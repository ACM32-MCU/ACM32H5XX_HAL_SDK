
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "sdram.h"


uint32_t SDRAM_Data_Test[TEST_DATA_LEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};    

uint8_t App_in_SDRAM(uint32_t * p_datain)
{
    int i;
    
    for (i = 0; i < TEST_DATA_LEN; i++)
    {
        p_datain[i] += 0xAA01;      
    }
    
    return 0;  
}

