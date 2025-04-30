
#include "app.h"  


/******************************************************************************
*@brief : Set Date and Time
*@param : none
*@return: none
******************************************************************************/
void RTC_SetDateTime(void)
{
    RTC_TimeTypeDef  temp_Time_Set;
    RTC_DateTypeDef  temp_Date_Set;
    /* Set RTC Time、Date */
    temp_Time_Set.Hour      = 0x10;
    temp_Time_Set.Minute    = 0x30;
    temp_Time_Set.Second    = 0x00;
    HAL_RTC_SetTime(&temp_Time_Set);

    temp_Date_Set.Year      = 0x22;    
    temp_Date_Set.Month     = RTC_MONTH_OCTOBER;
    temp_Date_Set.Date      = 0x21;
    temp_Date_Set.WeekDay   = RTC_WEEKDAY_SUNDAY;//周日
    HAL_RTC_SetDate(&temp_Date_Set);
}

/******************************************************************************
*@brief : get Date and Time
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
        printf("Time: %x:%x:%x \n", temp_Time_Get.Hour, temp_Time_Get.Minute, temp_Time_Get.Second);
    }
}



/******************************************************************************
*@brief : RTC date and time test
*@param : none
*@return: none
******************************************************************************/
void RTC_Date_Time_Test(void)
{
    RTC_ConfigTypeDef RTC_Handle;
    
    printfS("RTC Date&Time test\r\n");     
  
    RTC_Handle.ClockSource       = RTC_CLOCK_RC32K;
    RTC_Handle.Compensation      = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.CompensationValue = 0x00;                     // 开始时钟补偿，如不需补偿设置补偿值为0
    HAL_RTC_Config(&RTC_Handle);
    
    
    RTC_SetDateTime();
    while(1)
    {
        RTC_GetDateTime();
    }
}




