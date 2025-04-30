
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  


extern void RTC_Date_Time_Test(void);
extern void RTC_Alarm_Test(void);
extern void RTC_Tamper_Test(void);
extern void RTC_WakeupTimer_Test(void);



volatile uint32_t gRTCIrqFlag = 0;

void RTC_IRQCallback(void)
{
    gRTCIrqFlag = RTC->SR;
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_MIN))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_MIN);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_ALM))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_ALM);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_32S))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_32S);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_STP1F))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_STP1F);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_STP1R))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_STP1R);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_STP2F))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_STP2F);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_STP2R))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_STP2R);
    }
    if(SET == HAL_RTC_GetITStatus(RTC_IT_FLAG_WUT))
    {
        HAL_RTC_ClearITPendingBit(RTC_IT_FLAG_WUT);
    }
}


/******************************************************************************
*@brief : RTC test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void RTC_Test(Test_mode_t test_mode)
{
    switch(test_mode)
    {
        case Test_RTC_Date_Time:
        {
            RTC_Date_Time_Test();
        }break;
        case Test_RTC_Alarm:
        {
            RTC_Alarm_Test();
        }break;
        case Test_RTC_Tamper:
        {
            RTC_Tamper_Test();
        }break;
        case Test_RTC_WakeupTimer:
        {
            RTC_WakeupTimer_Test();
        }break;
        default:
        {
            printfS("This application does not support\r\n");
        }break;
    }
}

/******************************************************************************
* @brief : app test.
* @param : None
* @return: None
******************************************************************************/
void APP_Test(void)
{
    /*
    Test_RTC_Date_Time,
    Test_RTC_Alarm,
	Test_RTC_Tamper,
    Test_RTC_WakeupTimer,
    */
    
    RTC_Test(Test_RTC_WakeupTimer);

}



