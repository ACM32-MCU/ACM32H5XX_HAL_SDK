/******************************************
*   GD25LX256E
******************************************/
#include "xspi_flash.h"
#include "drv_norflash.h"
#include "drv_ospi.h"



#define XSPI_BUFF_LEN               (NORFLASH_PAGE_SIZE)
#define XSPI_HALF_BUFF_LEN          (XSPI_BUFF_LEN/2)
#define XSPI_QUARTER_UFF_LEN        (XSPI_BUFF_LEN/4)
#define XSPI_DATA_16BIT             (0x1616)
#define XSPI_DATA_32BIT             (0x32323232)

#define XSPI_SOFEWARE_CS            (0)
#define XSPI_HARDWARE_CS            (1)

OSPI_MemoryInitTypeDef xSPI_Memeory_Handle;
OSPI_OctalInitTypeDef GD25LX256E_Octal = {0};
DMA_HandleTypeDef GD25LX256E_DMA1_Ch0Handle;
DMA_HandleTypeDef GD25LX256E_DMA2_Ch1Handle;


/**************** GD25LX256E 1x/8x SDR/8x DTR start ****************/

//软件/硬件CS设置，mode；1为硬件CS， mode：0为软件CS
void GD25LX256E_OSPI_CS_Init(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_Handle = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

#ifdef SOFTWARE_CS_ENABLE    
    if(mode)//硬件CS
    {
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_13;
    }
    else//软件CS
    {
        GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_Handle.Pull = GPIO_NOPULL;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    }
    
#else
    GPIO_Handle.Mode = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull = GPIO_PULLUP;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_13;

#endif

    GPIO_Handle.Pin = GPIO_PIN_6;//CS-PB6-AF13
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
}

//OSPI初始化
void GD25LX256E_OSPI_Init(uint32_t xMode, uint32_t workMode)
{
    uint8_t ret;
    GPIO_InitTypeDef GPIO_Handle = {0};
    
    __HAL_RCC_OSPI1_RESET();
    __HAL_RCC_OSPI2_RESET();
    
    NORFLASH_Handle.Instance                = OSPI1;
    NORFLASH_Handle.Init.WorkMode           = workMode;
    NORFLASH_Handle.Init.XMode              = xMode;
    NORFLASH_Handle.Init.FirstBit           = OSPI_FIRSTBIT_MSB;
    NORFLASH_Handle.Init.BaudRatePrescaler  = OSPI_BAUDRATE_PRESCALER_2;  
    NORFLASH_Handle.Init.SampleShifting     = OSPI_SAMPLE_SHIFT_1HCLK;//OSPI_SAMPLE_SHIFT_NONE;
    NORFLASH_Handle.Init.FWMode             = OSPI_FIFO_BYTE;
    NORFLASH_Handle.Init.FRMode             = OSPI_FIFO_BYTE;
    NORFLASH_Handle.CSx                     = OSPI_CS_0;


    HAL_OSPI_Init(&NORFLASH_Handle); 
    
    /* Software CS */    
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    
    HAL_OSPI_WireConfig(&NORFLASH_Handle, OSPI_1X_MODE);
    
    /* First Set CS HIGH */
    __OSPI_CS_Release(); 
    
    
    /* Reset */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_NOPULL;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    GPIO_Handle.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);

    for(ret = 0; ret < 5; ret++)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
        
        HAL_SimpleDelay(5000);
        
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
        
        HAL_SimpleDelay(5000);
    }
        
}

//OSPI 八线初始化
void GD25LX256E_Octal_Init(uint32_t mode, uint32_t delay)
{
    GD25LX256E_Octal.DTRMode = mode;         //双倍传输速率模式DTR
    GD25LX256E_Octal.DQSMode = OSPI_DQSOE_ENABLE;     //数据选通使能
    GD25LX256E_Octal.MemoryType = OSPI_MEM_XSPI;   //存储器类型
    GD25LX256E_Octal.OutDelay = delay;  //DTR通信输出延迟
    GD25LX256E_Octal.DQSSample = OSPI_DQS_SAMPLE_DISABLE;       //DQS采样使能
    
    HAL_OSPI_OctalInit(&NORFLASH_Handle, &GD25LX256E_Octal);
    
    //HAL_OSPI_DQSEnableDisable(&NORFLASH_Handle, OSPI_DQSOE_ENABLE);//APM OPI存储器数据选通位
}

//GD25LX256E OSPI Flash初始化，获取ID以及片擦
void GD25LX256E_OSPI_Flash_Init(void)
{
    uint32_t lu32_ID;
    uint8_t temp; 
    GD25LX256E_OSPI_Init(OSPI_8X_MODE, OSPI_WORK_MODE_0);
    GD25LX256E_Octal_Init(OSPI_DTRM_STR, OSPI_TX_OUT_DELAY_HALF_HCLK); 
    NORFLASH_Reset();
    HAL_Delay(500);
    //获取Norflash 设备唯一序列号
    lu32_ID = NORFLASH_Read_Identification();
    printfS("GD25LX256 Identification  : 0x%08X \r\n", lu32_ID);  
    NORFLASH_EraseChip();
}

/************************************************************FIFO Start**************************************************************/
/************************************************************FIFO Start**************************************************************/
/************************************************************FIFO Start**************************************************************/

//1线、八线STR、八线DTR FIFO模式读写测试
void GD25LX256E_FIFO_Test(uint32_t xMode, uint32_t transferMode, uint32_t workMode)
{
    uint8_t TxBuffer[XSPI_BUFF_LEN];
    uint8_t RxBuffer[XSPI_BUFF_LEN];
    uint32_t i;
    uint8_t unique_id_buff[16]={0};
    uint32_t lu32_ID;
    uint32_t lu32_ErrCount = 0;
    uint8_t temp;
    uint16_t DTR_Buff[16];
    uint16_t DTR_Data;

    GD25LX256E_OSPI_Init(xMode, workMode);
    
    if(xMode == OSPI_8X_MODE)
    {
        GD25LX256E_Octal_Init(transferMode, OSPI_TX_OUT_DELAY_HALF_HCLK);
    }
    
    NORFLASH_Reset();
    
    HAL_SimpleDelay(50000);
                    
    
    //获取Norflash 设备唯一序列号
    lu32_ID = NORFLASH_Read_Identification();
    NORFLASH_Read_Unique_ID(unique_id_buff);
  
    printfS("GD25LX256 Info:\r\n");
    printfS("Identification  : 0x%08X \r\n", lu32_ID);  
    printfS("Unique ID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
                         unique_id_buff[0], unique_id_buff[1], unique_id_buff[2], unique_id_buff[3],
                         unique_id_buff[4], unique_id_buff[5], unique_id_buff[6], unique_id_buff[7],
                         unique_id_buff[8], unique_id_buff[9], unique_id_buff[10], unique_id_buff[11],
                         unique_id_buff[12], unique_id_buff[13], unique_id_buff[14], unique_id_buff[15]);

    /* Erase Chip */
    NORFLASH_EraseChip();
    
    NORFLASH_Read_Data(RxBuffer, 0, XSPI_BUFF_LEN);

    for (i = 0; i < XSPI_BUFF_LEN; i++)
    {
        if (RxBuffer[i] != 0xFF) 
        {
            lu32_ErrCount++;
        } 
    }

    if (lu32_ErrCount) 
        printfS("Erase Chip Fail!!! \r\n");
    else 
        printfS("Erase Chip Success!!! \r\n");

    /* Clear Error Count */
    lu32_ErrCount = 0;
    
    for (i = 0; i < XSPI_BUFF_LEN; i++)
    {
        TxBuffer[i] = i;
    }
    
    switch (NORFLASH_Handle.Init.XMode)
    {
        case OSPI_1X_MODE: 
        {
            NORFLASH_PageProgram(TxBuffer, 0x1000, XSPI_BUFF_LEN);
            NORFLASH_PageProgram(TxBuffer, 0x1100, XSPI_BUFF_LEN);
            NORFLASH_PageProgram(TxBuffer, 0x1200, XSPI_BUFF_LEN);
            NORFLASH_PageProgram(TxBuffer, 0x1300, XSPI_BUFF_LEN);
            
            NORFLASH_Read_Data(RxBuffer, 0x1000, XSPI_BUFF_LEN);
            
            for (i = 0; i < XSPI_BUFF_LEN; i++)
            {
                if (TxBuffer[i] != RxBuffer[i]) 
                {
                    lu32_ErrCount++;
                    printfS("[%d]: W[%02x] != R[%02x]\r\n", i, TxBuffer[i], RxBuffer[i]);
                } 
            }
            
            if (lu32_ErrCount) 
                printfS("OSPI_1X_MODE test is Fail!!! \r\n");
            else 
                printfS("OSPI_1X_MODE test is Success!!! \r\n");
        }break;

        case OSPI_8X_MODE: 
        {
            if(GD25LX256E_Octal.DTRMode == OSPI_DTRM_STR)
            {  
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8000, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8100, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8200, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8300, XSPI_BUFF_LEN);

                NORFLASH_Read_Octal_Output(RxBuffer, 0x8000, XSPI_BUFF_LEN);
                
                for (i = 0; i < XSPI_BUFF_LEN; i++)
                {
                    if (TxBuffer[i] != RxBuffer[i]) 
                    {
                        lu32_ErrCount++;
                        printfS("[%d]: W[%02x] != R[%02x]\r\n", i, TxBuffer[i], RxBuffer[i]);
                    } 
                }

                if (lu32_ErrCount) 
                    printfS("8X STR SPI Mode is Fail!!! \r\n");
                else 
                    printfS("8X STR SPI Mode is Success!!! \r\n");

                memset(RxBuffer, 0 , XSPI_BUFF_LEN);
                NORFLASH_Read_Octal_IO_STR(RxBuffer, 0x8000, XSPI_BUFF_LEN);
                
                for (i = 0; i < XSPI_BUFF_LEN; i++)
                {
                    if (TxBuffer[i] != RxBuffer[i]) 
                    {
                        lu32_ErrCount++;
                        printfS("[%d]: W[%02x] != R[%02x]\r\n", i, TxBuffer[i], RxBuffer[i]);
                    } 
                }

                if (lu32_ErrCount) 
                    printfS("NORFLASH_Read_Octal_IO is Fail!!! \r\n");
                else 
                    printfS("NORFLASH_Read_Octal_IO is Success!!! \r\n");

                memset(RxBuffer, 0 , XSPI_BUFF_LEN);
                NORFLASH_Read_Octal_IO_FourByte(RxBuffer, 0x8000, XSPI_BUFF_LEN);
                
                for (i = 0; i < XSPI_BUFF_LEN; i++)
                {
                    if (TxBuffer[i] != RxBuffer[i]) 
                    {
                        lu32_ErrCount++;
                    } 
                }

                if (lu32_ErrCount) 
                    printfS("NORFLASH_Read_Octal_IO_FourByte is Fail!!! \r\n");
                else 
                    printfS("NORFLASH_Read_Octal_IO_FourByte is Success!!! \r\n");
          
            }            
            else if(GD25LX256E_Octal.DTRMode == OSPI_DTRM_DTR)
            {
                printfS("8X DTR SPI Mode\r\n");
                __OSPI_SET_8X_STR_MODE();
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8000, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8100, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8200, XSPI_BUFF_LEN);
                NORFLASH_PageProgram_Octal(TxBuffer, 0x8300, XSPI_BUFF_LEN);
                
                memset(DTR_Buff, 0x0000, 16);
                NORFLASH_Read_Octal_IO_DTR(DTR_Buff, 0x8000, 16);
                
                for (i = 0; i < 16; i++)
                {
                    DTR_Data = (TxBuffer[2*i] + (uint16_t)(TxBuffer[2*i+1] << 8));
                    
                    if (DTR_Data != DTR_Buff[i]) 
                    {
                        lu32_ErrCount++;
                        printfS("[%d]: W[%04x] != R[%04x]\r\n", i, DTR_Data, DTR_Buff[i]);
                    } 
                }

                if (lu32_ErrCount) 
                    printfS("8X DTR SPI Mode is Fail!!! \r\n");
                else 
                    printfS("8X DTR SPI Mode is Success!!! \r\n");

            }
            
            
        }break;

        default: break; 
    }
}

//5AH Read Serial Flash Discoverable Parameter
void GD25LX256E_ReadSerialFlashDiscoverableParameter(void)
{
    uint8_t unique_id_buff[16]={0}, ParameterBuff[4];
    uint32_t lu32_ID;

    GD25LX256E_OSPI_Init(OSPI_1X_MODE, OSPI_WORK_MODE_0);
    
    NORFLASH_Reset();
    
    HAL_SimpleDelay(50000);
                    
    
    //获取Norflash 设备唯一序列号
    lu32_ID = NORFLASH_Read_Identification();
    NORFLASH_Read_Unique_ID(unique_id_buff);
  
    printfS("GD25LX256 Info:\r\n");
    printfS("Identification  : 0x%08X \r\n", lu32_ID);  
    printfS("Unique ID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
                         unique_id_buff[0], unique_id_buff[1], unique_id_buff[2], unique_id_buff[3],
                         unique_id_buff[4], unique_id_buff[5], unique_id_buff[6], unique_id_buff[7],
                         unique_id_buff[8], unique_id_buff[9], unique_id_buff[10], unique_id_buff[11],
                         unique_id_buff[12], unique_id_buff[13], unique_id_buff[14], unique_id_buff[15]);
    
    NORFLASH_ReadSerialFlashDiscoverableParameter(0x00000000, ParameterBuff,4);
    printfS("[0x0]24bit-DiscoverableParameter: %02X %02X %02X %02X \r\n", \
            ParameterBuff[0], ParameterBuff[1], ParameterBuff[2], ParameterBuff[3]);

    memset(ParameterBuff, 0 , 4);
    NORFLASH_ReadSerialFlashDiscoverableParameter(0x00000004, ParameterBuff,4);
    printfS("[0x4]24bit-DiscoverableParameter: %02X %02X %02X %02X \r\n", \
            ParameterBuff[0], ParameterBuff[1], ParameterBuff[2], ParameterBuff[3]);
    
    memset(ParameterBuff, 0 , 4);
    NORFLASH_ReadSerialFlashDiscoverableParameter_32BitAddr(0x00000000, ParameterBuff,4);
    printfS("[0x0]32bit-DiscoverableParameter: %02X %02X %02X %02X \r\n", \
            ParameterBuff[0], ParameterBuff[1], ParameterBuff[2], ParameterBuff[3]);

    memset(ParameterBuff, 0 , 4);
    NORFLASH_ReadSerialFlashDiscoverableParameter_32BitAddr(0x00000004, ParameterBuff,4);
    printfS("[0x4]32bit-DiscoverableParameter: %02X %02X %02X %02X \r\n", \
            ParameterBuff[0], ParameterBuff[1], ParameterBuff[2], ParameterBuff[3]);

}

//设置易失配置寄存器，配置成STR_OPI或者DTR_OPI模式测试
void GD25LX256E_VolatileRegsiter_STR_DTR_OPI_Test(void)
{
    uint32_t lu32_ID, lu32_ErrCount = 0;
    uint8_t temp;
    uint16_t DTR_Buff[16];
    uint16_t DTR_Data;
    uint8_t TxBuffer[XSPI_BUFF_LEN], RxBuffer[XSPI_BUFF_LEN];
    uint32_t i;

    GD25LX256E_OSPI_Init(OSPI_8X_MODE, OSPI_WORK_MODE_0);

    GD25LX256E_Octal_Init(OSPI_DTRM_STR, OSPI_TX_OUT_DELAY_HALF_HCLK);  

    NORFLASH_Reset();

    temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister(READ_VOLATILE_CONFIG_REGISTER, 0);
    printfS("Volatile Configuration Register I/O mode = 0x%x\r\n", temp);
                    
    //获取Norflash 设备唯一序列号
    lu32_ID = NORFLASH_Read_Identification();
    printfS("GD25LX256 Identification  : 0x%08X \r\n", lu32_ID);  

    NORFLASH_EraseChip();


    for (i = 0; i < XSPI_BUFF_LEN; i++)
    {
        TxBuffer[i] = i;
    }   
   
    NORFLASH_PageProgram_Octal(TxBuffer, 0x8000, XSPI_BUFF_LEN);
    NORFLASH_PageProgram_Octal(TxBuffer, 0x8100, XSPI_BUFF_LEN);
    NORFLASH_PageProgram_Octal(TxBuffer, 0x8200, XSPI_BUFF_LEN);
    NORFLASH_PageProgram_Octal(TxBuffer, 0x8300, XSPI_BUFF_LEN);
    
    memset(DTR_Buff, 0x0000, 16);
    NORFLASH_Read_Octal_IO_DTR(DTR_Buff, 0x8000, 16);
    
    for (i = 0; i < 16; i++)
    {
        DTR_Data = (TxBuffer[2*i] + (uint16_t)(TxBuffer[2*i+1] << 8));
        
        if (DTR_Data != DTR_Buff[i]) 
        {
           printfS("DTR_Data = 0x%04x, DTR_Buff[%d] = 0x%04x\r\n", DTR_Data, i, DTR_Buff[i]); 
           lu32_ErrCount++;
        } 
    }

    if (lu32_ErrCount) 
        printfS("Write & Read is Fail!!! \r\n");
    else 
        printfS("Write & Read is Success!!! \r\n");
    
    //设置易失配置寄存器 I/O Mode为 STR OPI
    printfS("************************************************\r\n");  
    printfS("Set Volatile Configuration Register I/O mode Octal STR with DQS\r\n");
    NORFLASH_WriteNonVolatile_VolatileStatusRegister(WRITE_VOLATILE_CONFIG_REGISTER, 0, REGISTER_STR_WITH_DQS);
    
    HAL_SimpleDelay(50000);
    temp = 0;
    temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8STR(READ_VOLATILE_CONFIG_REGISTER, 0);
    printfS("[x8STR]Volatile Configuration Register I/O mode = 0x%x\r\n", temp);
    lu32_ID = 0;
    lu32_ID = NORFLASH_Read_Identification_x8STR();
    printfS("[x8STR]GD25LX256 Identification  : 0x%08X \r\n", lu32_ID); 

    memset(RxBuffer, 0 , XSPI_BUFF_LEN);
    NORFLASH_Read_Octal_IO_STR_OPI(RxBuffer, 0x8000, XSPI_BUFF_LEN);

    for (i = 0; i < XSPI_BUFF_LEN; i++)
    {
    if (TxBuffer[i] != RxBuffer[i]) 
    {
        lu32_ErrCount++;
    } 
    }

    if (lu32_ErrCount) 
    printfS("NORFLASH_Read_Octal_IO_STR_OPI is Fail!!! \r\n");
    else 
    printfS("NORFLASH_Read_Octal_IO_STR_OPI is Success!!! \r\n"); 
    printfS("************************************************\r\n");  

    NORFLASH_Reset_8xMode();

    //设置易失配置寄存器 I/O Mode为 DTR OPI
    printfS("************************************************\r\n");  
    printfS("Set Volatile Configuration Register I/O mode Octal DTR with DQS\r\n");
    NORFLASH_WriteNonVolatile_VolatileStatusRegister(WRITE_VOLATILE_CONFIG_REGISTER, 0, REGISTER_DTR_WITH_DQS);
    
    HAL_SimpleDelay(50000);
    temp = 0;
    temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR(READ_VOLATILE_CONFIG_REGISTER, 0);
    printfS("[x8DTR]Volatile Configuration Register I/O mode = 0x%x\r\n", temp);
    lu32_ID = 0;
    lu32_ID = NORFLASH_Read_Identification_x8DTR();
    printfS("[x8DTR]GD25LX256 Identification  : 0x%08X \r\n", lu32_ID); 

    memset(DTR_Buff, 0x0000, 16);
    NORFLASH_Read_Octal_IO_DTR_OPI(DTR_Buff, 0x8000, 16);
    
    for (i = 0; i < 16; i++)
    {
        DTR_Data = (TxBuffer[2*i] + (uint16_t)(TxBuffer[2*i+1] << 8));
        
        if (DTR_Data != DTR_Buff[i]) 
        {
            printfS("DTR_Data = 0x%04x, DTR_Buff[%d] = 0x%04x\r\n", DTR_Data, i, DTR_Buff[i]);
            lu32_ErrCount++;
        } 
    }

    if (lu32_ErrCount) 
        printfS("NORFLASH_Read_Octal_IO_DTR_OPI is Fail!!! \r\n");
    else 
        printfS("NORFLASH_Read_Octal_IO_DTR_OPI is Success!!! \r\n");

    NORFLASH_Reset_8xMode();
   printfS("************************************************\r\n");     
}

/************************************************************FIFO End**************************************************************/
/************************************************************FIFO End**************************************************************/
/************************************************************FIFO End**************************************************************/



/************************************************************DTR Memory Start**************************************************************/
/************************************************************DTR Memory Start**************************************************************/
/************************************************************DTR Memory Start**************************************************************/
//Memory内存模式访问初始化
void GD25LX256E_OSPI_DTR_Memory_Init(void)
{
	xSPI_Memeory_Handle.WriteCmd = OCTAL_INPUT_FAST_PROGRAM;
	xSPI_Memeory_Handle.ReadCmd = FOUR_BYTE_DTR_OCTAL_IO_FAST_READ;      
	xSPI_Memeory_Handle.AlterByte = 0x00;  
    
    xSPI_Memeory_Handle.WrapSize = 0;
    xSPI_Memeory_Handle.BurstLen = 0;                     //突发长度（Burst Length）32Byte
    xSPI_Memeory_Handle.HyperXspiLC1 = 0;                    //Hyperbus或xSPI模式下，RWDS为一时的LC周期数 1个CLK
    xSPI_Memeory_Handle.HyperXspiLc0 = 15;                     //Hyperbus或xSPI模式下，RWDS为零时的LC周期数 16个CLK   
    
	xSPI_Memeory_Handle.HyperBurstType = 0;               //[27]突发是Linear的还是Wrapped的
	xSPI_Memeory_Handle.DataMode = 0;                      //[26:25]数据模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AlterByteMode = 0;                //[24:23]交替字节模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AddrMode = 0;                      //[22:21]地址模式，八线模式时，此位无效
	xSPI_Memeory_Handle.InstrMode = 0;                     //[20:19]指令模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AddrWidth = MEMOACC1_ADDR_WIDTH_32;                     //[18:17]地址长度（Hyper必须为32 bit）
	xSPI_Memeory_Handle.DummyCycleSize = 0;               //[16:12]dummy周期长度（Hyper/xSPI无效）
	xSPI_Memeory_Handle.ReadDummyByteEnable = MEMOACC1_READ_DUMMY_ENABLE;                       //[11]读操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	xSPI_Memeory_Handle.WriteummyByteEnable = 0;                       //[10]写操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	xSPI_Memeory_Handle.AlterByteSize = 0;                //[8:7]]交替字节长度 8 16 24 32，八线模式时，此位无效			
	xSPI_Memeory_Handle.ReadAlterByteEnable = 0;                       //[6]读操作交替字节使能位，八线模式时，此位无效
	xSPI_Memeory_Handle.WriteAlterByteEnable = 0;                       //[5]写操作交替字节使能位，八线模式时，此位无效
	xSPI_Memeory_Handle.SendInstrOnce = 0;                    //[4]仅发送指令一次
    xSPI_Memeory_Handle.ContinuousModeEnable = MEMOACC1_CON_ENABLE;                    //[3]连续读使能
	xSPI_Memeory_Handle.CsTimeoutEnable = 0;                     //[2]连续模式下CS拉高等待功能不使能
	HAL_OSPI_MemoryInit(&NORFLASH_Handle, &xSPI_Memeory_Handle);                         //[1]存储器访问使能    
}

//设置GD25LX256E为DTR OPI
void GD25LX256E_OSPI_DTR_OPI_Config(void)
{
    uint8_t temp;
    uint32_t lu32_ID; 
    //printfS("************************************************\r\n");     
    //设置易失配置寄存器 I/O Mode为 DTR OPI
    //printfS("Set Volatile Configuration Register I/O mode Octal DTR with DQS\r\n");
    NORFLASH_WriteNonVolatile_VolatileStatusRegister_FourAddr(WRITE_VOLATILE_CONFIG_REGISTER, 0, REGISTER_DTR_WITH_DQS);
    
    HAL_SimpleDelay(50000);
    temp = 0;
    temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR(READ_VOLATILE_CONFIG_REGISTER, 0);
    //printfS("[x8DTR]Volatile Configuration Register I/O mode = 0x%x\r\n", temp);
    //lu32_ID = 0;
    //lu32_ID = NORFLASH_Read_Identification_x8DTR();
    //printfS("[x8DTR]GD25LX256 Identification  : 0x%08X \r\n", lu32_ID); 
    
    // temp = 0;
    // temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR_1(READ_VOLATILE_CONFIG_REGISTER, 6);
    // printfS("[x8DTR]Volatile Configuration Register XIP Mode = 0x%x\r\n", temp);
    //printfS("************************************************\r\n"); 
}

/****************** DTR Memory Test Start ******************/
//DTR Memory模式8bit读写
void GD25LX256E_OSPI_DTR_Memory_8bit_Write_Read(void)
{
    volatile uint8_t *pMem8;
    uint16_t i, err_count = 0;
    uint8_t writeBuf[XSPI_BUFF_LEN];
    uint8_t readBuf[XSPI_BUFF_LEN]; 
    
//    NORFLASH_Handle.Instance->RX_CTL |= BIT8;
//    NORFLASH_Handle.Instance->RX_CTL = (NORFLASH_Handle.Instance->RX_CTL & (~(0xf << 24))) | (0x0 << 24);
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
    }
    //写
    printfS("Memory write in 8-bit mode...\r\n"); 
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        *(pMem8 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS); 
    /* Wait write End */   
    NORFLASH_WaitNotInProgress_DTR_OPI();
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    //读
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    printfS("Memory read in 8-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_BUFF_LEN);
    for( i = 0; i < XSPI_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem8 + i);
    }
	//比对
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 8-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 8-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");  
}
//DTR Memory模式8bit写16bit读
void GD25LX256E_OSPI_DTR_Memory_8bit_Write_16bitRead(void)
{
    volatile uint8_t *pMem8;
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint8_t writeBuf[XSPI_BUFF_LEN];
    uint16_t readBuf[XSPI_HALF_BUFF_LEN]; 
    uint16_t DTR_Data;   
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
    }
    
    //写
    printfS("Memory write in 8-bit mode...\r\n"); 
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        *(pMem8 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    //读
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    printfS("Memory read in 16-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_HALF_BUFF_LEN);
    for( i = 0; i < XSPI_HALF_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem16 + i);
    }
	//比对
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        DTR_Data = (writeBuf[2*i] + (uint16_t)(writeBuf[2*i] << 8));
        if(readBuf[i] != DTR_Data)
        {
            printfS("[%d]: Write = 0x%04x, Read = 0x%04x\r\n", i, DTR_Data, readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 8-bit Write 16bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 8-bit Write 16bit read  Success!!!\r\n");
    }
    printfS("************************************************\r\n"); 
    
    
}
//DTR Memory模式16bit写8bit读
void GD25LX256E_OSPI_DTR_Memory_16bit_Write_8bitRead(void)
{
    volatile uint8_t *pMem8;
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint16_t writeBuf[XSPI_HALF_BUFF_LEN];
    uint8_t readBuf[XSPI_BUFF_LEN]; 
    uint16_t DTR_Data; 
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
    }
    //写
    printfS("Memory write in 16-bit mode...\r\n"); 
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        *(pMem16 + i) = writeBuf[i];
    }  
    
    HAL_Delay(2000);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    //读
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    printfS("Memory read in 8-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_BUFF_LEN);
    for( i = 0; i < XSPI_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem8 + i);
    }
	//比对
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        DTR_Data = (readBuf[2*i] + (uint16_t)(readBuf[2*i+1] << 8));
        if(DTR_Data != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%04x, Read = 0x%04x\r\n", i, writeBuf[i], DTR_Data); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 16-bit Write 8bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 16-bit Write 8bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n"); 
    
    
    
}
//DTR Memory模式16bit读写
void GD25LX256E_OSPI_DTR_Memory_16bit_Write_Read(void)
{    
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint16_t writeBuf[XSPI_HALF_BUFF_LEN];
    uint16_t readBuf[XSPI_HALF_BUFF_LEN];
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x2000);
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        writeBuf[i] = (XSPI_DATA_16BIT + i);
    }
    //写
    printfS("Memory write in 16-bit mode...\r\n"); 
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        *(pMem16 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    //读
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x2000);
    printfS("Memory read in 16-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_HALF_BUFF_LEN);
    for( i = 0; i < XSPI_HALF_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem16 + i);
    }
	//比对
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%04x, Read = 0x%04x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 16-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 16-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");  

    NORFLASH_Handle.Instance->RX_CTL &= ~BIT8; 
    
}
//DTR Memory模式32bit读写
void GD25LX256E_OSPI_DTR_Memory_32bit_Write_Read(void)
{
    volatile uint32_t *pMem32;
    uint16_t i, err_count = 0;
    uint32_t writeBuf[XSPI_QUARTER_UFF_LEN];
    uint32_t readBuf[XSPI_QUARTER_UFF_LEN]; 
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    pMem32 = (volatile uint32_t *) (OSPI1_MEM_BASE_ADDR + 0x3000);
    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        writeBuf[i] = (XSPI_DATA_32BIT + i);
    }
    //写
    printfS("Memory write in 32-bit mode...\r\n"); 
    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        *(pMem32 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    //读
    pMem32 = (volatile uint32_t *) (OSPI1_MEM_BASE_ADDR + 0x3000);
    printfS("Memory read in 32-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_QUARTER_UFF_LEN);
    for( i = 0; i < XSPI_QUARTER_UFF_LEN; i ++)
    {
        readBuf[i] = *(pMem32 + i);
    }
	//比对
    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%08x, Read = 0x%08x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 32-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 32-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");  
    
    
}
//DTR Memory模式读写测试
void GD25LX256E_OSPI_DTR_Memory_Test(void)
{
    GD25LX256E_OSPI_Flash_Init();
    GD25LX256E_OSPI_DTR_OPI_Config();
    GD25LX256E_OSPI_DTR_Memory_Init();
    
    GD25LX256E_OSPI_DTR_Memory_16bit_Write_Read();
    GD25LX256E_OSPI_DTR_Memory_32bit_Write_Read();

    GD25LX256E_OSPI_DTR_Memory_8bit_Write_Read();
    GD25LX256E_OSPI_DTR_Memory_8bit_Write_16bitRead();
    GD25LX256E_OSPI_DTR_Memory_16bit_Write_8bitRead();   
}

/************************************************************DTR Memory End**************************************************************/
/************************************************************DTR Memory End**************************************************************/
/************************************************************DTR Memory End**************************************************************/




/************************************************************STR Memory Start**************************************************************/
/************************************************************STR Memory Start**************************************************************/
/************************************************************STR Memory Start**************************************************************/
//Memory内存模式访问初始化
void GD25LX256E_OSPI_STR_Memory_Init(void)
{
    //HAL_OSPI_RxSampleDelay(&NORFLASH_Handle, OSPI_SAMPLE_SHIFT_2HCLK);
	xSPI_Memeory_Handle.WriteCmd = OCTAL_INPUT_FAST_PROGRAM;
	xSPI_Memeory_Handle.ReadCmd = OCTAL_IO_FAST_READ;      
	xSPI_Memeory_Handle.AlterByte = 0x00;  
    
    xSPI_Memeory_Handle.WrapSize = 0;
    xSPI_Memeory_Handle.BurstLen = 0;                     //突发长度（Burst Length）32Byte
    xSPI_Memeory_Handle.HyperXspiLC1 = 0;                    //Hyperbus或xSPI DTR模式下，RWDS为一时的LC周期数 1个CLK
    xSPI_Memeory_Handle.HyperXspiLc0 = 0;                     //Hyperbus或xSPI DTR模式下，RWDS为零时的LC周期数 1个CLK   
    
	xSPI_Memeory_Handle.HyperBurstType = 0;               //[27]突发是Linear的还是Wrapped的
	xSPI_Memeory_Handle.DataMode = 0;                      //[26:25]数据模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AlterByteMode = 0;                //[24:23]交替字节模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AddrMode = 0;                      //[22:21]地址模式，八线模式时，此位无效
	xSPI_Memeory_Handle.InstrMode = 0;                     //[20:19]指令模式，八线模式时，此位无效
	xSPI_Memeory_Handle.AddrWidth = MEMOACC1_ADDR_WIDTH_24;                     //[18:17]地址长度（Hyper必须为32 bit）
	xSPI_Memeory_Handle.DummyCycleSize = MEMOACC1_DUMMY_CYCLE_16;               //[16:12]dummy周期长度（Hyper/xSPI DTR无效）
	xSPI_Memeory_Handle.ReadDummyByteEnable = MEMOACC1_READ_DUMMY_ENABLE;                       //[11]读操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	xSPI_Memeory_Handle.WriteummyByteEnable = 0;                       //[10]写操作空指令字节使能（Hyper必须为1）,dummy使能位,有dummy时必须置1
	xSPI_Memeory_Handle.AlterByteSize = 0;                //[8:7]]交替字节长度 8 16 24 32，八线模式时，此位无效			
	xSPI_Memeory_Handle.ReadAlterByteEnable = 0;                       //[6]读操作交替字节使能位，八线模式时，此位无效
	xSPI_Memeory_Handle.WriteAlterByteEnable = 0;                       //[5]写操作交替字节使能位，八线模式时，此位无效
	xSPI_Memeory_Handle.SendInstrOnce = 0;                    //[4]仅发送指令一次
    xSPI_Memeory_Handle.ContinuousModeEnable = MEMOACC1_CON_ENABLE;                    //[3]连续读使能
	xSPI_Memeory_Handle.CsTimeoutEnable = 0;                     //[2]连续模式下CS拉高等待功能不使能
	HAL_OSPI_MemoryInit(&NORFLASH_Handle, &xSPI_Memeory_Handle);   
}

//设置GD25LX256E为STR OPI
void GD25LX256E_OSPI_STR_OPI_Config(void)
{
    uint8_t temp;
    uint32_t lu32_ID; 
    printfS("************************************************\r\n");     
    //设置易失配置寄存器 I/O Mode为 STR OPI 
    printfS("Set Volatile Configuration Register I/O mode Octal STR with DQS\r\n");
    NORFLASH_WriteNonVolatile_VolatileStatusRegister(WRITE_VOLATILE_CONFIG_REGISTER, 0, REGISTER_STR_WITH_DQS);
    
    HAL_SimpleDelay(50000);
    temp = 0;
    temp = NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8STR(READ_VOLATILE_CONFIG_REGISTER, 0);
    printfS("[x8STR]Volatile Configuration Register I/O mode = 0x%x\r\n", temp);
    lu32_ID = 0;
    lu32_ID = NORFLASH_Read_Identification_x8STR();
    printfS("[x8STR]GD25LX256 Identification  : 0x%08X \r\n", lu32_ID); 
    
    printfS("************************************************\r\n"); 
}

/****************** STR Memory Test Start ******************/
//DTR Memory模式8bit读写
void GD25LX256E_OSPI_STR_Memory_8bit_Write_Read(void)
{
    volatile uint8_t *pMem8;
    uint16_t i, err_count = 0;
    uint8_t writeBuf[XSPI_BUFF_LEN];
    uint8_t readBuf[XSPI_BUFF_LEN]; 
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);
    /* Write Enable*/
    NORFLASH_WriteEnable_STR_OPI();
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能

    HAL_Delay(500);
    
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);

    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        writeBuf[i] = i;
    }
    
    //写
    printfS("Memory write in 8-bit mode...\r\n"); 
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        *(pMem8 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);  
    /* Wait write End */  
    NORFLASH_WaitNotInProgress_STR_OPI();
    /* Write Disable*/
    NORFLASH_WriteDisable_STR_OPI();
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    HAL_Delay(500);
    //读
    pMem8 = (volatile uint8_t *) (OSPI1_MEM_BASE_ADDR + 0x1000);
    printfS("Memory read in 8-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_BUFF_LEN);
    for( i = 0; i < XSPI_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem8 + i);
    }
	//比对
    for(i = 0; i < XSPI_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%02x, Read = 0x%02x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 8-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 8-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n"); 
    
    
}

//STR Memory模式16bit读写
void GD25LX256E_OSPI_STR_Memory_16bit_Write_Read(void)
{    
    volatile uint16_t *pMem16;
    uint16_t i, err_count = 0;
    uint16_t writeBuf[XSPI_HALF_BUFF_LEN];
    uint16_t readBuf[XSPI_HALF_BUFF_LEN]; 

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();   
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);

    /* Write Enable*/
    NORFLASH_WriteEnable_STR_OPI();
    
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x2000);

    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        writeBuf[i] = (XSPI_DATA_16BIT + i);
    }
    
    //写
    printfS("Memory write in 16-bit mode...\r\n"); 
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        *(pMem16 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 

    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    

    /* Write Disable*/
    NORFLASH_WriteDisable_STR_OPI();

    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能

    //读
    pMem16 = (volatile uint16_t *) (OSPI1_MEM_BASE_ADDR + 0x2000);
    printfS("Memory read in 16-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_HALF_BUFF_LEN);
    for( i = 0; i < XSPI_HALF_BUFF_LEN; i ++)
    {
        readBuf[i] = *(pMem16 + i);
    }
	//比对
    for(i = 0; i < XSPI_HALF_BUFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%04x, Read = 0x%04x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 16-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 16-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");   
    
}
//STR Memory模式32bit读写
void GD25LX256E_OSPI_STR_Memory_32bit_Write_Read(void)
{
    volatile uint32_t *pMem32;
    uint16_t i, err_count = 0;
    uint32_t writeBuf[XSPI_QUARTER_UFF_LEN];
    uint32_t readBuf[XSPI_QUARTER_UFF_LEN]; 
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止
    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);

    /* Write Enable*/
    NORFLASH_WriteEnable_STR_OPI();
    
    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);

    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能
    
    pMem32 = (volatile uint32_t *) (OSPI1_MEM_BASE_ADDR + 0x3000);

    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        writeBuf[i] = (XSPI_DATA_32BIT + i);
    }
    
    //写
    printfS("Memory write in 32-bit mode...\r\n"); 
    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        *(pMem32 + i) = writeBuf[i];
    }    

    HAL_Delay(2000);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_DISABLE); //[1]存储器访问禁止 

    GD25LX256E_OSPI_CS_Init(XSPI_SOFEWARE_CS);    

    /* Write Disable*/
    NORFLASH_WriteDisable_STR_OPI();

    GD25LX256E_OSPI_CS_Init(XSPI_HARDWARE_CS);
    HAL_OSPI_MemoryEnableDisable(&NORFLASH_Handle, MEMOACC1_MEMORY_ENABLE); //[1]存储器访问使能

    //读
    pMem32 = (volatile uint32_t *) (OSPI1_MEM_BASE_ADDR + 0x3000);
    printfS("Memory read in 32-bit mode...\r\n"); 
    memset(readBuf, 0 , XSPI_QUARTER_UFF_LEN);
    for( i = 0; i < XSPI_QUARTER_UFF_LEN; i ++)
    {
        readBuf[i] = *(pMem32 + i);
    }
	//比对
    for(i = 0; i < XSPI_QUARTER_UFF_LEN; i++)
    {
        if(readBuf[i] != writeBuf[i])
        {
            printfS("[%d]: Write = 0x%08x, Read = 0x%08x\r\n", i, writeBuf[i], readBuf[i]); 
            err_count++;
        }
    }
    
    if(err_count)
    {
        printfS("GD25LX256E Memory 32-bit read fail!!!\r\n");
    }
    else
    {
        printfS("GD25LX256E Memory 32-bit read Success!!!\r\n");
    }
    printfS("************************************************\r\n");  
    
    
}
//STR Memory模式读写测试
void GD25LX256E_OSPI_STR_Memory_Test(void)
{
    GD25LX256E_OSPI_Flash_Init();
    GD25LX256E_OSPI_STR_OPI_Config();
    GD25LX256E_OSPI_STR_Memory_Init();

    GD25LX256E_OSPI_STR_Memory_8bit_Write_Read();
    GD25LX256E_OSPI_STR_Memory_16bit_Write_Read();
    GD25LX256E_OSPI_STR_Memory_32bit_Write_Read();
}



