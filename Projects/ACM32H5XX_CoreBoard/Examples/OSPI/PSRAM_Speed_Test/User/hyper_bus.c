/******************************************
*   W958D8NBYA 256Mb HyperRAM
******************************************/

#include "hyper_bus.h"
#include "drv_ospi.h"


#ifdef RUN_CODE_NOP
#define HYPER_DEBUG_ON         0
#else
#define HYPER_DEBUG_ON         1
#endif

#define HYPER_DEBUG(fmt,arg...)          do{\
                                            if(HYPER_DEBUG_ON)\
                                                printfS("<<-hyper_bus->>[%d]:"fmt"\n",__LINE__, ##arg);\
                                          }while(0)







OSPI_HandleTypeDef HYPER_Handle;
OSPI_MemoryInitTypeDef Hyper_Memeory_Handle;
OSPI_OctalInitTypeDef HYPER_OSPI_Octal = {0};
DMA_HandleTypeDef HYPER_DMA1_Ch0Handle;
DMA_HandleTypeDef HYPER_DMA2_Ch1Handle;

#define HYPER_BUFF_LEN            (256)
#define HYPER_HALF_BUFF_LEN       (HYPER_BUFF_LEN/2)
#define HYPER_QUARTER_UFF_LEN     (HYPER_BUFF_LEN/4)
#define HYPER_DATA_16BIT          (0x33CC)
#define HYPER_DATA_32BIT          (0x5555AAAA)


#define HYPER_HALF_PAGE_SIZE        (16)


#define DEFAULT_CLK_LATENCY         (7)//Fixed 2 times Initial Latency(default)


uint8_t IR0_READ_CA[CA_LEN] = {HYPER_ID_REGISTER0_CA_47_40, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t IR1_READ_CA[CA_LEN] = {HYPER_ID_REGISTER1_CA_47_40, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t CR0_READ_CA[CA_LEN] = {HYPER_CONFIG_REGISTER0_READ_CA_47_40, 0x00, 0x01, 0x00, 0x00, 0x00};
uint8_t CR0_WRITE_CA[CA_LEN] = {HYPER_CONFIG_REGISTER0_WRITE_CA_47_40, 0x00, 0x01, 0x00, 0x00, 0x00};
uint8_t CR1_READ_CA[CA_LEN] = {HYPER_CONFIG_REGISTER1_READ_CA_47_40, 0x00, 0x01, 0x00, 0x00, 0x01};
uint8_t CR1_WRITE_CA[CA_LEN] = {HYPER_CONFIG_REGISTER1_WRITE_CA_47_40, 0x00, 0x01, 0x00, 0x00, 0x01};


/******************************************************************************
* @brief : W958D8NBYA OSPI initialization.
* @param : None
* @return: None
******************************************************************************/
void Hyper_OSPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Handle = {0};
    
    HYPER_DEBUG("To use DRAGON_XIP.bit, please use SLOT3_SUBA, switching IO mapping\r\n");
    
    __HAL_RCC_OSPI1_RESET();
    
    HYPER_Handle.Instance                = OSPI1;
    //OSPI工作模式
    HYPER_Handle.Init.WorkMode           = OSPI_WORK_MODE_0;
    //OSPI多线模式    
    HYPER_Handle.Init.XMode              = OSPI_8X_MODE;
    //MSB/LSB在前选择        
    HYPER_Handle.Init.FirstBit           = OSPI_FIRSTBIT_MSB;
    //分频系数   
    HYPER_Handle.Init.BaudRatePrescaler  = OSPI_BAUDRATE_PRESCALER_4;
    //采样延时     
    HYPER_Handle.Init.SampleShifting     = OSPI_SAMPLE_SHIFT_2HCLK;
    //FIFO写模式     
    HYPER_Handle.Init.FWMode             = OSPI_FIFO_HALFWORD;
    //FIFO读模式      
    HYPER_Handle.Init.FRMode             = OSPI_FIFO_HALFWORD;
    //CS控制信号      
    HYPER_Handle.CSx                     = OSPI_CS_0;       

    HAL_OSPI_Init(&HYPER_Handle); 

    //HRAM_RETN-PC0-GPIO 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_NOPULL;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    GPIO_Handle.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOC, &GPIO_Handle);
    
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);  
    HAL_DelayMs(2); 

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); 
    HAL_DelayMs(2);     
}

/******************************************************************************
* @brief : W958D8NBYA OSPI octal communication initialization.
* @param : None
* @return: None
******************************************************************************/
void Hyper_OSPI_Octal_Init(void)
{   
    //双倍传输速率模式DTR
    HYPER_OSPI_Octal.DTRMode = OSPI_DTRM_DTR;
    //数据选通使能                  
    HYPER_OSPI_Octal.DQSMode = OSPI_DQSE_ENABLE;
    //存储器类型               
    HYPER_OSPI_Octal.MemoryType = OSPI_MEM_HYPERBUS;
    //DTR通信输出延迟              
    HYPER_OSPI_Octal.OutDelay = OSPI_TX_OUT_DELAY_HALF_HCLK;
    //DQS采样使能    
    HYPER_OSPI_Octal.DQSSample = OSPI_DQS_SAMPLE_DISABLE;       
    
    HAL_OSPI_OctalInit(&HYPER_Handle, &HYPER_OSPI_Octal);
    
    //HAL_OSPI_DQSEnableDisable(&HYPER_Handle, OSPI_DQSE_ENABLE);//Hyperbuds存储器RWDS输出使能  ???禁止时是否有效
}

/******************************************************************************
* @brief : W958D8NBYA read register.
* @param : pCA: Pointer to read register command and address
* @param : pData: Pointer to read data buffer(Dn A is RG[15:8], Dn B is RG[7:0])
* @return: None
******************************************************************************/
void W958D8NBYA_ReadRegister(uint8_t *pCA, uint16_t *pData)
{
    uint16_t write_buff[10];
    uint16_t dummy_data = 0x0000;
    uint8_t LC = DEFAULT_CLK_LATENCY;
        
    write_buff[0] = pCA[0];         //CA0[47:40]
    write_buff[0] += pCA[1] << 8;   //CA0[39:32]

    write_buff[1] = pCA[2];         //CA1[31:24]
    write_buff[1] += pCA[3] << 8;   //CA1[23:16]

    write_buff[2] = pCA[4];         //CA2[15:8]
    write_buff[2] += pCA[5] << 8;   //CA2[7:0]
    
    memset(&write_buff[3], dummy_data, LC);

    OSPI_TwoTransmits_Recieve_ByHalfWord(&HYPER_Handle, write_buff, 3, dummy_data, (LC-1), (2*LC-1), pData, 1, 0);
}

/******************************************************************************
* @brief : W958D8NBYA write register.
* @param : pCA: Pointer to write register command and address
* @param : pData: Pointer to write data buffer
* @return: None
******************************************************************************/
void W958D8NBYA_WriteRegister(uint8_t *pCA, uint16_t pData)
{
    uint16_t write_buff[4];
        
    write_buff[0] = pCA[0];         //CA0[47:40]
    write_buff[0] += pCA[1] << 8;   //CA0[39:32]

    write_buff[1] = pCA[2];         //CA1[31:24]
    write_buff[1] += pCA[3] << 8;   //CA1[23:16]

    write_buff[2] = pCA[4];         //CA2[15:8]
    write_buff[2] += pCA[5] << 8;   //CA2[7:0]
    
    write_buff[3] = pData;

    HAL_OSPI_DQSEnableDisable(&HYPER_Handle, OSPI_DQSE_DISABLE);//Hyperbus存储器RWDS输出禁止
    HAL_OSPI_TransmitByHalfWord(&HYPER_Handle, write_buff, 4, 0);
    HAL_OSPI_DQSEnableDisable(&HYPER_Handle, OSPI_DQSE_ENABLE);//Hyperbus存储器RWDS输出使能  
}

/******************************************************************************
* @brief : W958D8NBYA Read data from memory.
* @param : burstType:  wrapped burst or linear burst, HYPER_WRAPPED_BURST/HYPER_LINEAR_BURST
* @param : addr: Read memory address 
* @param : pRdata: Pointer to write data buffer
* @param : RLen: The length of the write data
* @return: None
******************************************************************************/
void W958D8NBYA_ReadData(uint8_t burstType, uint32_t addr, uint16_t *pRdata, uint32_t RLen)
{
    uint16_t write_buff[3];
    uint16_t dummy_data = 0x0000;
    uint8_t LC = DEFAULT_CLK_LATENCY;
    uint8_t ca_buff[CA_LEN];
    
    memset(ca_buff, 0 , CA_LEN);
    
    ca_buff[0] = (uint8_t)(HYPER_MEMORY_READ | burstType | ((addr >> 27) & 0x1F));
    ca_buff[1] = (uint8_t)((addr >> 19) & 0xFF);
    ca_buff[2] = (uint8_t)((addr >> 11) & 0xFF);
    ca_buff[3] = (uint8_t)((addr >> 3) & 0xFF);
    ca_buff[4] = 0x00;
    ca_buff[5] = (uint8_t)(addr & 0x07);
        
    write_buff[0] = ca_buff[0];         //CA0[47:40]
    write_buff[0] += ca_buff[1] << 8;   //CA0[39:32]

    write_buff[1] = ca_buff[2];         //CA1[31:24]
    write_buff[1] += ca_buff[3] << 8;   //CA1[23:16]

    write_buff[2] = ca_buff[4];         //CA2[15:8]
    write_buff[2] += ca_buff[5] << 8;   //CA2[7:0]

    OSPI_TwoTransmits_Recieve_ByHalfWord(&HYPER_Handle, write_buff, 3, dummy_data, (LC-1), (2*LC-1), pRdata, RLen, 0);
}

/******************************************************************************
* @brief : W958D8NBYA Write data from memory.
* @param : burstType:  wrapped burst or linear burst, HYPER_WRAPPED_BURST/HYPER_LINEAR_BURST
* @param : addr: Write memory address 
* @param : pWdata: Pointer to write data buffer
* @param : WLen: The length of the write data
* @return: None
******************************************************************************/
void W958D8NBYA_WriteData(uint8_t burstType, uint32_t addr, uint16_t *pWdata, uint32_t WLen)
{
    uint16_t write_buff[3];
    uint16_t dummy_data = 0x0000;
    uint8_t LC = DEFAULT_CLK_LATENCY;
    uint8_t ca_buff[CA_LEN];
    
    memset(ca_buff, 0 , CA_LEN);
    
    ca_buff[0] = (uint8_t)(HYPER_MEMORY_WRITE | burstType | ((addr >> 27) & 0x1F));
    ca_buff[1] = (uint8_t)((addr >> 19) & 0xFF);
    ca_buff[2] = (uint8_t)((addr >> 11) & 0xFF);
    ca_buff[3] = (uint8_t)((addr >> 3) & 0xFF);
    ca_buff[4] = 0x00;
    ca_buff[5] = (uint8_t)(addr & 0x07);
        
    write_buff[0] = ca_buff[0];         //CA0[47:40]
    write_buff[0] += ca_buff[1] << 8;   //CA0[39:32]

    write_buff[1] = ca_buff[2];         //CA1[31:24]
    write_buff[1] += ca_buff[3] << 8;   //CA1[23:16]

    write_buff[2] = ca_buff[4];         //CA2[15:8]
    write_buff[2] += ca_buff[5] << 8;   //CA2[7:0]


    OSPI_ThreeTransmits_ByHalfWord(&HYPER_Handle, write_buff, 3, dummy_data, (LC-1), (2*LC-1), pWdata, WLen, 0);
}


/******************************************************************************
* @brief : W958D8NBYA read Vendor ID and Device ID.
* @param : None
* @return: Vendor ID(High 8 bit) and Device ID(Low 8 bit)
******************************************************************************/
uint16_t W958D8NBYA_ReadID(void)
{
    uint16_t id = 0;
    
    W958D8NBYA_ReadRegister(IR0_READ_CA, &id);

    return id;  
}


/******************************************************************************
* @brief : W958D8NBYA read registers test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_ReadRegister_Test(void)
{
    uint16_t read_buff[4];   
  
    HYPER_DEBUG("Read Register: \r\n");
    
    W958D8NBYA_ReadRegister(IR0_READ_CA, &read_buff[0]);
    HYPER_DEBUG("Identification Register 0 = 0x%02x%02x  \n", (read_buff[0]&0xff), ((read_buff[0] >> 8) & 0xff));

    W958D8NBYA_ReadRegister(IR1_READ_CA, &read_buff[1]);
    HYPER_DEBUG("Identification Register 1 = 0x%02x%02x  \n", (read_buff[1]&0xff), ((read_buff[1] >> 8) & 0xff));

    W958D8NBYA_ReadRegister(CR0_READ_CA, &read_buff[2]);
    HYPER_DEBUG("Configuration Register 0 = 0x%02x%02x  \n", (read_buff[2]&0xff), ((read_buff[2] >> 8) & 0xff));

    W958D8NBYA_ReadRegister(CR1_READ_CA, &read_buff[3]);
    HYPER_DEBUG("Configuration Register 1 = 0x%02x%02x  \n", (read_buff[3]&0xff), ((read_buff[3] >> 8) & 0xff));
    
}

/******************************************************************************
* @brief : W958D8NBYA read and write test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_Fifo_Read_Write_Test(void)
{
    uint16_t i, err_count = 0;
    uint16_t writeBuf[256]; 
    uint16_t readBuf[256];                            
    uint32_t test_addr = 0x00000000;
    uint16_t wirte_data =0x1234;
    
    for(i = 0; i < 256; i++)
    {
        writeBuf[i] = wirte_data + i;
    }
    
    W958D8NBYA_WriteData(HYPER_WRAPPED_BURST, test_addr, writeBuf, 16);
    
    memset(readBuf, 0 , 256);
    W958D8NBYA_ReadData(HYPER_WRAPPED_BURST,test_addr, readBuf, 8);
    W958D8NBYA_ReadData(HYPER_WRAPPED_BURST,test_addr+8, readBuf+8, 8);
    for(i = 0; i < 16; i++)
    {
        HYPER_DEBUG("Read[%d] = 0x%04x\r\n", i, readBuf[i]);
        if(readBuf[i] != writeBuf[i])
        {
            HYPER_DEBUG("[%d]: readBuf[0x%04x] != writeBuf[0x%04x]\r\n", (i), readBuf[i], writeBuf[i]);
            err_count++;
        }
    }
    
    if(err_count)
    {
        HYPER_DEBUG("W958D8NBYA Read Write Fail!!!\r\n");
    }
    else
    {
        HYPER_DEBUG("W958D8NBYA Read Write Success!!!\r\n");
    }
}

/******************************************************************************
* @brief : W958D8NBYA test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_Fifo_Test(void)
{
    Hyper_OSPI_Init();
    
    Hyper_OSPI_Octal_Init();
    
    W958D8NBYA_ReadRegister_Test();
    
    W958D8NBYA_Fifo_Read_Write_Test();
}

/******************************************************************************
* @brief : W958D8NBYA OSPI Memory mapping mode initialization.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_OSPI_Memory_Init(void)
{
	Hyper_Memeory_Handle.WriteCmd = 0x00;
	Hyper_Memeory_Handle.ReadCmd = 0x00;      
	Hyper_Memeory_Handle.AlterByte = 0x00;

    Hyper_Memeory_Handle.WrapSize = 0;
    Hyper_Memeory_Handle.BurstLen = MEMOACC2_BURST_LEN_32;                     //突发长度（Burst Length）32Byte
    Hyper_Memeory_Handle.HyperXspiLC1 = MEMOACC2_LATENCY1_13;                    //Hyperbus或xSPI模式下，RWDS为一时的LC周期数 14个CLK
    Hyper_Memeory_Handle.HyperXspiLc0 = MEMOACC2_LATENCY0_6;                     //Hyperbus或xSPI模式下，RWDS为零时的LC周期数 7个CLK    
    

	Hyper_Memeory_Handle.HyperBurstType = 0;               //[27]突发是Linear的还是Wrapped的
	Hyper_Memeory_Handle.DataMode = 0;                      //[26:25]数据模式，八线模式时，此位无效
	Hyper_Memeory_Handle.AlterByteMode = 0;                //[24:23]交替字节模式，八线模式时，此位无效
	Hyper_Memeory_Handle.AddrMode = 0;                      //[22:21]地址模式，八线模式时，此位无效
	Hyper_Memeory_Handle.InstrMode = 0;                     //[20:19]指令模式，八线模式时，此位无效
	Hyper_Memeory_Handle.AddrWidth = MEMOACC1_ADDR_WIDTH_32;                     //[18:17]地址长度（Hyper必须为32 bit）
	Hyper_Memeory_Handle.DummyCycleSize = 0;               //[16:12]dummy周期长度（Hyper无效）
	Hyper_Memeory_Handle.ReadDummyByteEnable = MEMOACC1_READ_DUMMY_ENABLE;                       //[11]读操作空指令字节使能（Hyper必须为1）
	Hyper_Memeory_Handle.WriteummyByteEnable = MEMOACC1_WRITE_DUMMY_ENABLE;                       //[10]写操作空指令字节使能（Hyper必须为1）
	Hyper_Memeory_Handle.AlterByteSize = 0;                //[8:7]]交替字节长度 8 16 24 32，八线模式时，此位无效			
	Hyper_Memeory_Handle.ReadAlterByteEnable = 0;                       //[6]读操作交替字节使能位，八线模式时，此位无效
	Hyper_Memeory_Handle.WriteAlterByteEnable = 0;                       //[5]写操作交替字节使能位，八线模式时，此位无效
	Hyper_Memeory_Handle.SendInstrOnce = 0;                    //[4]仅发送指令一次
    Hyper_Memeory_Handle.ContinuousModeEnable = MEMOACC1_CON_ENABLE;                    //[3]连续读使能
	Hyper_Memeory_Handle.CsTimeoutEnable = 0;                     //[2]连续模式下CS拉高等待功能不使能
	HAL_OSPI_MemoryInit(&HYPER_Handle, &Hyper_Memeory_Handle);                          //[1]存储器访问使能    
}

/******************************************************************************
* @brief : W958D8NBYA memory 8bit test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_Memory_8bit_Write_Read(void)
{
    volatile uint8_t *pMem8;
    uint16_t i, err_count = 0;
    uint8_t writeBuf[HYPER_BUFF_LEN]; 
    uint8_t readBuf[HYPER_BUFF_LEN];                            
    
    pMem8 = (volatile uint8_t *) OSPI1_MEM_BASE_ADDR;
    
       
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
    }
    
    //写
    HYPER_DEBUG("Memory write in 8-bit mode...\r\n"); 
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        *(pMem8 + i*2) = writeBuf[i];
    }
    HAL_DelayMs(100);
    //读
    HYPER_DEBUG("Memory read in 8-bit mode...\r\n"); 
    memset(readBuf, 0 , HYPER_BUFF_LEN);
    for( i = 0; i < HYPER_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem8 + i*2);
    }
	//比对
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            HYPER_DEBUG("[%d]: readBuf[0x%04x] != writeBuf[0x%04x]\r\n", (i), readBuf[i], writeBuf[i]);
            err_count++;
        }
    }
    
    if(err_count)
    {
        HYPER_DEBUG("W958D8NBYA Memory 16-bit write & read fail!!!\r\n");
    }
    else
    {
        HYPER_DEBUG("W958D8NBYA Memory 16-bit write & read Success!!!\r\n");
    }

}

/******************************************************************************
* @brief : W958D8NBYA memory 16bit test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_Memory_16bit_Write_Read(void)
{
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint16_t writeBuf[HYPER_BUFF_LEN]; 
    uint16_t readBuf[HYPER_BUFF_LEN];
                  
    
    pMem16 = (volatile uint16_t *) OSPI1_MEM_BASE_ADDR;  
       
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        writeBuf[i] = HYPER_DATA_16BIT + i;
    }
    
    //写
    HYPER_DEBUG("Memory write in 16-bit mode...\r\n"); 
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        *(pMem16 + i) = writeBuf[i];
    }
    HAL_DelayMs(100);

    

    //读
    // HYPER_DEBUG("Memory read in 16-bit mode...\r\n"); 
    memset(readBuf, 0 , HYPER_BUFF_LEN);
    for( i = 0; i < HYPER_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem16 + i);
    }
    //比对
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            HYPER_DEBUG("[%d]: readBuf[0x%04x] != writeBuf[0x%04x]\r\n", (i), readBuf[i], writeBuf[i]);
            err_count++;
        }
    }
    
    if(err_count)
    {
        HYPER_DEBUG("W958D8NBYA Memory 16-bit write & read fail!!!\r\n");
    }
    else
    {
        HYPER_DEBUG("W958D8NBYA Memory 16-bit write & read Success!!!\r\n");
        
    }

}

/******************************************************************************
* @brief : W958D8NBYA memory 32bit test.
* @param : None
* @return: None
******************************************************************************/
void W958D8NBYA_Memory_32bit_Write_Read(void)
{
    volatile uint32_t *pMem32;
    uint32_t i, err_count = 0;
    uint32_t writeBuf[HYPER_BUFF_LEN]; 
    uint32_t readBuf[HYPER_BUFF_LEN];                            
    
    pMem32 = (volatile uint32_t *) OSPI1_MEM_BASE_ADDR;
       
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        writeBuf[i] = HYPER_DATA_32BIT + i;
    }
    
    //写
    HYPER_DEBUG("Memory write in 32-bit mode...\r\n"); 
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        *(pMem32 + i) = writeBuf[i];
    }
    HAL_DelayMs(100);
    //读
    HYPER_DEBUG("Memory read in 32-bit mode...\r\n"); 
    memset(readBuf, 0 , HYPER_BUFF_LEN);
    for( i = 0; i < HYPER_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem32 + i);
    }
	//比对
    for(i = 0; i < HYPER_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            HYPER_DEBUG("[%d]: readBuf[0x%08x] != writeBuf[0x%08x]\r\n", (i), readBuf[i], writeBuf[i]);
            err_count++;
        }
    }
    
    if(err_count)
    {
        HYPER_DEBUG("W958D8NBYA Memory 32-bit write & read fail!!!\r\n");
    }
    else
    {
        HYPER_DEBUG("W958D8NBYA Memory 32-bit write & read Success!!!\r\n");
    }

}


void W958D8NBYA_OSPI_HyperBus_Test(void)
{
    Hyper_OSPI_Init();
    Hyper_OSPI_Octal_Init();
    W958D8NBYA_OSPI_Memory_Init();
    
    HYPER_Handle.Instance->RX_CTL |= OSPI_RX_CTL_MSDA_EN;
    
    W958D8NBYA_Memory_8bit_Write_Read();
    HAL_DelayMs(500);
    W958D8NBYA_Memory_16bit_Write_Read();
    HAL_DelayMs(500);
    W958D8NBYA_Memory_32bit_Write_Read();
}

