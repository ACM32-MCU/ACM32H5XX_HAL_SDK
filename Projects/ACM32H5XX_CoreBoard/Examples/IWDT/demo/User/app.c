
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  

extern void IWDT_Wakeup_Test(void);
extern void IWDT_Reset_Test(void);
extern void IWDT_Reset_Wakeup_Standby(void);

/******************************************************************************
*@brief : IWDT test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void IWDT_Test(Test_mode_t test_mode)
{
    switch(test_mode)
    {
        case Test_IWDT_Reset:
        {
            IWDT_Reset_Test();
        }break;
        case Test_IWDT_Wakeup:
        {
            IWDT_Wakeup_Test();
        }break;
        case Test_IWDT_Wakeup_Standby:
        {
            IWDT_Reset_Wakeup_Standby();            
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
    Test_IWDT_Reset,
	Test_IWDT_Wakeup,
    Test_IWDT_Wakeup_Standby,
    */
    
    IWDT_Test(Test_IWDT_Wakeup_Standby);

}



