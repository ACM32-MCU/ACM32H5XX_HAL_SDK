

#include "sdram.h"  



/* FMC SDRAM Data base address */ 
#define SDRAM_BANK_ADDR                 ((uint32_t)0xC0000000)    //((uint32_t)0x70000000)  //
/* Command target memory (SDRAM storage area) */
#define FMC_COMMAND_TARGET_BANK         FMC_SDRAM_CMD_TARGET_BANK1
/* timeout */
#define SDRAM_TIMEOUT                   ((uint32_t)0xFFFF)

#define SDRAM_SIZE                      (0x2000)




DMA_HandleTypeDef DMA1_Ch0Handle;
DMA_HandleTypeDef DMA2_Ch1Handle;
uint16_t gid_code;  


/******************************************************************************
*@brief : SDRAM initialization process
*@param : none
*@return: none
******************************************************************************/    
static void SDRAM_InitSequence(void)
{
    uint32_t tmpr = 0;
    FMC_SDRAMCommandTypeDef hsdram_Command;
  
    /* 第一步：配置命令：开启提供给SDRAM的时钟 */
    hsdram_Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    hsdram_Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    hsdram_Command.AutoRefreshNumber = 1;
    hsdram_Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&hsdram_Command, SDRAM_TIMEOUT);

    /* 第二步: 插入100us 最小延时 */ 
    HAL_SimpleDelay(50000);
    
    /* 第三步：配置命令：对所有的bank预充电 */ 
    hsdram_Command.CommandMode = FMC_SDRAM_CMD_PALL;
    hsdram_Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    hsdram_Command.AutoRefreshNumber = 1;
    hsdram_Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&hsdram_Command, SDRAM_TIMEOUT);   

    /* 第四步：配置命令：自动刷新 */   
    hsdram_Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    hsdram_Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    hsdram_Command.AutoRefreshNumber = 4;
    hsdram_Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&hsdram_Command, SDRAM_TIMEOUT);
  
    /* 第五步：设置加载模式寄存器配置 */
    tmpr = (uint32_t)FMC_SDRAM_LOAD_MODE_BURST_LENGTH_1          |
                     FMC_SDRAM_LOAD_MODE_BURST_TYPE_SEQUENTIAL   |
                     FMC_SDRAM_LOAD_MODE_CAS_LATENCY_2           |
                     FMC_SDRAM_LOAD_MODE_OPERATING_MODE_STANDARD |
                     FMC_SDRAM_LOAD_MODE_WRITEBURST_MODE_SINGLE;
  
    hsdram_Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    hsdram_Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    hsdram_Command.AutoRefreshNumber = 1;
    hsdram_Command.ModeRegisterDefinition = tmpr;
    HAL_FMC_SDRAM_SendCommand(&hsdram_Command, SDRAM_TIMEOUT);
  
    /* 第六步：设置刷新时间 */
    /* (7.8125 us x Freq) - 50 */
    HAL_FMC_SDRAM_ProgramRefreshRate(800);  


}


/******************************************************************************
*@brief : SDRAM initialization
*@param : none
*@return: none
******************************************************************************/   
void SDRAM_Init(void)
{
    FMC_SDRAMInitTypeDef hsdram_Init;
    FMC_SDRAMTimingInitTypeDef hsdram_Timing;
   
 
    /* 配置 FMC 相关参数 ---------------------------------------------------------*/

    /** Perform the SDRAM1 memory initialization sequence*/
    /* 要控制的Bank区域 */
    hsdram_Init.SDBank = FMC_SDRAM_BANK1;
    /* 列地址线宽度 */
    hsdram_Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
    /* 行地址线宽度 */
    hsdram_Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;

    HAL_EFUSE_GetICCode(&gid_code);  
 
    /* 数据线宽度 */
    if (gid_code == IC_D569XQH6_PE)
    {
        hsdram_Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;   
        
    }
    else
    {
        hsdram_Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;     
    }
    
    /* SDRAM内部bank数量*/
    hsdram_Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    /* CAS潜伏期 */
    hsdram_Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
    /* 禁止写保护*/
    hsdram_Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    /* SDCLK时钟分频因子，SDCLK = HCLK/SDCLOCK_PERIOD*/
    hsdram_Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
    /* 突发读模式设置*/
    hsdram_Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;    
    /* 读延迟配置 */
    hsdram_Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;

 
    /* SdramTiming */
    /* SDCLK = HCLK/SDClockPeriod = 220/2=110MHZ, 1clock = 1/110=9.091ns */ 
    
    /* TMRD: 加载模式寄存器到激活 (Load Mode Register to Active) min = 12ns */
    hsdram_Timing.LoadToActiveDelay = 2;
    
    /* TXSR: 退出自刷新延迟 (Exit Self-refresh delay) min= tRC+1.5 = 61.5ns */
    hsdram_Timing.ExitSelfRefreshDelay = 7;

    /* TRAS: 自刷新时间 (Self refresh time) min = 42ns */
    hsdram_Timing.SelfRefreshTime = 5;

    /* TRC: 行循环延迟 (Row cycle delay) min = 60ns */      
    hsdram_Timing.RowCycleDelay = 7;

    /* TWR: 恢复延迟 (Recovery delay)  min = 12ns */
    hsdram_Timing.WriteRecoveryTime = 2;

    /* TRP: 行预充电延迟 (Row precharge delay)  18ns */
    hsdram_Timing.RPDelay = 3;

    /* TRCD: 行到列延迟 (Row to column delay) 18ns */
    hsdram_Timing.RCDDelay = 3;


    HAL_FMC_SDRAM_Init(&hsdram_Init); 
    HAL_FMC_SDRAM_Timing_Init(&hsdram_Timing, hsdram_Init.SDBank); 

    /* FMC SDRAM device initialization sequence */
    SDRAM_InitSequence(); 

    if(hsdram_Init.SDClockPeriod == FMC_SDRAM_CLOCK_PERIOD_2)
    {
        printfS("SDRAM CLK = HCLK/2 = %d/2 = %d(HZ)\r\n", HAL_RCC_GetHCLKFreq(), HAL_RCC_GetHCLKFreq()/2);
    }
    else if(hsdram_Init.SDClockPeriod ==  FMC_SDRAM_CLOCK_PERIOD_3)
    {
        printfS("SDRAM CLK = HCLK/3 = %d/3 = %d(HZ)\r\n", HAL_RCC_GetHCLKFreq(), HAL_RCC_GetHCLKFreq()/3);
    }
    
    if (hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_8)
    {
        printfS("SDRAM memory bus width is 8 bit\r\n");
    }
    else if (hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_16) 
    {
        printfS("SDRAM memory bus width is 16 bit\r\n");
    }
    else if(hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_32)
    {
        printfS("SDRAM memory bus width is 32 bit\r\n");
    }     
}


/******************************************************************************
*@brief : SDRAM read and write test
*@param : none
*@return: none
******************************************************************************/    
void SDRAM_32168Write_Read_Test(void)
{
    
    uint32_t counter, err_cnt;
    uint8_t u8WriteData = 0x12, u8ReadData = 0;  
    uint16_t u16WriteData = 0x1234, u16ReadData = 0; 
    uint32_t u32WriteData = 0x12345678, u32ReadData = 0; 
     
    //printfS("reading and writing sdram in 8-bit mode...\r\n");
    err_cnt = 0;
    /* 清零 */
    for (counter = 0; counter < SDRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(0x0);
    }
    /* 向整个SDRAM写入数据 8位 */
    for (counter = 0; counter < SDRAM_SIZE; counter++)
    {
        *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(u8WriteData + counter);
    }
    
    /* 读取 SDRAM 数据并检测*/
    for(counter = 0; counter < SDRAM_SIZE; counter++ )
    {
        u8ReadData = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //从该地址读出数据

        if(u8ReadData != (uint8_t)(u8WriteData + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            //printfS("8-bit data read and write errors,u8ReadData = 0x%02x,u8WriteData=0x%02x,counter=%d\r\n",u8ReadData,u8WriteData,counter);
            err_cnt++;
        }
    }

    HAL_DelayMs(100);
    //printfS("reading and writing sdram in 16-bit mode...\r\n");

    /* 清零 */
    for (counter = 0; counter < SDRAM_SIZE; counter += 2)
    {
        *(__IO uint16_t*) (SDRAM_BANK_ADDR + counter) = (uint16_t)(0);
    }    
    /* 向整个SDRAM写入数据 16位 */
    for (counter = 0; counter < SDRAM_SIZE; counter += 2)
    {
        *(__IO uint16_t*) (SDRAM_BANK_ADDR + counter) = (uint16_t)(u16WriteData + counter/2);
    }
    /* 读取 SDRAM 数据并检测*/
    for(counter = 0; counter < SDRAM_SIZE; counter += 2)
    {
        u16ReadData = *(__IO uint16_t*)(SDRAM_BANK_ADDR + counter);  //从该地址读出数据

        if(u16ReadData != (uint16_t)(u16WriteData + counter/2))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            //printfS("16-bit data read and write errors,u16ReadData = 0x%08x,u16WriteData=0x%08x,counter=%d\r\n",u16ReadData,u16WriteData,counter);
            err_cnt++;
        }
    }

    HAL_DelayMs(100);
    //printfS("reading and writing sdram in 32-bit mode...\r\n");
    /* 清零 */
    for (counter = 0; counter < SDRAM_SIZE; counter += 4)
    {
        *(__IO uint32_t*) (SDRAM_BANK_ADDR + counter) = (uint32_t)(0);
    }
    /* 向整个SDRAM写入数据 32位 */
    for (counter = 0; counter < SDRAM_SIZE; counter += 4)
    {
        *(__IO uint32_t*) (SDRAM_BANK_ADDR + counter) = (uint32_t)(u32WriteData + counter/4);
    }
    /* 读取 SDRAM 数据并检测*/
    for(counter = 0; counter < SDRAM_SIZE; counter += 4)
    {
        u32ReadData = *(__IO uint32_t*)(SDRAM_BANK_ADDR + counter);  //从该地址读出数据

        if(u32ReadData != (uint32_t)(u32WriteData + counter/4))      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            //printfS("32-bit data read and write errors,u32ReadData=0x%08x,u32WriteData=0x%08x,counter=%d\r\n",u32ReadData,u32WriteData,counter);
            err_cnt++;
        }
    }
    if(err_cnt)
    {
        printfS("SDRAM_32168Write_Read_Test fail\r\n");            
    }
    else
    {
        printfS("SDRAM_32168Write_Read_Test success\r\n");          
    }
}


/******************************************************************************
*@brief : SDRAM memcpy mode read and write test
*@param : none
*@return: none
******************************************************************************/
void SDRAM_Memcpy_Test(void)
{
    uint8_t src[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88}; 
    uint32_t temp, i, err_cnt;
    //printfS("Start SDRAM_Memcpy_Test test!\r\n");
    err_cnt = 0;
    for(i = 0; i < 8; i++)
    {
        *( uint8_t*) (SDRAM_BANK_ADDR+i) = (uint8_t)0;
    }

    memcpy((uint32_t *)SDRAM_BANK_ADDR, src, 1);
    temp = *( uint32_t*) (SDRAM_BANK_ADDR); 
    if(temp != 0x11)  
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp);
    }
     

    memcpy((uint32_t *)SDRAM_BANK_ADDR, src, 2);
    temp =  *( uint32_t*) (SDRAM_BANK_ADDR); 
    if(temp != 0x2211)
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp); 
    }
    

    memcpy((uint32_t *)SDRAM_BANK_ADDR, src, 3);
    temp = *( uint32_t*) (SDRAM_BANK_ADDR); 
    if(temp != 0x332211)
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp); 
    }
    

    memcpy((uint32_t *)SDRAM_BANK_ADDR, src, 4);
    temp = *( uint32_t*) (SDRAM_BANK_ADDR); 
    if(temp != 0x44332211)
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp);
    }
     

    memcpy((uint32_t *)SDRAM_BANK_ADDR, src, 8);
    temp = *( uint32_t*) (SDRAM_BANK_ADDR); 
    if(temp != 0x44332211)
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp); 
    }
    

    temp =  *( uint32_t*) (SDRAM_BANK_ADDR+4); 
    if(temp != 0x88776655)
    {
        err_cnt++;
        printfS("data=0x%X \r\n", temp);
    }
     
    if(err_cnt)
    {
        printfS("SDRAM_Memcpy_Test fail\r\n");            
    }
    else
    {
        printfS("SDRAM_Memcpy_Test success\r\n");          
    }
}

/******************************************************************************
*@brief : SDRAM DMA mode read and write test
*@param : none
*@return: none
******************************************************************************/
void SDRAM_DMA_Test(void)
{  
    //printfS("Start SDRAM_DMA_Test test!\r\n"); 
    
    uint32_t writeBuf[24]={0xaabbaabb,0x11221122,0x55665566,0x33333333,0x33331111,0x66665555,0x77778888,0xffffffff,
                            0xaabbaabb,0x11221122,0x55665566,0xffffffff,0x33331111,0x66665555,0x77778888,0x00000001,
                            0xaabbaabb,0x11221122,0x55665566,0xffffffff,0x33331111,0x66665555,0x77778888,0x22222222}; 
    uint32_t i, readdata = 0, err_cnt; 
    uint32_t readBuf[24]= {0};  
    
	__HAL_RCC_DMA1_CLK_ENABLE();
    
	
	DMA1_Ch0Handle.Instance          = DMA1_Channel0;
	DMA1_Ch0Handle.Init.Mode         = DMA_MODE_NORMAL;
	DMA1_Ch0Handle.Init.DataFlow     = DMA_DATAFLOW_M2M;
	DMA1_Ch0Handle.Init.ReqID        = DMA1_REQ_M2M;
	DMA1_Ch0Handle.Init.SrcIncDec    = DMA_SRCINCDEC_INC;
	DMA1_Ch0Handle.Init.DestIncDec   = DMA_DESTINCDEC_INC;
	DMA1_Ch0Handle.Init.SrcWidth     = DMA_SRCWIDTH_WORD;
	DMA1_Ch0Handle.Init.DestWidth    = DMA_DESTWIDTH_WORD;
	DMA1_Ch0Handle.Init.SrcBurst     = DMA_SRCBURST_32;
	DMA1_Ch0Handle.Init.DestBurst    = DMA_DESTBURST_32;
	DMA1_Ch0Handle.Init.SrcMaster    = DMA_SRCMASTER_1;
	DMA1_Ch0Handle.Init.DestMaster   = DMA_DESTMASTER_1;
	HAL_DMA_Init(&DMA1_Ch0Handle); 
    
    __HAL_RCC_DMA2_CLK_ENABLE();
    DMA2_Ch1Handle.Instance          = DMA2_Channel1;
	DMA2_Ch1Handle.Init.Mode         = DMA_MODE_NORMAL;
	DMA2_Ch1Handle.Init.DataFlow     = DMA_DATAFLOW_M2M;
	DMA2_Ch1Handle.Init.ReqID        = DMA2_REQ_M2M;
	DMA2_Ch1Handle.Init.SrcIncDec    = DMA_SRCINCDEC_INC;
	DMA2_Ch1Handle.Init.DestIncDec   = DMA_DESTINCDEC_INC;
	DMA2_Ch1Handle.Init.SrcWidth     = DMA_SRCWIDTH_WORD;
	DMA2_Ch1Handle.Init.DestWidth    = DMA_DESTWIDTH_WORD;
	DMA2_Ch1Handle.Init.SrcBurst     = DMA_SRCBURST_32;
	DMA2_Ch1Handle.Init.DestBurst    = DMA_DESTBURST_32;
	DMA2_Ch1Handle.Init.SrcMaster    = DMA_SRCMASTER_1;
	DMA2_Ch1Handle.Init.DestMaster   = DMA_DESTMASTER_1;
	HAL_DMA_Init(&DMA2_Ch1Handle); 
    
    HAL_SimpleDelay(5000);
    
    err_cnt = 0;
    
    for (i = 0; i < SDRAM_SIZE; i+=4)
    {
        *(__IO uint32_t*) (SDRAM_BANK_ADDR + i) = (uint32_t)(0x00000000);
    }
    HAL_SimpleDelay(5000);
    
    HAL_DMA_Start(&DMA1_Ch0Handle, (uint32_t)writeBuf, SDRAM_BANK_ADDR, 24);
    /* Wait Transfer Finish */
    while (HAL_DMA_GetFlag(&DMA1_Ch0Handle, DMA_FLAG_RTC) == RESET);
    
    
    for(i = 0; i< 24*4; i+=4 )
    {
        readdata = *(__IO uint32_t*)(SDRAM_BANK_ADDR + i);  //从该地址读出数据

        if(readdata != writeBuf[i/4])      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("SDRAM DMA Write test Error\r\n");
            err_cnt++;
        }
    }
    
    
    HAL_SimpleDelay(5000);
    HAL_DMA_Start(&DMA2_Ch1Handle, SDRAM_BANK_ADDR, (uint32_t)readBuf, 24);
    /* Wait Transfer Finish */
    while (HAL_DMA_GetFlag(&DMA2_Ch1Handle, DMA_FLAG_RTC) == RESET);
    for(i = 0; i< 24; i++)
    {
        if(readBuf[i] != writeBuf[i])      //检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            printfS("SDRAM DMA Read test Error,[%d]readBuf[0x%08x] != writeBuf[0x%08x]\r\n",i,readBuf[i],writeBuf[i]);
            err_cnt++;
        }
    }
    
    if(err_cnt)
    {
        printfS("SDRAM_DMA_Test fail\r\n");            
    }
    else
    {
        printfS("SDRAM_DMA_Test success\r\n");          
    }   

    
}






