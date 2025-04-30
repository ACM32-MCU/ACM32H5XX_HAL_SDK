

#include "app.h"  


/******************************************************************************
*@brief : set alarm
*@param : mode :Week mode or day mode
*@return: none
******************************************************************************/
void RTC_SetAlarm(uint32_t mode)
{
    RTC_AlarmTypeDef temp_Alarm;
    temp_Alarm.u32_AlarmMode        = mode;                               
    temp_Alarm.u32_AlarmInterrupt   = RTC_ALARM_INT_ENABLE;               // 开启闹钟中断
    temp_Alarm.u32_DayMask          = RTC_ALARM_DAY_MASK_DISABLE;         // 天屏蔽关闭
    temp_Alarm.u32_HourMask         = RTC_ALARM_HOUR_MASK_DISABLE;        // 小时屏蔽关闭
    temp_Alarm.u32_MinMask          = RTC_ALARM_MIN_MASK_DISABLE;         // 分钟屏蔽关闭
    if(temp_Alarm.u32_AlarmMode == RTC_ALM_ALM_WDS_WEEK)// 使用星期模式
    {
        temp_Alarm.u32_AlarmWeek    = RTC_ALARM_WEEK_MONDAY | RTC_ALARM_WEEK_SUNDAY;    // 周一、周日
    }
    else
    {
        temp_Alarm.u32_AlarmDay     = 0x21;
    }
    temp_Alarm.u32_Hours            = 0x10;    // 时间 10：30：10
    temp_Alarm.u32_Minutes          = 0x30;
    temp_Alarm.u32_Seconds          = 0x10;
    HAL_RTC_AlarmConfig(&temp_Alarm);

    NVIC_ClearPendingIRQ(RTC_XTLSD_IRQn);
    NVIC_EnableIRQ(RTC_XTLSD_IRQn);
    /* RTC domain write enable */
    SET_BIT(PMU->CTRL0,PMU_CTRL0_RTCWE);
    HAL_RTC_AlarmEnable();
    __HAL_RTC_ENABLE_ALM_IT;
}


/******************************************************************************
*@brief : RTC alarm test
*@param : none
*@return: none
******************************************************************************/
void RTC_Alarm_Test(void)
{
    RTC_ConfigTypeDef RTC_Handle;
    
    printfS("RTC Alarm test\r\n");     
    gRTCIrqFlag = 0;

    RTC_Handle.ClockSource       = RTC_CLOCK_RC32K;
    RTC_Handle.Compensation      = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.CompensationValue = 0x00;                     // 开始时钟补偿，如不需补偿设置补偿值为0
    HAL_RTC_Config(&RTC_Handle);
    
    
    RTC_SetDateTime();
    RTC_SetAlarm(RTC_ALM_ALM_WDS_DAY);

    while(1)
    {
        RTC_GetDateTime();
        if(gRTCIrqFlag & RTC_SR_ALM_IF)
        {
            gRTCIrqFlag = 0;
            printfS("\r\n****************\r\n");
            printfS("Alarm Irq Occur\r\n");
            printfS("\r\n****************\r\n");
            RTC_SetDateTime();
        }
    }    
}





