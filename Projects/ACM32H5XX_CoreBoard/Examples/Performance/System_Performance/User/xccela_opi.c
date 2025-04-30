/******************************************
*   Double-Data-Rate Octal SPI PSRAM
*   APS6408L-OBM-BA
******************************************/
#include "xccela_opi.h"


OSPI_HandleTypeDef XCCELAOPI_Handle;
OSPI_MemoryInitTypeDef XCCELAOPI_Memeory_Handle;
OSPI_OctalInitTypeDef Xccela_OSPI_Octal = {0};


/******************************************************************************
* @brief : APS6408L-OBM-BA OSPI initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_OSPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Handle;
    
    __HAL_RCC_OSPI2_RESET();
   
    XCCELAOPI_Handle.Instance                = OSPI2;

    /* OSPI工作模式 */
    XCCELAOPI_Handle.Init.WorkMode           = OSPI_WORK_MODE_0;
    /* OSPI多线模式 */    
    XCCELAOPI_Handle.Init.XMode              = OSPI_8X_MODE;
    /* MSB/LSB在前选择 */        
    XCCELAOPI_Handle.Init.FirstBit           = OSPI_FIRSTBIT_MSB;
    /* 分频系数 */   
    XCCELAOPI_Handle.Init.BaudRatePrescaler  = OSPI_BAUDRATE_PRESCALER_2; 
    /* 采样延时 */     
    XCCELAOPI_Handle.Init.SampleShifting     = OSPI_SAMPLE_SHIFT_2_5HCLK;  //PSRAM is SOC, use OSPI_SAMPLE_SHIFT_2_5HCLK    
    /* FIFO写模式 */     
    XCCELAOPI_Handle.Init.FWMode             = OSPI_FIFO_HALFWORD;
    /* FIFO读模式 */      
    XCCELAOPI_Handle.Init.FRMode             = OSPI_FIFO_HALFWORD;
    /* CS控制信号 */      
    XCCELAOPI_Handle.CSx                     = OSPI_CS_0;       

    HAL_OSPI_Init(&XCCELAOPI_Handle); 

    
    XCCELA_RSTN_CLK_ENABLE();
    GPIO_Handle.Pin = XCCELA_RSTN_PIN;
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_NOPULL;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    HAL_GPIO_Init(XCCELA_RSTN_GPIO, &GPIO_Handle);
    HAL_GPIO_WritePin(XCCELA_RSTN_GPIO, XCCELA_RSTN_PIN, GPIO_PIN_RESET); 

    printfS("OSPI->BAUD = 0x%x\r\n", XCCELAOPI_Handle.Instance->BAUD);   
}

/******************************************************************************
* @brief : APS6408L-OBM-BA OSPI octal communication initialization.
* @param : None
* @return: None
******************************************************************************/
void  APS6408L_OBM_OSPI_Octal_Init(void)
{   
    /* 双倍传输速率模式DTR */
    Xccela_OSPI_Octal.DTRMode = OSPI_DTRM_DTR;
    /* 数据选通使能 */         
    Xccela_OSPI_Octal.DQSMode = OSPI_DQSOE_ENABLE;
    /* 存储器类型 */     
    Xccela_OSPI_Octal.MemoryType = OSPI_MEM_XCCELA_OPI;
    /* DTR通信输出延迟 */   
    Xccela_OSPI_Octal.OutDelay = OSPI_TX_OUT_DELAY_HALF_HCLK;
    /* DQS采样使能 */  
    Xccela_OSPI_Octal.DQSSample = OSPI_DQS_SAMPLE_DISABLE;       
    
    HAL_OSPI_OctalInit(&XCCELAOPI_Handle, &Xccela_OSPI_Octal);
   
}

/******************************************************************************
* @brief : APS6408L-OBM-BA reset.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_GlobalReset(void)
{
    uint16_t write_buff[5];
    //global reset 4clocks
    write_buff[0] = XCCELAOPI_GLOBAL_RESET;
    write_buff[0] += XCCELAOPI_GLOBAL_RESET << 8;//xx
    write_buff[1] = 0x0000;//dummy cycle 1
    write_buff[2] = 0x0000;//dummy cycle 2
    write_buff[3] = 0x0000;//dummy cycle 3
    write_buff[4] = 0x0000;//dummy cycle 4
    HAL_OSPI_TransmitByHalfWord(&XCCELAOPI_Handle, write_buff, 5, 0); 
}

/******************************************************************************
* @brief : APS6408L-OBM-BA power-up initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_PowerUp_Init(void)
{
    APS6408L_OBM_OSPI_Init();
    
    APS6408L_OBM_OSPI_Octal_Init(); 
    
    HAL_DelayUs(200); 

    HAL_GPIO_WritePin(XCCELA_RSTN_GPIO, XCCELA_RSTN_PIN, GPIO_PIN_SET);
    
    HAL_DelayMs(2);    
}

/******************************************************************************
* @brief : APS6408L-OBM-BA read control registers.
* @param : addr: control registers address
* @data  : pData: Pointer to read data buffer
* @return: None
******************************************************************************/
void APS6408L_OBM_ReadRegister(uint8_t addr, uint16_t *pData)
{
    uint16_t write_buff[8];
        
    //mode register read 
    write_buff[0] = XCCELAOPI_MODE_REGISTER_READ;
    write_buff[0] += XCCELAOPI_MODE_REGISTER_READ << 8;//xx

    write_buff[1] = 0x0000;//xxxx

    write_buff[2] = 0x00;//xx
    write_buff[2] += addr << 8;//MA

    write_buff[3] = 0x0000;//Latency cycle 1
    write_buff[4] = 0x0000;//Latency cycle 2
    write_buff[5] = 0x0000;//Latency cycle 3
    write_buff[6] = 0x0000;//Latency cycle 4

    HAL_OSPI_Transmit_Recieve_ByHalfWord(&XCCELAOPI_Handle, write_buff, 7, pData, 1, 0);   
}

/******************************************************************************
* @brief : APS6408L-OBM-BA write control registers.
* @param : addr: control registers address
* @data  : pData: Pointer to read data buffer
* @return: None
******************************************************************************/
void APS6408L_OBM_WriteRegister(uint8_t addr, uint16_t Data)
{
    uint16_t write_buff[8];
        
    //mode register read 
    write_buff[0] = XCCELAOPI_MODE_REGISTER_WRITE;
    write_buff[0] += XCCELAOPI_MODE_REGISTER_WRITE << 8;//xx

    write_buff[1] = 0x0000;//xxxx

    write_buff[2] = 0x00;//xx
    write_buff[2] += addr << 8;//MA

    write_buff[3] = Data;
    HAL_OSPI_TransmitByHalfWord(&XCCELAOPI_Handle, write_buff, 4, 0);   
}

/******************************************************************************
* @brief : APS6408L-OBM-BA synchronous write .
* @param : addr: control registers address
* @data  : pData: Pointer to read data buffer
* @data  : len: Amount of data to be read
* @return: None
******************************************************************************/
void APS6408L_OBM_Synchronous_Write(uint32_t addr, uint16_t *pData, uint32_t len)
{
    uint16_t write_buff[256+10]; 
    uint8_t pos = 0;
    uint32_t i;
	
    // Synchronous Write followed by No Operation (default WL0)
    memset(write_buff, 0, 256+10);
    
    write_buff[pos] = XCCELAOPI_SYNC_WRITE;
    write_buff[pos++] += XCCELAOPI_SYNC_WRITE << 8;
    
    write_buff[pos] = ((addr >> 24) & 0xff);//A3
    write_buff[pos++] += ((addr >> 16) & 0xff) << 8;//A2
    
    write_buff[pos] = ((addr >> 8) & 0xff);//A1
    write_buff[pos++] += (addr & 0xff) << 8;//A0 
    

    write_buff[pos++] = 0x0000;//dummy cycle 1
    write_buff[pos++] = 0x0000;//dummy cycle 2
    write_buff[pos++] = 0x0000;//dummy cycle 3
    write_buff[pos++] = 0x0000;//dummy cycle 4  
    
    // printfS("Write pos = %d\r\n", pos);
    for(i = 0; i < len; i++)
    {
        write_buff[pos + i] = pData[i];
    }
    HAL_OSPI_TransmitByHalfWord(&XCCELAOPI_Handle, write_buff, (len + pos), 0);
    
    
}


/******************************************************************************
* @brief : APS6408L-OBM-BA synchronous read .
* @param : addr: Control registers address
* @data  : pData: Pointer to read data buffer
* @data  : len: Amount of data to be read
* @return: None
******************************************************************************/
void APS6408L_OBM_Synchronous_Read(uint32_t addr, uint16_t *pData, uint32_t len)
{
    uint16_t write_buff[10];
    uint8_t pos = 0;

  	//Synchronous Read,Read Latency Code:MR0[4:2]
    memset(write_buff, 0, 6);
    write_buff[pos] = XCCELAOPI_SYNC_READ;
    write_buff[pos++] += XCCELAOPI_SYNC_READ << 8;

    write_buff[pos] = ((addr >> 24) & 0xff);//A3
    write_buff[pos++] += ((addr >> 16) & 0xff) << 8;//A2
    
    write_buff[pos] = ((addr >> 8) & 0xff);//A1
    write_buff[pos++] += (addr & 0xff) << 8;//A0
    
    write_buff[pos++] = 0x0000;//Read Latency cycle 1
    write_buff[pos++] = 0x0000;//Read Latency cycle 2
    write_buff[pos++] = 0x0000;//Read Latency cycle 3
    write_buff[pos++] = 0x0000;//Read Latency cycle 4
//    write_buff[pos++] = 0x0000;//Read Latency cycle 5
   
    // printfS("Read pos = %d\r\n", pos);
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&XCCELAOPI_Handle, write_buff, pos, pData, len, 0);
     
}


/******************************************************************************
* @brief : APS6408L-OBM-BA read Vendor ID and Device ID.
* @param : None
* @return: Vendor ID and Device ID
******************************************************************************/
uint16_t APS6408L_OBM_ReadID(void)
{
    uint16_t id = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_1, &id);
    printfS("Vendor ID & Device ID : 0x%04x  \n", id);
    return id;  
}


/******************************************************************************
* @brief : APS6408L-OBM-BA read control registers test.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_ReadRegister_Test(void)
{
    uint16_t read_data;    
    //mode register read 
    printfS("Read Mode Register MA'h00: 0x8D09\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_0, &read_data);
    printfS("MR0/MR1 = 0x%04x  \n",read_data);
  	
    printfS("Read Mode Register MA'h01: 0x938D\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_1, &read_data);
    printfS("MR1/MR2 = 0x%04x  \n",read_data);
    
    printfS("Read Mode Register MA'h02: 0xA093\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_2, &read_data);
    printfS("MR2/MR3 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h03: 0x40A0\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_3, &read_data);
    printfS("MR3/MR4 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h04: 0x0540\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_4, &read_data);
    printfS("MR4/MR8 = 0x%04x  \n",read_data); 
    
    printfS("Read Mode Register MA'h08: 0x0905\r\n");
    read_data = 0;
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_8, &read_data);
    printfS("MR8/MR1 = 0x%04x  \n",read_data);
}


/******************************************************************************
* @brief : APS6408L-OBM-BA Memory mode access initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_OSPI_DTR_Memory_Init(void)
{
    OSPI_DLYB_CfgTypeDef dlyb_cfg, dlyb_cfg_test;
    /* 写指令 */
	XCCELAOPI_Memeory_Handle.WriteCmd = XCCELAOPI_SYNC_WRITE_LINEAR_BURST;
    /* 读指令 */
	XCCELAOPI_Memeory_Handle.ReadCmd = XCCELAOPI_SYNC_READ_LINEAR_BURST;
    /* 交替字节 */
	XCCELAOPI_Memeory_Handle.AlterByte = 0x00;  
    /* 回卷大小 */
    XCCELAOPI_Memeory_Handle.WrapSize = 0;
    /* 突发长度（Burst Length）*/
    XCCELAOPI_Memeory_Handle.BurstLen = MEMOACC2_BURST_LEN_1024;
    /* Hyperbus或xSPI模式下，RWDS为一时的LC周期数 */                    
    XCCELAOPI_Memeory_Handle.HyperXspiLC1 = 0;
    /* Hyperbus或xSPI模式下，RWDS为零时的LC周期数 */                    
    XCCELAOPI_Memeory_Handle.HyperXspiLc0 = 0;                        
    
    /* [27]突发是Linear的还是Wrapped的 */
	XCCELAOPI_Memeory_Handle.HyperBurstType = MEMOACC1_BURST_WRAPPED;
    /* [26:25]数据模式，八线模式时，此位无效 */               
	XCCELAOPI_Memeory_Handle.DataMode = 0;
    /* [24:23]交替字节模式，八线模式时，此位无效 */                      
	XCCELAOPI_Memeory_Handle.AlterByteMode = 0;
    /* [22:21]地址模式，八线模式时，此位无效 */                
	XCCELAOPI_Memeory_Handle.AddrMode = 0;
    /* [20:19]指令模式，八线模式时，此位无效 */                      
	XCCELAOPI_Memeory_Handle.InstrMode = 0;
    /* [18:17]地址长度（Hyper必须为32 bit） */                     
	XCCELAOPI_Memeory_Handle.AddrWidth = MEMOACC1_ADDR_WIDTH_32;
    /* [16:12]dummy周期长度（Hyper/xSPI无效） */                     
	XCCELAOPI_Memeory_Handle.DummyCycleSize = MEMOACC1_DUMMY_CYCLE_4;
    /* [11]读操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1 */               
	XCCELAOPI_Memeory_Handle.ReadDummyByteEnable = MEMOACC1_READ_DUMMY_ENABLE;
    /* [10]写操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1 */                       
	XCCELAOPI_Memeory_Handle.WriteummyByteEnable = MEMOACC1_WRITE_DUMMY_ENABLE;
    /* [8:7]]交替字节长度 8 16 24 32，八线模式时，此位无效 */	                       
	XCCELAOPI_Memeory_Handle.AlterByteSize = 0; 
    /* [6]读操作交替字节使能位，八线模式时，此位无效 */               		
	XCCELAOPI_Memeory_Handle.ReadAlterByteEnable = 0; 
    /* [5]写操作交替字节使能位，八线模式时，此位无效 */                      
	XCCELAOPI_Memeory_Handle.WriteAlterByteEnable = 0; 
    /* [4]仅发送指令一次 */                      
	XCCELAOPI_Memeory_Handle.SendInstrOnce = 0; 
    /* [3]连续读使能 */                    
    XCCELAOPI_Memeory_Handle.ContinuousModeEnable = MEMOACC1_CON_ENABLE;
                  
	XCCELAOPI_Memeory_Handle.CsTimeoutEnable = MEMOACC1_CS_TIMEOUT_ENABLE;           
    XCCELAOPI_Memeory_Handle.CsTimeoutVal = 640;   // CsTimeoutVal * tHCLK <= 3us  for high temperature, CsTimeoutVal * tHCLK  <= 8us for room temperature as PSRAM Spec  
                

    HAL_OSPI_MemoryInit(&XCCELAOPI_Handle, &XCCELAOPI_Memeory_Handle);
    
    HAL_OSPI_MemoryEnableDisable(&XCCELAOPI_Handle,  MEMOACC1_MEMORY_ENABLE); //XIP Mode    
}

/******************************************************************************
* @brief : APS6408L-OBM-BA DTR Memory Mode 8-bit Write then Read Test. 
           Write and Read 1MB as 8-bit Mode
* @param : write_data: the data write to PSRAM 
* @return: 0: PASS; 1: FAIL  
******************************************************************************/
uint16_t OSPI_DTR_Memory_8bit_Write_Then_Read_Performance_Test(uint8_t write_data)
{    
    volatile uint8_t *pMem8;
    uint32_t i, j, ms_tick1, ms_tick2; 
 
    printfS("\r\n8bit_Write_Then_Read, 8KB Loop 128 times, Test Data: 0x%02x\r\n", write_data);      
    
    pMem8 = (volatile uint8_t *) (OSPI_MEMORY_ADDR + 0x0000);   
    
    HAL_Delay(2); 
    //Start Write 
    ms_tick1 = HAL_GetTick();  
    for(i = 0; i < 0x2000; i++)  // 8KB
    {
        // write and read 
        *(pMem8 + i) = write_data;  
        
        if (write_data != *(pMem8 + i) )  // Read and Compare 
        {
            printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem8 + i) );     
            return 1;  
        }       
    }   
    
    for (j = 0; j < 127; j++) //LOOP 127 times   
    {
        write_data++;
    
        for(i = 0; i < 0x2000; i++)  // 8KB  
        {
            // Modify and read 
            *(pMem8 + i) = write_data;  
        
            if (write_data != *(pMem8 + i) )  // Read and Compare   
            {
                printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem8 + i) );     
                return 1;  
            }       
        }  
    }
    
    ms_tick2 = HAL_GetTick();  
    
    printfS("Write Data: 0x%02x\n", write_data);  
    printfS("tick: %d, %d\n", ms_tick1, ms_tick2);     
    printfS("Write Then Read Time:%dms\n", ms_tick2-ms_tick1);      
    
    return 0; 
}

/******************************************************************************
* @brief : APS6408L-OBM-BA DTR Memory Mode 16-bit Write then Read Test. 
           Write and Read 1MB as 16-bit Mode
* @param : write_data: the data write to PSRAM 
* @return: 0: PASS; 1: FAIL  
******************************************************************************/
uint16_t OSPI_DTR_Memory_16bit_Write_Then_Read_Performance_Test(uint16_t write_data)
{    
    volatile uint16_t *pMem16;
    uint32_t i, j, ms_tick1, ms_tick2;  
 
    printfS("\r\n16bit_Write_Then_Read, 8KB Loop 128 times, Test Data: 0x%04x\r\n", write_data);    
    
    pMem16 = (volatile uint16_t *) (OSPI_MEMORY_ADDR + 0x0000);   
    
    HAL_Delay(2); 
    //Start Write 
    ms_tick1 = HAL_GetTick();  
    for(i = 0; i < 0x1000; i++)  // 8KB
    {
        *(pMem16 + i) = write_data;  
        if (write_data != *(pMem16 + i) )  
        {
            printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem16 + i) );    
            return 1;  
        }  
    }   
    
    for (j = 0; j < 127; j++) //LOOP 127 times   
    {
        write_data++;
    
        for(i = 0; i < 0x1000; i++)  // 8KB  
        {
            // Modify and read 
            *(pMem16 + i) = write_data;  
        
            if (write_data != *(pMem16 + i) )  // Read and Compare   
            {
                printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem16 + i) );     
                return 1;  
            }       
        }  
    }

    ms_tick2 = HAL_GetTick();   
    
    printfS("Write Data: 0x%04x\n", write_data);   
    
    printfS("tick: %d, %d\n", ms_tick1, ms_tick2);   
    printfS("Write Then Read Time:%dms\n", ms_tick2-ms_tick1);      
    
    return 0; 
}

/******************************************************************************
* @brief : APS6408L-OBM-BA DTR Memory Mode 32-bit Read Write Test. 
           Write and Read 1MB as 32-bit Mode
* @param : write_data: the data write to PSRAM  
* @return: 0: PASS; 1: FAIL  
******************************************************************************/
uint16_t OSPI_DTR_Memory_32bit_Write_Then_Read_Performance_Test(uint32_t write_data)
{    
    volatile uint32_t *pMem32;
    uint32_t i, j, ms_tick1, ms_tick2; 
 
    printfS("\r\n32bit_Write_Then_Read, 8KB Loop 128 times, Test Data: 0x%08x\r\n", write_data);    
    
    pMem32 = (volatile uint32_t *) (OSPI_MEMORY_ADDR + 0x0000);   
    
    HAL_Delay(2); 
    //Start Write 
    ms_tick1 = HAL_GetTick();  
    for(i = 0; i < 0x800; i++)  // 8KB 
    {
        *(pMem32 + i) = write_data;  
        if (write_data != *(pMem32 + i) )  
        {
            printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem32 + i) );      
            return 1;  
        }  
    }   
    
    for (j = 0; j < 127; j++) //LOOP 127 times   
    {
        write_data++;
    
        for(i = 0; i < 0x800; i++)  // 8KB  
        {
            // Modify and read 
            *(pMem32 + i) = write_data;  
        
            if (write_data != *(pMem32 + i) )  // Read and Compare   
            {
                printfS("Read Data Error: %d, 0x%x\r\n", i, *(pMem32 + i) );       
                return 1;  
            }       
        }  
    }
    ms_tick2 = HAL_GetTick();  
    
    printfS("Write Data: 0x%08x\n", write_data);   
    
    printfS("tick: %d, %d\n", ms_tick1, ms_tick2);   
    printfS("Write Then Read Time:%dms\n", ms_tick2-ms_tick1);      
    
    return 0; 
}


/******************************************************************************
* @brief : APS6408L-OBM-BA Test.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_OSPI_Xccela_Test(void)
{   
    APS6408L_OBM_PowerUp_Init();
    
    APS6408L_OBM_ReadID();

    APS6408L_OBM_OSPI_DTR_Memory_Init();
    
    OSPI_DTR_Memory_8bit_Write_Then_Read_Performance_Test(0xAA);   
    OSPI_DTR_Memory_8bit_Write_Then_Read_Performance_Test(0x55);     
    OSPI_DTR_Memory_16bit_Write_Then_Read_Performance_Test(0xCC33);   
    OSPI_DTR_Memory_16bit_Write_Then_Read_Performance_Test(0x33CC);   
    OSPI_DTR_Memory_32bit_Write_Then_Read_Performance_Test(0xAA5555AA); 
    OSPI_DTR_Memory_32bit_Write_Then_Read_Performance_Test(0x55AAAA55);   
    
    printfS("APS6408L_OBM_OSPI_Xccela_Test End\n");
}



