/******************************************
*   Double-Data-Rate Octal SPI PSRAM
*   APS6408L-OBM-BA
******************************************/
#include "xccela_opi.h"
#include "drv_ospi.h"


OSPI_HandleTypeDef XCCELAOPI_Handle;

uint16_t chip_id_code; 
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
    
    HAL_EFUSE_GetICCode(&chip_id_code); 
    
    if(IC_H538VMT6_TM ==  chip_id_code) 
    {
        XCCELAOPI_Handle.Reset_Pin = GPIO_PIN_0;       
    }
    else
    {
        XCCELAOPI_Handle.Reset_Pin = GPIO_PIN_6;      
    }
    
    GPIO_Handle.Pin = XCCELAOPI_Handle.Reset_Pin;  
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_NOPULL;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    HAL_GPIO_Init(XCCELA_RSTN_GPIO, &GPIO_Handle);
    HAL_GPIO_WritePin(XCCELA_RSTN_GPIO,  XCCELAOPI_Handle.Reset_Pin, GPIO_PIN_RESET);   

}

/******************************************************************************
* @brief : APS6408L-OBM-BA OSPI octal communication initialization.
* @param : None
* @return: None
******************************************************************************/
void  APS6408L_OBM_OSPI_Octal_Init(void)
{   
    OSPI_OctalInitTypeDef Xccela_OSPI_Octal = {0};
    
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
* @brief : APS6408L-OBM-BA power-up initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_PowerUp_Init(void)
{
    APS6408L_OBM_OSPI_Init();
    
    APS6408L_OBM_OSPI_Octal_Init(); 
    
    HAL_DWT_DelayUs(10);  

    HAL_GPIO_WritePin(XCCELA_RSTN_GPIO, XCCELAOPI_Handle.Reset_Pin, GPIO_PIN_SET);
    
    HAL_DWT_DelayUs(5);      
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
//    write_buff[7] = 0x0000;//Latency cycle 5
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
* @brief : APS6408L-OBM-BA read Vendor ID and Device ID.
* @param : None
* @return: Vendor ID and Device ID
******************************************************************************/
uint16_t APS6408L_OBM_ReadID(void)
{
    uint16_t id = 0; 
    
    APS6408L_OBM_ReadRegister(XCCELAOPI_MA_ADDR_1, &id);

    return id;  
}

/******************************************************************************
* @brief : APS6408L-OBM-BA Memory mode access initialization.
* @param : None
* @return: None
******************************************************************************/
void APS6408L_OBM_OSPI_DTR_Memory_Init(void)
{
    OSPI_MemoryInitTypeDef XCCELAOPI_Memeory_Handle = {0}; 
    
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

    HAL_OSPI_MemoryEnableDisable(&XCCELAOPI_Handle,  MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
}


/******************************************************************************
* @brief : APS6408L-OBM-BA Test.
* @param : None
* @return: None
******************************************************************************/
uint8_t APS6408L_OBM_OSPI_Xccela_XIP_Init(void)
{  
    uint8_t id; 
    
    APS6408L_OBM_PowerUp_Init();
    
    id = APS6408L_OBM_ReadID(); 
    
    if (OSPI_PSRAM_VENDOR_ID == id)
    {
        return 1;  
    }

    APS6408L_OBM_OSPI_DTR_Memory_Init();  
    
    return 0;   
}



