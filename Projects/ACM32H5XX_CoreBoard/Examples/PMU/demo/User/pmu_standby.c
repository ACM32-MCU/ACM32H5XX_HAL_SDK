


#include "hal.h"  
#include "rtc.h"
#include "lowpower.h"



/******************************************************************************
*@brief : get wake-up flag
*@param : none
*@return: none
******************************************************************************/
uint32_t Get_Wakeup_Source(void)
{
    uint32_t wakeup_source;
    printfS("PMU->SR = 0x%08x\r\n", PMU->SR);
    wakeup_source = HAL_PMU_GetWakeupSource();
    
    //清除所有唤醒标志
    HAL_PMU_ClearWakeupStatus(ALL_WANKEUP_STATUS);
    
    if((wakeup_source & PMU_SR_SBF) ==PMU_SR_SBF)
    {
        printfS("MCU Entered Standby \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_SBF);
    }
    if((wakeup_source & PMU_SR_WUPFX_0) == PMU_SR_WUPFX_0)
    {
        printfS("WakeUp by PA0(WAKEUP1) \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_WUPFX_0);
    }
    if((wakeup_source & PMU_SR_WUPFX_1) == PMU_SR_WUPFX_1)
    {
        printfS("WakeUp by PA2(WAKEUP2) \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_WUPFX_1);
    }
    if((wakeup_source & PMU_SR_WUPFX_2) == PMU_SR_WUPFX_2)
    {
        printfS("WakeUp by PI8(WAKEUP3) \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_WUPFX_2);
    }
    if((wakeup_source & PMU_SR_WUPFX_3) == PMU_SR_WUPFX_3)
    {
        printfS("WakeUp by PC13(WAKEUP4) \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_WUPFX_3);
    }
    if((wakeup_source & PMU_SR_WUPFX_4) == PMU_SR_WUPFX_4)
    {
        printfS("WakeUp by PI11(WAKEUP5) \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_WUPFX_4);
    }
    if((wakeup_source & PMU_SR_RTCWUF) == PMU_SR_RTCWUF)
    {
        printfS("WakeUp by RTC \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_RTCWUF);
    }
    if((wakeup_source & PMU_SR_RSTWUF) == PMU_SR_RSTWUF)
    {
        printfS("WakeUp by Reset \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_RSTWUF);
    }
    if((wakeup_source & PMU_SR_IWDTWUF) == PMU_SR_IWDTWUF)
    {
        printfS("WakeUp by IWDT \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_IWDTWUF);
    }
    if((wakeup_source & PMU_SR_BORWUF) == PMU_SR_BORWUF)
    {
        printfS("WakeUp by BOR \n");
        HAL_PMU_ClearWakeupStatus(PMU_SR_BORWUF);
    }
    return wakeup_source;
}

/******************************************************************************
*@brief : standby test
*@param : none
*@return: none
******************************************************************************/
void PMU_Standby_test(void)
{
    uint8_t cnt;
    printfS("WKUP4(PC13) pin high-level wake-up standby\r\n");
    
    Get_Wakeup_Source();
    RTC_Init();
    RTC_SetDateTime();    
    NVIC_ClearPendingIRQ(RTC_XTLSD_IRQn);
    NVIC_EnableIRQ(RTC_XTLSD_IRQn);
    gRTCIrqFlag = 0;
    cnt = 0;    

    RTC_Tamper_Init(RTC_TEMP_EDGE_RISING, RTC_TEMP_CLEAR_DISABLE);   
    HAL_PMU_StandbyWakeupRTCConfig(STANDBY_WAKEUP_STAMP1);
    
    HAL_PMU_WakeupIOInit(PMU_WAKEUP4, PMU_WAKEUP_HIGH);
    
    /* BKPSRAM不进入保持状态，进入睡眠状态 */
    RCC->AHB1CKENR |= RCC_AHB1CKENR_BKPSRAMCKEN; 
    PMU->CTRL0 = (PMU->CTRL0 & (~(PMU_CTRL0_BKPRAMREN | PMU_CTRL0_BKPRAMSEN))) | \
                        PMU_CTRL0_BKPRAMREN;
    while(1)
    {
        RTC_GetDateTime();        

        if(cnt++ > 10)
        {
            cnt = 0;
            printfS("MCU enter Standby mode \r\n");
            
            Enter_Standby_RunInSram(WAIT_FOR_INT);            
             
            printfS("MCU never run here \r\n");
        }            
        HAL_DelayMs(500);        
    }    
  
}


