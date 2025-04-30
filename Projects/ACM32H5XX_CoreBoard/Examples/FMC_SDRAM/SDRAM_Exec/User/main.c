/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h" 
#include "app.h"
#include "sdram.h" 

uint32_t sram_test_data[TEST_DATA_LEN];     

void SystemInit_ExtMemCtl(void)
{
    SDRAM_Init();  
}

void SystemInit(void)
{  
    SCB->VTOR = VECT_TAB_ADDR;   
    
    if (0 == (ITCM_CR & ITCM_CR_EN) ) 
    {
        ITCM_CR |= ITCM_CR_EN;     // If ITCM not enabled, enable ITCM 
    }        
    
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
		/* set CP10 and CP11 Full Access */
		SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
	#endif
    
    RCC->RCHCR |= RCC_RCHCR_RCHEN;
    __NOP();__NOP();

    while (!(RCC->RCHCR & RCC_RCHCR_RCHRDY)) {}
    RCC->RCHCR &= ~RCC_RCHCR_RCHSEL;
    RCC->CCR1 &= ~RCC_CCR1_SYSCLKSEL;
    
    RCC->PLL1CR &= ~RCC_PLL1CR_PLL1EN;
    RCC->PLL1SCR = 0;
    RCC->PLL1CR |= RCC_PLL1CR_PLL1SLEEP;
        
    RCC->PLL2CR &= ~RCC_PLL2CR_PLL2EN;
    RCC->PLL2SCR = 0;
    RCC->PLL2CR |= RCC_PLL2CR_PLL2SLEEP; 
        
    RCC->PLL3CR &= ~RCC_PLL3CR_PLL3EN;
    RCC->PLL3CR |= RCC_PLL3CR_PLL3SLEEP;
        
    RCC->RCHCR &= ~RCC_RCHCR_RCHDIV;
    
    RCC->CCR2 &= ~RCC_CCR2_SYSDIV0;
    __NOP();__NOP();

    while (!(RCC->CCR2 & RCC_CCR2_DIVDONE)) {}
    
    RCC->CCR2 &= ~RCC_CCR2_SYSDIV1;
    __NOP();__NOP();

    while (!(RCC->CCR2 & RCC_CCR2_DIVDONE)) {}
        
    __HAL_RCC_EFUSE1_CLK_ENABLE();   
    
    RCC->CCR2 &= ~(RCC_CCR2_PCLK1DIV | RCC_CCR2_PCLK2DIV | RCC_CCR2_PCLK3DIV | RCC_CCR2_PCLK4DIV);
        
	SystemCoreClock = HAL_RCC_GetRCHTrimFreq(); 
    HAL_DWT_Init();      
        
	#ifdef DATA_IN_ExtSRAM
	SystemInit_ExtMemCtl();  	
	#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */  
        
    HAL_DWT_DeInit(); 
        
	#if (INS_ACCELERATE_ENABLE)
		System_EnableIAccelerate();
    #else
		System_DisableIAccelerate();
	#endif    
  
}

/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{  
    int i; 
    
    HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);    

    BSP_UART_Init(USART1, 115200);    

    printfS("\r\n\r\n");
    printfS("************************************************************\r\n\r\n");
    printfS("system startup\r\n");

    get_reset_source();
    BSP_MCO_Init(RCC_MCO_RCH, ENABLE, 10);

    printfS("HCLK: %u\r\n", HAL_RCC_GetHCLKFreq());
    printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
    printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
    printfS("************************************************************\r\n\r\n");
    
    // Check data in SDRAM has been initiated or not 
    for (i = 0; i < TEST_DATA_LEN; i++)
    {
        printfS("SDRAM addr:0x%x, data:%d\n", (uint32_t)(SDRAM_Data_Test+i), SDRAM_Data_Test[i]);
    }
    
    for (i = 0; i < TEST_DATA_LEN; i++)
    {
        sram_test_data[i] = SDRAM_Data_Test[i];   
    }
    
    // App_in_SDRAM function is executed in SDRAM 
    App_in_SDRAM(sram_test_data);  
    
    for (i = 0; i < TEST_DATA_LEN; i++)
    {
        printfS("SDRAM addr:0x%x, data:0x%x\n", (uint32_t)(sram_test_data+i), sram_test_data[i]);   
        if (sram_test_data[i] != (SDRAM_Data_Test[i] + 0xAA01) )
        {
            printfS("\nData Error!!!\n");
            while(1);
        }
    }
    
    printfS("\nPASS!!!\n");   

    while(1)
    {

    }
    
}














