/******************************************************************************
*@file  : app.c
*@brief : SRAM Test 
******************************************************************************/
#include "app.h"


uint32_t SRAM_March_Plus_Test(uint32_t addr, uint32_t word_size) 
{
    int i;
    
    volatile uint32_t *p_data;   
    
    p_data = (uint32_t *)addr;    
    
    // Write 0 up 
    for (i = 0; i < word_size; i++)
    {
        p_data[i] = 0;
    }
    
    // Read 0 up 
    for (i = 0; i < word_size; i++)
    {
        if (0 != p_data[i] ) 
        {
            return 1;  
        }
    }
    
    // Write 1 up 
    for (i = 0; i < word_size; i++)
    {
        p_data[i] = 0xFFFFFFFFU;
    }
    
    // Read 1 up  
    for (i = 0; i < word_size; i++)
    {
        if (0xFFFFFFFFU != p_data[i] )   
        {
            return 2;  
        }
    }
    
    // Read 1 up  
    for (i = 0; i < word_size; i++)
    {
        if (0xFFFFFFFFU != p_data[i] )   
        {
            return 3;  
        }
    }
    
    // Write 0 up 
    for (i = 0; i < word_size; i++)
    {
        p_data[i] = 0;
    }
    
    // Read 0 up  
    for (i = 0; i < word_size; i++)
    {
        if (0 != p_data[i] ) 
        {
            return 4;  
        }
    }
    
    // Read 0 up 
    for (i = 0; i < word_size; i++)
    {
        if (0 != p_data[i] ) 
        {
            return 5;  
        }
    }
    
    // Read 0 down 
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);      
    for (i = 0; i < word_size; i++)
    {
        if (0 != *p_data ) 
        {
            return 6;  
        }
        
        p_data = p_data - 1; 
    }
    
    // Write 1 down
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        *p_data = 0xFFFFFFFFU;   
        
        p_data = p_data - 1; 
    }
    
    // Read 1 down 
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        if (0xFFFFFFFFU != *p_data )  
        {
            return 7;  
        }
        
        p_data = p_data - 1; 
    }
    
    // Read 1 down
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        if (0xFFFFFFFFU != *p_data )  
        {
            return 8;  
        }
        
        p_data = p_data - 1; 
    }
    
    // Write 0 down
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        *p_data = 0;     
        
        p_data = p_data - 1; 
    }
    
    // Read 0 down 
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        if (0 != *p_data )  
        {
            return 9;  
        }
        
        p_data = p_data - 1; 
    }
    
    // Read 0 down
    p_data = (uint32_t *)(addr + (word_size << 2) - 4);       
    for (i = 0; i < word_size; i++)
    {
        if (0 != *p_data )  
        {
            return 10;    
        }
        
        p_data = p_data - 1; 
    }
    
    return 0;   
}

int fputc(int ch, FILE *f)
{
    UNUSED(f);
    
    USART1->DR = (uint32_t)ch;    
    while ((USART1->FR & UART_FR_BUSY));        
    
    return ch;
}

/******************************************************************************
*@brief : SRAM March Plus Test 
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t result1, result2;
    
    __set_PRIMASK(1);  
	result1 = SRAM_March_Plus_Test(0x20008000, 0x58000 >> 2);    
    
    printfS("SRAM_March_Plus_Test 0x20008000:%u\r\n", result1);       
    
    __set_MSP(0x2000C000);     
    
    result2 = SRAM_March_Plus_Test(0x20000000, 0x8000 >> 2);    
    
    printfS("SRAM_March_Plus_Test 0x20000000:%u\r\n", result2);    
    
    
    if ( (0 == result1) && (0 == result2) )  
    {
        printfS("SRAM_March_Plus_Test PASS!!!\n");
    }
    else
    {
        printfS("SRAM_March_Plus_Test FAIL!!!\n");
    }
    __set_PRIMASK(0);      
    
    while(1)
    {
        
    }
}
