 
#include "sdram.h"  



/*信息输出*/
#define SDRAM_DEBUG_ON         1

#define SDRAM_INFO(fmt,arg...)           printf("<<-SDRAM-INFO->> [%d]"fmt"\n",__LINE__,##arg)
#define SDRAM_ERROR(fmt,arg...)          printf("<<-SDRAM-ERROR->> [%d]"fmt"\n",__LINE__,##arg)
#define SDRAM_DEBUG(fmt,arg...)          do{\
                                            if(SDRAM_DEBUG_ON)\
                                                printf("<<-SDRAM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)



//EXMC SDRAM 数据基地址  
#define SDRAM_BANK_ADDR                 ((uint32_t)0xC0000000)    //((uint32_t)0x70000000)  //
/* Command target memory (SDRAM storage area) */
#define FMC_COMMAND_TARGET_BANK        FMC_SDRAM_CMD_TARGET_BANK1
/* timeout */
#define SDRAM_TIMEOUT                   ((uint32_t)0xFFFF)


 
/*****************************************************************
@功能	初始化控制SDRAM的IO
@参数	无
@返回	无
******************************************************************/  
#if 0
static void SDRAM_GPIO_Config(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* 使能SDRAM相关的GPIO时钟 */
    __HAL_RCC_GPIOL_CLK_ENABLE();//开启GPIOL时钟
    __HAL_RCC_GPIOM_CLK_ENABLE();//开启GPIOM时钟
    __HAL_RCC_GPION_CLK_ENABLE();//开启GPION时钟
    __HAL_RCC_GPIOO_CLK_ENABLE();//开启GPIOO时钟
    __HAL_RCC_GPIOP_CLK_ENABLE();//开启GPIOP时钟
    __HAL_RCC_GPIOQ_CLK_ENABLE();//开启GPIOQ时钟

    /*-- GPIO 配置 -----------------------------------------------------*/

    /* 通用 GPIO 配置 */       
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//配置为复用功能
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_15;//AF15
    GPIO_InitStructure.Drive = GPIO_DRIVE_LEVEL3;
  
  
    /*A[0:12]地址信号线 针对引脚配置*/
    GPIO_InitStructure.Pin = SDRAM_A0_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A1_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A2_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A3_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A4_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A5_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A6_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A7_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A8_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A9_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A10_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A11_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_A12_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_A12_GPIO_PORT, &GPIO_InitStructure);


    /*D[0:31]数据信号线 针对引脚配置*/
    GPIO_InitStructure.Pin = SDRAM_D0_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D1_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D2_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D3_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D4_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D5_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D6_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D7_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D8_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D9_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D10_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D11_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D12_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D13_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D14_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_D15_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D15_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D16_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D16_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D17_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D17_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D18_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D18_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D19_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D19_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D20_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D20_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D21_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D21_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D22_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D22_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D23_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D23_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D24_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D24_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D25_GPIO_PIN;   
    HAL_GPIO_Init(SDRAM_D25_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D26_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D26_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D27_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D27_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D28_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D28_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D29_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D29_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D30_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D30_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = SDRAM_D31_GPIO_PIN; 
    HAL_GPIO_Init(SDRAM_D31_GPIO_PORT, &GPIO_InitStructure);


    /*控制信号线*/
    GPIO_InitStructure.Pin = SDRAM_CLK_GPIO_PIN;           //同步时钟信号 
    HAL_GPIO_Init(SDRAM_CLK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_WE_GPIO_PIN;            //写入使能(SDNWE)
    HAL_GPIO_Init(SDRAM_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_CKE_GPIO_PIN;           //SDRAM存储区域(1/2)时钟使能(SDCKE[0:1]) 
    HAL_GPIO_Init(SDRAM_CKE_GPIO_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.Pin = SDRAM_SDNE_CS_GPIO_PIN;       //SDRAM存储区域(1/2)芯片使能(SDNE[0:1])  
    HAL_GPIO_Init(SDRAM_SDNE_CS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_BA0_GPIO_PIN;           //Bank地址输入，选择要控制的Bank(BA[0:1]) 
    HAL_GPIO_Init(SDRAM_BA0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_BA1_GPIO_PIN;           //Bank地址输入，选择要控制的Bank(BA[0:1])
    HAL_GPIO_Init(SDRAM_BA1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_RAS_GPIO_PIN;           //行地址选通信号
    HAL_GPIO_Init(SDRAM_RAS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_CAS_GPIO_PIN;           //列地址选通信号
    HAL_GPIO_Init(SDRAM_CAS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_UDQM0_GPIO_PIN;         //数据输入输出掩码DQM(NBL[0])
    HAL_GPIO_Init(SDRAM_UDQM0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_LDQM0_GPIO_PIN;         //数据输入输出掩码DQM(NBL[1]) 
    HAL_GPIO_Init(SDRAM_LDQM0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_UDQM1_GPIO_PIN;         //数据输入输出掩码DQM(NBL[2])
    HAL_GPIO_Init(SDRAM_UDQM1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SDRAM_LDQM1_GPIO_PIN;         //数据输入输出掩码DQM(NBL[3]) 
    HAL_GPIO_Init(SDRAM_LDQM1_GPIO_PORT, &GPIO_InitStructure);
    
    SYSCFG->SYSCR |= BIT7;

}
#endif


/*****************************************************************
@功能	对SDRAM芯片进行初始化配置
@参数	无
@返回	无
******************************************************************/    
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
    HAL_Delay(500);
    
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
#ifdef M12L2561616A
    /* (7.8125 us x Freq) - 50 */
    HAL_FMC_SDRAM_ProgramRefreshRate(200);  
#endif
#ifdef M12L64164A
    /* (15.625 us x Freq) - 50 */
    HAL_FMC_SDRAM_ProgramRefreshRate(50); 
#endif
}


void SDRAM_Refresh_Rate(uint16_t rate)
{
    /* 设置刷新时间 */
    /* (7.8125 us x Freq) - 20 */
    HAL_FMC_SDRAM_ProgramRefreshRate(rate); 
}


/*****************************************************************
@功能	初始化配置使用SDRAM的EXMC及GPIO接口
@参数	无
@返回	无
******************************************************************/ 
void SDRAM_Init(void)
{
    FMC_SDRAMInitTypeDef hsdram_Init;
    FMC_SDRAMTimingInitTypeDef hsdram_Timing;
    /* 配置EXMC接口相关的 GPIO*/
//    SDRAM_GPIO_Config();
 
    /* 配置 EXMC 相关参数 ---------------------------------------------------------*/

    /** Perform the SDRAM1 memory initialization sequence*/
    /* 要控制的Bank区域 */
    hsdram_Init.SDBank = FMC_SDRAM_BANK1;
#ifdef M12L2561616A
    /* 行地址线宽度 */
    hsdram_Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
    /* 列地址线宽度 */
    hsdram_Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
#endif
#ifdef M12L64164A
    /* 行地址线宽度 */
    hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
    /* 列地址线宽度 */
    hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
#endif 
    /* 数据线宽度 */
    hsdram_Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
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
    hsdram_Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

    /* SdramTiming */
    /* SDCLK = HCLK/SDClockPeriod = 64/2=32MHZ, 1clock = 1/32=31.25ns */  
    /* TMRD: 加载模式寄存器到激活 (Load Mode Register to Active) min=12ns (1x31.25ns) */
    hsdram_Timing.LoadToActiveDelay = 1;
    /* TXSR: 退出自刷新延迟 (Exit Self-refresh delay) min= ?ns (1x31.25ns) */
    hsdram_Timing.ExitSelfRefreshDelay = 1;

    /* TRAS: 自刷新时间 (Self refresh time) min=42ns (2x31.25ns) */
//    hsdram_Timing.SelfRefreshTime = 2;
	hsdram_Timing.SelfRefreshTime = 1;

    /* TRC: 行循环延迟 (Row cycle delay) min=60 (2x31.25ns) */      
//    hsdram_Timing.RowCycleDelay = 2;
	hsdram_Timing.RowCycleDelay = 1;

    /* TWR: 恢复延迟 (Recovery delay)  min=12ns (1x31.25ns) */
    hsdram_Timing.WriteRecoveryTime = 1;

    /* TRP: 行预充电延迟 (Row precharge delay)  18ns => 1x31.25ns */
//    hsdram_Timing.RPDelay = 2;
	hsdram_Timing.RPDelay = 1;

    /* TRCD: 行到列延迟 (Row to column delay) 18ns => 1x31.25ns */
    hsdram_Timing.RCDDelay = 1;

    HAL_FMC_SDRAM_Init(&hsdram_Init); 
    HAL_FMC_SDRAM_Timing_Init(&hsdram_Timing, hsdram_Init.SDBank);   
    
    
    /* EXMC SDRAM device initialization sequence */
    SDRAM_InitSequence(); 
	
	/* Set burst size */
	HAL_FMC_SDRAM_BurstReadFifoLength(FMC_SDRAM_BURST_READ_FIFO_LEN_7, FMC_SDRAM_BANK1);
	HAL_FMC_SDRAM_BurstReadFifoLength(FMC_SDRAM_BURST_READ_FIFO_LEN_7, FMC_SDRAM_BANK2);

    if(hsdram_Init.SDClockPeriod == FMC_SDRAM_CLOCK_PERIOD_2)
    {
        SDRAM_INFO("SDRAM CLK = HCLK/2 = %d/2 = %d(HZ)\r\n", HAL_RCC_GetHCLKFreq(), HAL_RCC_GetHCLKFreq()/2);
    }
    else if(hsdram_Init.SDClockPeriod ==  FMC_SDRAM_CLOCK_PERIOD_3)
    {
        SDRAM_INFO("SDRAM CLK = HCLK/3 = %d/3 = %d(HZ)\r\n", HAL_RCC_GetHCLKFreq(), HAL_RCC_GetHCLKFreq()/3);
    }
    
    if (hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_8)
    {
        SDRAM_INFO("SDRAM memory bus width is 8 bit\r\n");
    }
    else if (hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_16) 
    {
        SDRAM_INFO("SDRAM memory bus width is 16 bit\r\n");
    }
    else if(hsdram_Init.MemoryDataWidth == FMC_SDRAM_MEM_BUS_WIDTH_32)
    {
        SDRAM_INFO("SDRAM memory bus width is 32 bit\r\n");
    }     
}



void SDRAM_IRQHandler(void)
{
    /* Check SDRAM interrupt Rising edge flag */
    if (SET == HAL_FMC_SDRAM_GetITStatus())
    {
        SDRAM_Refresh_Rate(200);
        /* SDRAM refresh error interrupt callback */
        printfS("SDRAM Refresh Error Irq, SDRSR = 0x%08x\r\n", HAL_FMC_SDRAM_GetStatus());
        /* Clear SDRAM refresh error interrupt pending bit */
        HAL_FMC_SDRAM_ClearITPendingBit();
        printfS("SDRSR = 0x%08x\r\n", HAL_FMC_SDRAM_GetStatus());
    }
}





