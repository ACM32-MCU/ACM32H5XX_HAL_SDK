
#include "rtc.h"  


volatile uint32_t gRTCIrqFlag = 0;

/******************************************************************************
*@brief : RTC IRQHandler
*@param : none
*@return: none
******************************************************************************/
void RTC_XTLSD_IRQHandler(void)
{
    /* RTC domain write enable */
	PMU->CTRL0 |= PMU_CTRL0_RTCWE;
    if(RTC->SR)
    {
        gRTCIrqFlag |= RTC->SR;
        RTC->SR = gRTCIrqFlag; 
    } 
}

/******************************************************************************
*@brief : RTC initialization 
*@param : none
*@return: none
******************************************************************************/
void RTC_Init(void)
{
    RTC_ConfigTypeDef RTC_Handle;

    RTC_Handle.ClockSource          = RTC_CLOCK_RC32K;
    RTC_Handle.Compensation         = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.CompensationValue    = 0x00;                     // 开始时钟补偿，如不需补偿设置补偿值为0
    HAL_RTC_Config(&RTC_Handle);
}

/******************************************************************************
*@brief : Set date and time
*@param : none
*@return: none
******************************************************************************/
void RTC_SetDateTime(void)
{
    RTC_TimeTypeDef  temp_Time_Set;
    RTC_DateTypeDef  temp_Date_Set;

    /* Set RTC Time、Date */
    temp_Time_Set.Hour              = 0x16;
    temp_Time_Set.Minute            = 0x15;
    temp_Time_Set.Second            = 0x00;
    HAL_RTC_SetTime(&temp_Time_Set);

    temp_Date_Set.Year              = 0x24;    
    temp_Date_Set.Month             = 0x07;
    temp_Date_Set.Date              = 0x19;
    temp_Date_Set.WeekDay           = 0x05;
    HAL_RTC_SetDate(&temp_Date_Set);

    /* Write-Protect Disable */
    RTC->WP = RTC_WRITE_PROTECT_DISABLE;
    RTC->MSECCNT                    = 0x00;
    /* Write-Protect Enable */
    RTC->WP = RTC_WRITE_PROTECT_ENABLE;
}

/******************************************************************************
*@brief : Get date and time
*@param : none
*@return: none
******************************************************************************/
void RTC_GetDateTime(void)
{
    static uint32_t fu32_Seconds;
    RTC_TimeTypeDef  temp_Time_Get;
    RTC_TimeTypeDef  temp_Time_Get_Again;
    RTC_DateTypeDef  temp_Date_Get;
    HAL_RTC_GetTime(&temp_Time_Get);
    HAL_RTC_GetDate(&temp_Date_Get);
    HAL_RTC_GetTime(&temp_Time_Get_Again);
    if (temp_Time_Get.Second != temp_Time_Get_Again.Second) 
    {
        return;   
    }
    if (fu32_Seconds != temp_Time_Get.Second) 
    {
        fu32_Seconds = temp_Time_Get.Second;
        
        printf("Date: 20%x-%x-%x week:%x \n", temp_Date_Get.Year, temp_Date_Get.Month, temp_Date_Get.Date, temp_Date_Get.WeekDay);
        printf("Time: %x:%x:%x:%d \n", temp_Time_Get.Hour, temp_Time_Get.Minute, temp_Time_Get.Second, RTC->MSECCNT);
    }
}

/******************************************************************************
*@brief : RTC wakeup timer initialization 
*@param : none
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

}


/******************************************************************************
*@brief : RTC temper initialization
*@param : adge :trigger edge
*@param : clearBkp :Clear backup register
*@return: none
******************************************************************************/
void RTC_Tamper_Init(uint32_t adge, uint32_t clearBkp)
{
    GPIO_InitTypeDef GPIO_Handle;
    RTC_TemperTypeDef temp_Temper;
    
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* PC13--RTC_TAMP1, PI8--RTC_TAMP2 */         
    GPIO_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Pin       = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_Handle);

    //GPIO_Handle.Pin       = GPIO_PIN_8;
    //HAL_GPIO_Init(GPIOI, &GPIO_Handle);

        
    temp_Temper.u32_TemperEdge  = adge;
    temp_Temper.u32_InterruptEN = RTC_TEMP_INT_ENABLE;
    temp_Temper.u32_ClearBackup = clearBkp;
    temp_Temper.u32_FilterClk   = RTC_TEMP_FILTER_512_RTCCLK;
    temp_Temper.u32_Filter      = RTC_TEMP_FILTER_4_RTCCLK;
    temp_Temper.u32_FilterEn    = DISABLE;
    HAL_RTC_Tamper(RTC_TEMPER_1, &temp_Temper);
    //HAL_RTC_Tamper(RTC_TEMPER_2, &temp_Temper);
}


