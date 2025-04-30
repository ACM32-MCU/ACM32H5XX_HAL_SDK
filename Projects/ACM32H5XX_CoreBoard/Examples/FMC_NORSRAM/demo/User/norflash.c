/***************************************************************
*Norflash: SST39VF400A
****************************************************************/


#include "norflash.h"




#define NORFLASH_SIZE               0x1000

#define BLOCKERASETIMEOUT           ((uint32_t)0x000A0000)
#define CHIPERASETIMEOUT            ((uint32_t)0x30000000) 
#define PROGRAMTIMEOUT              ((uint32_t)0x00001400)


/******************************************************************************
* @brief : Reads NOR memory's Manufacturer and Device Code.
* @param : NOR_ID: pointer to a NOR_IDTypeDef structure which will hold
*                  the Manufacturer and Device Code.
* @return: none
******************************************************************************/
void FMC_NOR_ReadID(NOR_IDTypeDef* NOR_ID)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x0090);

    NOR_ID->Manufacturer_Code = *(vu16 *) NOR_ADDR_SHIFT(0x0000, 1);
    NOR_ID->Device_Code = *(vu16 *) NOR_ADDR_SHIFT(0x0001, 1);
}


/******************************************************************************
* @brief : Returns the NOR operation status.
* @param : NOR_ID: Timeout: NOR progamming Timeout.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_GetStatus(uint32_t Timeout)
{ 
    uint32_t index=0;
    uint16_t val1 = 0x00, val2 = 0x00;
    FMC_NOR_StatusTypeDef status = FMC_NOR_STATUS_ONGOING; 
    uint32_t timeout = Timeout;
    /* Poll on NOR memory Ready/Busy signal ------------------------------------*/
    /* Get the NOR memory operation status -------------------------------------*/
    while((Timeout != 0x00) && (status != FMC_NOR_STATUS_SUCCESS))
    {
        Timeout--;
        /* Read DQ6 and DQ5 */
        val1 = *(volatile uint16_t *)(FMC_BANK_NOR);
        val2 = *(volatile uint16_t *)(FMC_BANK_NOR);
        /* If DQ6 did not toggle between the two reads then return NOR_Success */
        if((val1 & NOR_MASK_STATUS_DQ6) == (val2 & NOR_MASK_STATUS_DQ6)) 
        {
            return FMC_NOR_STATUS_SUCCESS;
        }
        val1 = *(vu16 *)(FMC_BANK_NOR);
        val2 = *(vu16 *)(FMC_BANK_NOR);

        if((val1 & NOR_MASK_STATUS_DQ6) == (val2 & NOR_MASK_STATUS_DQ6)) 
        {
            return FMC_NOR_STATUS_SUCCESS;
        }
    }
    if(Timeout == 0x00)
    {
        status = FMC_NOR_STATUS_TIMEOUT;
        printfS("Timeout\r\n");
    }

    /* Return the operation status */
    return (status);
}

/******************************************************************************
* @brief : Erases the specified Nor memory block.
* @param : BlockAddr: address of the block to erase.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_EraseBlock(uint32_t BlockAddr)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x0080);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE((FMC_BANK_NOR + BlockAddr), 0x50);

    return (FMC_NOR_GetStatus(BLOCKERASETIMEOUT));
}

/******************************************************************************
* @brief : Erases the entire chip.
* @param : None.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_EraseChip(void)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x0080);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x0010);

    return (FMC_NOR_GetStatus(CHIPERASETIMEOUT));
}

/******************************************************************************
* @brief : Writes a word to the NOR memory.
* @param : WriteAddr : NOR memory internal address to write to.
* @param : Data : Data to write.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_WriteWord(uint32_t WriteAddr, uint32_t Data)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00A0);
    (*(__IO uint32_t *)((FMC_BANK_NOR + WriteAddr)) = (Data));
    
    return (FMC_NOR_GetStatus(PROGRAMTIMEOUT));
}

/******************************************************************************
* @brief : Writes a half-word to the NOR memory.
* @param : WriteAddr : NOR memory internal address to write to.
* @param : Data : Data to write.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_WriteHalfWord(uint32_t WriteAddr, uint16_t Data)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00A0);
    NOR_WRITE((FMC_BANK_NOR + WriteAddr), Data);

    return (FMC_NOR_GetStatus(PROGRAMTIMEOUT));
}

/******************************************************************************
* @brief : Writes a Byte to the NOR memory.
* @param : WriteAddr : NOR memory internal address to write to.
* @param : Data : Data to write.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_WriteByte(uint32_t WriteAddr, uint8_t Data)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00A0);
    
    (*(__IO uint8_t *)((FMC_BANK_NOR + WriteAddr)) = (Data));

    return (FMC_NOR_GetStatus(PROGRAMTIMEOUT));
}

/******************************************************************************
* @brief : Writes a half-word buffer to the FMC NOR memory. 
* @param : pBuffer : pointer to buffer. 
* @param : WriteAddr : NOR memory internal address from which the data will be written.
* @param : NumHalfwordToWrite : number of Half words to write.
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
    FMC_NOR_StatusTypeDef status = FMC_NOR_STATUS_ONGOING;

    do
    {
        /* Transfer data to the memory */
        status = FMC_NOR_WriteHalfWord(WriteAddr, *pBuffer++);
        WriteAddr = WriteAddr + 2;
        NumHalfwordToWrite--;
    }
    while((status == FMC_NOR_STATUS_SUCCESS) && (NumHalfwordToWrite != 0));

    return (status); 
}

/******************************************************************************
* @brief : Writes a half-word buffer to the FMC NOR memory. This function 
*           must be used only with S29GL128P NOR memory.
* @param : pBuffer : pointer to buffer. 
* @param : WriteAddr : NOR memory internal address from which the data will be written.
* @param : NumHalfwordToWrite: number of Half words to write.
*           The maximum allowed value is 32 Half words (64 bytes).
* @return: FMC_NOR_StatusTypeDef:The returned value can be: FMC_NOR_STATUS_SUCCESS, NOR_ERROR 
*                           or FMC_NOR_STATUS_TIMEOUT
******************************************************************************/
FMC_NOR_StatusTypeDef FMC_NOR_ProgramBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{       
    uint32_t lastloadedaddress = 0x00;
    uint32_t currentaddress = 0x00;
    uint32_t endaddress = 0x00;

    /* Initialize variables */
    currentaddress = WriteAddr;
    endaddress = WriteAddr + NumHalfwordToWrite - 1;
    lastloadedaddress = WriteAddr;

    /* Issue unlock command sequence */
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA);

    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055); 

    /* Write Write Buffer Load Command */
    NOR_WRITE(NOR_ADDR_SHIFT(WriteAddr, 1), 0x0025);
    NOR_WRITE(NOR_ADDR_SHIFT(WriteAddr, 1), (NumHalfwordToWrite - 1));

    /* Load Data into NOR Buffer */
    while(currentaddress <= endaddress)
    {
        /* Store last loaded address & data value (for polling) */
        lastloadedaddress = currentaddress;

        NOR_WRITE(NOR_ADDR_SHIFT(currentaddress, 1), *pBuffer++);
        currentaddress += 1; 
    }

    NOR_WRITE(NOR_ADDR_SHIFT(lastloadedaddress, 1), 0x29);

    return(FMC_NOR_GetStatus(PROGRAMTIMEOUT));
}


/******************************************************************************
* @brief : Reads a byte from the NOR memory. 
* @param : ReadAddr : NOR memory internal address to read from.
* @return: byte read from the NOR memory.
******************************************************************************/
uint8_t FMC_NOR_ReadByte(uint32_t ReadAddr)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA); 
    NOR_WRITE(NOR_ADDR_SHIFT(0x002AAA, 1), 0x0055);  
    NOR_WRITE((FMC_BANK_NOR + ReadAddr), 0x00F0 );

    return (*(vu8 *)((FMC_BANK_NOR + ReadAddr)));
 
}

/******************************************************************************
* @brief : Reads a half-word from the NOR memory. 
* @param : ReadAddr : NOR memory internal address to read from.
* @return: Half-word read from the NOR memory.
******************************************************************************/
uint16_t FMC_NOR_ReadHalfWord(uint32_t ReadAddr)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA); 
    NOR_WRITE(NOR_ADDR_SHIFT(0x002AAA, 1), 0x0055);  
    NOR_WRITE((FMC_BANK_NOR + ReadAddr), 0x00F0 );

    return (*(vu16 *)((FMC_BANK_NOR + ReadAddr)));
}
  
/******************************************************************************
* @brief : Reads a word from the NOR memory. 
* @param : ReadAddr : NOR memory internal address to read from.
* @return: word read from the NOR memory.
******************************************************************************/  
uint32_t FMC_NOR_ReadWord(uint32_t ReadAddr)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA); 
    NOR_WRITE(NOR_ADDR_SHIFT(0x002AAA, 1), 0x0055);  
    NOR_WRITE((FMC_BANK_NOR + ReadAddr), 0x00F0 );

    return (*(vu32 *)((FMC_BANK_NOR + ReadAddr)));
}

/******************************************************************************
* @brief : Reads a block of data from the FMC NOR memory.
* @param : pBuffer : pointer to the buffer that receives the data read 
*                    from the NOR memory.
* @param : ReadAddr : NOR memory internal address to read from.
* @param : NumHalfwordToRead : number of Half word to read.
* @return: None.
******************************************************************************/ 
void FMC_NOR_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x05555, 1), 0x00AA);
    NOR_WRITE(NOR_ADDR_SHIFT(0x02AAA, 1), 0x0055);
    NOR_WRITE((FMC_BANK_NOR + ReadAddr), 0x00F0);

    for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) /* while there is data to read */
    {
        /* Read a Halfword from the NOR */
        *pBuffer++ = *(vu16 *)((FMC_BANK_NOR + ReadAddr));
        ReadAddr = ReadAddr + 2; 
    }  
}

/******************************************************************************
* @brief : Returns the NOR memory to Read mode.
* @param : None.
* @return: None.
******************************************************************************/ 
FMC_NOR_StatusTypeDef FMC_NOR_ReturnToReadMode(void)
{
    NOR_WRITE(FMC_BANK_NOR, 0x00F0);
    return (FMC_NOR_STATUS_SUCCESS);
}

/******************************************************************************
* @brief : Returns the NOR memory to Read mode and resets the errors in
*                  the NOR memory Status Register.
* @param : None.
* @return: FMC_NOR_STATUS_SUCCESS.
******************************************************************************/ 
FMC_NOR_StatusTypeDef FMC_NOR_Reset(void)
{
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA); 
    NOR_WRITE(NOR_ADDR_SHIFT(0x002AAA, 1), 0x0055); 
    NOR_WRITE(FMC_BANK_NOR, 0x00F0);

    return (FMC_NOR_STATUS_SUCCESS);
}


/******************************************************************************
* @brief : Initialize the IO control of Norflash.
* @param : none
* @return: none
******************************************************************************/
static void FMC_NORFLASH_GPIO_Init(void)
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
       
    GPIO_InitStructure.Pin = FMC_A19_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A19_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A20_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A20_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A21_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A21_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A22_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A22_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A23_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A23_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A24_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A24_GPIO_PORT, &GPIO_InitStructure);
 

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
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;       //AF10
    GPIO_InitStructure.Pin = FMC_NORFLASH_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_NORFLASH_CS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;       
    GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_OE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_OE_GPIO_PORT, &GPIO_InitStructure);   

 
}


/******************************************************************************
* @brief : Initialize FMC NORSRAM.
* @param : none
* @return: none
******************************************************************************/
void FMC_NORFLASH_Init(uint32_t Width)
{
    FMC_NORSRAMInitTypeDef hnorflash_Init;
    FMC_NORSRAMTimingInitTypeDef  hnorflash_Timing;
    
    
    /* 配置FMC接口相关的 GPIO*/
    FMC_NORFLASH_GPIO_Init();

 
    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    hnorflash_Init.NSBank                 = FMC_NORSRAM_BANK1;//use CS1 control，Addr=0x60000000   
    hnorflash_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
    hnorflash_Init.MemoryType             = FMC_MEMORY_TYPE_NOR;
    hnorflash_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hnorflash_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
    hnorflash_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hnorflash_Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
    hnorflash_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_BEFOREWAITSTATE;
    hnorflash_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;
    hnorflash_Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
    hnorflash_Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
    hnorflash_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hnorflash_Init.WriteMode              = FMC_WRITE_ASY_MODE;
    hnorflash_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;

    /* NOR device configuration */
    
    /* TAS: Address Setup Time 0ns */
    hnorflash_Timing.AddressSetupTime       = 0;
    /* TAH: Address Hold Time 30ns */
    hnorflash_Timing.AddressHoldTime        = 8;
    /* TDS: Data Setup Time */
    hnorflash_Timing.DataSetupTime          = 8;
    hnorflash_Timing.BusTurnAroundDuration  = 0;
    hnorflash_Timing.CLKDivision            = 0;
    hnorflash_Timing.DataLatency            = 0;
    hnorflash_Timing.AccessMode             = FMC_ACCESS_MODE_B;//nor flash

    HAL_FMC_NORSRAM_Init(&hnorflash_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hnorflash_Timing, hnorflash_Init.NSBank);
}

/******************************************************************************
* @brief : 8/16/32-bit write and read.
* @param : none
* @return: none
******************************************************************************/
void Nor_Write_Read_Test(void)
{
    uint32_t temp=0,i=0,j=0,errortime=0;
    
    printfS("Start Nor_Write_Read_Test test!\r\n"); 
    
    FMC_NOR_EraseBlock(0);

    for(i=0;i<NORFLASH_SIZE/2;i++)
    {
        j++;
        if(j>0xffff)
        {        
            j=0;
        }
        FMC_NOR_WriteHalfWord(2*i, j); 
    }


    j=0;
    for(i=0;i<NORFLASH_SIZE/2;i++)
    {
        j++;
        if(j>0xffff) 
        {
            j=0;
        }
        temp = FMC_NOR_ReadHalfWord(2*i);
        if(temp!=j)
        {    
            errortime++;
            printfS("Addr=%0.8x,W=%0.8x,R=%0.8x,\r\n",2*i,j,temp); 
        }
    }

    if(errortime==0)
    {
        printfS("Nor_Write_Read_Test Pass!\r\n"); 
    }
    else
    {
        printfS("Nor_Write_Read_Test Error!\r\n"); 
    }
    printfS("******************************************\r\n\r\n");  
}

/******************************************************************************
* @brief : 8/16/32-bit alternate write read.
* @param : none
* @return: none
******************************************************************************/
static void Nor_32168Write_Read_Test(void)
{
    uint32_t temp=0,i=0,j=0,errortime=0,test_addr=0;
    
    printfS("Start Nor_32168Write_Read_Test test!\r\n"); 
    
    //写入8位，用16/32读出
    FMC_NOR_EraseBlock(test_addr);

    FMC_NOR_WriteByte(test_addr, 0xAA);

    temp = FMC_NOR_ReadHalfWord(test_addr);
    if(temp!=0x00aa) 
    {
        errortime++; 
    }
    temp = FMC_NOR_ReadWord(test_addr);
    if(temp!=0xffff00aa)
    {
        errortime++; 
    }
    
    
    //写入16位，用16/32读出
    FMC_NOR_EraseBlock(test_addr);


    FMC_NOR_WriteHalfWord(test_addr, 0x1122);

    temp = FMC_NOR_ReadHalfWord(test_addr);
    if(temp!=0x1122)
    {
        errortime++; 
    }
    temp = FMC_NOR_ReadWord(test_addr);
    if(temp!=0xffff1122)
    {
        errortime++; 
    }
    
    //写入32位，用16/32读出
    FMC_NOR_EraseBlock(test_addr);  

    FMC_NOR_WriteWord(test_addr, 0x66778899);  


    temp = FMC_NOR_ReadHalfWord(test_addr);
    if(temp!=0x8899)
    {
        errortime++; 
    }
    temp = FMC_NOR_ReadHalfWord((test_addr+2));
    if(temp!=0xffff)
    {
        errortime++;
    }    
    temp = FMC_NOR_ReadWord(test_addr);
    if(temp!=0xffff8899)
    {
        errortime++; 
    }
    
    if(errortime==0)
    {
        printfS("Nor_32168Write_Read_Test Pass!\r\n"); 
    }
    else
    {
        printfS("Nor_32168Write_Read_Test Error!\r\n"); 
    }
    printfS("******************************************\r\n\r\n");    
}

/******************************************************************************
* @brief : Read and write in memcpy mode.
* @param : none
* @return: none
******************************************************************************/
static void Nor_Memcpy_Test(void)
{
    uint16_t RxBuffer[8];
    uint32_t temp=0,i=0,j=0,errortime=0,test_addr=0; 
    uint32_t uwReadAddr;  
    
    printfS("Start Nor_Memcpy_Test test!\r\n");
    
    FMC_NOR_EraseBlock(test_addr);
    
    FMC_NOR_WriteHalfWord(0, 0x1122); 
    FMC_NOR_WriteHalfWord(2, 0x3344); 
    FMC_NOR_WriteHalfWord(4, 0x5566); 
    FMC_NOR_WriteHalfWord(6, 0x7788); 

    /* Send read data command */
    NOR_WRITE(NOR_ADDR_SHIFT(0x005555, 1), 0x00AA); 
    NOR_WRITE(NOR_ADDR_SHIFT(0x002AAA, 1), 0x0055);  
    NOR_WRITE((FMC_BANK_NOR + test_addr), 0x00F0 );
    memcpy(RxBuffer,(uint32_t *)(FMC_BANK_NOR + test_addr),8); 
    if(RxBuffer[0]!=0x1122) 
    {
        errortime++;
    }
    if(RxBuffer[1]!=0x3344)
    {
        errortime++; 
    }
    if(RxBuffer[2]!=0x5566)
    {
        errortime++; 
    }
    if(RxBuffer[3]!=0x7788)
    {
        errortime++; 
    }     
    if(errortime==0)
    {
        printfS("Nor_Memcpy_Test Pass!\r\n"); 
    }
    else
    {
        printfS("Nor_Memcpy_Test Error!\r\n");    
    }
    printfS("******************************************\r\n\r\n");    
}

/******************************************************************************
* @brief : Odd address read and write.
* @param : none
* @return: none
******************************************************************************/
static void Nor_OddAddr_Test(void)
{
    uint32_t i=0,j=0,errortime=0,test_addr=0; 
    uint16_t temp=0; 
    
    printfS("Start Nor_OddAddr_Test test!\r\n");
    
    /*奇地址读写*/
    FMC_NOR_EraseBlock(test_addr);
    

    FMC_NOR_WriteHalfWord(test_addr, 0x1122);
    
    FMC_NOR_WriteHalfWord(test_addr+2, 0x3344);

    temp = FMC_NOR_ReadHalfWord(test_addr);
    if(temp!=0x1122)
    {    
        errortime++; 
    }
    
    temp = FMC_NOR_ReadHalfWord(test_addr+1);
    if(temp!=0x4411)
    {
        errortime++; 
    }
    
    temp = FMC_NOR_ReadHalfWord(test_addr+2);
    if(temp!=0x3344)
    {
        errortime++; 
    }
     
    if(errortime==0)
    {
        printfS("Nor_OddAddr_Test Pass!\r\n"); 
    }
    else
    {
        printfS("Nor_OddAddr_Test Error!\r\n");    
    }
    printfS("******************************************\r\n\r\n");    

}





void FMC_Norflash_Test(void)
{  
    NOR_IDTypeDef NOR_Id;
    uint16_t temp = 0;
    uint32_t test_addr = 0;
    uint8_t i = 0;
    
    FMC_NORFLASH_Init(FMC_NORSRAM_MEM_BUS_WIDTH_16);

    FMC_NOR_ReadID(&NOR_Id);
    printfS("Manufacturer Code = 0x%04x\r\n",NOR_Id.Manufacturer_Code);
    printfS("Device Code1 = 0x%04x\r\n",NOR_Id.Device_Code);
   
    FMC_NOR_ReturnToReadMode();  

    //擦
    FMC_NOR_EraseBlock(0);
    
    FMC_NOR_WriteHalfWord((test_addr+0), 0xAAAA);
    FMC_NOR_WriteHalfWord((test_addr+2), 0xCCCC);
    FMC_NOR_WriteHalfWord((test_addr+4), 0x3333);
    FMC_NOR_WriteHalfWord((test_addr+6), 0x5555);
    
    temp = FMC_NOR_ReadWord(test_addr+0);
    printfS("Addr = 0x%08x, Data = 0x%0x(?=0xAAAA)\r\n", NOR_READ_ADDR_SHIFT((test_addr+6),WRITE_READ_SHIFT), temp); 
    
    temp = FMC_NOR_ReadWord(test_addr+2);
    printfS("Addr = 0x%08x, Data = 0x%0x(?=0xCCCC)\r\n", NOR_READ_ADDR_SHIFT((test_addr+8),WRITE_READ_SHIFT), temp); 
   
    temp = FMC_NOR_ReadWord(test_addr+4);
    printfS("Addr = 0x%08x, Data = 0x%0x(?=0x3333)\r\n", NOR_READ_ADDR_SHIFT((test_addr+10),WRITE_READ_SHIFT), temp);   
    
    temp = FMC_NOR_ReadWord(test_addr+6);
    printfS("Addr = 0x%08x, Data = 0x%0x(?=0x5555)\r\n", NOR_READ_ADDR_SHIFT((test_addr+12),WRITE_READ_SHIFT), temp);       
    printfS("******************************************\r\n\r\n");

    Nor_Write_Read_Test();
    
    
    Nor_32168Write_Read_Test();


    Nor_Memcpy_Test();


    Nor_OddAddr_Test();
     

      
}






