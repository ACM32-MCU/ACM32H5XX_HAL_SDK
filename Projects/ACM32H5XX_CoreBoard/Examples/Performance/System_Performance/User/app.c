/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

extern uint32_t fast_access_data[4096]; 

void DTCM_Access_Test(void)
{
    uint32_t i;  
    
    printfS("\nDTCM Write and Read Test:\n");
    
    for(i = 0; i < 4096; i++)
    {
        fast_access_data[i] = i;   
    }
    
    i = 0;  
	while(1)
    {
        printfS("Data Addr:0x%x, Data:%d\r\n", (uint32_t)(&(fast_access_data[i%4096])), fast_access_data[i%4096]);
        HAL_Delay(1000); 
        i++;   
    }
}

void Copy_Vector_Table_Remap(void)
{
    uint32_t i; 
    volatile uint32_t * itcm_ptr; 
    volatile uint32_t *spi_ptr; 
    
    __set_PRIMASK(1);  
    itcm_ptr = (uint32_t *)ITCM_BASE_ADDRESS; 
    spi_ptr = (uint32_t *)0x08002000;   
    
    for(i = 0; i < 166; i++) // Copy The Whole Vector Table, You can only copy the first 16 vectors and the vectors used in your application 
    {
        itcm_ptr[i] = spi_ptr[i];   
    }
    
    SCB->VTOR = 0;  
    __set_PRIMASK(0);    
}

