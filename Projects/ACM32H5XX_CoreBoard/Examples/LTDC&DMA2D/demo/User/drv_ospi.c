
#include "drv_ospi.h"



void OSPI_DelayBlockConfig(DLYB_TypeDef * ospi_dlyb, uint32_t Timeout)
{
    uint32_t i, len;
    __IO uint32_t uiTimeout = Timeout;
    /* Check the DelayBlock instance */
    assert_param(IS_OSPI_DLYB_ALL_INSTANCE(ospi_dlyb));

    /* Enable the DelayBlock */
    SET_BIT(ospi_dlyb->CR, DLYB_CR_DEN);

    /* Enable the length sampling */
    SET_BIT(ospi_dlyb->CR, DLYB_CR_SEN);
    
    ospi_dlyb->CFGR |= OSPI_DLYB_MAX_SELECT;
    
    if (uiTimeout == 0) 
    {
        while(ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF);
    }
    else
    {
        while(ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF)
        {
            if(uiTimeout-- == 0) 
            {
                /* New check to avoid false timeout detection in case of preemption */
                if ((ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF) == OSPI_DLYB_FLAG_LENF)
                {
                    return;
                }
            }
        }
    }
    len = (ospi_dlyb->CFGR & DLYB_CFGR_LEN_Msk);
    printfS("len = %x\r\n", len);
    
    i = 0;
    while((!(len & OSPI_DLYB_LNG_10_0_MASK)) || (((len & OSPI_DLYB_LNG_11_10_MASK) == OSPI_DLYB_LNG_11_10_MASK)))
    {
        i++;
        ospi_dlyb->CFGR = OSPI_DLYB_MAX_SELECT | (i << DLYB_CFGR_UNIT_Pos);
        if (uiTimeout == 0) 
        {
            while((ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF) == 0U);
        }
        else
        {
            while((ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF) == 0U)
            {
                if(uiTimeout-- == 0) 
                {
                    /* New check to avoid false timeout detection in case of preemption */
                    if ((ospi_dlyb->CFGR & OSPI_DLYB_FLAG_LENF) == 0U)
                    {
                        return;
                    }
                }
            }
        }
        len = (ospi_dlyb->CFGR & DLYB_CFGR_LEN_Msk);
        printfS("uint = %d, len = %x\r\n", i, len);
    }
    ospi_dlyb->CR &= ~DLYB_CR_SEN;//clear sen
}


void OSPI_DelayBlockSelConfig(DLYB_TypeDef * ospi_dlyb, uint8_t sel)
{
    /* Check the DelayBlock instance */
    assert_param(IS_OSPI_DLYB_ALL_INSTANCE(ospi_dlyb));

    /* Enable the DelayBlock */
    SET_BIT(ospi_dlyb->CR, DLYB_CR_DEN);

    /* Enable the length sampling */
    SET_BIT(ospi_dlyb->CR, DLYB_CR_SEN);
    
    ospi_dlyb->CFGR = (ospi_dlyb->CFGR & (~DLYB_CFGR_SEL_Msk)) | sel;
    
    ospi_dlyb->CR &= ~DLYB_CR_SEN;
}




/******************************************************************************
*@brief : transmits an amount of data based on RWDS status before receiving an amount of data       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*         the configuration information for the specified OSPI.
*@param : pTxCA : Pointer to transmit CA buffer
*@param : TxLCData : Latency Dummy Data
*@param : pRxData : Pointer to recieve data buffer
*@param : TxSize  : Amount of data to be sent
*@param : RxSize  : Amount of data to be receive
*@param : Timeout : sent and receive timeout
*@return: HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef OSPI_TwoTransmits_Recieve_ByHalfWord(OSPI_HandleTypeDef *hospi, uint16_t *pTxCA, uint32_t TxCASize, uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint16_t *pRxData, uint32_t RXSize, uint32_t Timeout)
{
    uint32_t u32TxSizeTemp;
    __IO uint32_t u32RegTemp;
    __IO uint32_t uiTimeout;
    HAL_StatusTypeDef Status = HAL_OK;

    /* Check SPI Parameter */
    assert_param (IS_OSPI_ALL_INSTANCE(hospi->Instance));
    
    if ((pTxCA == NULL) || (pRxData == NULL))    return HAL_ERROR;
    
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET); 

    uiTimeout = Timeout;
    
    /************ Transmit CA Start ************/
    hospi->Instance->CTL &= ~OSPI_DQSOE_ENABLE;
    /* Set Data Size */
    u32TxSizeTemp = TxCASize;
    
    hospi->Instance->BATCH = (u32TxSizeTemp * 2);
 
    /* Tx Enable */
    hospi->Instance->TX_CTL |= OSPI_TX_CTL_TX_EN;

     /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = *pTxCA++;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit CA End ************/
    
    /************ Transmit LC Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    if(hospi->Instance->STATUS & OSPI_STATUS_RWDS)
    {
        // printfS("17\n");
        u32TxSizeTemp = TxLC1Size;
    }
    else
    {
        u32TxSizeTemp = TxLC0Size;
    }
    hospi->Instance->BATCH = (u32TxSizeTemp * 2);
    hospi->Instance->CTL |= OSPI_DQSOE_ENABLE;
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    uiTimeout = Timeout;
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = TxDummyData;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit LC End ************/

    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_TX_BATCH_DONE));
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);
    /* Tx Disable */
    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);

    /* Set Data Size */
    hospi->Instance->BATCH = (RXSize * 2);

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

/******************************************************************************
*@brief : transmits an amount of data based on RWDS status before receiving an amount of data       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*         the configuration information for the specified OSPI.
*@param : pTxCA : Pointer to transmit CA buffer
*@param : TxLCData : Latency Dummy Data
*@param : pRxData : Pointer to recieve data buffer
*@param : TxSize  : Amount of data to be sent
*@param : RxSize  : Amount of data to be receive
*@param : Timeout : sent and receive timeout
*@return: HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef OSPI_TwoTransmits_Recieve_ByWord(OSPI_HandleTypeDef *hospi, uint32_t *pTxCA, uint32_t TxCASize, uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint32_t *pRxData, uint32_t RXSize, uint32_t Timeout)
{
    uint32_t u32TxSizeTemp;
    __IO uint32_t u32RegTemp;
    __IO uint32_t uiTimeout;
    HAL_StatusTypeDef Status = HAL_OK;

    /* Check SPI Parameter */
    assert_param (IS_OSPI_ALL_INSTANCE(hospi->Instance));
    
    if ((pTxCA == NULL) || (pRxData == NULL))    return HAL_ERROR;

    uiTimeout = Timeout;
    
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    
    /* Clear RX FIFO */
    SET_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->RX_CTL, OSPI_RX_CTL_RX_FIFO_RESET); 
    
    /************ Transmit CA Start ************/
    hospi->Instance->CTL &= ~OSPI_DQSOE_ENABLE;
    /* Set Data Size */
    u32TxSizeTemp = TxCASize;
    
    hospi->Instance->BATCH = (u32TxSizeTemp * 4);
 
    /* Tx Enable */
    hospi->Instance->TX_CTL |= OSPI_TX_CTL_TX_EN;

    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = *pTxCA++;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit CA End ************/
    
    /************ Transmit LC Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    if(hospi->Instance->STATUS & OSPI_STATUS_RWDS)
    {
        u32TxSizeTemp = TxLC1Size;
    }
    else
    {
        u32TxSizeTemp = TxLC0Size;
    }
    
    hospi->Instance->BATCH = (u32TxSizeTemp * 4);

    hospi->Instance->CTL |= OSPI_DQSOE_ENABLE;

    uiTimeout = Timeout;
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);

    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = TxDummyData;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit LC End ************/

    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_TX_BATCH_DONE));
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);
    /* Tx Disable */
    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);

    /* Set Data Size */
    hospi->Instance->BATCH = (RXSize * 4);

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

/******************************************************************************
*@brief : transmits an amount of data based on RWDS status before receiving an amount of data       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*         the configuration information for the specified OSPI.
*@param : pTxCA : Pointer to transmit CA buffer
*@param : TxLCData : Latency Dummy Data
*@param : pRxData : Pointer to recieve data buffer
*@param : TxSize  : Amount of data to be sent
*@param : RxSize  : Amount of data to be receive
*@param : Timeout : sent and receive timeout
*@return: HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef OSPI_ThreeTransmits_ByHalfWord(OSPI_HandleTypeDef *hospi, uint16_t *pTxCA, uint32_t TxCASize, uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint16_t *pTxData, uint32_t TxSize, uint32_t Timeout)
{
    uint32_t u32TxSizeTemp;
    __IO uint32_t u32RegTemp;
    __IO uint32_t uiTimeout;
    HAL_StatusTypeDef Status = HAL_OK;

    /* Check SPI Parameter */
    assert_param (IS_OSPI_ALL_INSTANCE(hospi->Instance));
    
    if ((pTxCA == NULL) || (pTxData == NULL))    return HAL_ERROR;

    uiTimeout = Timeout;

    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
   
    /************ Transmit CA Start ************/
    hospi->Instance->CTL &= ~OSPI_DQSOE_ENABLE;//CA�ڼ�ر�RWDS���
    /* Set Data Size */
    u32TxSizeTemp = TxCASize;
    hospi->Instance->BATCH = (u32TxSizeTemp * 2);
 
    /* Tx Enable */
    hospi->Instance->TX_CTL |= OSPI_TX_CTL_TX_EN;

    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = *pTxCA++;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    /************ Transmit CA End ************/
    
    /************ Transmit Dummy Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    
    if(hospi->Instance->STATUS & OSPI_STATUS_RWDS)
    {
        u32TxSizeTemp = TxLC1Size;
    }
    else
    {
        u32TxSizeTemp = TxLC0Size;
    }
    hospi->Instance->BATCH = (u32TxSizeTemp * 2);
    hospi->Instance->CTL |= OSPI_DQSOE_ENABLE;
    uiTimeout = Timeout;
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = TxDummyData;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit Dummy End ************/
    
    /************ Transmit Data Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    
    /* Set Data Size */
    u32TxSizeTemp = TxSize;
    hospi->Instance->BATCH = (u32TxSizeTemp * 2);
    
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    uiTimeout = Timeout;
    
    while(u32TxSizeTemp)
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
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }    
    /************ Transmit Data End ************/
    
    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_TX_BATCH_DONE));
    Status = HAL_OK;
    
    
End:
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);
    
    /* Transmit End */
    HAL_OSPI_CS_Release(hospi);
    

    return Status;
}

/******************************************************************************
*@brief : transmits an amount of data based on RWDS status before receiving an amount of data       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*         the configuration information for the specified OSPI.
*@param : pTxCA : Pointer to transmit CA buffer
*@param : TxLCData : Latency Dummy Data
*@param : pRxData : Pointer to recieve data buffer
*@param : TxSize  : Amount of data to be sent
*@param : RxSize  : Amount of data to be receive
*@param : Timeout : sent and receive timeout
*@return: HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef OSPI_ThreeTransmits_ByWord(OSPI_HandleTypeDef *hospi, uint32_t *pTxCA, uint32_t TxCASize,uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint32_t *pTxData, uint32_t TxSize, uint32_t Timeout)
{
    uint32_t u32TxSizeTemp;
    __IO uint32_t u32RegTemp;
    __IO uint32_t uiTimeout;
    HAL_StatusTypeDef Status = HAL_OK;

    /* Check SPI Parameter */
    assert_param (IS_OSPI_ALL_INSTANCE(hospi->Instance));
    
    if ((pTxCA == NULL) || (pTxData == NULL))    return HAL_ERROR;

    uiTimeout = Timeout;

    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Clear TX FIFO */
    SET_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
    CLEAR_BIT(hospi->Instance->TX_CTL, OSPI_TX_CTL_TX_FIFO_RESET);
   
    /************ Transmit CA Start ************/
    hospi->Instance->CTL &= ~OSPI_DQSOE_ENABLE;
    /* Set Data Size */
    u32TxSizeTemp = TxCASize;
    hospi->Instance->BATCH = (u32TxSizeTemp * 4);
 
    /* Tx Enable */
    hospi->Instance->TX_CTL |= OSPI_TX_CTL_TX_EN;

    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = *pTxCA++;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit CA End ************/
    
    /************ Transmit Dummy Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    
    if(hospi->Instance->STATUS & OSPI_STATUS_RWDS)
    {
        u32TxSizeTemp = TxLC1Size;
    }
    else
    {
        u32TxSizeTemp = TxLC0Size;
    }
    
    hospi->Instance->BATCH = (u32TxSizeTemp * 4);
    hospi->Instance->CTL |= OSPI_DQSOE_ENABLE;
    uiTimeout = Timeout;
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    while(u32TxSizeTemp)
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
        hospi->Instance->DAT = TxDummyData;
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }
    /************ Transmit Dummy End ************/
    
    /************ Transmit Data Start ************/
    /* Wait Not TX_Busy */
    while ((hospi->Instance->STATUS & OSPI_STATUS_TX_BUSY) == OSPI_STATUS_TX_BUSY);
    
    /* Set Data Size */
    u32TxSizeTemp = TxSize;
    hospi->Instance->BATCH = (u32TxSizeTemp * 4);
    
    /* Transmit Start */
    HAL_OSPI_CS_Select(hospi);
    
    uiTimeout = Timeout;
    
    while(u32TxSizeTemp)
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
        u32TxSizeTemp--;
        uiTimeout = Timeout;
    }    
    /************ Transmit Data End ************/
    
    /* Wait Transmit Done */
    while (!(hospi->Instance->STATUS & OSPI_STATUS_TX_BATCH_DONE));
    Status = HAL_OK;
    
    
End:
    /* Clear Batch Done Flag  */
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_TX_BATCH_DONE);
    SET_BIT(hospi->Instance->STATUS, OSPI_STATUS_BATCH_DONE);

    /* Tx Disable */
    hospi->Instance->TX_CTL &= (~OSPI_TX_CTL_TX_EN);
    
    /* Transmit End */
    HAL_OSPI_CS_Release(hospi);
    

    return Status;
}


void disable_all_irq(void)
{
    __set_PRIMASK(1);    // disable interrupt 
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick 
    SCB->ICSR = BIT25;   // clear systick pending bit  
}


void enable_all_irq(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick              
    __set_PRIMASK(0);    // enable interrupt     
}


