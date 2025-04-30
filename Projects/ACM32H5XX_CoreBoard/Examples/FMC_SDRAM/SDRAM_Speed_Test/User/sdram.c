

#include "sdram.h"  
#include <stdlib.h> // 包含标准库


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
    FMC_SDRAMCommandTypeDef Command;
  
    /* 第一步：配置命令：开启提供给SDRAM的时钟 */
    Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&Command, SDRAM_TIMEOUT);

    /* 第二步: 插入100us 最小延时 */ 
    HAL_SimpleDelay(50000);
    
    /* 第三步：配置命令：对所有的bank预充电 */ 
    Command.CommandMode = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&Command, SDRAM_TIMEOUT);   

    /* 第四步：配置命令：自动刷新 */   
    Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Command.AutoRefreshNumber = 4;
    Command.ModeRegisterDefinition = 0;
    HAL_FMC_SDRAM_SendCommand(&Command, SDRAM_TIMEOUT);
  
    /* 第五步：设置加载模式寄存器配置 */
    tmpr = (uint32_t)FMC_SDRAM_LOAD_MODE_BURST_LENGTH_1          |
                     FMC_SDRAM_LOAD_MODE_BURST_TYPE_SEQUENTIAL   |
                     FMC_SDRAM_LOAD_MODE_CAS_LATENCY_2           |
                     FMC_SDRAM_LOAD_MODE_OPERATING_MODE_STANDARD |
                     FMC_SDRAM_LOAD_MODE_WRITEBURST_MODE_SINGLE;
  
    Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = tmpr;
    HAL_FMC_SDRAM_SendCommand(&Command, SDRAM_TIMEOUT);
  
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
    /* 行地址线宽度 */
    hsdram_Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
    /* 列地址线宽度 */
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

#define OBM_BUFF_LEN            (1024)
#define STEP_SIZE   64
void ram_8bit_write_read_test(volatile uint8_t *buff,int stepFlag)
{ 
    int i,errCount=0;
    uint8_t writeBuf[OBM_BUFF_LEN];
    uint8_t readBuf[OBM_BUFF_LEN];
    uint32_t startTick,endTick;
    int count=OBM_BUFF_LEN;

    srand(100);    
    for(i = 0; i < count; i++)
    {
        writeBuf[i] = (uint8_t)rand();
    }
    
    
    //write   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            buff[i*STEP_SIZE] = writeBuf[i];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            buff[i] = writeBuf[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("write time: %d clks/byte\r\n",(endTick-startTick)/count);

    //read   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i*STEP_SIZE];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("read time: %d clks/byte\r\n",(endTick-startTick)/count);
    

    for(i = 0; i < count; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, writeBuf[i], readBuf[i]);
            errCount++;
        }
    }
    if(errCount)
    {
        printfS("ram 8bit test fail\r\n******************************************\r\n");
    }
    else
    {
        printfS("ram 8bit test pass\r\n******************************************\r\n");
    } 
}

void ram_16bit_write_read_test(volatile uint16_t *buff,int stepFlag)
{ 
    int i,errCount=0;
    uint16_t writeBuf[OBM_BUFF_LEN];
    uint16_t readBuf[OBM_BUFF_LEN];
    uint32_t startTick,endTick;
    int count=OBM_BUFF_LEN;

    srand(100);    
    for(i = 0; i < count; i++)
    {
        writeBuf[i] = (uint16_t)rand();
    }
    
    
    //write   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            buff[i*STEP_SIZE] = writeBuf[i];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            buff[i] = writeBuf[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("write time: %d clks/halfword\r\n",(endTick-startTick)/count);

    //read   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i*STEP_SIZE];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("read time: %d clks/halfword\r\n",(endTick-startTick)/count);
    

    for(i = 0; i < count; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, writeBuf[i], readBuf[i]);
            errCount++;
        }
    }
    if(errCount)
    {
        printfS("ram 16bit test fail\r\n******************************************\r\n");
    }
    else
    {
        printfS("ram 16bit test pass\r\n******************************************\r\n");
    } 
}

void ram_32bit_write_read_test(volatile uint32_t *buff,int stepFlag)
{ 
    int i,errCount=0;
    uint32_t writeBuf[OBM_BUFF_LEN];
    uint32_t readBuf[OBM_BUFF_LEN];
    uint32_t startTick,endTick;
    int count=OBM_BUFF_LEN;

    srand(100);    
    for(i = 0; i < count; i++)
    {
        writeBuf[i] = (uint32_t)rand();
    }
    
    
    //write   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            buff[i*STEP_SIZE] = writeBuf[i];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            buff[i] = writeBuf[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("write time: %d clks/word\r\n",(endTick-startTick)/count);

    //read   
    startTick = _HAL_DWT_GET_CLK_TICK();
    if(stepFlag)
    {        
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i*STEP_SIZE];
        }
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            readBuf[i] = buff[i];
        }
    }
    endTick =  _HAL_DWT_GET_CLK_TICK();      
    
    printfS("read time: %d clks/word\r\n",(endTick-startTick)/count);
    

    for(i = 0; i < count; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, writeBuf[i], readBuf[i]);
            errCount++;
        }
    }
    if(errCount)
    {
        printfS("ram 32bit test fail\r\n******************************************\r\n");
    }
    else
    {
        printfS("ram 32bit test pass\r\n******************************************\r\n");
    } 
}

/******************************************************************************
*@brief : SDRAM read and write test
*@param : none
*@return: none
******************************************************************************/    
void SDRAM_32168Write_Read_Test(void)
{
    volatile uint8_t *pMem8;
    volatile uint16_t *pMem16;    
    volatile uint32_t *pMem32;    
    
    pMem8 = ( uint8_t *) (SDRAM_BANK_ADDR + 0x1000);
    pMem16 = ( uint16_t *) (SDRAM_BANK_ADDR + 0x1000);    
    pMem32 = ( uint32_t *) (SDRAM_BANK_ADDR + 0x1000);   
    
    printfS("*************************************************\r\n");    
    printfS("sdram 8bit continue test\r\n");
    ram_8bit_write_read_test(pMem8,0);
    printfS("sdram 8bit jump test\r\n");    
    ram_8bit_write_read_test(pMem8,1); 
    
    printfS("*************************************************\r\n");
    
    printfS("sdram 16bit continue test\r\n");
    ram_16bit_write_read_test(pMem16,0);
    printfS("sdram 16bit jump test\r\n");    
    ram_16bit_write_read_test(pMem16,1); 
    
    printfS("*************************************************\r\n");

    printfS("sdram 32bit continue test\r\n");
    ram_32bit_write_read_test(pMem32,0);
    printfS("sdram 32bit jump test\r\n");    
    ram_32bit_write_read_test(pMem32,1); 

    

}

