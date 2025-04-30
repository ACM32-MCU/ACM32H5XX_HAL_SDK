

#include "sdram.h"  



/* FMC SDRAM Data base address */ 
#define SDRAM_BANK_ADDR                 ((uint32_t)0x70000000)    
/* Command target memory (SDRAM storage area) */
#define FMC_COMMAND_TARGET_BANK         FMC_SDRAM_CMD_TARGET_BANK1
/* timeout */
#define SDRAM_TIMEOUT                   ((uint32_t)0xFFFF)

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
    HAL_DWT_DelayUs(120);  
    
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
    
    //Read_Values_From_Registers();
 
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

    //Read_Values_From_Registers();
    /* FMC SDRAM device initialization sequence */
    SDRAM_InitSequence(); 
   
}







