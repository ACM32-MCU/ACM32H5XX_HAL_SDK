
/******************************************************************************
*@file  : app.c
*@brief : application
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h"   
#include "reg_scan.h"  

uint8_t COMP_RegDefaultScanTest( void )
{
    uint32_t i;
    uint8_t status = 0, status_tmp = 0;
    uint32_t BaseAdr = COMP1_BASE_ADDR;
    uint32_t RegMap[][2] =
	{
        { 0x00, 0x00 }, //地址偏移，默认值
        { 0x04, 0x00 },        
        { 0xffffffff, 0 }, //结束标志
    };
    __HAL_RCC_CMP1_RESET();
    __HAL_RCC_CMP1_CLK_ENABLE();
    for(i = 0; RegMap[i][0] != 0xffffffff; i++)
    {
        status_tmp = reg_default_compare(BaseAdr + RegMap[i][0], RegMap[i][1]);
        if(status_tmp)
        {
            status = 1;
            //break;
        }
    }
    
    __HAL_RCC_CMP1_RESET();
    if(status)
    {
        printfS("CASE: '%s' Failed! status=%x\n", __FUNCTION__, status);
        return 1;
    }
    else
    {
        printf("CASE: '%s' Pass! \n", __FUNCTION__);
        return 0;
    }
}

uint8_t COMP_RegWriteScanTest( void ) //部分位在应用中测试
{
    uint32_t i, j;
    uint8_t status = 0, status_tmp = 0;
    uint32_t BaseAdr = COMP1_BASE_ADDR;
    uint32_t RegMap[][10] =
	{
        { 0x00, 0xE0008000, 0xffffffff, 0xffffffff, 0xaaaaaaaa, 0xaaaaaaaa, 0x55555555, 0x55555555, 0x00000000, 0x00000000 }, 
        { 0x04, 0x00000000, 0xffffffff, 0x00000000, 0xaaaaaaaa, 0x00000000, 0x55555555, 0x00000000, 0x00000000, 0x00000000 },
        { 0xffffffff }, //结束标志
    };
    
    //Test the LOCK
     uint32_t RegMap1[][10] =
	{
        { 0x00, 0x00000000, 0xffffffff, 0x80000000, 0xaaaaaaaa, 0x80000000, 0x55555555, 0x80000000, 0x00000000, 0x80000000 }, 
        { 0x04, 0x00000000, 0xffffffff, 0x00000000, 0xaaaaaaaa, 0x00000000, 0xd5555555, 0x00000000, 0x80000000, 0x00000000 },
        { 0xffffffff }, //结束标志
    };

    __HAL_RCC_CMP1_RESET();
    __HAL_RCC_CMP1_CLK_ENABLE();
    for(i = 0; RegMap[i][0] != 0xffffffff; i++)
    {
        for(j = 0; j < 4; j++)
        {
            status_tmp = reg_write_compare_mask(BaseAdr + RegMap[i][0], RegMap[i][1], RegMap[i][2 + 2 * j], RegMap[i][3 + 2 * j]);
            if(status_tmp)
            {
                status = 1;
                //break;
            }
        }
    }
    __HAL_RCC_CMP1_RESET();
    __HAL_RCC_CMP1_CLK_ENABLE();
    
    //Test the LOCK
    SET_BIT(COMP1->CR,COMP_CR_LOCK);

    
    for(i = 0; RegMap1[i][0] != 0xffffffff; i++)
    {
        for(j = 0; j < 4; j++)
        {
            status_tmp = reg_write_compare_mask(BaseAdr + RegMap1[i][0], RegMap1[i][1], RegMap1[i][2 + 2 * j], RegMap1[i][3 + 2 * j]);
            if(status_tmp)
            {
                status = 1;
                printf("CASE: '%s' LOCK test Failed!\r\n", __FUNCTION__);
                //break;
            }
        }
    }
    __HAL_RCC_CMP1_RESET();
    if(status)
    {
        printf("CASE: '%s' Failed! status=%x\r\n", __FUNCTION__, status);
        return 1;
    }
    else
    {
        printf("CASE: '%s' LOCK test Pass!\r\n", __FUNCTION__);
        printf("CASE: '%s' Pass! \r\n", __FUNCTION__);
        return 0;
    }
}



COMP_HandleTypeDef COMP1_handle;

void COMP1_Init(void)
{
    HAL_RCC_FLTClockSourceConfig(RCC_FLT_CLK_SOURCE_PCLK1_DIV32);
    
    COMP1_handle.Instance = COMP1;//赋值COMP基地址
    COMP1_handle.Init.InPSel = COMP1_INPSEL_PB0;//指定正端输入
    COMP1_handle.Init.InMSel = COMP1_INMSEL_PC4;//指定负端输入
    COMP1_handle.Init.Polarity = COMP_POLARITY_NOINVERT;//输出极性为同相
    COMP1_handle.Init.HYS = COMP_HYS_4;//迟滞窗口选择40mv
    COMP1_handle.Init.BlankSel = COMP_BLANKSEL_NONE;//不选择切断源
    COMP1_handle.Init.FltEn = COMP_FLT_ENABLE;// COMP_FLT_DISABLE;//滤波使能
    COMP1_handle.Init.FltTime = COMP_FLTTIME_1CLK;//滤波周期为1个周期
    
    HAL_COMP_Init(&COMP1_handle);//执行初始化
}


void APP_Test()
{
    int count=0;
//    COMP_RegDefaultScanTest();
//    COMP_RegWriteScanTest();
    
    COMP1_Init();
    HAL_COMP_Enable(&COMP1_handle);
    printfS("COMP1_CR: %08x\r\n",COMP1_handle.Instance->CR);
    while(1)
    {
        HAL_DWT_DelayMs(50);
        HAL_COMP_GetOutputLevel(&COMP1_handle);
        printfS("%d",COMP1_handle.OutputLevel);
        count++;
        if((count%100)==0)
        {
            printfS("\r\n");
        }
    }
    
}



