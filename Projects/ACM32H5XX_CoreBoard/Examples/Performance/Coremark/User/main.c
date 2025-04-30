/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 



/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    HAL_Init();
    
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, 1, 1, 1, 1); 

    BSP_UART_Init(USART1, 115200);      
    HAL_DWT_Init();
    printfS("\r\n\r\n");
    printfS("************************************************************\r\n\r\n");
    
    printfS("Firmware compiled in %s %s\r\n", __DATE__, __TIME__);

    get_reset_source();
    BSP_MCO_Init(RCC_MCO_RCH, DISABLE, 10);

    printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
    printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
    printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
    printfS("************************************************************\r\n\r\n");
    
    
#ifdef KEIL_MDK_ARM   
    printfS("KeilV5.33£¬ARM Compiler V6.15\r\n");
    printfS("Optimizations[-ofast], close link-time optimization\r\n");
#else
    printfS("IAR8.50.9.33462\r\n");
    printfS("Optimizations[High-SPeed-No Size constraints]\r\n");
#endif    
    
    
    printfS("IRAM1: 0x20038000\n");
    
    
#if (INS_ACCELERATE_ENABLE == 1)
    printfS("Open ICACHE, ");
#else
    printfS("Close ICACHE, ");
#endif

#if (DATA_ACCELERATE_ENABLE == 1)
    printfS("Open DCACHE\r\n");
#else
    printfS("Close DCACHE\r\n");
#endif 

    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;   // disable SysTick IRQ
    SCB->ICSR = BIT25;   // clear systick pending bit           
    __set_PRIMASK(1);    // disable interrupt 

    
     
    extern int benchmark_coremark_main(void);
    printfS("\r\nACM32H5 coremark: Addr[0x%08x]\r\n", (uint32_t)&benchmark_coremark_main);
    benchmark_coremark_main();

   
    
    extern void Nop_ExecutionTime(void);
    //¹Ø±ÕICACHE+DCACHE
    System_DisableIAccelerate();
    System_DisableDAccelerate();
    printfS("\r\nACM32H5 Nop Execution Time: Addr[0x%08x]\r\n", (uint32_t)&Nop_ExecutionTime);
    Nop_ExecutionTime();
    
    __set_PRIMASK(0);    // enable interrupt 
    SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk; // Enable SysTick IRQ               
    

    while(1)
    {

    }
}