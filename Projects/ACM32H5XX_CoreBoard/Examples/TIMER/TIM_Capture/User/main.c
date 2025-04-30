/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 
#include "app.h"   




/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    uint8_t received_byte; 
    
	HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    
    
    BSP_UART_Init(USART1, 115200);  
	
	printfS("\r\n\r\n");
	printfS("************************************************************\r\n\r\n");
	printfS("system startup\r\n");
	
	get_reset_source();
    RCC->CLKOCR = (RCC->CLKOCR & ~(0xFFFF << 6) ) | (7999 << 6); 
    
	printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
	printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
	printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
	printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
	printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
	printfS("************************************************************\r\n\r\n");  
    
    printfS("MCO:0x%08x, div1:%d, div2:%d\n", RCC->CLKOCR, (RCC->CLKOCR >> 6) & 0xFFFF, (RCC->CLKOCR >> 24) & 0x3F  );   
    
	Timer_Capture_Test(); 
    
//    Timer_Capture_Inter_Connect_Test(TIM5, TIM_CHANNEL_1, 10 << 0); // TI1-10, delta value = 1751    
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_1, 1 << 0);   
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_1, 2 << 0);     
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_1, 3 << 0);    
// 
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_1, 4 << 0);  // delta is about 1710       
//    
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_1, 6 << 0);  //      
//    
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_2, 1 << 8); 
//    
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_2, 2 << 8);   
//    
//    Timer_Capture_Inter_Connect_Test(TIM15, TIM_CHANNEL_2, 3 << 8); 

//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_1, 1 << 0);   
//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_1, 2 << 0);   
//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_1, 3 << 0);  
//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_1, 4 << 0);  

//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_2, 1 << 8); 
//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_2, 2 << 8);        
//    Timer_Capture_Inter_Connect_Test(TIM25, TIM_CHANNEL_2, 3 << 8);     

//    Timer_Capture_Inter_Connect_Test(TIM16, TIM_CHANNEL_1, 1 << 0);   
//    
//    Timer_Capture_Inter_Connect_Test(TIM16, TIM_CHANNEL_1, 6 << 0);     
    
    while(1)
    {

    }
}














