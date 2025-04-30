
#include "rtc.h"
#include "lowpower.h"


/******************************************************************************
*@brief : VBAT power supply test
*@param : none
*@return: none
******************************************************************************/
void VBAT_supply_Power_Test(void)
{
    uint32_t source;
    uint8_t cnt;
    
    printfS("VBAT power supply test\r\n");
    
    /* Enable BKPSRAM CLK */
    __HAL_RCC_BKPSRAM_CLK_ENABLE();
    
    RTC_Init();
    
    printfS("\r\n****************************************\r\n");
    printfS("backup0  = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR0));
    printfS("backup5  = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR5));
    printfS("backup10 = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR10));
    printfS("backup15 = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR15));
    printfS("bkpSram  = 0x%08x\r\n", *(volatile uint32_t*)(BKPSRAM_BASE_ADDR));
    printfS("****************************************\r\n");
    
    if((0x11111111 != HAL_RTC_ReadBackupRegister(RTC_BKP_DR0)) && (0x55555555 != (*(volatile uint32_t*)(BKPSRAM_BASE_ADDR))))
    {
        printfS("\r\n****************************************\r\n");
        printfS("Write RTC BACKUP & BAKSRAM\r\n");
        HAL_RTC_WriteBackupRegister(RTC_BKP_DR0, 0x11111111);
        HAL_RTC_WriteBackupRegister(RTC_BKP_DR5, 0x22222222);
        HAL_RTC_WriteBackupRegister(RTC_BKP_DR10, 0x33333333);
        HAL_RTC_WriteBackupRegister(RTC_BKP_DR15, 0x44444444);
        *(volatile uint32_t*)(BKPSRAM_BASE_ADDR) = 0x55555555;      
        PMU->CTRL0 &= ~PMU_CTRL0_BKPRAMSEN;
        PMU->CTRL0 |= PMU_CTRL0_BKPRAMREN;    
        
        printfS("backup0  = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR0));
        printfS("backup5  = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR5));
        printfS("backup10 = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR10));
        printfS("backup15 = 0x%08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR15));
        printfS("bkpSram  = 0x%08x\r\n", *(volatile uint32_t*)(BKPSRAM_BASE_ADDR));
        printfS("****************************************\r\n");
    }
    
    RTC_Tamper_Init(RTC_TEMP_EDGE_RISING, RTC_TEMP_CLEAR_DISABLE);
    HAL_PMU_WakeupIOInit(PMU_WAKEUP4, PMU_WAKEUP_HIGH);    
    HAL_PMU_StandbyWakeupRTCConfig(STANDBY_WAKEUP_STAMP1);
    
    cnt = 0;
    while(1)
    {
        //RTC_GetDateTime();
        
        if(cnt++ > 20)
        {
            cnt = 0;
            printfS("MCU enter Standby mode \r\n");
            
            __set_PRIMASK(1);    // disable interrupt
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick 
            SCB->ICSR = BIT25;   // clear systick pending bit  

            Enter_Standby_RunInSram(WAIT_FOR_INT); 

            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick            
            __set_PRIMASK(0);    // enable interrupt    

            printfS("MCU never run here \r\n");
        }
        printfS("[%d] \r\n", cnt);
        HAL_DelayMs(500);
    }        
}  