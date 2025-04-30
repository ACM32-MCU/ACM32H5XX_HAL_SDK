
#include "sram.h"  



 
#define SRAM_SIZE              0x100//0x80000  //SRAM: 512 Byte


/******************************************************************************
* @brief : SRAM IO Configuration
* @param : None
* @return: none
******************************************************************************/
static void SRAM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable GPIOs clock */
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

    GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A13_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A14_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A15_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A15_GPIO_PORT, &GPIO_InitStructure);	


    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;//AF10  
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
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;       //AF11
    GPIO_InitStructure.Pin = FMC_SRAM_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_SRAM_CS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;       //AF10
    GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_OE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_OE_GPIO_PORT, &GPIO_InitStructure);    

    GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);  

    GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);  

}

/******************************************************************************
* @brief : FMC SRAM initialization function.
* @param : none
* @return: none
******************************************************************************/
void FMC_SRAM_Init(void)
{
    FMC_NORSRAMInitTypeDef hsram_Init;
    FMC_NORSRAMTimingInitTypeDef hsram_Timing, hsram_WTiming;
    /* 配置FMC接口相关的 GPIO*/
    SRAM_GPIO_Config();
 
    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    /* 设置要控制的Bank区域 */
    hsram_Init.NSBank                 = FMC_NORSRAM_BANK2;//use CS2 control，Addr=0x64000000
    /* 设置地址总线与数据总线是否复用 */
    hsram_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
    /* 设置要控制的存储器类型 */
    hsram_Init.MemoryType             = FMC_MEMORY_TYPE_SRAM;
    /* 设置要控制的存储器的数据宽度 */
    hsram_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    /* 设置是否使用突发访问模式 */
    hsram_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
    /* 设置等待信号的有效极性 */
    hsram_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
    /* 设置是否支持对齐的突发模式 */
    hsram_Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
    
    hsram_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_BEFOREWAITSTATE;
    /* 设置是否写使能 */
    hsram_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;    
    /* 设置当存储器处于突发传输模式时，是否允许通过NWAIT信号插入等待状态 */
    hsram_Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
    /* 设置是否使用扩展模式 */
    hsram_Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
    /* 设置是否使能在同步传输时使用的等待信号 */
    hsram_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    /* 设置是否使能写突发操作 */
    hsram_Init.WriteMode              = FMC_WRITE_ASY_MODE;

    hsram_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;

    /* NOR device configuration */  
    hsram_Timing.AddressSetupTime       = 6;
    hsram_Timing.DataSetupTime          = 6;
    hsram_Timing.AddressHoldTime        = 6;   

    hsram_Timing.BusTurnAroundDuration  = 0x06;

    hsram_Timing.CLKDivision            = 0;
    hsram_Timing.DataLatency            = 0;
    hsram_Timing.AccessMode             = FMC_ACCESS_MODE_A;//nor flash

    HAL_FMC_NORSRAM_Init(&hsram_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hsram_Timing, hsram_Init.NSBank);

}



/******************************************************************************
* @brief : SRAM test.
* @param : None
* @return: none
******************************************************************************/
void FMC_SRAM_Test(void)
{
    uint16_t temp = 0;
    /*写入数据计数器*/
    uint32_t counter=0;

    /* 8位的数据 */
    uint8_t u8WriteData = 0x12, u8ReadData = 0;  

    /* 16位的数据 */
    uint16_t u16WriteData = 0x1234, u16ReadData = 0; 

    /* 32位的数据 */
    uint32_t u32WriteData = 0x12345678, u32ReadData = 0; 
    
    FMC_SRAM_Init();
    
    for (counter = 0; counter < SRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_SRAM + counter) = 0x0000;
    }
       
    *( uint16_t*) (FMC_BANK_SRAM+0) =   (uint16_t)0x0000; 

    *( uint16_t*) (FMC_BANK_SRAM+2) =   (uint16_t)0xFFFF; 
    
    *( uint16_t*) (FMC_BANK_SRAM+4) =   (uint16_t)0x00FF; 

    *( uint16_t*) (FMC_BANK_SRAM+6) =   (uint16_t)0xFF00; 

    *( uint16_t*) (FMC_BANK_SRAM+8) =   (uint16_t)0xAA55; 

    *( uint16_t*) (FMC_BANK_SRAM+10) =   (uint16_t)0x55AA; 

    *( uint16_t*) (FMC_BANK_SRAM+12) =   (uint16_t)0x5555; 
    
    *( uint16_t*) (FMC_BANK_SRAM+14) =   (uint16_t)0xAAAA; 

    temp =  *( uint16_t*) (FMC_BANK_SRAM+0); 
    printfS("temp[0]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM+2);   
    printfS("temp[2]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*)(FMC_BANK_SRAM+4);   
    printfS("temp[4]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM+6);
    printfS("temp[6]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM+8);   
    printfS("temp[8]=0x%0.4X \r\n",temp); 
    temp =  *( uint16_t*) (FMC_BANK_SRAM+10);   
    printfS("temp[10]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_SRAM+12);   
    printfS("temp[12]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_SRAM+14);   
    printfS("temp[14]=0x%0.4X \r\n",temp);
   
    printfS("Detecting SRAM, reading and writing sram in 8-bit mode... \r\n");
    for (counter = 0; counter < SRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (FMC_BANK_SRAM + counter) = (uint8_t)(u8WriteData + counter);
    }
    
    for(counter = 0; counter<SRAM_SIZE;counter++ )
    {
        u8ReadData = *(__IO uint8_t*)(FMC_BANK_SRAM + counter);  //从该地址读出数据

        if(u8ReadData != (uint8_t)(u8WriteData + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("8-bit data read and write errors,u8ReadData = 0x%02x,u8WriteData=0x%02x,counter=%d\r\n",u8ReadData,u8WriteData,counter);
            return;
        }
    }
  
    printfS("Detecting SRAM, reading and writing sram in 16-bit mode...\r\n");    
    for (counter = 0; counter < SRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_SRAM + counter) = (uint16_t)(u16WriteData + counter/2);
    }
    for(counter = 0; counter<SRAM_SIZE;counter+=2)
    {
        u16ReadData = *(__IO uint16_t*)(FMC_BANK_SRAM + counter);  //从该地址读出数据

        if(u16ReadData != (uint16_t)(u16WriteData + counter/2))       //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("16-bit data read and write errors,u16ReadData = 0x%04x,u16WriteData=0x%04x,counter=%d\r\n",u16ReadData,u16WriteData,counter);
            return;
        }
    }
       
    printfS("Detecting SRAM, reading and writing sdram in 32-bit mode...\r\n");
    for (counter = 0; counter < SRAM_SIZE;counter+=4)
    {
        *(__IO uint32_t*) (FMC_BANK_SRAM + counter) = (uint32_t)(u32WriteData + counter/4);
    }
    for(counter = 0; counter<SRAM_SIZE/4;counter+=4)
    {
        u32ReadData = *(__IO uint32_t*)(FMC_BANK_SRAM + counter);  //从该地址读出数据

        if(u32ReadData != (uint32_t)(u32WriteData + counter/4))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("32-bit data read and write errors,u32ReadData=0x%08x,u32WriteData=0x%08x,counter=%d\r\n",u32ReadData,u32WriteData,counter);
            return;
        }
    }

    printfS("SRAM read and write test successful!\r\n"); 
}





/*****************************************************************
@功能	初始化控制SRAM复用的IO
@参数	无
@返回	无
******************************************************************/  
static void SRAM_MUX_GPIO_Config(void)
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
    
//    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;//AF11
//    GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A13_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A13_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A14_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A14_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A15_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A15_GPIO_PORT, &GPIO_InitStructure);	
    
    
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;//AF10 
//    GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

//    GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
//    HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);

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
   
    
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;       //AF11
    GPIO_InitStructure.Pin = FMC_SRAM_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_SRAM_CS_GPIO_PORT, &GPIO_InitStructure);  
    
    GPIO_InitStructure.Pin = FMC_NADV_GPIO_PIN; 
    HAL_GPIO_Init(FMC_NADV_GPIO_PORT, &GPIO_InitStructure);
      
}

static void FMC_SRAM_MUX_Init(void)
{
    FMC_NORSRAMInitTypeDef hsram_Init;
    FMC_NORSRAMTimingInitTypeDef hsram_Timing, hsram_WTiming;
    /* 配置FMC接口相关的 GPIO*/
    SRAM_MUX_GPIO_Config();
 
    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    /* 设置要控制的Bank区域 */
    hsram_Init.NSBank                 = FMC_NORSRAM_BANK3;//use CS3 control，Addr=0x68000000
    /* 设置地址总线与数据总线是否复用 */
    hsram_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_ENABLE;
    /* 设置要控制的存储器类型 */
    hsram_Init.MemoryType             = FMC_MEMORY_TYPE_SRAM;//FMC_MEMORY_TYPE_NOR;
    /* 设置要控制的存储器的数据宽度 */
    hsram_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    /* 设置是否使用突发访问模式 */
    hsram_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
    /* 设置等待信号的有效极性 */
    hsram_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
    /* 设置是否支持对齐的突发模式 */
    hsram_Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
    
    hsram_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_BEFOREWAITSTATE;
    /* 设置是否写使能 */
    hsram_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;    
    /* 设置当存储器处于突发传输模式时，是否允许通过NWAIT信号插入等待状态 */
    hsram_Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
    /* 设置是否使用扩展模式 */
    hsram_Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
    /* 设置是否使能在同步传输时使用的等待信号 */
    hsram_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    /* 设置是否使能写突发操作 */
    hsram_Init.WriteMode              = FMC_WRITE_ASY_MODE;

    hsram_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;

    /* NOR device configuration */  
    hsram_Timing.AddressSetupTime       = 6;
    hsram_Timing.DataSetupTime          = 6;
    hsram_Timing.AddressHoldTime        = 6;   

    hsram_Timing.BusTurnAroundDuration  = 0x06;

    hsram_Timing.CLKDivision            = 0;
    hsram_Timing.DataLatency            = 0;
    hsram_Timing.AccessMode             = FMC_ACCESS_MODE_D;

    HAL_FMC_NORSRAM_Init(&hsram_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hsram_Timing, hsram_Init.NSBank);

}


static void SRAM_MUX_Read_Write_Test(void)
{
    #define DELAY_TIME  50
    uint32_t err_cnt = 0;
    /*写入数据计数器*/
    uint32_t counter=0;

    /* 8位的数据 */
    uint8_t u8WriteData = 0x12, u8ReadData = 0;  

    /* 16位的数据 */
    uint16_t u16WriteData = 0x1234, u16ReadData = 0; 

    /* 32位的数据 */
    uint32_t u32WriteData = 0x12345678, u32ReadData = 0; 
      
    //printfS("Detecting SRAM MUX, reading and writing sdram in 8-bit mode...");
    for (counter = 0; counter < SRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (FMC_BANK_SRAM_MUX + counter) = (uint8_t)(0x00);//HAL_SimpleDelay(DELAY_TIME);
    }
    
    for (counter = 0; counter < SRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (FMC_BANK_SRAM_MUX + counter) = (uint8_t)(u8WriteData + counter);//HAL_SimpleDelay(DELAY_TIME);
    }
    
    for(counter = 0; counter<SRAM_SIZE;counter++ )
    {
        u8ReadData = *(__IO uint8_t*)(FMC_BANK_SRAM_MUX + counter);  //从该地址读出数据
        HAL_SimpleDelay(DELAY_TIME);
        if(u8ReadData != (uint8_t)(u8WriteData + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("8-bit data read and write errors,u8ReadData = 0x%02x,u8WriteData=0x%02x,counter=%d\r\n",u8ReadData,u8WriteData,counter);
            //return;
            err_cnt++;
        }
    }
    
    if(err_cnt)
    {
        printfS("SRAM MUX 8-bit read and write test fail!\r\n"); 
    }
    else
    {
        printfS("SRAM MUX 8-bit read and write test successful!\r\n"); 
    }
  
    //printfS("Detecting SRAM MUX, reading and writing sdram in 16-bit mode...");
    err_cnt = 0;    
    for (counter = 0; counter < SRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_SRAM_MUX + counter) = (uint16_t)(0x0000);//HAL_SimpleDelay(DELAY_TIME);
    }    
    for (counter = 0; counter < SRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_SRAM_MUX + counter) = (uint16_t)(u16WriteData + counter/2);//HAL_SimpleDelay(DELAY_TIME);
    }
    for(counter = 0; counter<SRAM_SIZE;counter+=2)
    {
        u16ReadData = *(__IO uint16_t*)(FMC_BANK_SRAM_MUX + counter);  //从该地址读出数据
        HAL_SimpleDelay(DELAY_TIME);
        if(u16ReadData != (uint16_t)(u16WriteData + counter/2))       //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("16-bit data read and write errors,u16ReadData = 0x%04x,u16WriteData=0x%04x,counter=%d\r\n",u16ReadData,u16WriteData,counter);
            //return;
            err_cnt++;
        }
    }
    if(err_cnt)
    {
        printfS("SRAM MUX 16-bit read and write test fail!\r\n"); 
    }
    else
    {
        printfS("SRAM MUX 16-bit read and write test successful!\r\n"); 
    }
    
    //printfS("Detecting SRAM MUX, reading and writing sdram in 32-bit mode...");  
    err_cnt = 0;        
    for (counter = 0; counter < SRAM_SIZE;counter+=4)
    {
        *(__IO uint32_t*) (FMC_BANK_SRAM_MUX + counter) = (uint32_t)(0x00000000);//HAL_SimpleDelay(DELAY_TIME);
    }
    for (counter = 0; counter < SRAM_SIZE;counter+=4)
    {
        *(__IO uint32_t*) (FMC_BANK_SRAM_MUX + counter) = (uint32_t)(u32WriteData + counter/4);//HAL_SimpleDelay(DELAY_TIME);
    }
    for(counter = 0; counter<SRAM_SIZE/4;counter+=4)
    {
        u32ReadData = *(__IO uint32_t*)(FMC_BANK_SRAM_MUX + counter);  //从该地址读出数据
        HAL_SimpleDelay(DELAY_TIME);
        if(u32ReadData != (uint32_t)(u32WriteData + counter/4))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("32-bit data read and write errors,u32ReadData=0x%08x,u32WriteData=0x%08x,counter=%d\r\n",u32ReadData,u32WriteData,counter);
            //return;
            err_cnt++;
        }
    }
    
    if(err_cnt)
    {
        printfS("SRAM MUX 32 bit read and write test fail!\r\n"); 
    }
    else
    {
        printfS("SRAM MUX 32 bit read and write test successful!\r\n"); 
    }
}


void FMC_SRAM_MUX_Test(void)
{
    uint16_t temp=0;
    uint32_t counter=0;
    
    FMC_SRAM_MUX_Init();
    
    for (counter = 0; counter < SRAM_SIZE;counter+=2)
    {
        *(__IO uint16_t*) (FMC_BANK_SRAM_MUX + counter) = 0x0000;
    }
       
    *( uint16_t*) (FMC_BANK_SRAM_MUX+0) =   (uint16_t)0x0000; 

    *( uint16_t*) (FMC_BANK_SRAM_MUX+2) =   (uint16_t)0xFFFF; 
    
    *( uint16_t*) (FMC_BANK_SRAM_MUX+4) =   (uint16_t)0x00FF; 

    *( uint16_t*) (FMC_BANK_SRAM_MUX+6) =   (uint16_t)0xFF00; 

    *( uint16_t*) (FMC_BANK_SRAM_MUX+8) =   (uint16_t)0xAA55; 

    *( uint16_t*) (FMC_BANK_SRAM_MUX+10) =   (uint16_t)0x55AA; 

    *( uint16_t*) (FMC_BANK_SRAM_MUX+12) =   (uint16_t)0x5555; 
    
    *( uint16_t*) (FMC_BANK_SRAM_MUX+14) =   (uint16_t)0xAAAA; 

    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+0); 
    printfS("temp[0]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+2);   
    printfS("temp[2]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*)(FMC_BANK_SRAM_MUX+4);   
    printfS("temp[4]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+6);
    printfS("temp[6]=0x%0.4X \r\n",temp);   
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+8);   
    printfS("temp[8]=0x%0.4X \r\n",temp); 
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+10);   
    printfS("temp[10]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+12);   
    printfS("temp[12]=0x%0.4X \r\n",temp);
    temp =  *( uint16_t*) (FMC_BANK_SRAM_MUX+14);   
    printfS("temp[14]=0x%0.4X \r\n",temp);
    
    SRAM_MUX_Read_Write_Test();
}




