
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "apm_opi.h"
#include "xccela_opi.h"
#include "hyper_ram.h"
#include "xspi_flash.h"

uint32_t test_fail_flag=0;
/******************************************************************************
* @brief : app test.
* @param : None
* @return: None
******************************************************************************/
void APP_Test(void)
{
    APS6408L_OBM_OSPI_Xccela_Test();
    if(test_fail_flag)
    {
        printfS("psram test fail\r\n"); 
        printfS("<FT_FAIL>\r\n");
    }
    else
    {
        printfS("psram test pass\r\n"); 
        printfS("<FT_PASS>\r\n");
    }
}



