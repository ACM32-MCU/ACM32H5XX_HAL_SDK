
#include "app.h"  


/******************************************************************************
*@brief : RTC wake-up timer initialization
*@param : source :wake-up clock selection
*@param : counter :wake-up reload
*@return: none
******************************************************************************/
void RTC_WakeupTimer_Init(uint32_t source, uint32_t counter)
{
    RTC_WUTimerTypeDef  WuTimer_Handler;
    //Wakeup Timer config 
    WuTimer_Handler.u32_WuckSel      = source;  
    WuTimer_Handler.u32_InterruptEN  = ENABLE;
    WuTimer_Handler.WakeUpCounter    = counter;
    HAL_RTC_SetWakeUpTimer(&WuTimer_Handler);
    
    __HAL_RTC_ENABLE_WUTIE_IT;
    
    NVIC_ClearPendingIRQ(RTC_XTLSD_IRQn);
    NVIC_EnableIRQ(RTC_XTLSD_IRQn);
}

/******************************************************************************
*@brief : Enter standby
*@param : none
*@return: none
******************************************************************************/
void Enter_Standby(void)
{
    printfS("MCU enter standby mode \r\n");  
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;   // disable systick        
    __set_PRIMASK(1);    // disable interrupt 
    SCB->ICSR = BIT25;   // clear systick pending bit
    
    HAL_PMU_EnterStandbyMode(WAIT_FOR_INT);
    
    __set_PRIMASK(0);    // enable interrupt
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* Enable SysTick IRQ */               
     
    printfS("MCU never run here \r\n");
    
}


/******************************************************************************
*@brief : RTC wake-up timer test
*@param : none
*@return: none
******************************************************************************/
void RTC_WakeupTimer_Test(void)
{
    RTC_ConfigTypeDef RTC_Handle;
    printfS("RTC WakeupTimer test\r\n");
    
    HAL_PMU_Init();
    printfS("PMU->SR =0x%x\r\n", PMU->SR);
    if((PMU->SR & PMU_FLAG_RTC)&&(PMU->SR & PMU_FLAG_STANDBY))
    {
        printfS("MCU wakeup by RTC\r\n");            
    }
    RTC_Handle.ClockSource       = RTC_CLOCK_RC32K;
    RTC_Handle.Compensation      = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.CompensationValue = 0x00;                     // 开始时钟补偿，如不需补偿设置补偿值为0
    HAL_RTC_Config(&RTC_Handle);    
    
    RTC_SetDateTime();
    RTC_WakeupTimer_Init(RTC_WAKEUPCLOCK_1HZ, 3);
    gRTCIrqFlag = 0;
    RTC->SR = RTC_SR_WUTF; 
    while(1)
    {
        RTC_GetDateTime();
        if (gRTCIrqFlag & RTC_SR_WUTF) 
        {
            gRTCIrqFlag = 0;
            printfS("Wake-Timer interrupt has been triggered!!! \r\n");
            HAL_PMU_StandbyWakeupRTCConfig(STANDBY_WAKEUP_WUTIE); 
            Enter_Standby();
        }
    }
  
}




