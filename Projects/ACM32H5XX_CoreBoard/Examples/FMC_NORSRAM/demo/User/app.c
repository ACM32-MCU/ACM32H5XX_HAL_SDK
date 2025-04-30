
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "lcd8080.h"
#include "sram.h"
#include "psram.h"
#include "norflash.h"

/******************************************************************************
*@brief : NORSRAM test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void NORSRAM_Test(Test_mode_t test_mode)
{
    switch(test_mode)
    {
        case Test_LCD8080:
        {
            FMC_LCD8080_Test();
        }break;
        case Test_SRAM:
        {
            FMC_SRAM_Test();
        }break;
        case Test_PSRAM:
        {
            FMC_PSRAM_Test();
        }break;
        case Test_NORFLASH:
        {
            FMC_Norflash_Test();
        }break;
        case Test_SRAM_MUX:
        {
            FMC_SRAM_MUX_Test();
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
    Test_LCD8080,
	Test_SRAM,
    Test_PSRAM,
    Test_NORFLASH,
    Test_SRAM_MUX,
    */
    NORSRAM_Test(Test_NORFLASH);
}



