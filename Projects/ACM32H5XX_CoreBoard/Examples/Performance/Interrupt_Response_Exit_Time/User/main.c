/******************************************************************************
*@file  : main.c
*@brief : This file provides UART Demos
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"  


uint32_t int_index, tick_test;
extern volatile uint32_t systick_value_test1, systick_value_test2, systick_value_test3, svc_flag; 
#define NVIC_INT_CNT   166
#define ITCM_VECTOR_ADDRESS       0 
#define SPI_FLASH_VECTOR_ADDRESS  0x08002000  

extern void Generate_SVC_Int(void);  


int main(void)
{
    HAL_Init();  
    
    SystemClock_Config(SYSCLK_160M_SRC_RCH, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    __set_PRIMASK(1);   
    for(int_index = 0; int_index < NVIC_INT_CNT; int_index++)
    {
        *(volatile uint32_t *)(ITCM_VECTOR_ADDRESS + (int_index << 2) ) = *(volatile uint32_t *)(SPI_FLASH_VECTOR_ADDRESS + (int_index << 2) );      
    }
    
    SCB->VTOR = ITCM_VECTOR_ADDRESS;     
    __set_PRIMASK(0);   
    
    BSP_UART_Init(USART1, 115200); 
    
    printfS("Interrupt Response and EXit Time Test\n");  
    
    tick_test = 0;
    
    while(tick_test < 3) 
    {
        HAL_Delay(1000);
        tick_test++;  
        printfS("tick_test:%d\n", tick_test);  
    }
    
    svc_flag = 0;  

    Generate_SVC_Int(); 
    while(0 == svc_flag)
    {
        
    }
    
    printfS("systick1:%08x, systick2:%08x, systick3:%08x, Interrupt Response Time:%dHCLK, Interrupt Exit Time:%dHCLK\n", 
           systick_value_test1, systick_value_test2, systick_value_test3, systick_value_test1 - systick_value_test2, systick_value_test2 - systick_value_test3);  
    svc_flag = 0; 
    
    while(1)
    {
        
    }
}
