
#include "hal.h"  

static WDT_HandleTypeDef WDT_Handle;
static volatile uint32_t g_IRQFlag = 0;
static volatile uint32_t g_wdtRis = 0;

void WDT_IRQHandler(void)
{
    HAL_WDT_Callback(&WDT_Handle);
}


/******************************************************************************
*@brief : WDT interrupt callback function 
*@param : none
*@return: none
******************************************************************************/
void HAL_WDT_Callback(WDT_HandleTypeDef *hwdt)
{
    g_wdtRis = WDT->RIS;
    //printfS("WDT->RIS = 0x%08x\r\n", WDT->RIS);
    /* 喂狗，清除中断标志 */
    HAL_WDT_Refresh(&WDT_Handle);
    NVIC_ClearPendingIRQ(WDT_IRQn);
    g_IRQFlag = 1;
}


/******************************************************************************
*@brief : IWDT interrup initialization 
*@param : none
*@return: none
******************************************************************************/
void WDT_Interrupt_Test(void)
{  
    uint32_t irqTime;
    
    printfS("WDT Interrupt test\r\n");       

    /* 使能：WDT复位导致系统复位 */
    HAL_RCC_WDTResetConfig(ENABLE);
    
    __HAL_RCC_WDT_CLK_ENABLE();
    __HAL_RCC_WDT_RESET();
    
    /* 中断时间：1000ms*/
    irqTime = HAL_RCC_GetPCLK1Freq() / 128;  
    
    /* 配置WDT */
    WDT_Handle.Instance         = WDT;
    WDT_Handle.Init.Prescaler   = WDT_CLOCK_PRESCALER_128;
    WDT_Handle.Init.Mode        = WDT_MODE_INTERRUPT;
    WDT_Handle.Init.Load        = irqTime ;
    WDT_Handle.Init.IntClrTime  = 0xffff;
    
    HAL_WDT_Init(&WDT_Handle);
    

    /* 使能中断 */
    WDT_GET_IT_ENABLE(&WDT_Handle);
    NVIC_ClearPendingIRQ(WDT_IRQn);
    NVIC_SetPriority(WDT_IRQn, 0x00);
    NVIC_EnableIRQ(WDT_IRQn);

    HAL_WDT_Refresh(&WDT_Handle);
    g_IRQFlag = 0;
    
    while(1)
    {
        if (g_IRQFlag)
        {
            printfS("WDT IRQ Occurs! wdt_ris = 0x%08x\n", g_wdtRis); 
            g_wdtRis = 0;
            g_IRQFlag = 0;
        } 
    }
    
}





