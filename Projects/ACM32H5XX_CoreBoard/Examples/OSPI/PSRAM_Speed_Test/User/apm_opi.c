/******************************************
*   Double-Data-Rate OPI PSRAM
*   APS6408L-OTX-BJ 
******************************************/
#include "apm_opi.h"  
#include "drv_ospi.h"



OSPI_HandleTypeDef APMOPI_Handle;
OSPI_MemoryInitTypeDef APMOPI_Memeory_Handle;
OSPI_OctalInitTypeDef APMOPI_OSPI_Octal = {0};


/******************************************************************************
* @brief : APS6408L-OTX-BJ OSPI initialization.
* @param : None
* @return: None
******************************************************************************/
void APMOPI_OSPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Handle = {0};
    
    //注意：OSPI2_DQS硬件为E27-JP17-23，2023-08-21 V7.8 之前版本的BIT文件为E25-JP17-21，OSPI2_DM硬件为A29-JP17-34，BIT文件为E21-JP17-40,需要跳线，不然读写失败 
    
    __HAL_RCC_OSPI1_RESET();
    __HAL_RCC_OSPI2_RESET();
    
    APMOPI_Handle.Instance                = OSPI2;
    APMOPI_Handle.Init.WorkMode           = OSPI_WORK_MODE_0;
    APMOPI_Handle.Init.XMode              = OSPI_8X_MODE;
    APMOPI_Handle.Init.FirstBit           = OSPI_FIRSTBIT_MSB;
    APMOPI_Handle.Init.BaudRatePrescaler  = OSPI_BAUDRATE_PRESCALER_8;  
    APMOPI_Handle.Init.SampleShifting     = OSPI_SAMPLE_SHIFT_2HCLK;
    APMOPI_Handle.Init.FWMode             = OSPI_FIFO_HALFWORD;
    APMOPI_Handle.Init.FRMode             = OSPI_FIFO_HALFWORD;
    APMOPI_Handle.CSx                     = OSPI_CS_0;

    HAL_OSPI_Init(&APMOPI_Handle); 

    //OSPI2_DM    :   PF6--GPIO 
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_PULLUP;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    GPIO_Handle.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);    
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ OSPI octal communication initialization.
* @param : None
* @return: None
******************************************************************************/
void  APMOPI_OSPI_Octal_Init(void)
{
    APMOPI_OSPI_Octal.DTRMode = OSPI_DTRM_DTR;         //双倍传输速率模式DTR
    APMOPI_OSPI_Octal.DQSMode = OSPI_DQSOE_ENABLE;     //数据选通使能
    APMOPI_OSPI_Octal.MemoryType = OSPI_MEM_APM_OPI;   //存储器类型
    APMOPI_OSPI_Octal.OutDelay = OSPI_TX_OUT_DELAY_HALF_HCLK;  //DTR通信输出延迟
    APMOPI_OSPI_Octal.DQSSample = OSPI_DQS_SAMPLE_DISABLE;       //DQS采样使能
    
    HAL_OSPI_OctalInit(&APMOPI_Handle, &APMOPI_OSPI_Octal);
    
    HAL_OSPI_APMDummyClock(&APMOPI_Handle, OSPI_APM_DUMMY_3CLOCK);//APM OPI 协议传输结束后dummy clock数
    
    //HAL_OSPI_DQSOutputEnableDisable(&APMOPI_Handle, OSPI_DQSOE_ENABLE);//APM OPI存储器数据选通位
}


/******************************************************************************
* @brief : APS6408L-OTX-BJ reset.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OTX_GlobalReset(void)
{
    uint16_t write_buff[5];
    //global reset 4clocks
    write_buff[0] = APMOPI_GLOBAL_RESET;
    write_buff[0] += 0x00 << 8;//xx
    write_buff[1] = 0x0000;//dummy cycle 1
    write_buff[2] = 0x0000;//dummy cycle 2
    write_buff[3] = 0x0000;//dummy cycle 3
    write_buff[4] = 0x0000;//dummy cycle 4
    HAL_OSPI_TransmitByHalfWord(&APMOPI_Handle, write_buff, 5, 0); 
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ power-up initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OTX_PowerUp_Init(void)
{
    APMOPI_OSPI_Init();
    
    APMOPI_OSPI_Octal_Init(); 
    
    HAL_DelayUs(200); 

    APS6408L_OTX_GlobalReset();
    
    HAL_DelayMs(2);    
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ read control registers.
* @param : addr: control registers address
* @data  : pData: Pointer to read data buffer
* @return: None
******************************************************************************/
void APS6408L_OTX_ReadRegister(uint8_t addr, uint16_t *pData)
{
    uint16_t write_buff[7];    
    //mode register read 
    write_buff[0] = APMOPI_MODE_REGISTER_READ;
    write_buff[0] += 0x00 << 8;//xx
    write_buff[1] = 0x00;//xx
    write_buff[1] += addr << 8;//MA
    write_buff[2] = 0x0000;//Latency cycle 1
    write_buff[3] = 0x0000;//Latency cycle 2
    write_buff[4] = 0x0000;//Latency cycle 3
    write_buff[5] = 0x0000;//Latency cycle 4
    write_buff[6] = 0x0000;//Latency cycle 5
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&APMOPI_Handle, write_buff, 7, pData, 1, 0);   
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ synchronous write .
* @param : addr: control registers address
* @data  : pData: Pointer to read data buffer
* @data  : len: Amount of data to be read
* @return: None
******************************************************************************/
void APS6408L_OTX_Synchronous_Write(uint32_t addr, uint16_t *pData, uint32_t len)
{
    uint16_t write_buff[256+2]; 
	uint32_t i;
    // Synchronous Write followed by No Operation (default WL0)
    memset(write_buff, 0, 256+2);
    
    write_buff[0] = APMOPI_SYNC_WRITE;
    write_buff[0] += ((addr >> 16) & 0xff) << 8;//A2
    write_buff[1] = ((addr >> 8) & 0xff);//A1
    write_buff[1] += (addr & 0xff) << 8;//A0 
    
    for(i = 0; i < len; i++)
    {
        write_buff[2 + i] = pData[i];
    }

//    write_buff[2+len] = 0x0000;//dummy cycle 1
//    write_buff[2+len+1] = 0x0000;//dummy cycle 2
//    write_buff[2+len+2] = 0x0000;//dummy cycle 3
//    write_buff[2+len+3] = 0x0000;//dummy cycle 4 
    
    HAL_OSPI_TransmitByHalfWord(&APMOPI_Handle, write_buff, (len + 2), 0);
    
    
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ synchronous read .
* @param : addr: Control registers address
* @data  : pData: Pointer to read data buffer
* @data  : len: Amount of data to be read
* @return: None
******************************************************************************/
void APS6408L_OTX_Synchronous_Read(uint32_t addr, uint16_t *pData, uint32_t len)
{
    uint16_t write_buff[6];
       
  	//Synchronous Read,Read Latency Code:MR0[4:2]
    memset(write_buff, 0, 6);
    write_buff[0] = APMOPI_SYNC_READ;
    write_buff[0] += ((addr >> 16) & 0xff) << 8;//A2
    write_buff[1] = ((addr >> 8) & 0xff);//A1
    write_buff[1] += (addr & 0xff) << 8;//A0
    write_buff[2] = 0x0000;//Read Latency cycle 1
    write_buff[3] = 0x0000;//Read Latency cycle 2
    write_buff[4] = 0x0000;//Read Latency cycle 3
    write_buff[5] = 0x0000;//Read Latency cycle 4
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&APMOPI_Handle, write_buff, 6, pData, len, 0);
     
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ read Vendor ID and Device ID.
* @param : None
* @return: Vendor ID(High 8 bit) and Device ID(Low 8 bit)
******************************************************************************/
uint8_t APS6408L_OTX_ReadID(void)
{
    uint16_t id = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_1, &id);
    printfS("Vendor ID & Device ID : 0x%02x  \n", (id & 0x1F));
    return (id & 0x1F);  
}


/******************************************************************************
* @brief : APS6408L-OTX-BJ read control registers test.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OTX_ReadRegister_Test(void)
{
    uint16_t read_data;   
    //mode register read 
    printfS("Read Mode Register MA'h00: 0xAD13\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_0, &read_data);
    printfS("MR0/MR1 = 0x%04x  \n",read_data);
  	
    printfS("Read Mode Register MA'h01: 0xD1AD\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_1, &read_data);
    printfS("MR1/MR2 = 0x%04x  \n",read_data);
    
    printfS("Read Mode Register MA'h02: 0x20D1\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_2, &read_data);
    printfS("MR2/MR3 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h03: 0x0020\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_3, &read_data);
    printfS("MR3/MR4 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h04: 0x0300\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_4, &read_data);
    printfS("MR4/MR8 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h08: 0x1303\r\n");
    read_data = 0;
    APS6408L_OTX_ReadRegister(APMOPI_MA_ADDR_8, &read_data);
    printfS("MR8/MR1 = 0x%04x  \n",read_data); 
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ read and write test.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OTX_Fifo_Read_Write(void)
{
    uint16_t i, err_count = 0;
    uint16_t temp_buff[12];
    uint16_t writeBuf[256]; 
    uint16_t readBuf[16] = { 0 };                            
    uint32_t test_addr = 0x000000;
    uint16_t read_data = 0;
    uint16_t wirte_data = 0x1234;
    
    for(i = 0; i < 256; i++)
    {
        writeBuf[i] = wirte_data + i;
    }
	
    // Synchronous Write followed by No Operation (default WL0)

    APS6408L_OTX_Synchronous_Write(test_addr, writeBuf, 256); 
    
    APS6408L_OTX_Synchronous_Read(test_addr, readBuf, 8);

    
    for(i = 0; i < 8; i++)
    {
        printfS("Write = 0x%04x, Read = 0x%04x\r\n", writeBuf[i], readBuf[i]);
        if(readBuf[i] != writeBuf[i])
        {
            //printfS("Write = 0x%04x, Read = 0x%04x\r\n", writeBuf[i], readBuf[i]);
            err_count++;
        }
    }
    if(err_count)
    {
        printfS("APS6408L_OTX Read Write Fail!!!\r\n");
    }
    else
    {
        printfS("APS6408L_OTX Read Write Success!!!\r\n");
    }
}

/******************************************************************************
* @brief : APS6408L-OTX-BJ test.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OTX_BJ_Fifo_Test(void)
{
    APS6408L_OTX_PowerUp_Init();
    
    APS6408L_OTX_ReadRegister_Test();
    
    printfS("APM Dummy Done status = %ld\r\n", (APMOPI_Handle.Instance->STATUS & OSPI_STATUS_APM_DUMY_DONE_Msk) >> OSPI_STATUS_APM_DUMY_DONE_Pos);
    
    //注意：OSPI2_DQS硬件为E27-JP17-23，2023-08-21 V7.8 之前版本的BIT文件为E25-JP17-21，OSPI2_DM硬件为A29-JP17-34，BIT文件为E21-JP17-40,需要跳线，不然读写失败 
    APS6408L_OTX_Fifo_Read_Write();
}



//Memory内存模式访问初始化
void APS6408L_OTX_OSPI_DTR_Memory_Init(void)
{
	APMOPI_Memeory_Handle.WriteCmd = APMOPI_SYNC_WRITE;
	APMOPI_Memeory_Handle.ReadCmd = APMOPI_SYNC_READ;      
	APMOPI_Memeory_Handle.AlterByte = 0x00;  
    
    APMOPI_Memeory_Handle.WrapSize = 0;
    APMOPI_Memeory_Handle.BurstLen = MEMOACC2_BURST_LEN_1024;                     //突发长度（Burst Length）32Byte
    APMOPI_Memeory_Handle.HyperXspiLC1 = 0;                    //Hyperbus或xSPI模式下，RWDS为一时的LC周期数 1个CLK
    APMOPI_Memeory_Handle.HyperXspiLc0 = 0;                     //Hyperbus或xSPI模式下，RWDS为零时的LC周期数 16个CLK   
    
	APMOPI_Memeory_Handle.HyperBurstType = 0;               //[27]突发是Linear的还是Wrapped的
	APMOPI_Memeory_Handle.DataMode = 0;                      //[26:25]数据模式，八线模式时，此位无效
	APMOPI_Memeory_Handle.AlterByteMode = 0;                //[24:23]交替字节模式，八线模式时，此位无效
	APMOPI_Memeory_Handle.AddrMode = 0;                      //[22:21]地址模式，八线模式时，此位无效
	APMOPI_Memeory_Handle.InstrMode = 0;                     //[20:19]指令模式，八线模式时，此位无效
	APMOPI_Memeory_Handle.AddrWidth = MEMOACC1_ADDR_WIDTH_24;                     //[18:17]地址长度（Hyper必须为32 bit）
	APMOPI_Memeory_Handle.DummyCycleSize = MEMOACC1_DUMMY_CYCLE_4;               //[16:12]dummy周期长度（Hyper/xSPI无效）
	APMOPI_Memeory_Handle.ReadDummyByteEnable = MEMOACC1_READ_DUMMY_ENABLE;                       //[11]读操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	APMOPI_Memeory_Handle.WriteummyByteEnable = 0;                       //[10]写操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	APMOPI_Memeory_Handle.AlterByteSize = 0;                //[8:7]]交替字节长度 8 16 24 32，八线模式时，此位无效			
	APMOPI_Memeory_Handle.ReadAlterByteEnable = 0;                       //[6]读操作交替字节使能位，八线模式时，此位无效
	APMOPI_Memeory_Handle.WriteAlterByteEnable = 0;                       //[5]写操作交替字节使能位，八线模式时，此位无效
	APMOPI_Memeory_Handle.SendInstrOnce = 0;                    //[4]仅发送指令一次
    APMOPI_Memeory_Handle.ContinuousModeEnable = MEMOACC1_CON_ENABLE;                    //[3]连续读使能
	APMOPI_Memeory_Handle.CsTimeoutEnable = 0;                     //[2]连续模式下CS拉高等待功能不使能  
    
    HAL_OSPI_MemoryInit(&APMOPI_Handle, &APMOPI_Memeory_Handle);
    
    HAL_OSPI_MemoryEnableDisable(&APMOPI_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
}


/****************** DTR Memory Test Start ******************/
//DTR Memory模式8bit读写
void APS6408L_OTX_OSPI_DTR_Memory_8bit_Write_Read(void)
{
    volatile uint8_t *pMem8;
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint8_t writeBuf[OTX_BUFF_LEN];
    uint8_t readBuf[OTX_BUFF_LEN]; 
    uint8_t temp;
    
    
    HAL_OSPI_MemoryEnableDisable(&APMOPI_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    
    pMem8 = (volatile uint8_t *) (OSPI2_MEM_BASE_ADDR + 0x1000);
    pMem16= (volatile uint16_t *) (OSPI2_MEM_BASE_ADDR + 0x1000);
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
        *(pMem16 + i) = (uint16_t )0x0000;
    }

    //写
    printfS("Memory write in 8-bit mode...\r\n"); 
    
    APMOPI_Handle.Instance->CTL &= ~OSPI_CTL_CS_TIME_Msk;
    APMOPI_Handle.Instance->CTL |= (0x5 << OSPI_CTL_CS_TIME_Pos);
    
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        *(pMem8 + i) = writeBuf[i];
    }    

    HAL_Delay(500);

    //读
    pMem8 = (volatile uint8_t *) (OSPI2_MEM_BASE_ADDR + 0x1000);
    printfS("Memory read in 8-bit mode...\r\n"); 
    memset(readBuf, 0 , OTX_BUFF_LEN);
    for( i = 0; i < OTX_BUFF_LEN; i++)
    {
        readBuf[i] = *(pMem8 + i);
    }
	//比对
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        if(i % 2 == 0)
        {
            temp = writeBuf[i+1];
        }
        else
        {
            temp = writeBuf[i];
        }
        if(readBuf[i] != temp)
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, temp, readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("APS6408L_OTX Memory 8-bit read fail!!!\r\n");
    }
    else
    {
        printfS("APS6408L_OTX Memory 8-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n"); 
    
    
}


//DTR Memory模式16bit读写
void APS6408L_OTX_OSPI_DTR_Memory_16bit_Write_Read(void)
{    
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint16_t writeBuf[OTX_BUFF_LEN];
    uint16_t readBuf[OTX_BUFF_LEN];
 

    HAL_OSPI_MemoryEnableDisable(&APMOPI_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    
    pMem16 = (volatile uint16_t *) (OSPI2_MEM_BASE_ADDR + 0x2000);
    
    APMOPI_Handle.Instance->CTL &= ~OSPI_CTL_CS_TIME_Msk;
    APMOPI_Handle.Instance->CTL |= (0x32 << OSPI_CTL_CS_TIME_Pos);
    
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        writeBuf[i] = (OTX_DATA_16BIT + i);
    }  
    //写
    printfS("Memory write in 16-bit mode...\r\n"); 
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        *(pMem16 + i) = writeBuf[i];
    }    

    HAL_Delay(500);

    //读
    pMem16 = (volatile uint16_t *) (OSPI2_MEM_BASE_ADDR + 0x2000);
    printfS("Memory read in 16-bit mode...\r\n"); 
    memset(readBuf, 0 , OTX_BUFF_LEN);
    for( i = 0; i < OTX_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem16 + i);
    }
	//比对
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%04x, Read = 0x%04x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("APS6408L_OTX Memory 16-bit read fail!!!\r\n");
    }
    else
    {
        printfS("APS6408L_OTX Memory 16-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");   
    
}
//DTR Memory模式32bit读写
void APS6408L_OTX_OSPI_DTR_Memory_32bit_Write_Read(void)
{
    volatile uint32_t *pMem32;
    uint16_t i, err_count = 0;
    uint32_t writeBuf[OTX_BUFF_LEN];
    uint32_t readBuf[OTX_BUFF_LEN]; 
    
    HAL_OSPI_MemoryEnableDisable(&APMOPI_Handle, MEMOACC1_MEMORY_ENABLE);//[1]存储器访问使能
    
    pMem32 = (volatile uint32_t *) (OSPI2_MEM_BASE_ADDR + 0x3000);
    
    APMOPI_Handle.Instance->CTL &= ~OSPI_CTL_CS_TIME_Msk;
    APMOPI_Handle.Instance->CTL |= (0xC8 << OSPI_CTL_CS_TIME_Pos);
    
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        writeBuf[i] = (OTX_DATA_32BIT + i);
    }

    //写
    printfS("Memory write in 32-bit mode...\r\n"); 
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        *(pMem32 + i) = writeBuf[i];
    }    

    HAL_Delay(500);

    //读
    pMem32 = (volatile uint32_t *) (OSPI2_MEM_BASE_ADDR + 0x3000);
    printfS("Memory read in 32-bit mode...\r\n"); 
    memset(readBuf, 0 , OTX_BUFF_LEN);
    for( i = 0; i < OTX_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem32 + i);
    }
	//比对
    for(i = 0; i < OTX_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%08x, Read = 0x%08x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("APS6408L_OTX Memory 32-bit read fail!!!\r\n");
    }
    else
    {
        printfS("APS6408L_OTX Memory 32-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");  
    
    
}
//DTR Memory模式读写测试
void APS6408L_OTX_OSPI_DTR_Memory_Test(void)
{
    APS6408L_OTX_PowerUp_Init();
    APS6408L_OTX_ReadID();
    APS6408L_OTX_OSPI_DTR_Memory_Init();

    APS6408L_OTX_OSPI_DTR_Memory_8bit_Write_Read();
    APS6408L_OTX_OSPI_DTR_Memory_16bit_Write_Read();
    APS6408L_OTX_OSPI_DTR_Memory_32bit_Write_Read();

}






