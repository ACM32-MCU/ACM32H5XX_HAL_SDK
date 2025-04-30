
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 
#include "rtc.h"

volatile uint32_t g_extiLine = 0;


extern void PMU_Sleep_test(void);
extern void PMU_Stop_test(void);
extern void PMU_Standby_test(void);
extern void PMU_BOR_test(void);
extern void PMU_LVD_test(void);
extern void VBAT_supply_Power_Test(void);
extern void PMU_ANAOUT_Test(void);



/******************************************************************************
*@brief : EXTI interrupt callback function
*@param : line: exti line
*@return: none
******************************************************************************/
void HAL_EXTI_LineCallback(uint32_t Line)
{		
    g_extiLine = Line;
    if(Line == 0)
    {
        g_extiLine = 0xff;
    }        
}



/******************************************************************************
*@brief : PMU test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void PMU_Test(Test_mode_t test_mode)
{
    HAL_PMU_Init();
    
    switch(test_mode)
    {
        case Test_PMU_Sleep:
        {
            PMU_Sleep_test();
        }break;
        case Test_PMU_Stop:
        {
            PMU_Stop_test();
        }break;
        case Test_PMU_Standby:
        {
            PMU_Standby_test();
        }break;
        case Test_PMU_BOR:
        {
            PMU_BOR_test();            
        }break;
        case Test_PMU_LVD:
        {
            PMU_LVD_test();            
        }break;
        case Test_PMU_VBAT:
        {
            VBAT_supply_Power_Test();            
        }break;
        case Test_PMU_ANAOUT:
        {
            PMU_ANAOUT_Test();            
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
    HAL_DWT_Init();
    /*
    Test_PMU_Sleep,
    Test_PMU_Stop,
    Test_PMU_Standby,
    Test_PMU_BOR,
    Test_PMU_LVD,
    Test_PMU_VBAT,
    Test_PMU_ANAOUT,
    */
    
    PMU_Test(Test_PMU_Stop);

}



