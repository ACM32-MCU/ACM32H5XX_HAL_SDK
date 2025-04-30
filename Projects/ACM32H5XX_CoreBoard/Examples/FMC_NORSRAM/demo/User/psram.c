
#include "psram.h"




#define PSRAM_SIZE              0x1000//0x80000  //PSRAM: 512 Byte



/*****************************************************************
@功能	初始化控制PSRAM的IO
@参数	无
@返回	无
******************************************************************/  
static void PSRAM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable GPIOs clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();


    /*-- GPIO 配置 -----------------------------------------------------*/

    /* 通用 GPIO 配置 */
    GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;       //配置为复用功能
    GPIO_InitStructure.Pull  = GPIO_PULLUP; 
    GPIO_InitStructure.Drive = GPIO_DRIVE_LEVEL3;     

    /*A地址信号线 针对引脚配置*/
    
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;//AF11
    GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A13_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A14_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A15_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A15_GPIO_PORT, &GPIO_InitStructure);	
    
    
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;//AF10 
    GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A16_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A16_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A17_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A17_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A18_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A18_GPIO_PORT, &GPIO_InitStructure);
    
 
    /*DQ数据信号线 针对引脚配置*/
    GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

    /*控制信号线*/
    GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_OE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_OE_GPIO_PORT, &GPIO_InitStructure);    

    GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);  

    GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.Pin = FMC_NWATI_GPIO_PIN; 
    HAL_GPIO_Init(FMC_NWATI_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN; 
    HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);     
    
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;       //AF11
    GPIO_InitStructure.Pin = FMC_PSRAM_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_PSRAM_CS_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_NADV_GPIO_PIN; 
    HAL_GPIO_Init(FMC_NADV_GPIO_PORT, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.Pin = PSRAM_CRE_GPIO_PIN; 
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Drive = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(PSRAM_CRE_GPIO_PORT, &GPIO_InitStructure);  

}



void FMC_PSRAM_Init_ASY(void)//异步
{
    FMC_NORSRAMInitTypeDef hpsram_Init;
    FMC_NORSRAMTimingInitTypeDef hpsram_Timing, hpsram_WTiming;
    /* 配置FMC接口相关的 GPIO*/
    PSRAM_GPIO_Config();

    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    /* 设置要控制的Bank区域 */
    hpsram_Init.NSBank                 = FMC_NORSRAM_BANK3;//use CS3 control，Addr=0x68000000
    hpsram_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
    hpsram_Init.MemoryType             = FMC_MEMORY_TYPE_PSRAM;
    hpsram_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hpsram_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
    hpsram_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_HIGH;
    hpsram_Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
    hpsram_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_BEFOREWAITSTATE;
    hpsram_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;    
    hpsram_Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
    hpsram_Init.ExtendedMode           = FMC_EXTENDED_MODE_ENABLE;
    hpsram_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hpsram_Init.WriteMode              = FMC_WRITE_ASY_MODE;
    hpsram_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;
    /* PSRAM device configuration */  
    hpsram_Timing.AddressSetupTime       = 1;
    hpsram_Timing.DataSetupTime          = 3;
    hpsram_Timing.AddressHoldTime        = 1;   

    hpsram_Timing.BusTurnAroundDuration  = 0x06;

    hpsram_Timing.CLKDivision            = 0;
    hpsram_Timing.DataLatency            = 0;
    hpsram_Timing.AccessMode             = FMC_ACCESS_MODE_D;

    hpsram_WTiming.AddressSetupTime       = 1;
    hpsram_WTiming.DataSetupTime          = 3;
    hpsram_WTiming.AddressHoldTime        = 1;   

    hpsram_WTiming.BusTurnAroundDuration  = 0x06;

    hpsram_WTiming.CLKDivision            = 0;
    hpsram_WTiming.DataLatency            = 0;
    hpsram_WTiming.AccessMode             = FMC_ACCESS_MODE_D;

    HAL_FMC_NORSRAM_Init(&hpsram_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hpsram_Timing, hpsram_Init.NSBank);
    HAL_FMC_NORSRAM_Extended_Timing_Init(&hpsram_WTiming, hpsram_Init.NSBank, hpsram_Init.ExtendedMode);
}

void FMC_PSRAM_Init_SY(void)//同步
{
    FMC_NORSRAMInitTypeDef hpsram_Init;
    FMC_NORSRAMTimingInitTypeDef  hpsram_Timing;
    /* 配置FMC接口相关的 GPIO*/
    PSRAM_GPIO_Config();

    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    /* 设置要控制的Bank区域 */
    hpsram_Init.NSBank                 = FMC_NORSRAM_BANK3;//use CS3 control，Addr=0x68000000
    hpsram_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
    hpsram_Init.MemoryType             = FMC_MEMORY_TYPE_PSRAM;
    hpsram_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hpsram_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_ENABLE;
    hpsram_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_HIGH;
    hpsram_Init.WrapMode               = FMC_WRAP_MODE_ENABLE;
    hpsram_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_DURINGWAITSTATE;
    hpsram_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;   
    hpsram_Init.WaitSignal             = FMC_WAIT_SIGNAL_ENABLE;
    hpsram_Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
    hpsram_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_ENABLE;
    hpsram_Init.WriteMode              = FMC_WRITE_SY_MODE;
    hpsram_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;

    /* PSRAM device configuration */  
    hpsram_Timing.AddressSetupTime       = 1;
    hpsram_Timing.DataSetupTime          = 3;
    hpsram_Timing.AddressHoldTime        = 1;   

    hpsram_Timing.BusTurnAroundDuration  = 0x06;

    hpsram_Timing.CLKDivision            = 1;
    hpsram_Timing.DataLatency            = 0;
    hpsram_Timing.AccessMode             = FMC_ACCESS_MODE_A;//PSRAM

    HAL_FMC_NORSRAM_Init(&hpsram_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hpsram_Timing, hpsram_Init.NSBank);
}


static void PSRAM_Read_Write_Test(void)
{
    /*写入数据计数器*/
    uint32_t counter=0;

    /* 8位的数据 */
    uint8_t u8WriteData = 0x12, u8ReadData = 0;  

    /* 16位的数据 */
    uint16_t u16WriteData = 0x1234, u16ReadData = 0; 

    /* 32位的数据 */
    uint32_t u32WriteData = 0x12345678, u32ReadData = 0; 
    
   
    printfS("Detecting PSRAM, reading and writing sdram in 8-bit mode...\r\n");
    for (counter = 0; counter < PSRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (FMC_BANK_PSRAM + counter) = (uint8_t)(u8WriteData + counter);
    }
    
    for(counter = 0; counter<PSRAM_SIZE;counter++ )
    {
        u8ReadData = *(__IO uint8_t*)(FMC_BANK_PSRAM + counter);  //从该地址读出数据

        if(u8ReadData != (uint8_t)(u8WriteData + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("8-bit data read and write errors,u8ReadData = 0x%02x,u8WriteData=0x%02x,counter=%d\r\n",u8ReadData,u8WriteData,counter);
            return;
        }
    }
  
    printfS("Detecting PSRAM, reading and writing sdram in 16-bit mode...\r\n");    
    for (counter = 0; counter < PSRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_PSRAM + counter) = (uint16_t)(u16WriteData + counter/2);
    }
    for(counter = 0; counter<PSRAM_SIZE;counter+=2)
    {
        u16ReadData = *(__IO uint16_t*)(FMC_BANK_PSRAM + counter);  //从该地址读出数据

        if(u16ReadData != (uint16_t)(u16WriteData + counter/2))       //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("16-bit data read and write errors,u16ReadData = 0x%04x,u16WriteData=0x%04x,counter=%d\r\n",u16ReadData,u16WriteData,counter);
            return;
        }
    }
       
    printfS("Detecting PSRAM, reading and writing sdram in 32-bit mode...\r\n");
    for (counter = 0; counter < PSRAM_SIZE;counter+=4)
    {
        *(__IO uint32_t*) (FMC_BANK_PSRAM + counter) = (uint32_t)(u32WriteData + counter/4);
    }
    for(counter = 0; counter<PSRAM_SIZE/4;counter+=4)
    {
        u32ReadData = *(__IO uint32_t*)(FMC_BANK_PSRAM + counter);  //从该地址读出数据

        if(u32ReadData != (uint32_t)(u32WriteData + counter/4))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("32-bit data read and write errors,u32ReadData=0x%08x,u32WriteData=0x%08x,counter=%d\r\n",u32ReadData,u32WriteData,counter);
            return;
        }
    }

    printfS("PSRAM read and write test successful!\r\n"); 
}


void FMC_PSRAM_Test(void)
{  
    uint32_t temp = 0;
    uint32_t counter = 0;
    
    FMC_PSRAM_Init_ASY();
    
    for (counter = 0; counter < PSRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_PSRAM + counter) = 0x0000;
    }
    
    *( uint16_t*) (FMC_BANK_PSRAM+0) =   (uint16_t)0x0000; 

    *( uint16_t*) (FMC_BANK_PSRAM+2) =   (uint16_t)0xFFFF; 
    
    *( uint16_t*) (FMC_BANK_PSRAM+4) =   (uint16_t)0x00FF; 

    *( uint16_t*) (FMC_BANK_PSRAM+6) =   (uint16_t)0xFF00; 

    *( uint16_t*) (FMC_BANK_PSRAM+8) =   (uint16_t)0xAA55; 

    *( uint16_t*) (FMC_BANK_PSRAM+10) =   (uint16_t)0x55AA; 

    *( uint16_t*) (FMC_BANK_PSRAM+12) =   (uint16_t)0x5555; 
    
    *( uint16_t*) (FMC_BANK_PSRAM+14) =   (uint16_t)0xAAAA; 
    

    temp =  *( uint16_t*) (FMC_BANK_PSRAM+0); 
    printfS("temp[0]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+2);   
    printfS("temp[2]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*)(FMC_BANK_PSRAM+4);   
    printfS("temp[4]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+6);
    printfS("temp[6]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+8);   
    printfS("temp[8]=0x%0.4X \r\n",temp); 
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+10);   
    printfS("temp[10]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+12);   
    printfS("temp[12]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+14);   
    printfS("temp[14]=0x%0.4X \r\n",temp);


    PSRAM_Read_Write_Test();//8/16/32bit 读写测试

    //同步模式
    *( uint16_t*) (FMC_BANK_PSRAM+0x10383E) =   (uint16_t)0xCCCC; 
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+0x10383E);   
    printfS("temp[10383E]=0x%0.4X \r\n",temp);
    HAL_GPIO_WritePin(PSRAM_CRE_GPIO_PORT, PSRAM_CRE_GPIO_PIN, GPIO_PIN_SET);
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+0x10383E); 
    printfS("BCR=0x%X \r\n",temp);

    *( uint16_t*) (FMC_BANK_PSRAM+0x10383E) = 1 ; //选择同步模式
    temp =  *( uint16_t*) (FMC_BANK_PSRAM+0x10383E); 

    //*( uint16_t*) (FMC_BANK_PSRAM+0x81C1F) = 1 ; //选择同步模式
    //temp =  *( uint16_t*) (FMC_BANK_PSRAM+0x81C1F); 

    printfS("BCR=0x%X \r\n",temp);
    HAL_GPIO_WritePin(PSRAM_CRE_GPIO_PORT, PSRAM_CRE_GPIO_PIN, GPIO_PIN_RESET);  

    FMC_PSRAM_Init_SY();//同步模式

    PSRAM_Read_Write_Test();

}



