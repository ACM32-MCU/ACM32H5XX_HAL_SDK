
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  

extern void WDT_Interrupt_Test(void);
extern void WDT_Reset_Test(void);


/******************************************************************************
*@brief : WDT test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void WDT_Test(Test_mode_t test_mode)
{
    switch(test_mode)
    {
        case Test_WDT_Reset:
        {
            WDT_Reset_Test();
        }break;
        case Test_WDT_IRQ:
        {
            WDT_Interrupt_Test();
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

    printfS("Application running on SPI Flash\r\n");

    
    /*
    Test_WDT_Reset,
    Test_WDT_IRQ,
    */
    WDT_Test(Test_WDT_IRQ);

}



