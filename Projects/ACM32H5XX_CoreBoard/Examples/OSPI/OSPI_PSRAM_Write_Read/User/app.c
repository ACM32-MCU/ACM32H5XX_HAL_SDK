
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "xccela_opi.h"
#include "hyper_ram.h"
#include "xspi_flash.h"


/******************************************************************************
*@brief : OSPI test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void OSPI_Test(Test_mode_t test_mode)
{
    switch(test_mode)
    {
        case Test_Xccela:
        {
            APS6408L_OBM_OSPI_Xccela_Test();
        }break;
        case Test_Hyper:
        {
            W958D8NBYA_OSPI_HyperBus_Test();
        }break;
        case Test_xSPI:
        {
            GD25LX256E_FIFO_Test(OSPI_8X_MODE, OSPI_DTRM_STR, OSPI_WORK_MODE_3);
            GD25LX256E_FIFO_Test(OSPI_8X_MODE, OSPI_DTRM_DTR, OSPI_WORK_MODE_0);
            
            GD25LX256E_OSPI_STR_Memory_Test();
            GD25LX256E_OSPI_DTR_Memory_Test();
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
    Test_Xccela,
    Test_Hyper,
    Test_xSPI,
    */

    OSPI_Test(Test_Xccela);

}



