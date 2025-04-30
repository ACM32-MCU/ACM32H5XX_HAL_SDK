/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 
#include "app.h"


#define Function_Invalid_Instuction     ((void (*)(uint32_t, uint32_t ))(0x08100001))
#define Function_Jump_Function_Middle   ((void (*)(uint32_t, uint32_t ))(0x08004001))

void Enable_Fault_Capture(void)
{
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk; 
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;  
}

void BusFault_ErrorAddress_Call_NewFunc(void)
{
    *(uint32_t *)0x20060000 = 0; 
    printfS("BusFault_ErrorAddress\n");
}

void BusFault_ErrorAddress(void) 
{
    *(uint32_t *)0x20060000 = 0; 
}

void UsageFault_UnAlign(void)
{
    *(uint32_t *)0x54000001 = 0; 
}

extern uint32_t __initial_sp; 
volatile uint32_t sp_end; 

/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    uint32_t sp_start, sp_size;  
    
	HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);       
    
    BSP_UART_Init(USART1, 115200);  
	
    
	printfS("\r\n\r\n");
	printfS("************************************************************\r\n\r\n");
	printfS("system startup\r\n");
    
    #if defined ( __CC_ARM   ) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))  
    sp_start = (uint32_t)&__initial_sp;  
    sp_size =  0x2000; // Stack_Size 
    sp_end = sp_start - sp_size; 
    *(uint32_t *)(sp_end + 4) = 0xAA5555AA;  // Flag   
	printfS("sp_start: 0x%x, sp_end: 0x%x\r\n", sp_start, sp_end);       	
    #endif
    
    get_reset_source();
    
    BSP_MCO_Init(RCC_MCO_HCLK, ENABLE, 10);   
    
	printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
	printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
	printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
	printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
	printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
	printfS("************************************************************\r\n\r\n"); 
    
    Enable_Fault_Capture();  
    
    BusFault_ErrorAddress();  
//    BusFault_ErrorAddress_Call_NewFunc(); 
//    UsageFault_UnAlign();  
//    Function_Jump_Function_Middle(1, 2);   
//    Function_Invalid_Instuction(1, 2);  
    
    while(1)
    {
        
    }
}














