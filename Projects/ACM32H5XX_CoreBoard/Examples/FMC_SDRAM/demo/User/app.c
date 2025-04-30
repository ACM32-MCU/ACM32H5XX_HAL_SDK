
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "sdram.h"


/******************************************************************************
*@brief : SDRAM test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void SDRAM_Test(Test_mode_t test_mode)
{
    /*初始化SDRAM模块*/
    SDRAM_Init();
    
    switch(test_mode)
    {
        case Test_8_16_32Bit:
        {
            SDRAM_32168Write_Read_Test();//8/16/32bit 读写测试
        }
        break;       
        case Test_Memcpy:
        {
            SDRAM_Memcpy_Test();//memcpy方式进行8位/16位/32位的方式写入数据
        }
        break;
        case Test_DMA:
        {
             SDRAM_DMA_Test();//DMA测试
        }
        break;
        default:
        {
            printfS("This test mode is not supported!\r\n"); 
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
    //SDRAM型号为M12L2561616A，注意不同的SDRAM行地址线宽度和列地址线宽度不同
		
    /*
    Test_8_16_32Bit,
    Test_Memcpy,
    Test_DMA,
    */
    SDRAM_Test(Test_8_16_32Bit);
}



