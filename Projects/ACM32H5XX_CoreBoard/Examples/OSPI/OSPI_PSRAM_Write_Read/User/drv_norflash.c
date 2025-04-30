

#include "drv_norflash.h"


OSPI_HandleTypeDef NORFLASH_Handle;


/*********************************************************************************
* function    :  __OSPI_CS_Release
* Description :  CS Release
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_CS_Release(void)
{
#ifdef SOFTWARE_CS_ENABLE
    if(NORFLASH_Handle.Instance == OSPI1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    }
    else if(NORFLASH_Handle.Instance == OSPI2)
    {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
    } 
#endif
}

/*********************************************************************************
* function    :  __OSPI_CS_Select
* Description :  CS Select
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_CS_Select(void)
{
#ifdef SOFTWARE_CS_ENABLE
    if(NORFLASH_Handle.Instance == OSPI1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    }
    else if(NORFLASH_Handle.Instance == OSPI2)
    {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
    } 
#endif    
}

/*********************************************************************************
* function    :  __OSPI_SET_1X_MODE
* Description :  OSPI 1X Mode
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_SET_1X_MODE(void)
{
    HAL_OSPI_WireConfig(&NORFLASH_Handle, OSPI_1X_MODE);
} 

/*********************************************************************************
* function    :  __OSPI_SET_2X_MODE
* Description :  OSPI 2X Mode
* Input       :  
* Output      : 
**********************************************************************************/                      
void __OSPI_SET_2X_MODE(void)
{
    HAL_OSPI_WireConfig(&NORFLASH_Handle, OSPI_2X_MODE);
}

/*********************************************************************************
* function    :  __OSPI_SET_4X_MODE
* Description :  OSPI 4X Mode
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_SET_4X_MODE(void)
{
    HAL_OSPI_WireConfig(&NORFLASH_Handle, OSPI_4X_MODE);
}

/*********************************************************************************
* function    :  __OSPI_SET_8X_MODE
* Description :  OSPI 8X Mode
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_SET_8X_MODE(void)
{
    HAL_OSPI_WireConfig(&NORFLASH_Handle, OSPI_8X_MODE);
}

/*********************************************************************************
* function    :  __OSPI_SET_STR_MODE
* Description :  OSPI 8X STR Mode
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_SET_8X_STR_MODE(void)
{
    HAL_OSPI_TransmitRateMode(&NORFLASH_Handle, OSPI_DTRM_STR);
}

/*********************************************************************************
* function    :  __OSPI_SET_8X_DTR_MODE
* Description :  OSPI 8X DTR Mode
* Input       :  
* Output      : 
**********************************************************************************/
void __OSPI_SET_8X_DTR_MODE(void)
{
    HAL_OSPI_TransmitRateMode(&NORFLASH_Handle, OSPI_DTRM_DTR);
}

/*********************************************************************************
* function    :  __OSPI_SET_FIFO_MODE
* Description :  OSPI FIFO wite Mode
* Input       :  FIFO Write mode
* Output      : 
**********************************************************************************/
void __OSPI_SET_FIFO_MODE(uint32_t mode)
{
    HAL_OSPI_FifoWriteMode(&NORFLASH_Handle, mode);
    HAL_OSPI_FifoReadMode(&NORFLASH_Handle, mode);
}


/*********************************************************************************
* function    :  __OSPI_Read_Data
* Description :  Read data
* Input       :  Pointer to data buffer
* Input       :  Amount of data to receive
* Output      : 
**********************************************************************************/
void __OSPI_Read_Data(uint8_t *pData, uint32_t size)
{
    HAL_OSPI_Receive(&NORFLASH_Handle, pData, size, 0);
}

/*********************************************************************************
* function    :  __OSPI_Write_Data
* Description :  Write data
* Input       :  Pointer to data buffer
* Input       :  Amount of data to be sent
* Output      : 
**********************************************************************************/
void __OSPI_Write_Data(uint8_t *pData, uint32_t size)
{
    HAL_OSPI_Transmit(&NORFLASH_Handle, pData, size, 0);
}

/******************************************************************************
*@brief : transmits an amount of data in FIFO Half Word mode before receiving an amount of data through FIFO Byte       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*         the configuration information for the specified OSPI.
*@param : pTxData : Pointer to transmit data buffer
*@param : pRxData : Pointer to recieve data buffer
*@param : TxSize  : Amount of data to be sent
*@param : RxSize  : Amount of data to be receive
*@param : Timeout : sent and receive timeout
*@return: HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef NORFLASH_TransmitByHalfWord_Recieve(OSPI_HandleTypeDef *hospi, uint16_t *pTxData, \
                            uint8_t *pRxData, uint32_t TXSize, uint32_t RXSize, uint32_t Timeout)
{
    __IO uint32_t uiTimeout;
    HAL_StatusTypeDef Status = HAL_OK;

    /* Check SPI Parameter */
    assert_param (IS_OSPI_ALL_INSTANCE(hospi->Instance));
    
    if ((pTxData == NULL)||(pRxData == NULL))    return HAL_ERROR;
    
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET); 
    
    /* Set Data Size */
    hospi->Instance->BATCH = (TXSize * 2);

    /* Tx Enable */
    hospi->Instance->TX_CTL |= OSPI_TX_CTL_TX_EN;

    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    uiTimeout = Timeout;
    
    while(TXSize)
    {
        /* Wait Tx FIFO Not Full */       
        while(hospi->Instance->STATUS & OSPI_STATUS_TX_FIFO_FULL)
        {
            if(uiTimeout)
            {
                uiTimeout--;
                if (uiTimeout == 0)
                {
                    /* Clear Batch Done Flag  */
                    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
                    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

                    /* Tx Disable */
                    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);

                    /* Transmit End */
                    HAL_OSPI_CS_Release(hospi);

                    Status = HAL_TIMEOUT;
                    return Status;
                }
            }
        }            
        hospi->Instance->DAT = *pTxData++;
        TXSize--;
        uiTimeout = Timeout;
    }

    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_TX_BATCH_DONE));
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);
    /* Tx Disable */
    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);

    /* Set Data Size */
    hospi->Instance->BATCH = RXSize;

    /* Rx Enable */
    hospi->Instance->RX_CTL |= OSPI_RX_CTL_RX_EN;

    /* recv reStart */
    HAL_OSPI_CS_Select(hospi);
    
    uiTimeout = Timeout;
    
    while(RXSize)
    {
        /* Wait Rx FIFO Not Empty */     
        while (hospi->Instance->STATUS & OSPI_STATUS_RX_FIFO_EMPTY)
        {
            if(uiTimeout)
            {
                uiTimeout--;
                if (uiTimeout == 0)
                {
                    Status = HAL_TIMEOUT;
                    goto End;
                }
            }
        }
        *pRxData++ = hospi->Instance->DAT;
        RXSize--;
        uiTimeout = Timeout;
    }

    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_RX_BATCH_DONE));
    
End:
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_RX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);
    /* Rx Disable */
    hospi->Instance->RX_CTL &= (~OSPI_RX_CTL_RX_EN);

    /* Transmit End */
    HAL_OSPI_CS_Release(hospi);

    return Status;
}


/*********************************************************************************
* function    :  NORFLASH_WriteEnable
* Description :  Write Enable
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteEnable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_ENABLE;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}

void NORFLASH_WriteEnable_DTR_OPI(void)
{
    uint16_t lu16_DataBuffer[1];
    
    lu16_DataBuffer[0] = WRITE_ENABLE;
    lu16_DataBuffer[0] += WRITE_ENABLE << 8;

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
    
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);
    
    /* CS Realse */
    __OSPI_CS_Release();
}

void NORFLASH_WriteEnable_STR_OPI(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_ENABLE;

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE); 
    
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    
    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_WriteDisable
* Description :  Write Disable 
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteDisable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_DISABLE;

    /* CS Select */
    __OSPI_CS_Select();
     /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();    
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}


void NORFLASH_WriteDisable_DTR_OPI(void)
{
    uint16_t lu16_DataBuffer[1];
    
    lu16_DataBuffer[0] = WRITE_DISABLE;
    lu16_DataBuffer[0] += WRITE_DISABLE << 8;

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
    
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);
    
    /* CS Realse */
    __OSPI_CS_Release();
}

void NORFLASH_WriteDisable_STR_OPI(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_DISABLE;

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE); 
    
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function    :  NORFLASH_WriteNonVolatile_VolatileStatusRegister
* Description :  Write Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteNonVolatile_VolatileStatusRegister(uint8_t cmd, uint8_t regAddr, uint8_t data)
{
    uint8_t lu8_DataBuffer[5];
    //uint8_t flag;
    
    lu8_DataBuffer[0] = cmd;
    lu8_DataBuffer[1] = 0x00;
    lu8_DataBuffer[2] = 0x00;
    lu8_DataBuffer[3] = (uint8_t)(regAddr  & 0xFF);
    lu8_DataBuffer[4] = data;
    
    /* Write Enable*/
    NORFLASH_WriteEnable();
    
    //flag = NORFLASH_Read_Register(READ_STATUS_REGISTER);
    //printfS("Flag Status Register = 0x%x \r\n", flag);
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 5);
    /* CS Realse */
    __OSPI_CS_Release();
    
    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
    
    /* Write Disable*/
    NORFLASH_WriteDisable();
}


void NORFLASH_WriteNonVolatile_VolatileStatusRegister_FourAddr(uint8_t cmd, uint8_t regAddr, uint8_t data)
{
    uint8_t lu8_DataBuffer[6];
    //uint8_t flag;
    
    lu8_DataBuffer[0] = cmd;
    lu8_DataBuffer[1] = 0x00;
    lu8_DataBuffer[2] = 0x00;
    lu8_DataBuffer[3] = 0x00;
    lu8_DataBuffer[4] = (uint8_t)(regAddr  & 0xFF);
    lu8_DataBuffer[5] = data;
    
    
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
    
    /* Write Enable*/
    NORFLASH_WriteEnable();
    
    //flag = NORFLASH_Read_Register(READ_STATUS_REGISTER);
    //printfS("Flag Status Register = 0x%x \r\n", flag);
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 6);
    /* CS Realse */
    __OSPI_CS_Release();
    
    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
    
    /* Write Disable*/
    NORFLASH_WriteDisable();
}

/*********************************************************************************
* function    :  NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8STR
* Description :  Write Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8STR(uint8_t cmd, uint8_t regAddr, uint8_t data)
{
    uint8_t lu8_DataBuffer[5];
    
    lu8_DataBuffer[0] = cmd;
    lu8_DataBuffer[1] = 0x00;
    lu8_DataBuffer[2] = 0x00;
    lu8_DataBuffer[3] = (uint8_t)(regAddr  & 0xFF);
    lu8_DataBuffer[4] = data;
    
    /* Write Enable*/
    NORFLASH_WriteEnable();
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);  
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 5);
    /* CS Realse */
    __OSPI_CS_Release();
    
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
    
    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
    
    /* Write Disable*/
    NORFLASH_WriteDisable();
}

/*********************************************************************************
* function    :  NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8DTR
* Description :  Write Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8DTR(uint8_t cmd, uint8_t regAddr, uint8_t data)
{
    uint16_t WriteBuffer[4];
    
    WriteBuffer[0] = (uint16_t)cmd;         
    WriteBuffer[0] += (uint16_t)cmd << 8; 
    
    WriteBuffer[1] = ((0x00 >> 24) & 0xff);//A3
    WriteBuffer[1] += ((0x00 >> 16) & 0xff) << 8;//A2
    
    WriteBuffer[2] = ((0x00 >> 8) & 0xff);//A1
    WriteBuffer[2] += (regAddr & 0xff) << 8;//A0 
    
    WriteBuffer[3] = ((data >> 8) & 0xff);
    WriteBuffer[3] += (data & 0xff) << 8;//CR
    
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();
    
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);
    
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, WriteBuffer, 4, 0);
    
    /* CS Realse */
    __OSPI_CS_Release();    

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress_DTR_OPI();    
    
}

/*********************************************************************************
* function    :  NORFLASH_WriteNonVolatile_VolatileStatusRegister
* Description :  read Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
uint8_t NORFLASH_ReadNonVolatile_VolatileStatusRegister(uint8_t cmd, uint8_t regAddr)
{
    
    uint8_t lu8_DataBuffer[5];
    uint8_t read_buff[1];
    
    lu8_DataBuffer[0] = cmd;
    lu8_DataBuffer[1] = 0x00;
    lu8_DataBuffer[2] = 0x00;
    lu8_DataBuffer[3] = (uint8_t)(regAddr  & 0xFF);
    lu8_DataBuffer[4] = 0x00; //dummy  
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);

    // /* Recieve Status Register */
    // __OSPI_Read_Data(read_buff, 1);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 5, read_buff, 1, 0);
    
    __OSPI_SET_1X_MODE();
    
    /* CS Realse */
    __OSPI_CS_Release();
    
    return read_buff[0];
}

/*********************************************************************************
* function    :  NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8STR
* Description :  read Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
uint8_t NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8STR(uint8_t cmd, uint8_t regAddr)
{
    
    uint8_t lu8_DataBuffer[12];
    uint8_t read_buff[1];
    
    lu8_DataBuffer[0] = cmd;
    lu8_DataBuffer[1] = 0x00;
    lu8_DataBuffer[2] = 0x00;
    lu8_DataBuffer[3] = (uint8_t)(regAddr  & 0xFF);
    
    lu8_DataBuffer[4] = 0x00;//dummy 1  
    lu8_DataBuffer[5] = 0x00;//dummy 2  
    lu8_DataBuffer[6] = 0x00;//dummy 3  
    lu8_DataBuffer[7] = 0x00;//dummy 4  
    lu8_DataBuffer[8] = 0x00;//dummy 5  
    lu8_DataBuffer[9] = 0x00;//dummy 6  
    lu8_DataBuffer[10] = 0x00;//dummy 7  
    lu8_DataBuffer[11] = 0x00;//dummy 8    
    
    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);    
    
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 12);

    // /* Recieve Status Register */
    // __OSPI_Read_Data(read_buff, 1);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 12, read_buff, 1, 0);
    
    __OSPI_SET_1X_MODE();
    
    /* CS Realse */
    __OSPI_CS_Release();
    
    return lu8_DataBuffer[0];
}


/*********************************************************************************
* function    :  NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR
* Description :  read Volatile Status Register
* Input       :  
* Output      : 
**********************************************************************************/
uint16_t NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR(uint8_t cmd, uint8_t regAddr)
{
    uint16_t WriteBuffer[12];
    uint16_t ReadBuffer[1];
    
    WriteBuffer[0] = (uint16_t)cmd;         
    WriteBuffer[0] += (uint16_t)cmd << 8; 
    
    WriteBuffer[1] = ((0x00 >> 24) & 0xff);//A3
    WriteBuffer[1] += ((0x00 >> 16) & 0xff) << 8;//A2
    
    WriteBuffer[2] = ((0x00 >> 8) & 0xff);//A1
    WriteBuffer[2] += (regAddr & 0xff) << 8;//A0 
    
    WriteBuffer[3] = 0x0000;//dummy cycle 1
    WriteBuffer[4] = 0x0000;//dummy cycle 2
    WriteBuffer[5] = 0x0000;//dummy cycle 3
    WriteBuffer[6] = 0x0000;//dummy cycle 4
    WriteBuffer[7] = 0x0000;//dummy cycle 5
    WriteBuffer[8] = 0x0000;//dummy cycle 6
    WriteBuffer[9] = 0x0000;//dummy cycle 7
    WriteBuffer[10] = 0x0000;//dummy cycle 8     
    
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);
    
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&NORFLASH_Handle, WriteBuffer, 12, ReadBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();    
    
    return (ReadBuffer[0] & 0xff) ;
}


void NORFLASH_ReadSerialFlashDiscoverableParameter(uint32_t addr, uint8_t *pData, uint8_t len)
{
    uint8_t lu8_DataBuffer[5];
    
    lu8_DataBuffer[0] = 0x5A;
    lu8_DataBuffer[1] = (uint8_t)(addr >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(addr >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(addr >> 0  & 0xFF);
    lu8_DataBuffer[4] = 0x00;//dummy
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);

    // /* Recieve Status Register */
    // __OSPI_Read_Data(pData, len);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 5, pData, len, 0);
    
    __OSPI_SET_1X_MODE();
    
    /* CS Realse */
    __OSPI_CS_Release();
    
}

void NORFLASH_ReadSerialFlashDiscoverableParameter_32BitAddr(uint32_t addr, uint8_t *pData, uint8_t len)
{
    uint8_t lu8_DataBuffer[5];
    
    lu8_DataBuffer[0] = 0x5A;
    lu8_DataBuffer[1] = (uint8_t)(addr >> 24 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(addr >> 16  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(addr >> 8  & 0xFF);
    lu8_DataBuffer[4] = (uint8_t)(addr >> 0  & 0xFF);

    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode();
    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);

    // /* Recieve Status Register */
    // __OSPI_Read_Data(pData, len);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 5, pData, len, 0);
    
    __OSPI_SET_1X_MODE();
    
    /* CS Realse */
    __OSPI_CS_Release();
    
}


/*********************************************************************************
* function    :  NORFLASH_WriteEnableFourByteMode
* Description :  Write Enable 4-byte Mode
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteEnableFourByteMode(void)
{
    uint8_t lu8_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable();
    
    lu8_DataBuffer[0] = ENTER_FOUR_BYTE_ADDR_MODE;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
    
    /* Write Disable*/
    NORFLASH_WriteDisable();
}

void NORFLASH_WriteEnableFourByteMode_DTR_OPI(void)
{
    uint16_t lu16_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    
    lu16_DataBuffer[0] = ENTER_FOUR_BYTE_ADDR_MODE;
    lu16_DataBuffer[0] += ENTER_FOUR_BYTE_ADDR_MODE << 8;

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();
    
    /* Write Disable*/
    NORFLASH_WriteDisable_DTR_OPI();
}

void NORFLASH_WriteEnableFourByteMode_STR_OPI(void)
{
    uint8_t lu8_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable_STR_OPI();
    
    lu8_DataBuffer[0] = ENTER_FOUR_BYTE_ADDR_MODE;

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE); 
    
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_STR_OPI();
    
    /* Write Disable*/
    NORFLASH_WriteDisable_STR_OPI();
}


/*********************************************************************************
* function    :  NORFLASH_WriteDisableFourByteMode
* Description :  Write Disable 4-byte Mode
* Input       :  
* Output      : 
**********************************************************************************/
void NORFLASH_WriteDisableFourByteMode(void)
{
    uint8_t lu8_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable();
    
    lu8_DataBuffer[0] = EXIT_FOUR_BYTE_ADDR_MODE;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
}

void NORFLASH_WriteDisableFourByteMode_DTR_OPI(void)
{
    uint16_t lu16_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable_DTR_OPI();
    
    lu16_DataBuffer[0] = EXIT_FOUR_BYTE_ADDR_MODE;
    lu16_DataBuffer[0] += EXIT_FOUR_BYTE_ADDR_MODE << 8;

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();
}

void NORFLASH_WriteDisableFourByteMode_STR_OPI(void)
{
    uint8_t lu8_DataBuffer[1];
    
    /* Write Enable*/
    NORFLASH_WriteEnable_STR_OPI();
    
    lu8_DataBuffer[0] = EXIT_FOUR_BYTE_ADDR_MODE;

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE); 
    /* Send command */
    HAL_OSPI_Transmit(&NORFLASH_Handle, lu8_DataBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_STR_OPI();
}

/*********************************************************************************
* function   :  NORFLASH_WriteRegister
* Description :  Write Register
* Input       :  Register
* Output      : 
**********************************************************************************/
void NORFLASH_WriteRegister(uint8_t reg)
{
    uint8_t lu8_DataBuffer[2];

    lu8_DataBuffer[0] = WRITE_STATUS_REGISTER;
    lu8_DataBuffer[1] = reg;

    /* Write Enable */
    NORFLASH_WriteEnable();

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 2);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_WriteRegister_EX
* Description :  Write Register
* Input       :  Register1
* Input       :  Register2
* Output      : 
**********************************************************************************/
void NORFLASH_WriteRegister_EX(uint8_t reg1, uint8_t reg2)
{
    uint8_t lu8_DataBuffer[3];

    lu8_DataBuffer[0] = WRITE_STATUS_REGISTER;
    lu8_DataBuffer[1] = reg1;
    lu8_DataBuffer[2] = reg2;

    /* Write Enable */
    NORFLASH_WriteEnable();

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();    
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 3);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();
}


/*********************************************************************************
* function   :  NORFLASH_Read_Identification
* Description :  Read Identification
* Input       :  
* Output      :  Identification
**********************************************************************************/
uint32_t NORFLASH_Read_Identification(void)
{
    uint8_t lu8_DataBuffer[4];
    uint8_t read_buff[4];
    
    lu8_DataBuffer[0] = READ_ID;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();     
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    // /* Recieve Manufacture ID and Device ID */
    // __OSPI_Read_Data(read_buff, 4);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 1, read_buff, 4, 0);
    /* CS Realse */
    __OSPI_CS_Release();
    
    return ((uint32_t)read_buff[0] << 24 | (uint32_t)read_buff[1] << 16 | (uint32_t)read_buff[2] << 8 | (uint32_t)read_buff[3]);
}

/*********************************************************************************
* function   :  NORFLASH_Read_Identification_x8STR
* Description :  Read Identification
* Input       :  
* Output      :  Identification
**********************************************************************************/
uint32_t NORFLASH_Read_Identification_x8STR(void)
{
    uint8_t lu8_DataBuffer[9];
    uint8_t read_buff[4];
    
    lu8_DataBuffer[0] = READ_ID;
    lu8_DataBuffer[1] = 0x0000;//dummy cycle 1
    lu8_DataBuffer[2] = 0x0000;//dummy cycle 2
    lu8_DataBuffer[3] = 0x0000;//dummy cycle 3
    lu8_DataBuffer[4] = 0x0000;//dummy cycle 4
    lu8_DataBuffer[5] = 0x0000;//dummy cycle 5
    lu8_DataBuffer[6] = 0x0000;//dummy cycle 6
    lu8_DataBuffer[7] = 0x0000;//dummy cycle 7
    lu8_DataBuffer[8] = 0x0000;//dummy cycle 8  

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);
    
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 9);
    // /* Recieve Manufacture ID and Device ID */
    // __OSPI_Read_Data(read_buff, 4);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 9, read_buff, 4, 0);

    /* CS Realse */
    __OSPI_CS_Release();
    
    return ((uint32_t)read_buff[0] << 24 | (uint32_t)read_buff[1] << 16 | (uint32_t)read_buff[2] << 8 | (uint32_t)read_buff[3]);
}

/*********************************************************************************
* function   :  NORFLASH_Read_Identification_x8DTR
* Description :  Read Identification
* Input       :  
* Output      :  Identification
**********************************************************************************/
uint32_t NORFLASH_Read_Identification_x8DTR(void)
{
    uint16_t WriteBuffer[9];
    uint16_t ReadBuffer[4];
    
    WriteBuffer[0] = READ_ID;         
    WriteBuffer[0] += READ_ID << 8; 

    WriteBuffer[1] = 0x0000; 
    WriteBuffer[2] = 0x0000; 
    WriteBuffer[3] = 0x0000; 
    WriteBuffer[4] = 0x0000; 
    WriteBuffer[5] = 0x0000; 
    WriteBuffer[6] = 0x0000; 
    WriteBuffer[7] = 0x0000; 
    WriteBuffer[8] = 0x0000;  
    
    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
   
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&NORFLASH_Handle, WriteBuffer, 9, ReadBuffer, 2, 0);
    
    /* CS Realse */
    __OSPI_CS_Release();
    
    // printfS("%x,%x\r\n", ReadBuffer[0], ReadBuffer[1]);
    return ((uint32_t)ReadBuffer[0] << 16 | (uint32_t)ReadBuffer[1]);
}

/*********************************************************************************
* function   :  NORFLASH_Read_Manufacture_Device_ID
* Description :  Read Manufacture ID and Device ID
* Input       :  
* Output      :  Manufacture ID and Device ID
**********************************************************************************/
uint16_t NORFLASH_Read_Manufacture_Device_ID(void)
{
    uint8_t lu8_DataBuffer[4];
    uint8_t read_buff[2];
    
    lu8_DataBuffer[0] = READ_MID;
    lu8_DataBuffer[1] = 0;
    lu8_DataBuffer[2] = 0;
    lu8_DataBuffer[3] = 0;

    /* CS Select */
    __OSPI_CS_Select();
     /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();    
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    // /* Recieve Manufacture ID and Device ID */
    // __OSPI_Read_Data(read_buff, 2);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 4, read_buff, 2, 0);

    /* CS Realse */
    __OSPI_CS_Release();
    
    return ((uint16_t)read_buff[0] << 8 | (uint16_t)read_buff[1]);
}

/*********************************************************************************
* function   :  NORFLASH_Read_Unique_ID
* Description :  Read Unique ID
* Input       :  
* Output      :  Unique ID
**********************************************************************************/
void NORFLASH_Read_Unique_ID(uint8_t *uid)
{
    uint8_t lu8_DataBuffer[5];
    
    lu8_DataBuffer[0] = READ_UNIQUE_ID;
    lu8_DataBuffer[1] = 0;
    lu8_DataBuffer[2] = 0;
    lu8_DataBuffer[3] = 0;
    lu8_DataBuffer[4] = 0;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();     
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);
    // /* Recieve Manufacture ID and Device ID */
    // __OSPI_Read_Data(uid, 16);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 5, uid, 16, 0);

    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Read_RegisterS07_S00
* Description :  Read Status Register S07 ~ S00
* Input       :  
* Output      :  Status Register S07 ~ S00
**********************************************************************************/
uint8_t NORFLASH_Read_Register(uint8_t cmd)
{
    uint8_t lu8_DataBuffer[1];
    uint8_t read_buff[1];
    
    lu8_DataBuffer[0] = cmd;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();     
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    // /* Recieve Status Register */
    // __OSPI_Read_Data(read_buff, 1);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 1, read_buff, 1, 0);


    /* CS Realse */
    __OSPI_CS_Release();
    
    return read_buff[0];
}


uint8_t NORFLASH_Read_Register_DTR_OPI(uint8_t cmd)
{
    uint16_t lu16_DataBuffer[9], ReadBuffer[1];

    lu16_DataBuffer[0] = cmd;
    lu16_DataBuffer[0] += cmd << 8;

    lu16_DataBuffer[1] = 0x0000; 
    lu16_DataBuffer[2] = 0x0000; 
    lu16_DataBuffer[3] = 0x0000; 
    lu16_DataBuffer[4] = 0x0000; 
    lu16_DataBuffer[5] = 0x0000; 
    lu16_DataBuffer[6] = 0x0000; 
    lu16_DataBuffer[7] = 0x0000; 
    lu16_DataBuffer[8] = 0x0000;  
    
    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD); 
   
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 9, ReadBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();
    
    return (ReadBuffer[0] & 0xff);
}

uint8_t NORFLASH_Read_Register_STR_OPI(uint8_t cmd)
{
    uint8_t lu8_DataBuffer[9], ReadBuffer[1];

    lu8_DataBuffer[0] = cmd;

    lu8_DataBuffer[1] = 0x00; 
    lu8_DataBuffer[2] = 0x00; 
    lu8_DataBuffer[3] = 0x00; 
    lu8_DataBuffer[4] = 0x00; 
    lu8_DataBuffer[5] = 0x00; 
    lu8_DataBuffer[6] = 0x00; 
    lu8_DataBuffer[7] = 0x00; 
    lu8_DataBuffer[8] = 0x00;  
    
    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire STR Mode */
    __OSPI_SET_8X_STR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE); 
   
    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 9, ReadBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();
    
    return (ReadBuffer[0] & 0xff);
}

/*********************************************************************************
* function   :  NORFLASH_Read_RegisterS15_S08
* Description :  Read Status Register S15 ~ S08
* Input       :  
* Output      :  Status Register S15 ~ S08
**********************************************************************************/
uint8_t NORFLASH_Read_RegisterS15_S08(void)
{
    uint8_t lu8_DataBuffer[1];
    uint8_t read_buff[1];
    
    lu8_DataBuffer[0] = READ_STATUS_REGISTER_S15_S08;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();     
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    // /* Recieve Status Register S15 ~ S08 */
    // __OSPI_Read_Data(read_buff, 1);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 1, read_buff, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();
    
    return read_buff[0];
}

/*********************************************************************************
* function   :  NORFLASH_Read_Data
* Description :  Read Data
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Data(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = READ_DATA;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    
    /* CS Select */
    __OSPI_CS_Select();
     /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();    
    // /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 4, pu8_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Read_Dual_IO
* Description :  Dual Io Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Dual_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = DUAL_IO_FAST_READ;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);

    lu8_DataBuffer[0] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0xC8);

    /* Set SPI Work In 2 Wire Mode */
    __OSPI_SET_2X_MODE();

    // /* Send Address */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 4, pu8_Buffer, fu32_Length, 0);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  NORFLASH_Read_Dual_Output
* Description :  Dual Output Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Dual_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = DUAL_OUTPUT_FAST_READ;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0x00);
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    //__OSPI_Write_Data(lu8_DataBuffer, 5);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 5, 0);

    /* Set SPI Work In 2 Wire Mode */
    __OSPI_SET_2X_MODE();

    /* Recieve Data */
    __OSPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  NORFLASH_Read_Quad_IO
* Description :  Quad Io Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Quad_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[6];

    lu8_DataBuffer[0] = QUAD_IO_FAST_READ;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 1, 0);

    lu8_DataBuffer[0] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0x00);

    lu8_DataBuffer[4] = (0x00);
    lu8_DataBuffer[5] = (0x00);

    /* Set SPI Work In 4 Wire Mode */
    __OSPI_SET_4X_MODE();

    // /* Send Address */
    // __OSPI_Write_Data(lu8_DataBuffer, 6);
    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);
    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 6, pu8_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  NORFLASH_Read_Quad_Output
* Description :  Quad Output Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Quad_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[6];

    lu8_DataBuffer[0] = QUAD_OUTPUT_FAST_READ;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[4] = (0x00);
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 5, 0);

    /* Set SPI Work In 4 Wire Mode */
    __OSPI_SET_4X_MODE();

    /* Recieve Data */
    __OSPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  NORFLASH_PageProgram
* Description :  Page Program
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = PAGE_PROGARM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 4, 0);
    /* Send Data */
    __OSPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();
    
    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_PageProgram_Dual
* Description :  Quad Page Program 
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram_Dual(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = DUAL_PAGE_PROGARM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 4, 0);

    /* Set SPI Work In 2 Wire Mode */
    __OSPI_SET_2X_MODE();

    /* Send Data */
    __OSPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_PageProgram_Quad
* Description :  Quad Page Program 
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram_Quad(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = QUAD_PAGE_PROGRAM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 4, 0);

    /* Set SPI Work In 4 Wire Mode */
    __OSPI_SET_4X_MODE();

    /* Send Data */
    __OSPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_EraseSector
* Description :  Erease The specific Sector
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_EraseSector(uint32_t fu32_DataAddress)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = SECTOR_ERASE;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 4);
    /* CS Realse */
    __OSPI_CS_Release();
    
    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_EraseChip
* Description :  Erease The Whole Chip
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_EraseChip(void)
{
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = CHIP_ERASE;

    /* Write Enable */
    NORFLASH_WriteEnable();

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_QuadConfig
* Description :  Quad Function Config
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_QuadConfig(bool fb_Config)
{
    if (fb_Config == true) 
    {
        /* Set Quad Enable */
        NORFLASH_WriteRegister_EX(REGISTER_NULL, REGISTER_S15_S08_QE);
    }
    else 
    {
        /* Set Quad Disable */
        NORFLASH_WriteRegister_EX(REGISTER_NULL, REGISTER_NULL);
    }
}

/*********************************************************************************
* function   :  NORFLASH_WaitNotInProgress
* Description :  Wait IC Not in progress
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_WaitNotInProgress(void)
{   
    /* Wait IC Not Busy */
    while(NORFLASH_Read_Register(READ_STATUS_REGISTER) & REGISTER_S07_S00_WIP);
}

void NORFLASH_WaitNotInProgress_DTR_OPI(void)
{   
    /* Wait IC Not Busy */
    while(NORFLASH_Read_Register_DTR_OPI(READ_STATUS_REGISTER) & REGISTER_S07_S00_WIP);
}

void NORFLASH_WaitNotInProgress_STR_OPI(void)
{   
    /* Wait IC Not Busy */
    while(NORFLASH_Read_Register_STR_OPI(READ_STATUS_REGISTER) & REGISTER_S07_S00_WIP);
}

/*********************************************************************************
* function   :  NORFLASH_WaitNotBusy
* Description :  Wait IC Not Busy
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_WaitNotBusy(void)
{   
    /* Wait IC Not Busy */
    while(NORFLASH_Read_Register(READ_FLAG_STATUS_REGISTER) & REGISTER_FS7_RY_BY);
}

/*********************************************************************************
* function   :  NORFLASH_PowerDown
* Description :  
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PowerDown(void)
{ 
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = DEEP_POWER_DOWN;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Wakeup
* Description :  
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Wakeup(void)
{  
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = RELEASE_FORM_DEEP_POWER_DOWN;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Reset
* Description :  NORFLASH Reset
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Reset(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = ENABLE_RESET;

    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();


    lu8_DataBuffer[0] = RESET;
    
    /* CS Select */
    __OSPI_CS_Select();

    /* Send command */
    __OSPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Reset_8xMode
* Description :  NORFLASH Reset
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Reset_8xMode(void)
{
    uint16_t lu16_DataBuffer[1];

    lu16_DataBuffer[0] = ENABLE_RESET;
    lu16_DataBuffer[0] += ENABLE_RESET << 8;

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();
    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);

    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();


    lu16_DataBuffer[0] = RESET;
    lu16_DataBuffer[0] += RESET << 8;

    /* CS Select */
    __OSPI_CS_Select();
    /* Send command */
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 1, 0);

    /* CS Realse */
    __OSPI_CS_Release();
}

/*********************************************************************************
* function   :  NORFLASH_Read_Octal_Output
* Description :  Octal Output Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[6];

    lu8_DataBuffer[0] = OCTAL_OUTPUT_FAST_READ;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[4] = (0x00);
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 5);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 5, 0);

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);

    /* Recieve Data */
    __OSPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();
}




/*********************************************************************************
* function   :  NORFLASH_Read_Octal_IO_STR
* Description :  Octal IO Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_IO_STR(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[20];

    lu8_DataBuffer[0] = OCTAL_IO_FAST_READ;

    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
 
    
    lu8_DataBuffer[4] = 0x00;//dummy cycle 1
    lu8_DataBuffer[5] = 0x00;//dummy cycle 2
    lu8_DataBuffer[6] = 0x00;//dummy cycle 3
    lu8_DataBuffer[7] = 0x00;//dummy cycle 4
    lu8_DataBuffer[8] = 0x00;//dummy cycle 5
    lu8_DataBuffer[9] = 0x00;//dummy cycle 6
    lu8_DataBuffer[10] = 0x00;//dummy cycle 7
    lu8_DataBuffer[11] = 0x00;//dummy cycle 8
    lu8_DataBuffer[12] = 0x00;//dummy cycle 9
    lu8_DataBuffer[13] = 0x00;//dummy cycle 10
    lu8_DataBuffer[14] = 0x00;//dummy cycle 11
    lu8_DataBuffer[15] = 0x00;//dummy cycle 12
    lu8_DataBuffer[16] = 0x00;//dummy cycle 13
    lu8_DataBuffer[17] = 0x00;//dummy cycle 14
    lu8_DataBuffer[18] = 0x00;//dummy cycle 15
    lu8_DataBuffer[19] = 0x00;//dummy cycle 16
    
      /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 

    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 1, 0);
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);
    
    // /* Send Address+Dummy */
    // __OSPI_Write_Data(&lu8_DataBuffer[1], 19);

    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, &lu8_DataBuffer[1], 19, pu8_Buffer, fu32_Length, 0);


    /* CS Realse */
    __OSPI_CS_Release();    
    
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

}


/*********************************************************************************
* function   :  NORFLASH_Read_Octal_IO_STR_OPI
* Description :  Octal IO Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_IO_STR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[20];

    lu8_DataBuffer[0] = OCTAL_IO_FAST_READ;

    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
 
    lu8_DataBuffer[4] = 0x00;//dummy cycle 1
    lu8_DataBuffer[5] = 0x00;//dummy cycle 2
    lu8_DataBuffer[6] = 0x00;//dummy cycle 3
    lu8_DataBuffer[7] = 0x00;//dummy cycle 4
    lu8_DataBuffer[8] = 0x00;//dummy cycle 5
    lu8_DataBuffer[9] = 0x00;//dummy cycle 6
    lu8_DataBuffer[10] = 0x00;//dummy cycle 7
    lu8_DataBuffer[11] = 0x00;//dummy cycle 8
    lu8_DataBuffer[12] = 0x00;//dummy cycle 9
    lu8_DataBuffer[13] = 0x00;//dummy cycle 10
    lu8_DataBuffer[14] = 0x00;//dummy cycle 11
    lu8_DataBuffer[15] = 0x00;//dummy cycle 12
    lu8_DataBuffer[16] = 0x00;//dummy cycle 13
    lu8_DataBuffer[17] = 0x00;//dummy cycle 14
    lu8_DataBuffer[18] = 0x00;//dummy cycle 15
    lu8_DataBuffer[19] = 0x00;//dummy cycle 16
    
    
      /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);
    
    // /* Send Address+Dummy */
    // __OSPI_Write_Data(lu8_DataBuffer, 20);

    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, lu8_DataBuffer, 20, pu8_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();    
    
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

}

/*********************************************************************************
* function   :  NORFLASH_Read_Octal_IO_FourByte
* Description :  4-Byte Octal IO Fast Read with 
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_IO_FourByte(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[21];

    lu8_DataBuffer[0] = FOUR_BYTE_OCTAL_IO_FAST_READ;

    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 24 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 16  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[4] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    
    lu8_DataBuffer[5] = 0x00;//dummy cycle 1
    lu8_DataBuffer[6] = 0x00;//dummy cycle 2
    lu8_DataBuffer[7] = 0x00;//dummy cycle 3
    lu8_DataBuffer[8] = 0x00;//dummy cycle 4
    lu8_DataBuffer[9] = 0x00;//dummy cycle 5
    lu8_DataBuffer[10] = 0x00;//dummy cycle 6
    lu8_DataBuffer[11] = 0x00;//dummy cycle 7
    lu8_DataBuffer[12] = 0x00;//dummy cycle 8
    lu8_DataBuffer[13] = 0x00;//dummy cycle 9
    lu8_DataBuffer[14] = 0x00;//dummy cycle 10
    lu8_DataBuffer[15] = 0x00;//dummy cycle 11
    lu8_DataBuffer[16] = 0x00;//dummy cycle 12
    lu8_DataBuffer[17] = 0x00;//dummy cycle 13
    lu8_DataBuffer[18] = 0x00;//dummy cycle 14
    lu8_DataBuffer[19] = 0x00;//dummy cycle 15
    lu8_DataBuffer[20] = 0x00;//dummy cycle 16
    
    
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 

    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 1, 0);
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);
    
    // /* Send Address+Dummy */
    // __OSPI_Write_Data(&lu8_DataBuffer[1], 20);

    // /* Recieve Data */
    // __OSPI_Read_Data(pu8_Buffer, fu32_Length);

    HAL_OSPI_Transmit_Recieve_ByByte(&NORFLASH_Handle, &lu8_DataBuffer[1], 20, pu8_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();    
    
    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Disable 4-byte Mode */
    NORFLASH_WriteDisableFourByteMode();
}


/*********************************************************************************
* function   :  NORFLASH_Read_Octal_IO_DTR
* Description :  Octal Output Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_IO_DTR(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[1];
    uint16_t lu16_DataBuffer[18];

    lu8_DataBuffer[0] = FOUR_BYTE_DTR_OCTAL_IO_FAST_READ;

    lu16_DataBuffer[0] = ((fu32_DataAddress >> 24) & 0xff);//A3
    lu16_DataBuffer[0] += ((fu32_DataAddress >> 16) & 0xff) << 8;//A2
    
    lu16_DataBuffer[1] = ((fu32_DataAddress >> 8) & 0xff);//A1
    lu16_DataBuffer[1] += (fu32_DataAddress & 0xff) << 8;//A0
    
    lu16_DataBuffer[2] = 0x0000;//dummy cycle 1
    lu16_DataBuffer[3] = 0x0000;//dummy cycle 2
    lu16_DataBuffer[4] = 0x0000;//dummy cycle 3
    lu16_DataBuffer[5] = 0x0000;//dummy cycle 4
    lu16_DataBuffer[6] = 0x0000;//dummy cycle 5
    lu16_DataBuffer[7] = 0x0000;//dummy cycle 6
    lu16_DataBuffer[8] = 0x0000;//dummy cycle 7
    lu16_DataBuffer[9] = 0x0000;//dummy cycle 8
    lu16_DataBuffer[10] = 0x0000;//dummy cycle 9
    lu16_DataBuffer[11] = 0x0000;//dummy cycle 10
    lu16_DataBuffer[12] = 0x0000;//dummy cycle 11
    lu16_DataBuffer[13] = 0x0000;//dummy cycle 12
    lu16_DataBuffer[14] = 0x0000;//dummy cycle 13
    lu16_DataBuffer[15] = 0x0000;//dummy cycle 14
    lu16_DataBuffer[16] = 0x0000;//dummy cycle 15
    lu16_DataBuffer[17] = 0x0000;//dummy cycle 16    
    
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress();

    /* CS Select */
    __OSPI_CS_Select();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE(); 

    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 1);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 1, 0);
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);
    
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&NORFLASH_Handle, lu16_DataBuffer,  18, pu16_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();    
    
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);

    /* Disable 4-byte Mode */
    NORFLASH_WriteDisableFourByteMode();
}


/*********************************************************************************
* function   :  NORFLASH_Read_Octal_IO_DTR_OPI
* Description :  Octal Output Fast Read
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_Read_Octal_IO_DTR_OPI(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint16_t lu16_DataBuffer[19];

    lu16_DataBuffer[0] = FOUR_BYTE_DTR_OCTAL_IO_FAST_READ;
    lu16_DataBuffer[0] += (FOUR_BYTE_DTR_OCTAL_IO_FAST_READ << 8);

    lu16_DataBuffer[1] = ((fu32_DataAddress >> 24) & 0xff);//A3
    lu16_DataBuffer[1] += ((fu32_DataAddress >> 16) & 0xff) << 8;//A2
    
    lu16_DataBuffer[2] = ((fu32_DataAddress >> 8) & 0xff);//A1
    lu16_DataBuffer[2] += (fu32_DataAddress & 0xff) << 8;//A0
    
    lu16_DataBuffer[3] = 0x0000;//dummy cycle 1
    lu16_DataBuffer[4] = 0x0000;//dummy cycle 2
    lu16_DataBuffer[5] = 0x0000;//dummy cycle 3
    lu16_DataBuffer[6] = 0x0000;//dummy cycle 4
    lu16_DataBuffer[7] = 0x0000;//dummy cycle 5
    lu16_DataBuffer[8] = 0x0000;//dummy cycle 6
    lu16_DataBuffer[9] = 0x0000;//dummy cycle 7
    lu16_DataBuffer[10] = 0x0000;//dummy cycle 8
    lu16_DataBuffer[11] = 0x0000;//dummy cycle 9
    lu16_DataBuffer[12] = 0x0000;//dummy cycle 10
    lu16_DataBuffer[13] = 0x0000;//dummy cycle 11
    lu16_DataBuffer[14] = 0x0000;//dummy cycle 12
    lu16_DataBuffer[15] = 0x0000;//dummy cycle 13
    lu16_DataBuffer[16] = 0x0000;//dummy cycle 14
    lu16_DataBuffer[17] = 0x0000;//dummy cycle 15
    lu16_DataBuffer[18] = 0x0000;//dummy cycle 16   
   
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);
    
    HAL_OSPI_Transmit_Recieve_ByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, 19, pu16_Buffer, fu32_Length, 0);

    /* CS Realse */
    __OSPI_CS_Release();     

}

/*********************************************************************************
* function   :  NORFLASH_PageProgram_Octal
* Description :  Octal Page Program 
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram_Octal(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = OCTAL_INPUT_FAST_PROGRAM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 4, 0);

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);

    /* Send Data */
    __OSPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}

/*********************************************************************************
* function   :  NORFLASH_PageProgram_Octal_OPI
* Description :  Octal Page Program 
* Input       : fu32_Length <= 256 
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram_Octal_OPI(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint16_t lu16_DataBuffer[3+256*4];

    lu16_DataBuffer[0] = OCTAL_INPUT_FAST_PROGRAM;
    lu16_DataBuffer[0] += (OCTAL_INPUT_FAST_PROGRAM << 8);

    lu16_DataBuffer[1] = ((fu32_DataAddress >> 24) & 0xff);//A3
    lu16_DataBuffer[1] += ((fu32_DataAddress >> 16) & 0xff) << 8;//A2
    
    lu16_DataBuffer[2] = ((fu32_DataAddress >> 8) & 0xff);//A1
    lu16_DataBuffer[2] += (fu32_DataAddress & 0xff) << 8;//A0

    memcpy(&lu16_DataBuffer[3], pu16_Buffer, fu32_Length*4);//长度为要复制的字节数 32bit数据长度需要x4  
    
   
    /* Enable 4-byte Mode */
    NORFLASH_WriteEnableFourByteMode_DTR_OPI();

    /* Wait Write register End */
    NORFLASH_WaitNotInProgress_DTR_OPI();

    /* Write Enable */
    NORFLASH_WriteEnable_DTR_OPI();    

    /* CS Select */
    __OSPI_CS_Select();
    
    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();

    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_DTR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_HALFWORD);
    
    HAL_OSPI_TransmitByHalfWord(&NORFLASH_Handle, lu16_DataBuffer, fu32_Length + 3, 0);

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress_DTR_OPI();
}
/*********************************************************************************
* function   :  NORFLASH_PageProgram_Extend_Octal
* Description : Extend Octal Page Program 
* Input       :  
* Output      :  
**********************************************************************************/
void NORFLASH_PageProgram_Extend_Octal(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = EXTENDED_OCTAL_INPUT_FAST_PROGRAM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    NORFLASH_WriteEnable();
    
    /* CS Select */
    __OSPI_CS_Select();
    /* Send command */
    // __OSPI_Write_Data(lu8_DataBuffer, 4);
    HAL_OSPI_TransmitKeepCS(&NORFLASH_Handle, lu8_DataBuffer, 4, 0);

    /* Set SPI Work In 8 Wire Mode */
    __OSPI_SET_8X_MODE();
    /* Set SPI Work In 8 Wire DTR Mode */
    __OSPI_SET_8X_STR_MODE();

    /* Set FIFO Write Mode */
    __OSPI_SET_FIFO_MODE(OSPI_FIFO_BYTE);

    /* Send Data */
    __OSPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __OSPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __OSPI_SET_1X_MODE();

    /* Wait Erase End */
    NORFLASH_WaitNotInProgress();
}











