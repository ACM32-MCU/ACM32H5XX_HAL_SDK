

#include "hal.h"  

static WDT_HandleTypeDef WDT_Handle;


/******************************************************************************
*@brief : WDT reset initialization 
*@param : none
*@return: none
******************************************************************************/
void WDT_Reset_Test(void)
{
    uint8_t cnt; 
    uint32_t resetTime;
    printfS("WDT Reset test\r\n");     
    
    /* 使能：WDT复位导致系统复位 */
    HAL_RCC_WDTResetConfig(ENABLE);
    
    __HAL_RCC_WDT_CLK_ENABLE();
    __HAL_RCC_WDT_RESET();
    /* 复位时间：1秒 */
    resetTime = HAL_RCC_GetPCLK1Freq() / 4;  
    
    /* 配置WDT */
    WDT_Handle.Instance         = WDT;
    WDT_Handle.Init.Prescaler   = WDT_CLOCK_PRESCALER_4;
    WDT_Handle.Init.Mode        = WDT_MODE_RESET;
    WDT_Handle.Init.Load        = resetTime;
    
    HAL_WDT_Init(&WDT_Handle);

    HAL_WDT_Refresh(&WDT_Handle);
    
    cnt = 0;
    
    while(1)
    {
        HAL_DelayMs(900);
        if(cnt++ < 20)
        {
            HAL_WDT_Refresh(&WDT_Handle);
            printfS("[%d]feed wachdog time: %u\r\n", cnt, HAL_GetTick()); 
        }
        else
        {
            printfS("[%d]: %u\r\n", cnt, HAL_GetTick());
        }    
        
    } 
}

