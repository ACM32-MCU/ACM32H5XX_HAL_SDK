/******************************************************************************
*@file  : hal_eth.c
*@brief : ETH HAL module driver.
******************************************************************************/
#include "math.h"
#include "hal.h"

//  ****************************************************************************
//  
//                              Interrupt
//  
//  ****************************************************************************

/******************************************************************************
*@brief : This function handles ETH interrupt request.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_IRQHandler(ETH_HandleTypeDef *heth)
{
    /* Frame received */
    if (ETH->DMASR & ETH_DMASR_RS)
    {
        if (ETH->DMAIER & ETH_DMAIER_RIE)
        {
            HAL_ETH_RxCpltCallback(heth);

            ETH->DMASR = ETH_DMASR_RS | ETH_DMASR_NIS;
            return;
        }
    }
    
    /* Frame transmitted */
    if (ETH->DMASR & ETH_DMASR_TS)
    {
        if (ETH->DMAIER & ETH_DMAIER_TIE)
        {
            HAL_ETH_TxCpltCallback(heth);

            ETH->DMASR = ETH_DMASR_TS | ETH_DMASR_NIS;
            return;
        }
    }
    
    /* ETH DMA Error */
    if(__HAL_ETH_DMA_GET_FLAG(heth, ETH_DMASR_AIS))
    {
        if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMAIER_AISE)) 
        {
            HAL_ETH_DMAErrorCallback(heth);
            if (__HAL_ETH_DMA_GET_FLAG(heth, ETH_DMASR_FBES))
            {
                __HAL_ETH_DMA_CLEAR_FLAG(heth, ETH_DMASR_FBES | ETH_DMASR_RPSS | \
                                               ETH_DMASR_TPSS | ETH_DMASR_AIS);
            }
            else
            {
                __HAL_ETH_DMA_CLEAR_FLAG(heth, ETH_DMASR_ETS | ETH_DMASR_RWTS | \
                                               ETH_DMASR_RBUS | ETH_DMASR_TUS | \
                                               ETH_DMASR_ROS | ETH_DMASR_TJTS | \
                                               ETH_DMASR_AIS);
            }
        }
    }
    
    #ifdef ETH_PMT_SUPPORT
    volatile uint32_t temp1 = 0;
    if (READ_BIT(heth->Instance->MACISR, ETH_MACISR_PMTS))
    {
        if (READ_BIT(heth->Instance->MACIMR, ETH_MACIMR_PIM) == 0)
        {
            temp1 = heth->Instance->MACPMTCSR;
            HAL_ETH_PMTCallback(heth);
        }
    }
    #endif
    
    #ifdef ETH_MMC_SUPPORT
    if (READ_BIT(heth->Instance->MMCTIR, ETH_MMCTIR_TGFIS))
    {
        if (READ_BIT(heth->Instance->MMCTIMR, ETH_MMCTIMR_TGFIM) == 0)
        {
            heth->MMCCounter.TxGoodFrame = heth->Instance->MMCTGFCR;
            HAL_ETH_MMCTxCallback(heth);
        }
    }
    if (READ_BIT(heth->Instance->MMCRIR, ETH_MMCRIR_RGUFIS))
    {
        if (READ_BIT(heth->Instance->MMCRIMR, ETH_MMCRIMR_RGUFIM) == 0)
        {
            heth->MMCCounter.RxGoodUnicast = heth->Instance->MMCRGUFCR;
            HAL_ETH_MMCRxCallback(heth);
        }
    }
    #endif
    
    #ifdef ETH_LPI_SUPPORT
    volatile uint32_t temp2 = 0;
    if (READ_BIT(heth->Instance->DMASR, ETH_DMASR_LPIS))
    {
        temp2 = heth->Instance->MACLPICSR;
        if (temp2 & ETH_MACLPICSR_TLPIEN)
        {
            HAL_ETH_LPITransmitEntryCallback(heth);
        }
        if (temp2 & ETH_MACLPICSR_TLPIEX)
        {
            HAL_ETH_LPITransmitExitCallback(heth);
        }
        if (temp2 & ETH_MACLPICSR_RLPIEN)
        {
            HAL_ETH_LPIReceiveEntryCallback(heth);
        }
        if (temp2 & ETH_MACLPICSR_RLPIEX)
        {
            HAL_ETH_LPIReceiveExitCallback(heth);
        }
    }
    #endif
}

#ifdef ETH_WAKEUP_SUPPORT
/******************************************************************************
*@brief : This function handles ETH Wakeup interrupt request.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_WakeupIRQHandler(ETH_HandleTypeDef *heth)
{
    volatile uint32_t temp;
    
    temp = heth->Instance->MACPMTCSR;
    
    /* Enable the DMA reception */
    heth->Instance->DMAOMR |= ETH_DMAOMR_SR;
    
    /* Enable the DMA transmission */
    heth->Instance->DMAOMR |= ETH_DMAOMR_ST;
    
    /* Enable the MAC transmission */
    heth->Instance->MACCR |= ETH_MACCR_TE;
    
    HAL_ETH_WakeupCallback(heth);
}
#endif

/******************************************************************************
*@brief : Tx Transfer completed callbacks.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : Rx Transfer completed callbacks.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : Ethernet transfer error callbacks.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_DMAErrorCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

#ifdef ETH_MMC_SUPPORT
/******************************************************************************
*@brief : MMC tx callbacks.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_MMCTxCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : MMC rx callbacks.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_MMCRxCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}
#endif

#ifdef ETH_PMT_SUPPORT
/******************************************************************************
*@brief : PMT interrupt callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_PMTCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}
#endif

#ifdef ETH_LPI_SUPPORT
/******************************************************************************
*@brief : Transmit LPI entry callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_LPITransmitEntryCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : Transmit LPI exit callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_LPITransmitExitCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : Receive LPI entry callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_LPIReceiveEntryCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : Receive LPI exit callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_LPIReceiveExitCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}
#endif

#ifdef ETH_WAKEUP_SUPPORT
/******************************************************************************
*@brief : Wakeup interrupt callback
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_WakeupCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}
#endif

/******************************************************************************
*@brief : Initializes the Ethernet MAC and DMA according to default parameters.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_Init(ETH_HandleTypeDef *heth)
{
    uint32_t timeout;

    assert_param(heth);
    
    HAL_ETH_MspInit(heth);
    
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    
    if (heth->Init.MediaInterface == ETH_MEDIA_INTERFACE_MII)
        SYSCFG->SYSCR &= ~SYSCFG_SYSCR_EPIS;
    else if (heth->Init.MediaInterface == ETH_MEDIA_INTERFACE_RMII)
        SYSCFG->SYSCR = (SYSCFG->SYSCR & ~SYSCFG_SYSCR_EPIS) | SYSCFG_SYSCR_EPIS_2;
    else
        return HAL_ERROR;
    
    SET_BIT(heth->Instance->DMABMR, ETH_DMABMR_SWR);
    timeout = ETH_TIMEOUT_SWRESET;
    while (READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_SWR) != (uint32_t)RESET)
    {
        if(--timeout == 0)
            return HAL_ERROR;
    }
    
    HAL_ETH_RxClockDelayConfig(heth, heth->Init.Delay.Uint, heth->Init.Delay.Len);
    
    if (HAL_ETH_MACAddrConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_SpeedConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_DuplexModeConfig(heth) != HAL_OK)
        return HAL_ERROR;
    
    if (HAL_ETH_ConfigSMI(heth) != HAL_OK)
        return HAL_ERROR;
    
    if (HAL_ETH_MACConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_DMAConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_AddrFilterConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_VLANFilterConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_L3L4FilterConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_MMCConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_PMTConfig(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_PTPConfig(heth) != HAL_OK)
        return HAL_ERROR;
    
    /* Set Receive Buffs Len (must be a multiple of 4) */
    if ((heth->Init.RxBuffLen % 0x4U) != 0x0U)
    {
        return HAL_ERROR;
    }
    
    if (HAL_ETH_DMATxDescListInit(heth) != HAL_OK)
        return HAL_ERROR;
    if (HAL_ETH_DMARxDescListInit(heth) != HAL_OK)
        return HAL_ERROR;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : De-Initializes the ETH peripheral. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DeInit(ETH_HandleTypeDef *heth)
{
    HAL_ETH_MspDeInit(heth);
    return HAL_OK;
}

/******************************************************************************
*@brief : Initializes the ETH MSP. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}

/******************************************************************************
*@brief : DeInitializes the ETH MSP. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
}
   
//  ****************************************************************************
//  
//                              DMA descriptors
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initializes the DMA tx descriptors list. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DMATxDescListInit(ETH_HandleTypeDef *heth)
{
    uint32_t i;
    uint32_t buff;
    ETH_DMADescTypeDef *tx_desc;
    
    if (heth->Init.TxDescNbr == 0)
        return HAL_ERROR;
    
    if (heth->Init.TxBuffNbr != 0)
    {
        if (heth->Init.TxDescListMode == ETH_DESC_LIST_MODE_RING)
        {
            if (heth->Init.TxBuffNbr < (heth->Init.TxDescNbr << 1))
                return HAL_ERROR;
        }
        else
        {
            if (heth->Init.TxBuffNbr < heth->Init.TxDescNbr)
                return HAL_ERROR;
        }
    }
    
    buff = (uint32_t)heth->Init.TxBuff;
    
    /* Fill each DMATxDesc descriptor with the right values */
    for(i=0; i<heth->Init.TxDescNbr; i++)
    {
        tx_desc = heth->Init.TxDesc + i;
        
        WRITE_REG(tx_desc->DESC0, 0);
        WRITE_REG(tx_desc->DESC1, 0);
        WRITE_REG(tx_desc->DESC2, 0);
        WRITE_REG(tx_desc->DESC3, 0);
        WRITE_REG(tx_desc->DESC4, 0);
        WRITE_REG(tx_desc->DESC5, 0);
        WRITE_REG(tx_desc->DESC6, 0);
        WRITE_REG(tx_desc->DESC7, 0);
        WRITE_REG(tx_desc->Buff1, 0);
        WRITE_REG(tx_desc->Buff2, 0);
        
        if (heth->Init.TxDescListMode == ETH_DESC_LIST_MODE_RING)
        {
            if (i == (heth->Init.TxDescNbr - 1))
                SET_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_TER);
            
            if (heth->Init.TxBuffNbr != 0)
            {
                WRITE_REG(tx_desc->Buff1, buff + (heth->Init.TxBuffLen * (i << 1)));
                WRITE_REG(tx_desc->Buff2, buff + (heth->Init.TxBuffLen * ((i << 1) + 1)));
            }
            
            if ((heth->Init.TxBuffTab != 0) && (heth->Init.TxBuffNodeNbr != 0))
            {
                heth->Init.TxBuffTab[(i << 1) + 0].Buff = (uint8_t *)tx_desc->Buff1;
                heth->Init.TxBuffTab[(i << 1) + 0].Len = 0;
                heth->Init.TxBuffTab[(i << 1) + 0].next = 0;
                heth->Init.TxBuffTab[(i << 1) + 1].Buff = (uint8_t *)tx_desc->Buff2;
                heth->Init.TxBuffTab[(i << 1) + 1].Len = 0;
                heth->Init.TxBuffTab[(i << 1) + 1].next = 0;
            }
        }
        else
        {
            SET_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_TCH);
            
            // next desc
            if (i == (heth->Init.TxDescNbr - 1))
                WRITE_REG(tx_desc->DESC3, (uint32_t)heth->Init.TxDesc);
            else
                WRITE_REG(tx_desc->DESC3, (uint32_t)(tx_desc + 1));
            
            if (heth->Init.TxBuffNbr != 0)
                WRITE_REG(tx_desc->Buff1, buff + (heth->Init.TxBuffLen * i));
            
            WRITE_REG(tx_desc->Buff2, tx_desc->DESC3);
            
            if ((heth->Init.TxBuffTab != 0) && (heth->Init.TxBuffNodeNbr != 0))
            {
                heth->Init.TxBuffTab[i].Buff = (uint8_t *)tx_desc->Buff1;
                heth->Init.TxBuffTab[i].Len = 0;
                heth->Init.TxBuffTab[i].next = 0;
            }
        }
    }
    
    heth->TxDescList.TxDescTab = heth->Init.TxDesc;
    heth->TxDescList.CurTxDesc = 0;

    /* Set Transmit Descriptor List Address Register */
    heth->Instance->DMATDLAR = (uint32_t)heth->Init.TxDesc;

    return HAL_OK;
}

/******************************************************************************
*@brief : Initializes the DMA rx descriptors list. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DMARxDescListInit(ETH_HandleTypeDef *heth)
{
    uint32_t i;
    uint32_t buff_addr;
    ETH_DMADescTypeDef *rx_desc;

    if ((heth->Init.RxDescNbr == 0) || (heth->Init.RxBuffTab == 0))
        return HAL_ERROR;
    
    if (heth->Init.RxDescListMode == ETH_DESC_LIST_MODE_RING)
    {
        if (heth->Init.RxBuffNbr < (heth->Init.RxDescNbr << 1))
            return HAL_ERROR;
        if (heth->Init.RxBuffNodeNbr < (heth->Init.RxDescNbr << 1))
            return HAL_ERROR;
    }
    else
    {
        if (heth->Init.RxBuffNbr < heth->Init.RxDescNbr)
            return HAL_ERROR;
        if (heth->Init.RxBuffNodeNbr < heth->Init.RxDescNbr)
            return HAL_ERROR;
    }
    
    buff_addr = (uint32_t)heth->Init.RxBuff;
    
    /* Fill each DMARxDesc descriptor with the right values */
    for(i=0; i<heth->Init.RxDescNbr; i++)
    {
        rx_desc = heth->Init.RxDesc + i;
        
        WRITE_REG(rx_desc->DESC0, 0);
        WRITE_REG(rx_desc->DESC1, 0);
        WRITE_REG(rx_desc->DESC2, 0);
        WRITE_REG(rx_desc->DESC3, 0);
        WRITE_REG(rx_desc->DESC4, 0);
        WRITE_REG(rx_desc->DESC5, 0);
        WRITE_REG(rx_desc->DESC6, 0);
        WRITE_REG(rx_desc->DESC7, 0);
        WRITE_REG(rx_desc->Buff1, 0);
        WRITE_REG(rx_desc->Buff2, 0);
        
        if (heth->IT.RxComplete == DISABLE)
            SET_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC1_DIC);
        
        if (heth->Init.RxDescListMode == ETH_DESC_LIST_MODE_RING)
        {
            if (i == (heth->Init.RxDescNbr - 1))
                SET_BIT(rx_desc->DESC1, ETH_DMA_RX_DESC1_RER);
            
            // buff len
            MODIFY_REG(rx_desc->DESC1, ETH_DMA_RX_DESC1_RBS1, (heth->Init.RxBuffLen << ETH_DMA_RX_DESC1_RBS1_Pos));
            MODIFY_REG(rx_desc->DESC1, ETH_DMA_RX_DESC1_RBS2, (heth->Init.RxBuffLen << ETH_DMA_RX_DESC1_RBS2_Pos));
            
            // rx buff
            WRITE_REG(rx_desc->DESC2, buff_addr + (heth->Init.RxBuffLen * (i << 1)));
            WRITE_REG(rx_desc->DESC3, buff_addr + (heth->Init.RxBuffLen * ((i << 1) + 1)));
            
            heth->Init.RxBuffTab[(i << 1) + 0].Buff = (uint8_t *)rx_desc->DESC2;
            heth->Init.RxBuffTab[(i << 1) + 0].Len = 0;
            heth->Init.RxBuffTab[(i << 1) + 0].next = 0;
            heth->Init.RxBuffTab[(i << 1) + 1].Buff = (uint8_t *)rx_desc->DESC3;
            heth->Init.RxBuffTab[(i << 1) + 1].Len = 0;
            heth->Init.RxBuffTab[(i << 1) + 1].next = 0;
        }
        else
        {
            SET_BIT(rx_desc->DESC1, ETH_DMA_RX_DESC1_RCH);
            
            // next desc
            if (i == (heth->Init.RxDescNbr - 1))
                WRITE_REG(rx_desc->DESC3, (uint32_t)heth->Init.RxDesc);
            else
                WRITE_REG(rx_desc->DESC3, (uint32_t)(rx_desc + 1));
            
            // buff len
            MODIFY_REG(rx_desc->DESC1, ETH_DMA_RX_DESC1_RBS1, (heth->Init.RxBuffLen << ETH_DMA_RX_DESC1_RBS1_Pos));
            
            // rx buff
            WRITE_REG(rx_desc->DESC2, buff_addr + (heth->Init.RxBuffLen * i));
            
            heth->Init.RxBuffTab[i].Buff = (uint8_t *)rx_desc->DESC2;
            heth->Init.RxBuffTab[i].Len = 0;
            heth->Init.RxBuffTab[i].next = 0;
        }
        
        WRITE_REG(rx_desc->Buff1, rx_desc->DESC2);
        WRITE_REG(rx_desc->Buff2, rx_desc->DESC3);
        
        SET_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC0_OWN);
        
    }
    
    heth->RxDescList.RxDescTab = heth->Init.RxDesc;
    heth->RxDescList.CurRxDesc = 0;
    heth->RxDescList.FirstRxDesc = 0;
    heth->RxDescList.RxDescNbr = 0;
    
    /* Set Transmit Descriptor List Address Register */
    heth->Instance->DMARDLAR = (uint32_t)heth->Init.RxDesc;

    return HAL_OK;
}

//  ****************************************************************************
//  
//                              Transmit Receive
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Gets tx buffer. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : buff: pointer to tx buffer pointer.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetTxBuff(ETH_HandleTypeDef *heth, ETH_BuffTypeDef **buff)
{
    uint32_t nbr;
    uint32_t index;
    uint32_t tx_buff_index;
    uint32_t tx_buff_next_index;
    ETH_BuffTypeDef *tx_buff;
    ETH_TxDescListTypeDef *tx_desc_list;
    ETH_DMADescTypeDef *tx_desc;

    if ((heth == NULL) || (buff == NULL))
        return HAL_ERROR;
    
    nbr = heth->Init.TxDescNbr;
    if (nbr == 0)
    {
        *buff = NULL;
        return HAL_OK;
    }
    
    tx_desc_list = (ETH_TxDescListTypeDef *)&heth->TxDescList;
    index = tx_desc_list->CurTxDesc;
    tx_desc = &tx_desc_list->TxDescTab[index];
    
    if (heth->Init.TxDescListMode == ETH_DESC_LIST_MODE_RING)
        *buff = &heth->Init.TxBuffTab[index << 1];
    else
        *buff = &heth->Init.TxBuffTab[index];
    tx_buff = heth->Init.TxBuffTab;
    
    if (READ_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_OWN) == 0)
    {
        *buff = NULL;
        return HAL_OK;
    }
    while (1)
    {
        if (heth->Init.TxDescListMode == ETH_DESC_LIST_MODE_RING)
        {
            tx_buff_index = index << 1;
            tx_buff_next_index = tx_buff_index;
            if (++tx_buff_next_index >= heth->Init.TxBuffNodeNbr)
                tx_buff_next_index = 0;
            
            tx_buff[tx_buff_index].Buff = (uint8_t *)tx_desc->Buff1;
            tx_buff[tx_buff_index].Len = heth->Init.TxBuffLen;
            tx_buff[tx_buff_index].next = &heth->Init.TxBuffTab[tx_buff_next_index];
        
            tx_buff_index = tx_buff_next_index;
            if (++tx_buff_next_index >= heth->Init.TxBuffNodeNbr)
                tx_buff_next_index = 0;
        
            tx_buff[tx_buff_index].Buff = (uint8_t *)tx_desc->Buff1;
            tx_buff[tx_buff_index].Len = heth->Init.TxBuffLen;
            tx_buff[tx_buff_index].next = &heth->Init.TxBuffTab[tx_buff_next_index];
        }
        else
        {
            tx_buff_index = index;
            tx_buff_next_index = tx_buff_index;
            if (++tx_buff_next_index >= heth->Init.TxBuffNodeNbr)
                tx_buff_next_index = 0;
            
            tx_buff[tx_buff_index].Buff = (uint8_t *)tx_desc->Buff1;
            tx_buff[tx_buff_index].Len = heth->Init.TxBuffLen;
            tx_buff[tx_buff_index].next = &heth->Init.TxBuffTab[tx_buff_next_index];
        }
        
        index++;
        if (index >= heth->Init.TxDescNbr)
            index = 0;
        tx_desc = &tx_desc_list->TxDescTab[index];
        
        if ((--nbr == 0) || (READ_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_OWN) == 0))
        {
            tx_buff[tx_buff_index].next = 0;
            return HAL_OK;
        }
    }
}

/******************************************************************************
*@brief : Transmission Ethernet Packets. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : buff: pointer to tx buffer.
*@param : mode: transmission mode.
*             This parameter can be a value of @reg ETH_TX_Mode.
*@param : pStatus: return to transmission packet status.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_Transmit(ETH_HandleTypeDef *heth, ETH_BuffTypeDef *buff, uint32_t mode, ETH_TxStatusTypeDef *pStatus)
{
    uint32_t i;
    uint32_t len;
    uint32_t index;
    uint32_t nbr;
    uint32_t desc0;
    uint32_t auto_crc;
    uint32_t auto_pad;
    uint32_t auto_checksum_insertion;
    ETH_InitTypeDef *init;
    ETH_TxDescListTypeDef *tx_desc_list;
    ETH_DMADescTypeDef *tx_desc;
    uint32_t timeout;

    if ((heth == NULL) || (buff == NULL) || (buff->Buff == NULL) || (buff->Len == 0))
        return HAL_ERROR;
    
    init = &heth->Init;
    
    if ((mode & ETH_TX_MODE_DATA_COPY) && (init->TxBuffNbr == 0))
        return HAL_ERROR;
    
    nbr = 0;
    tx_desc_list = (ETH_TxDescListTypeDef *)&heth->TxDescList;
    index = tx_desc_list->CurTxDesc;
    tx_desc = &tx_desc_list->TxDescTab[index];
    
    if (heth->Init.AutoCRC)
        auto_crc = 1;
    else
        auto_crc = 0;
    if (heth->Init.AutoPad)
        auto_pad = 1;
    else
        auto_pad = 0;
    auto_checksum_insertion = (heth->Init.AutoChecksumInsertion << ETH_DMA_TX_DESC0_CIC_Pos) & ETH_DMA_TX_DESC0_CIC;
    
    for (i=0; i<init->TxDescNbr; i++)
    {
        if (READ_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_OWN))
            return HAL_BUSY;
        
        desc0 = 0;
        if (heth->IT.TxComplete != DISABLE)
        {
            SET_BIT(desc0, ETH_DMA_TX_DESC0_IC);
        }
        
        
        MODIFY_REG(desc0, ETH_DMA_TX_DESC0_CIC, auto_checksum_insertion);
        
        if (init->TxDescListMode != ETH_DESC_LIST_MODE_RING)
            SET_BIT(desc0, ETH_DMA_TX_DESC0_TCH);
        else if (index == (init->TxDescNbr - 1))
            SET_BIT(desc0, ETH_DMA_TX_DESC0_TER);
        
        if (nbr == 0)
        {
            SET_BIT(desc0, ETH_DMA_TX_DESC0_FS);
            if (auto_crc == 0)
                SET_BIT(desc0, ETH_DMA_TX_DESC0_DC);
            if (auto_pad == 0)
                SET_BIT(desc0, ETH_DMA_TX_DESC0_DP);
            if (mode & ETH_TX_MODE_TIMESTAMP)
                SET_BIT(desc0, ETH_DMA_TX_DESC0_TTSE);
        }
        WRITE_REG(tx_desc->DESC0, desc0);

        // buff1 len
        if (buff->Len > init->TxBuffLen)
            len = init->TxBuffLen;
        else
            len = buff->Len;
        buff->Len -= len;
        WRITE_REG(tx_desc->DESC1, len  << ETH_DMA_TX_DESC1_TBS1_Pos);
        
        // buffer 1
        if (mode & ETH_TX_MODE_DATA_COPY)
        {
            // direct data copy
            WRITE_REG(tx_desc->DESC2, tx_desc->Buff1);
            memcpy((void *)tx_desc->DESC2, (void *)buff->Buff, len);
        }
        else
        {
            // address pointer transmission
            WRITE_REG(tx_desc->DESC2, (uint32_t)buff->Buff);
        }
        buff->Buff += len;
        
        while (1)
        {
            if ((buff->Len != 0) || (buff->next == 0))
                break;
            buff = buff->next;
        }
        
        // buffer 2
        if (init->TxDescListMode == ETH_DESC_LIST_MODE_RING)
        {
            if (buff->Len != 0)
            {
                // buff2 len
                if (buff->Len > init->TxBuffLen)
                    len = init->TxBuffLen;
                else
                    len = buff->Len;
                buff->Len -= len;
                MODIFY_REG(tx_desc->DESC1, ETH_DMA_TX_DESC1_TBS2, (len << ETH_DMA_TX_DESC1_TBS2_Pos));
        
                if (mode & ETH_TX_MODE_DATA_COPY)
                {
                    // direct data copy
                    WRITE_REG(tx_desc->DESC3, tx_desc->Buff2);
                    memcpy((void *)tx_desc->DESC3, (void *)buff->Buff, len);
                }
                else
                {
                    // address pointer transmission
                    WRITE_REG(tx_desc->DESC3, (uint32_t)buff->Buff);
                }
                buff->Buff += len;
                
                while (1)
                {
                    if ((buff->Len != 0) || (buff->next == 0))
                        break;
                    buff = buff->next;
                }
            }
        }
        nbr++;
        
        if (buff->Len == 0)
            break;
    
        index++;
        if (index >= init->TxDescNbr)
            index = 0;
        tx_desc = &tx_desc_list->TxDescTab[index];
    }
    SET_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_LS);
    
    index = tx_desc_list->CurTxDesc;
    tx_desc = &tx_desc_list->TxDescTab[index];
    while (nbr--)
    {
        SET_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_OWN);
        
        index++;
        if (index >= init->TxDescNbr)
            index = 0;
        tx_desc = &tx_desc_list->TxDescTab[index];
    }
    tx_desc_list->CurTxDesc = index;
    
    heth->Instance->DMASR = ETH_DMASR_TBUS;
    heth->Instance->DMATPDR = 0;
    
    // wati data packet transmit complete
    if (mode & (ETH_TX_MODE_WAIT_TX_COMPLETE | ETH_TX_MODE_TIMESTAMP))
    {
        index = tx_desc_list->CurTxDesc;
        if (index == 0)
            index = init->TxDescNbr - 1;
        else
            index--;
        tx_desc = &tx_desc_list->TxDescTab[index];
        
        timeout = 0xffffffff;
        while (1)
        {
            if (READ_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_OWN) == 0)
                break;
            timeout--;
            if (timeout == 0)
                return HAL_ERROR;
        }
        if (pStatus)
        {
            pStatus->Status0 = READ_REG(tx_desc->DESC0);
            if (READ_BIT(tx_desc->DESC0, ETH_DMA_TX_DESC0_TTSS))
            {
                pStatus->TimestampValid = ENABLE;
                pStatus->Timestamp.sec = READ_REG(tx_desc->DESC7);
                pStatus->Timestamp.nsec = READ_REG(tx_desc->DESC6);
            }
            else
            {
                pStatus->TimestampValid = DISABLE;
            }
        }
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Receive Ethernet packets, and return the data buffer address and receiving status.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : buff: pointer to tx buffer.
*@param : pStatus: return to receive packet status.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_Receive(ETH_HandleTypeDef *heth, ETH_BuffTypeDef **buff, ETH_RxStatusTypeDef *pStatus)
{
    uint32_t i;
    uint32_t first_desc_index;
    uint32_t desc_nbr;
    uint32_t desc_index;
    uint32_t first_rx_buff_index;
    uint32_t curr_rx_buff_index;
    uint32_t next_rx_buff_index;
    uint32_t desc0;
    uint32_t packet_len;
    uint32_t len;
    ETH_InitTypeDef *init;
    ETH_BuffTypeDef *rx_buff;
    ETH_DMADescTypeDef *rx_desc;
    ETH_RxDescListTypeDef *rx_desc_list;

    rx_desc_list = &heth->RxDescList;
    
    if(rx_desc_list->RxDescNbr != 0U)
        return HAL_OK;
    
    *buff = 0;
    init = &heth->Init;
    rx_buff = init->RxBuffTab;
    while (1)
    {
        desc_index = rx_desc_list->CurRxDesc;
        desc_nbr = 0;
        first_desc_index = 0;
        rx_desc = &rx_desc_list->RxDescTab[desc_index];
        packet_len = 0;
        len = 0;
        first_rx_buff_index = 0;

        for (i=0; i<init->RxDescNbr; i++)
        {
            if (READ_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC0_OWN))
            {
                if (desc_nbr == 0)
                    return HAL_OK;
                break;
            }

            desc0 = READ_REG(rx_desc->DESC0);
            
            if (READ_BIT(desc0, ETH_DMA_RX_DESC0_LS))
            {
                if (READ_BIT(desc0, ETH_DMA_RX_DESC0_FS))
                {
                    if (desc_nbr)
                        break;
                    desc_nbr = 1;
                    
                    first_desc_index = desc_index;
                    packet_len = (desc0 & ETH_DMA_RX_DESC0_FL) >> ETH_DMA_RX_DESC0_FL_Pos;
                    
                    if (init->RxDescListMode == ETH_DESC_LIST_MODE_RING)
                        first_rx_buff_index = desc_index << 1;
                    else
                        first_rx_buff_index = desc_index;
                }
                else
                {
                    desc_nbr++;
                    if (desc_nbr == 1)
                        break;
                    
                    packet_len = (desc0 & ETH_DMA_RX_DESC0_FL) >> ETH_DMA_RX_DESC0_FL_Pos;
                    if (packet_len < len)
                        break;
                    packet_len -= len;
                }
                
                if (init->RxDescListMode == ETH_DESC_LIST_MODE_RING)
                {
                    if (packet_len > (init->RxBuffLen << 1))
                        break;
                    
                    curr_rx_buff_index = desc_index << 1;
                    
                    if (packet_len <= init->RxBuffLen)
                    {
                        rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff1;
                        rx_buff[curr_rx_buff_index].Len = packet_len;
                        rx_buff[curr_rx_buff_index].next = 0;
                    }
                    else
                    {
                        packet_len -= init->RxBuffLen;
                        
                        next_rx_buff_index = curr_rx_buff_index;
                        next_rx_buff_index++;
                        if (next_rx_buff_index >= init->RxBuffNodeNbr)
                            next_rx_buff_index = 0;
                        
                        rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff1;
                        rx_buff[curr_rx_buff_index].Len = init->RxBuffLen;
                        rx_buff[curr_rx_buff_index].next = &rx_buff[next_rx_buff_index];
                    
                        rx_buff[next_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff2;
                        rx_buff[next_rx_buff_index].Len = packet_len;
                        rx_buff[next_rx_buff_index].next = 0;
                    }
                }
                else
                {
                    if (packet_len > init->RxBuffLen)
                        break;
                    
                    rx_buff[desc_index].Buff = (uint8_t *)rx_desc->Buff1;
                    rx_buff[desc_index].Len = packet_len;
                    rx_buff[desc_index].next = 0;
                }
                
                desc_index++;
                if (desc_index >= init->RxDescNbr)
                    desc_index = 0;
                
                rx_desc_list->CurRxDesc = desc_index;
                rx_desc_list->FirstRxDesc = first_desc_index;
                rx_desc_list->RxDescNbr = 1;
                
                *buff = &init->RxBuffTab[first_rx_buff_index];
                if (pStatus)
                {
                    pStatus->Status0 = rx_desc->DESC0;
                    if (READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_EDFE))
                    {
                        if (READ_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC0_TSV))
                        {
                            pStatus->TimestampValid = ENABLE;
                            pStatus->Timestamp.sec = rx_desc->DESC7;
                            pStatus->Timestamp.nsec = rx_desc->DESC6;
                        }
                    }
                }
                return HAL_OK;
            }
            else if (READ_BIT(desc0, ETH_DMA_RX_DESC0_FS))
            {
                if (desc_nbr)
                    break;
                desc_nbr = 1;
                
                first_desc_index = desc_index;
                len = 0;
                
                if (init->RxDescListMode == ETH_DESC_LIST_MODE_RING)
                {
                    len += init->RxBuffLen << 1;
                    
                    curr_rx_buff_index = desc_index << 1;
                    
                    next_rx_buff_index = curr_rx_buff_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff1;
                    rx_buff[curr_rx_buff_index].Len = init->RxBuffLen;
                    rx_buff[curr_rx_buff_index].next = &rx_buff[next_rx_buff_index];
                    
                    curr_rx_buff_index = next_rx_buff_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff2;
                    rx_buff[curr_rx_buff_index].Len = init->RxBuffLen;
                    rx_buff[curr_rx_buff_index].next = &rx_buff[next_rx_buff_index];
                }
                else
                {
                    len += init->RxBuffLen;
                    
                    next_rx_buff_index = desc_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[desc_index].Buff = (uint8_t *)rx_desc->Buff1;
                    rx_buff[desc_index].Len = init->RxBuffLen;
                    rx_buff[desc_index].next = &rx_buff[next_rx_buff_index];
                }
                
                desc_index++;
                if (desc_index >= init->RxDescNbr)
                    desc_index = 0;
                rx_desc = &rx_desc_list->RxDescTab[desc_index];
            }
            else
            {
                desc_nbr++;
                if (desc_nbr == 1)
                    break;
                
                if (init->RxDescListMode == ETH_DESC_LIST_MODE_RING)
                {
                    len += init->RxBuffLen << 1;
                    
                    curr_rx_buff_index = desc_index << 1;
                    
                    next_rx_buff_index = curr_rx_buff_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff1;
                    rx_buff[curr_rx_buff_index].Len = init->RxBuffLen;
                    rx_buff[curr_rx_buff_index].next = &rx_buff[next_rx_buff_index];
                    
                    curr_rx_buff_index = next_rx_buff_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[curr_rx_buff_index].Buff = (uint8_t *)rx_desc->Buff2;
                    rx_buff[curr_rx_buff_index].Len = init->RxBuffLen;
                    rx_buff[curr_rx_buff_index].next = &rx_buff[next_rx_buff_index];
                }
                else
                {
                    len += init->RxBuffLen;
                    
                    next_rx_buff_index = desc_index;
                    next_rx_buff_index++;
                    if (next_rx_buff_index >= init->RxBuffNodeNbr)
                        next_rx_buff_index = 0;
                    
                    rx_buff[desc_index].Buff = (uint8_t *)rx_desc->Buff1;
                    rx_buff[desc_index].Len = init->RxBuffLen;
                    rx_buff[desc_index].next = &rx_buff[next_rx_buff_index];
                }
                
                desc_index++;
                if (desc_index >= init->RxDescNbr)
                    desc_index = 0;
                rx_desc = &rx_desc_list->RxDescTab[desc_index];
            }
        }

        desc_index = rx_desc_list->CurRxDesc;
        rx_desc = &rx_desc_list->RxDescTab[desc_index];

        while (desc_nbr--)
        {
            WRITE_REG(rx_desc->DESC2, rx_desc->Buff1);
            WRITE_REG(rx_desc->DESC3, rx_desc->Buff2);
            
            SET_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC0_OWN);
            
            desc_index++;
            if (desc_index >= init->RxDescNbr)
                desc_index = 0;
            rx_desc = &rx_desc_list->RxDescTab[desc_index];
        }
        
        rx_desc_list->CurRxDesc = desc_index;
        rx_desc_list->FirstRxDesc = 0;
        rx_desc_list->RxDescNbr = 0;
    }
}

/******************************************************************************
*@brief : Release DMA receive descriptor.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReleaseRxDescriptors(ETH_HandleTypeDef *heth)
{
    uint32_t desc_nbr;
    uint32_t desc_index;
    ETH_InitTypeDef *init;
    ETH_DMADescTypeDef *rx_desc;
    ETH_RxDescListTypeDef *rx_desc_list;

    rx_desc_list = &heth->RxDescList;
    desc_nbr = rx_desc_list->RxDescNbr;
    
    if(desc_nbr == 0U)
        return HAL_OK;
    
    init = &heth->Init;
    
    desc_index = rx_desc_list->FirstRxDesc;
    
    while (desc_nbr--)
    {
        rx_desc = &rx_desc_list->RxDescTab[desc_index];
        
        WRITE_REG(rx_desc->DESC2, rx_desc->Buff1);
        WRITE_REG(rx_desc->DESC3, rx_desc->Buff2);
        
        SET_BIT(rx_desc->DESC0, ETH_DMA_RX_DESC0_OWN);
        
        desc_index++;
        if (desc_index >= init->RxDescNbr)
            desc_index = 0;
    }
    
    rx_desc_list->CurRxDesc = desc_index;
    rx_desc_list->FirstRxDesc = 0;
    rx_desc_list->RxDescNbr = 0;

    return HAL_OK;
}

/******************************************************************************
*@brief : Enables Ethernet MAC and DMA reception/transmission in Interrupt mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_Start(ETH_HandleTypeDef *heth)
{
    /* Enable the MAC transmission */
    heth->Instance->MACCR |= ETH_MACCR_TE;

    /* Enable the MAC reception */
    heth->Instance->MACCR |= ETH_MACCR_RE;

    /* Set the Flush Transmit FIFO bit */
    heth->Instance->DMAOMR |= ETH_DMAOMR_FTF;

    /* Enable the DMA transmission */
    heth->Instance->DMAOMR |= ETH_DMAOMR_ST;

    /* Enable the DMA reception */
    heth->Instance->DMAOMR |= ETH_DMAOMR_SR;  

    HAL_ETH_ITConfig(heth);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Stop Ethernet MAC and DMA reception/transmission.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_Stop(ETH_HandleTypeDef *heth)
{
    /* Disable the DMA transmission */
    heth->Instance->DMAOMR &= ~ETH_DMAOMR_ST;

    /* Disable the DMA reception */
    heth->Instance->DMAOMR &= ~ETH_DMAOMR_SR;

    /* Disable the MAC reception */
    heth->Instance->MACCR &= ~ETH_MACCR_RE;

    /* Set the Flush Transmit FIFO bit */
    heth->Instance->DMAOMR |= ETH_DMAOMR_FTF;

    /* Disable the MAC transmission */
    heth->Instance->MACCR &= ~ETH_MACCR_TE;

    return HAL_OK;
}
    
//  ****************************************************************************
//  
//                              Interrupt
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Ethernet interrupt config.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
__attribute__((weak)) HAL_StatusTypeDef HAL_ETH_ITConfig(ETH_HandleTypeDef *heth)
{
    if (heth->IT.TxComplete == DISABLE)
    {
        heth->Instance->DMAIER &= ~ETH_DMAIER_TIE;
    }
    else
    {
        heth->Instance->DMAIER |= ETH_DMAIER_TIE | ETH_DMAIER_NISE;
    }
    
    if (heth->IT.RxComplete == DISABLE)
    {
        heth->Instance->DMAIER &= ~ETH_DMAIER_RIE;
    }
    else
    {
        heth->Instance->DMAIER |= ETH_DMAIER_RIE | ETH_DMAIER_NISE;
    }
    
    if ((heth->IT.TxComplete == DISABLE) && (heth->IT.RxComplete == DISABLE))
    {
        heth->Instance->DMAIER &= ~ETH_DMAIER_NISE;
    }
    
    if (heth->IT.Error == DISABLE)
    {
        heth->Instance->DMAIER &= ~ETH_DMAIER_AISE;
    }
    else
    {
        heth->Instance->DMAIER |= 0 | ETH_DMAIER_AISE;
    }
    
    #ifdef ETH_MMC_SUPPORT
    if (heth->IT.MMCTx == DISABLE)
    {
        heth->Instance->MMCTIMR |= ETH_MMCTIMR_TGFIM | ETH_MMCTIMR_TMCGFCIM | ETH_MMCTIMR_TSCGFCIM;
    }
    else
    {
        heth->Instance->MMCTIMR &= ~(ETH_MMCTIMR_TGFIM | ETH_MMCTIMR_TMCGFCIM | ETH_MMCTIMR_TSCGFCIM);
    }
    
    if (heth->IT.MMCRx == DISABLE)
    {
        heth->Instance->MMCRIMR |= ETH_MMCRIMR_RGUFIM | ETH_MMCRIMR_RFAEIM | ETH_MMCRIMR_RFCEIM;
    }
    else
    {
        heth->Instance->MMCRIMR &= ~(ETH_MMCRIMR_RGUFIM | ETH_MMCRIMR_RFAEIM | ETH_MMCRIMR_RFCEIM);
    }
    #else
    if (1)
    {
        heth->Instance->MMCTIMR |= ETH_MMCTIMR_TGFIM | ETH_MMCTIMR_TMCGFCIM | ETH_MMCTIMR_TSCGFCIM;
        heth->Instance->MMCRIMR |= ETH_MMCRIMR_RGUFIM | ETH_MMCRIMR_RFAEIM | ETH_MMCRIMR_RFCEIM;
    }
    #endif
    
    #ifdef ETH_LPI_SUPPORT
    if (heth->IT.LPI == DISABLE)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_LPIIM;
    }
    else
    {
        heth->Instance->MACIMR &= ~ETH_MACIMR_LPIIM;
    }
    #else
    if (1)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_LPIIM;
    }
    #endif
    
    #ifdef ETH_PTP_SUPPORT
    if (heth->IT.PTP == DISABLE)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_TIM;
    }
    else
    {
        heth->Instance->MACIMR &= ~ETH_MACIMR_TIM;
    }
    #else
    if (1)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_TIM;
    }
    #endif
    
    
    #ifdef ETH_PMT_SUPPORT
    if (heth->IT.PMT == DISABLE)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_PIM;
    }
    else
    {
        heth->Instance->MACIMR &= ~ETH_MACIMR_PIM;
    }
    #else
    if (1)
    {
        heth->Instance->MACIMR |= ETH_MACIMR_PIM;
    }
    #endif
    return HAL_OK;
}
    
//  ****************************************************************************
//  
//                              Configuration Parameters
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialize the Ethernet configuration parameters to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: None.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitDefaultParamter(ETH_HandleTypeDef *heth)
{
    memset((void *)heth, 0, sizeof(ETH_HandleTypeDef));
    
    heth->Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    heth->Init.AutoNegotiation = ENABLE;
    heth->Init.Speed = ETH_SPEED_100M;
    heth->Init.DuplexMode = ETH_MODE_FULL_DUPLEX;
    heth->Init.AutoCRC = ENABLE;
    heth->Init.AutoPad = ENABLE;
    heth->Init.AutoChecksumInsertion = ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD_PAYLOAD;
    heth->Init.PhyAddress = 0;
    
    HAL_ETH_InitMACAddrDefaultParamter(heth);
    
    // Filter
    HAL_ETH_InitFilterDefaultParamter(heth);
    
    // MMC
    HAL_ETH_InitMMCDefaultParamter(heth);
    
    // PMT
    HAL_ETH_InitPMTDefaultParamter(heth);
    
    // PTP
    HAL_ETH_InitPTPDefaultParamter(heth);
    
    // LPI
    HAL_ETH_InitLPIDefaultParamter(heth);
    
    // MAC
    HAL_ETH_InitMACDefaultParamter(heth);
    
    // DMA
    HAL_ETH_InitDMADefaultParamter(heth);
    
    return HAL_OK;
}
    
//  ****************************************************************************
//  
//                              Speed
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization the speed to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitSpeedDefaultParamter(ETH_HandleTypeDef *heth)
{
    heth->Init.Speed = ETH_SPEED_100M;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the speed parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetSpeedParameter(ETH_HandleTypeDef *heth)
{
    if (heth->Instance->MACCR & ETH_MACCR_FES)
        heth->Init.Speed = ETH_SPEED_100M;
    else
        heth->Init.Speed = ETH_SPEED_10M;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration speed. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_SpeedConfig(ETH_HandleTypeDef *heth)
{
    assert_param(IS_ETH_SPEED(heth->Init.Speed));
    
    if (heth->Init.Speed == ETH_SPEED_100M)
        heth->Instance->MACCR |= ETH_MACCR_FES;
    else
        heth->Instance->MACCR &= ~ETH_MACCR_FES;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration speed to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_SpeedDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitSpeedDefaultParamter(heth);
    HAL_ETH_SpeedConfig(heth);
    
    return HAL_OK;
}

//  ****************************************************************************
//  
//                              Duplex Mode
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization duplex mode to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitDuplexModeDefaultParamter(ETH_HandleTypeDef *heth)
{
    heth->Init.DuplexMode = ETH_MODE_FULL_DUPLEX;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the duplex mode parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetDuplexModeParameter(ETH_HandleTypeDef *heth)
{
    if (heth->Instance->MACCR & ETH_MACCR_DM)
        heth->Init.DuplexMode = ETH_MODE_FULL_DUPLEX;
    else
        heth->Init.DuplexMode = ETH_MODE_HALF_DUPLEX;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration duplex mode. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DuplexModeConfig(ETH_HandleTypeDef *heth)
{
    assert_param(IS_ETH_SPEED(heth->Init.DuplexMode));
    
    if (heth->Init.DuplexMode != ETH_MODE_HALF_DUPLEX)
        heth->Instance->MACCR |= ETH_MACCR_DM;
    else
        heth->Instance->MACCR &= ~ETH_MACCR_DM;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration duplex mode to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DuplexModeDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitDuplexModeDefaultParamter(heth);
    HAL_ETH_DuplexModeConfig(heth);
    
    return HAL_OK;
}
  
    
//  ****************************************************************************
//  
//                              MAC Address
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization MAC address to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitMACAddrDefaultParamter(ETH_HandleTypeDef *heth)
{
    memset((void *)heth->Init.MACAddr, 0xffu, 6);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the MAC address parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetMACAddrParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    tmpreg = heth->Instance->MACA0HR;
    heth->Init.MACAddr[5] = (tmpreg >> 8) & 0xffu;
    heth->Init.MACAddr[4] = (tmpreg >> 0) & 0xffu;
    
    tmpreg = heth->Instance->MACA0LR;
    heth->Init.MACAddr[3] = (tmpreg >> 24) & 0xffu;
    heth->Init.MACAddr[2] = (tmpreg >> 16) & 0xffu;
    heth->Init.MACAddr[1] = (tmpreg >> 8) & 0xffu;
    heth->Init.MACAddr[0] = (tmpreg >> 0) & 0xffu;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration MAC address. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MACAddrConfig(ETH_HandleTypeDef *heth)
{
    heth->Instance->MACA0HR = ETH_MACA0HR_AE | \
                               ((uint32_t)heth->Init.MACAddr[5] << 8) | \
                               ((uint32_t)heth->Init.MACAddr[4] << 0);
    
    // MACA1LR
    heth->Instance->MACA0LR = ((uint32_t)heth->Init.MACAddr[3] << 24) | \
                               ((uint32_t)heth->Init.MACAddr[2] << 16) | \
                               ((uint32_t)heth->Init.MACAddr[1] << 8) | \
                               ((uint32_t)heth->Init.MACAddr[0] << 0);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration MAC address to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MACAddrDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitMACAddrDefaultParamter(heth);
    HAL_ETH_MACAddrConfig(heth);
    
    return HAL_OK;
}


//	****************************************************************************
//  
//                              MAC
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization MAC parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitMACDefaultParamter(ETH_HandleTypeDef *heth)
{
    heth->MACConfig.SourceAddrControl = ETH_SOURCE_ADDRESS_REPLACE_ADDR0;
    heth->MACConfig.Support2KPacket = ENABLE;
    heth->MACConfig.CRCStripTypePacket = ENABLE;
    heth->MACConfig.Watchdog = DISABLE;
    heth->MACConfig.WatchdogTimeout = 1523U;
    heth->MACConfig.Jabber = DISABLE;
    heth->MACConfig.JumboPacket = ENABLE;
    heth->MACConfig.InterFrameGap = ETH_INTERFRAME_GAP_96BIT;
    heth->MACConfig.CarrierSense = DISABLE;
    heth->MACConfig.ReceiveOwn = ENABLE;
    heth->MACConfig.LoopbackMode = DISABLE;
    heth->MACConfig.ChecksumOffload = ENABLE;
    heth->MACConfig.RetryTransmission = ENABLE;
    heth->MACConfig.AutoPadCRCStrip = ENABLE;
    heth->MACConfig.BackOffLimit = ETH_BACK_OFF_LIMIT_10;
    heth->MACConfig.DeferralCheck = DISABLE;
    heth->MACConfig.PreambleLen = ETH_PREAMBLE_LEN_7B;

    HAL_ETH_InitVLANDefaultParamter(heth);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the MAC parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetMACParameter(ETH_HandleTypeDef *heth)
{
    uint32_t err;
    volatile uint32_t tmpreg = 0;
    ETH_MACInitTypeDef *pMACConfig = &heth->MACConfig;

    if (heth == NULL)
        return (HAL_ERROR);

    memset((void *)pMACConfig, 0, sizeof(ETH_MACInitTypeDef));
    err = 0;
    
    // MACCR
    tmpreg = heth->Instance->MACCR;
    
    if ((tmpreg & ETH_MACCR_SARC_1) == 0)
        pMACConfig->SourceAddrControl = ETH_SOURCE_ADDRESS_DISABLE;
    else if ((tmpreg & ETH_MACCR_SARC) == (ETH_MACCR_SARC_1))
        pMACConfig->SourceAddrControl = ETH_SOURCE_ADDRESS_INSERT_ADDR0;
    else if ((tmpreg & ETH_MACCR_SARC) == (ETH_MACCR_SARC_2 | ETH_MACCR_SARC_1))
        pMACConfig->SourceAddrControl = ETH_SOURCE_ADDRESS_INSERT_ADDR1;
    else if ((tmpreg & ETH_MACCR_SARC) == (ETH_MACCR_SARC_1 | ETH_MACCR_SARC_0))
        pMACConfig->SourceAddrControl = ETH_SOURCE_ADDRESS_REPLACE_ADDR0;
    else if ((tmpreg & ETH_MACCR_SARC) == (ETH_MACCR_SARC_2 | ETH_MACCR_SARC_1 | ETH_MACCR_SARC_0))
        pMACConfig->SourceAddrControl = ETH_SOURCE_ADDRESS_REPLACE_ADDR1;
    
    if (tmpreg & ETH_MACCR_S2KP)
        pMACConfig->Support2KPacket = ENABLE;
    else
        pMACConfig->Support2KPacket = DISABLE;
    
    if (tmpreg & ETH_MACCR_CSTF)
        pMACConfig->CRCStripTypePacket = ENABLE;
    else
        pMACConfig->CRCStripTypePacket = DISABLE;
    
    if (((tmpreg & ETH_MACCR_WD) == 0) && (heth->Instance->MACWTR & ETH_MACWTR_PWE))
        pMACConfig->Watchdog = ENABLE;
    else
        pMACConfig->Watchdog = DISABLE;
    
    if (tmpreg & ETH_MACCR_JD)
        pMACConfig->Jabber = ENABLE;
    else
        pMACConfig->Jabber = DISABLE;
    
    if (tmpreg & ETH_MACCR_JE)
        pMACConfig->JumboPacket = ENABLE;
    else
        pMACConfig->JumboPacket = DISABLE;
    
    if ((tmpreg & ETH_MACCR_IFG) == 0)
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_96BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == ETH_MACCR_IFG_0)
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_88BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == ETH_MACCR_IFG_1)
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_80BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == (ETH_MACCR_IFG_1 | ETH_MACCR_IFG_0))
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_72BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == ETH_MACCR_IFG_2)
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_64BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == (ETH_MACCR_IFG_2 | ETH_MACCR_IFG_0))
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_56BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == (ETH_MACCR_IFG_2 | ETH_MACCR_IFG_1))
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_48BIT;
    else if ((tmpreg & ETH_MACCR_IFG) == (ETH_MACCR_IFG_2 | ETH_MACCR_IFG_1 | ETH_MACCR_IFG_0))
        pMACConfig->InterFrameGap = ETH_INTERFRAME_GAP_40BIT;
    
    if (tmpreg & ETH_MACCR_CSD)
        pMACConfig->JumboPacket = DISABLE;
    else
        pMACConfig->JumboPacket = ENABLE;
    
    if (tmpreg & ETH_MACCR_ROD)
        pMACConfig->ReceiveOwn = DISABLE;
    else
        pMACConfig->ReceiveOwn = ENABLE;
    
    if (tmpreg & ETH_MACCR_LM)
        pMACConfig->LoopbackMode = ENABLE;
    else
        pMACConfig->LoopbackMode = DISABLE;
    
    if (tmpreg & ETH_MACCR_IPCO)
        pMACConfig->ChecksumOffload = ENABLE;
    else
        pMACConfig->ChecksumOffload = DISABLE;
    
    if (tmpreg & ETH_MACCR_DR)
        pMACConfig->RetryTransmission = DISABLE;
    else
        pMACConfig->RetryTransmission = ENABLE;
    
    if (tmpreg & ETH_MACCR_APCS)
        pMACConfig->AutoPadCRCStrip = ENABLE;
    else
        pMACConfig->AutoPadCRCStrip = DISABLE;
    
    if ((tmpreg & ETH_MACCR_BL) == 0)
        pMACConfig->BackOffLimit = ETH_BACK_OFF_LIMIT_10;
    else if ((tmpreg & ETH_MACCR_BL) == ETH_MACCR_BL_0)
        pMACConfig->BackOffLimit = ETH_BACK_OFF_LIMIT_8;
    else if ((tmpreg & ETH_MACCR_BL) == ETH_MACCR_BL_1)
        pMACConfig->BackOffLimit = ETH_BACK_OFF_LIMIT_4;
    else if ((tmpreg & ETH_MACCR_BL) == (ETH_MACCR_BL_1 | ETH_MACCR_BL_0))
        pMACConfig->BackOffLimit = ETH_BACK_OFF_LIMIT_1;
    
    if (tmpreg & ETH_MACCR_DC)
        pMACConfig->DeferralCheck = ENABLE;
    else
        pMACConfig->DeferralCheck = DISABLE;
    
    if ((tmpreg & ETH_MACCR_PRELEN) == 0)
        pMACConfig->PreambleLen = ETH_PREAMBLE_LEN_7B;
    else if ((tmpreg & ETH_MACCR_BL) == ETH_MACCR_PRELEN_0)
        pMACConfig->PreambleLen = ETH_PREAMBLE_LEN_5B;
    else if ((tmpreg & ETH_MACCR_BL) == ETH_MACCR_PRELEN_1)
        pMACConfig->PreambleLen = ETH_PREAMBLE_LEN_3B;
    else
        err = 1;
    
    // MACWTR
    pMACConfig->WatchdogTimeout = (heth->Instance->MACWTR & ETH_MACWTR_WTO) >> ETH_MACWTR_WTO_Pos;
    
    err |= HAL_ETH_GetVLANParameter(heth);
    
    if (err == 0)
        return HAL_OK;
    else
        return HAL_ERROR;
}

/******************************************************************************
*@brief : Configuration the MAC parameter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MACConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg = 0;
    ETH_MACInitTypeDef *pMACConfig = &heth->MACConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    assert_param(IS_ETH_SOURCE_ADDR_CONTROL(pMACConfig->SourceAddrControl));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->Support2KPacket));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->CRCStripTypePacket));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->Watchdog));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->Jabber));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->JumboPacket));
    assert_param(IS_ETH_INTERFRAME_GAP(pMACConfig->InterFrameGap));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->CarrierSense));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->ReceiveOwn));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->LoopbackMode));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->ChecksumOffload));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->RetryTransmission));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->AutoPadCRCStrip));
    assert_param(IS_ETH_BACK_OFF_LIMIT(pMACConfig->BackOffLimit));
    assert_param(IS_FUNCTIONAL_STATE(pMACConfig->DeferralCheck));
    assert_param(IS_ETH_PREAMBLE_LEN(pMACConfig->PreambleLen));

    // MACCR
    tmpreg = heth->Instance->MACCR;
    tmpreg &= 0x8520c90c;

    if (pMACConfig->SourceAddrControl == ETH_SOURCE_ADDRESS_DISABLE)
        tmpreg |= 0U;
    if (pMACConfig->SourceAddrControl == ETH_SOURCE_ADDRESS_INSERT_ADDR0)
        tmpreg |= ETH_MACCR_SARC_1;
    if (pMACConfig->SourceAddrControl == ETH_SOURCE_ADDRESS_INSERT_ADDR1)
        tmpreg |= ETH_MACCR_SARC_2 | ETH_MACCR_SARC_1;
    if (pMACConfig->SourceAddrControl == ETH_SOURCE_ADDRESS_REPLACE_ADDR0)
        tmpreg |= ETH_MACCR_SARC_1 | ETH_MACCR_SARC_0;
    if (pMACConfig->SourceAddrControl == ETH_SOURCE_ADDRESS_REPLACE_ADDR1)
        tmpreg |= ETH_MACCR_SARC_2 | ETH_MACCR_SARC_1 | ETH_MACCR_SARC_0;
    
    if (pMACConfig->Support2KPacket != DISABLE)
        tmpreg |= ETH_MACCR_S2KP;
    
    if (pMACConfig->CRCStripTypePacket != DISABLE)
        tmpreg |= ETH_MACCR_CSTF;
    
    if (pMACConfig->Watchdog == DISABLE)
        tmpreg |= ETH_MACCR_WD;
        
    if (pMACConfig->Jabber != DISABLE)
        tmpreg |= ETH_MACCR_JD;
    
    if (pMACConfig->JumboPacket != DISABLE)
        tmpreg |= ETH_MACCR_JE;
    
    if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_96BIT)
        tmpreg |= 0U;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_88BIT)
        tmpreg |= ETH_MACCR_IFG_0;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_80BIT)
        tmpreg |= ETH_MACCR_IFG_1;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_72BIT)
        tmpreg |= ETH_MACCR_IFG_1 | ETH_MACCR_IFG_0;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_64BIT)
        tmpreg |= ETH_MACCR_IFG_2;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_56BIT)
        tmpreg |= ETH_MACCR_IFG_2 | ETH_MACCR_IFG_0;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_48BIT)
        tmpreg |= ETH_MACCR_IFG_2 | ETH_MACCR_IFG_1;
    else if (pMACConfig->InterFrameGap == ETH_INTERFRAME_GAP_40BIT)
        tmpreg |= ETH_MACCR_IFG_2 | ETH_MACCR_IFG_1 | ETH_MACCR_IFG_0;
    
    if (pMACConfig->CarrierSense == DISABLE)
        tmpreg |= ETH_MACCR_CSD;
    
    if (pMACConfig->ReceiveOwn == DISABLE)
        tmpreg |= ETH_MACCR_ROD;
    
    if (pMACConfig->LoopbackMode != DISABLE)
        tmpreg |= ETH_MACCR_LM;
    
    if (pMACConfig->ChecksumOffload != DISABLE)
        tmpreg |= ETH_MACCR_IPCO;
    
    if (pMACConfig->RetryTransmission == DISABLE)
        tmpreg |= ETH_MACCR_DR;
    
    if (pMACConfig->AutoPadCRCStrip != DISABLE)
        tmpreg |= ETH_MACCR_APCS;
    
    if (pMACConfig->BackOffLimit == ETH_BACK_OFF_LIMIT_10)
        tmpreg |= 0U;
    else if (pMACConfig->BackOffLimit == ETH_BACK_OFF_LIMIT_8)
        tmpreg |= ETH_MACCR_BL_0;
    else if (pMACConfig->BackOffLimit == ETH_BACK_OFF_LIMIT_4)
        tmpreg |= ETH_MACCR_BL_1;
    else if (pMACConfig->BackOffLimit == ETH_BACK_OFF_LIMIT_1)
        tmpreg |= ETH_MACCR_BL_1 | ETH_MACCR_BL_1;
    
    if (pMACConfig->DeferralCheck != DISABLE)
        tmpreg |= ETH_MACCR_DC;
    
    if (pMACConfig->PreambleLen == ETH_PREAMBLE_LEN_7B)
        tmpreg |= 0U;
    else if (pMACConfig->PreambleLen == ETH_PREAMBLE_LEN_5B)
        tmpreg |= ETH_MACCR_PRELEN_0;
    else if (pMACConfig->PreambleLen == ETH_PREAMBLE_LEN_3B)
        tmpreg |= ETH_MACCR_PRELEN_1;
    
    heth->Instance->MACCR = tmpreg;

    // MACWTR
    if (pMACConfig->Watchdog)
        heth->Instance->MACWTR = ETH_MACWTR_PWE | ((pMACConfig->WatchdogTimeout << ETH_MACWTR_WTO_Pos) & ETH_MACWTR_WTO);
    else
        heth->Instance->MACWTR = (pMACConfig->WatchdogTimeout << ETH_MACWTR_WTO_Pos) & ETH_MACWTR_WTO;
    
    // VLAN
    HAL_ETH_VLANConfig(heth);
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the MAC parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MACDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitMACDefaultParamter(heth);
    
    return HAL_ETH_MACConfig(heth);
}

//	****************************************************************************
//  
//                              DMA
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization the DMA parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitDMADefaultParamter(ETH_HandleTypeDef *heth)
{
    memset((void *)&heth->DMAConfig, 0, sizeof(ETH_DMAInitTypeDef));
    
    // DMA?????
    heth->DMAConfig.DropTCPIPChecksumErrorFrame = ENABLE;
    heth->DMAConfig.ReceiveStoreForward = ENABLE;
    heth->DMAConfig.FlushReceivedFrame = ENABLE;
    heth->DMAConfig.TransmitStoreForward = ENABLE;
    heth->DMAConfig.TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_64BYTES;
    heth->DMAConfig.ForwardErrorFrames = DISABLE;
    heth->DMAConfig.ForwardUndersizedGoodFrames = DISABLE;
    heth->DMAConfig.ReceiveThresholdControl = ETH_RECEIVED_THRESHOLD_CONTROL_64BYTES;
    heth->DMAConfig.SecondFrameOperate = ENABLE;

    heth->DMAConfig.MixedBurst = DISABLE;
    heth->DMAConfig.AddressAlignedBeats = ENABLE;
    heth->DMAConfig.PBL8xMode = DISABLE;
    heth->DMAConfig.UseSeparatePBL = ENABLE;
    heth->DMAConfig.RxDMABurstLen = ETH_RX_DMA_BURST_LEN_32BEAT;
    heth->DMAConfig.FixedBurst = ENABLE;
    heth->DMAConfig.RxTxPriorityRatio = ETH_RX_TX_PRIORITY_RATIO_4_1;
    heth->DMAConfig.TxDMABurstLen = ETH_TX_DMA_BURST_LEN_32BEAT;
    heth->DMAConfig.EnhancedDescriptorFormat = ENABLE;
    heth->DMAConfig.DescriptorSkipLen = 0x02;
    heth->DMAConfig.DMAArbitration = ETH_DMA_ARBITRATION_ROUNDROBIN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the DMA parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetDMAParameter(ETH_HandleTypeDef *heth)
{
    uint32_t err;
    volatile uint32_t tmpreg = 0;
    ETH_DMAInitTypeDef *pDMAConfig = &heth->DMAConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    err = 0;
    
    // DMAOMR
    tmpreg = heth->Instance->DMAOMR;
    
    if (tmpreg & ETH_DMAOMR_DTCEFD)
        pDMAConfig->DropTCPIPChecksumErrorFrame = ENABLE;
    else
        pDMAConfig->DropTCPIPChecksumErrorFrame = DISABLE;
    
    if (tmpreg & ETH_DMAOMR_RSF)
        pDMAConfig->ReceiveStoreForward = ENABLE;
    else
        pDMAConfig->ReceiveStoreForward = DISABLE;

    if (tmpreg & ETH_DMAOMR_DFRF)
        pDMAConfig->FlushReceivedFrame = DISABLE;
    else
        pDMAConfig->FlushReceivedFrame = ENABLE;
    
    if (tmpreg & ETH_DMAOMR_TSF)
        pDMAConfig->TransmitStoreForward = ENABLE;
    else
        pDMAConfig->TransmitStoreForward = DISABLE;

    if ((tmpreg & ETH_DMAOMR_TTC) == 0)
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_64BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == ETH_DMAOMR_TTC_0)
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_128BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == ETH_DMAOMR_TTC_1)
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_192BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == (ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0))
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_256BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == ETH_DMAOMR_TTC_2)
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_40BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == (ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_0))
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_32BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == (ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1))
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_24BYTES;
    else if ((tmpreg & ETH_DMAOMR_TTC) == (ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0))
        pDMAConfig->TransmitThresholdControl = ETH_TRANSMIT_THRESHOLD_CONTROL_16BYTES;
    
    if (tmpreg & ETH_DMAOMR_FEF)
        pDMAConfig->ForwardErrorFrames = ENABLE;
    else
        pDMAConfig->ForwardErrorFrames = DISABLE;
    
    if (tmpreg & ETH_DMAOMR_FUGF)
        pDMAConfig->ForwardUndersizedGoodFrames = ENABLE;
    else
        pDMAConfig->ForwardUndersizedGoodFrames = DISABLE;
    
    if ((tmpreg & ETH_DMAOMR_RTC) == 0)
        pDMAConfig->ReceiveThresholdControl = ETH_RECEIVED_THRESHOLD_CONTROL_64BYTES;
    else if ((tmpreg & ETH_DMAOMR_RTC) == ETH_DMAOMR_RTC_0)
        pDMAConfig->ReceiveThresholdControl = ETH_RECEIVED_THRESHOLD_CONTROL_32BYTES;
    else if ((tmpreg & ETH_DMAOMR_RTC) == ETH_DMAOMR_RTC_1)
        pDMAConfig->ReceiveThresholdControl = ETH_RECEIVED_THRESHOLD_CONTROL_96BYTES;
    else if ((tmpreg & ETH_DMAOMR_RTC) == (ETH_DMAOMR_RTC_1 | ETH_DMAOMR_RTC_0))
        pDMAConfig->ReceiveThresholdControl = ETH_RECEIVED_THRESHOLD_CONTROL_128BYTES;
    
    if (tmpreg & ETH_DMAOMR_OSF)
        pDMAConfig->SecondFrameOperate = ENABLE;
    else
        pDMAConfig->SecondFrameOperate = DISABLE;
    
    // DMABMR
    tmpreg = heth->Instance->DMABMR;
    
    if (tmpreg & ETH_DMABMR_MB)
        pDMAConfig->MixedBurst = ENABLE;
    else
        pDMAConfig->MixedBurst = DISABLE;
    
    if (tmpreg & ETH_DMABMR_AAB)
        pDMAConfig->AddressAlignedBeats = ENABLE;
    else
        pDMAConfig->AddressAlignedBeats = DISABLE;
    
    if (tmpreg & ETH_DMABMR_EPM)
        pDMAConfig->PBL8xMode = ENABLE;
    else
        pDMAConfig->PBL8xMode = DISABLE;
    
    if (tmpreg & ETH_DMABMR_USP)
        pDMAConfig->UseSeparatePBL = ENABLE;
    else
        pDMAConfig->UseSeparatePBL = DISABLE;
    
    if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_RDP_0)
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_1BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_RDP_1)
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_2BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_RDP_2)
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_4BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_RDP_3))
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_8BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_RDP_4))
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_16BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_RDP_5))
        pDMAConfig->RxDMABurstLen = ETH_RX_DMA_BURST_LEN_32BEAT;
    else
        err = 1;
    
    if (tmpreg & ETH_DMABMR_FB)
        pDMAConfig->FixedBurst = ENABLE;
    else
        pDMAConfig->FixedBurst = DISABLE;
    
    if ((tmpreg & ETH_DMABMR_PM) == 0)
        pDMAConfig->RxTxPriorityRatio = ETH_RX_TX_PRIORITY_RATIO_1_1;
    else if ((tmpreg & ETH_DMABMR_PM) == ETH_DMABMR_PM_0)
        pDMAConfig->RxTxPriorityRatio = ETH_RX_TX_PRIORITY_RATIO_2_1;
    else if ((tmpreg & ETH_DMABMR_PM) == ETH_DMABMR_PM_1)
        pDMAConfig->RxTxPriorityRatio = ETH_RX_TX_PRIORITY_RATIO_3_1;
    else if ((tmpreg & ETH_DMABMR_PM) == (ETH_DMABMR_PM_1 | ETH_DMABMR_PM_0))
        pDMAConfig->RxTxPriorityRatio = ETH_RX_TX_PRIORITY_RATIO_4_1;
    else
        err = 1;
    
    if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_PBL_0)
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_1BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_PBL_1)
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_2BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == ETH_DMABMR_PBL_2)
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_4BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_PBL_3))
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_8BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_PBL_4))
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_16BEAT;
    else if ((tmpreg & ETH_DMABMR_RDP) == (ETH_DMABMR_PBL_5))
        pDMAConfig->TxDMABurstLen = ETH_RX_DMA_BURST_LEN_32BEAT;
    else
        err = 1;
    
    if (tmpreg & ETH_DMABMR_EDFE)
        pDMAConfig->EnhancedDescriptorFormat = ENABLE;
    else 
        pDMAConfig->EnhancedDescriptorFormat = DISABLE;
    
    pDMAConfig->EnhancedDescriptorFormat = (tmpreg & ETH_DMABMR_EDFE) >> ETH_DMABMR_EDFE_Pos;
    
    pDMAConfig->DescriptorSkipLen = (tmpreg & ETH_DMABMR_DSL) >> ETH_DMABMR_DSL_Pos;
    
    if (tmpreg & ETH_DMABMR_DA)
        pDMAConfig->DMAArbitration = ETH_DMA_ARBITRATION_RX_PRIOR_TX;
    else
        pDMAConfig->DMAArbitration = ETH_DMA_ARBITRATION_ROUNDROBIN;
    
    if (err == 0)
        return HAL_OK;
    else
        return HAL_ERROR;
}

/******************************************************************************
*@brief : Configuration the DMA parameter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DMAConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg = 0;
    ETH_DMAInitTypeDef *pDMAConfig = &heth->DMAConfig;

    if (heth == NULL)
        return (HAL_ERROR);

    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->DropTCPIPChecksumErrorFrame));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->ReceiveStoreForward));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->FlushReceivedFrame));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->TransmitStoreForward));
    assert_param(IS_ETH_TRANSMIT_THRESHOLD_CONTROL(pDMAConfig->TransmitThresholdControl));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->ForwardErrorFrames));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->ForwardUndersizedGoodFrames));
    assert_param(IS_ETH_RECEIVED_THRESHOLD_CONTROL(pDMAConfig->ReceiveThresholdControl));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->SecondFrameOperate));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->MixedBurst));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->AddressAlignedBeats));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->PBL8xMode));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->UseSeparatePBL));
    assert_param(IS_ETH_RX_DMA_BURST_LEN(pDMAConfig->RxDMABurstLen));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->FixedBurst));
    assert_param(IS_ETH_RX_TX_PRIORITY_RATIO(pDMAConfig->RxTxPriorityRatio));
    assert_param(IS_ETH_TX_DMA_BURST_LEN(pDMAConfig->TxDMABurstLen));
    assert_param(IS_FUNCTIONAL_STATE(pDMAConfig->EnhancedDescriptorFormat));
    assert_param(IS_ETH_DESCRIPTOR_SKIP_LEN(pDMAConfig->DescriptorSkipLen));
    assert_param(IS_ETH_DMA_ARBITRATION(pDMAConfig->DMAArbitration));

    // DMAOMR
    tmpreg = heth->Instance->DMAOMR;
    tmpreg &= 0xF8DE3F23U;
    
    if (pDMAConfig->DropTCPIPChecksumErrorFrame != DISABLE)
        tmpreg |= ETH_DMAOMR_DTCEFD;
    
    if (pDMAConfig->ReceiveStoreForward != DISABLE)
        tmpreg |= ETH_DMAOMR_RSF;
    
    if (pDMAConfig->FlushReceivedFrame == DISABLE)
        tmpreg |= ETH_DMAOMR_DFRF;
    
    if (pDMAConfig->TransmitStoreForward != DISABLE)
        tmpreg |= ETH_DMAOMR_TSF;
    
    if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_64BYTES)
        tmpreg |= 0;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_128BYTES)
        tmpreg |= ETH_DMAOMR_TTC_0;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_192BYTES)
        tmpreg |= ETH_DMAOMR_TTC_1;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_256BYTES)
        tmpreg |= ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_40BYTES)
        tmpreg |= ETH_DMAOMR_TTC_2;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_32BYTES)
        tmpreg |= ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_0;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_24BYTES)
        tmpreg |= ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1;
    else if (pDMAConfig->TransmitThresholdControl == ETH_TRANSMIT_THRESHOLD_CONTROL_16BYTES)
        tmpreg |= ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0;
    
    if (pDMAConfig->ForwardErrorFrames != DISABLE)
        tmpreg |= ETH_DMAOMR_FEF;
    
    if (pDMAConfig->ForwardUndersizedGoodFrames != DISABLE)
        tmpreg |= ETH_DMAOMR_FUGF;

    if (pDMAConfig->ReceiveThresholdControl == ETH_RECEIVED_THRESHOLD_CONTROL_64BYTES)
        tmpreg |= 0;
    else if (pDMAConfig->ReceiveThresholdControl == ETH_RECEIVED_THRESHOLD_CONTROL_32BYTES)
        tmpreg |= ETH_DMAOMR_RTC_0;
    else if (pDMAConfig->ReceiveThresholdControl == ETH_RECEIVED_THRESHOLD_CONTROL_96BYTES)
        tmpreg |= ETH_DMAOMR_RTC_1;
    else if (pDMAConfig->ReceiveThresholdControl == ETH_RECEIVED_THRESHOLD_CONTROL_128BYTES)
        tmpreg |= ETH_DMAOMR_RTC_1 | ETH_DMAOMR_RTC_0;
    
    if (pDMAConfig->SecondFrameOperate != DISABLE)
        tmpreg |= ETH_DMAOMR_OSF;
    
    heth->Instance->DMAOMR = tmpreg;
    
    // DMABMR
    tmpreg = 0;
    
    if (pDMAConfig->MixedBurst != DISABLE)
        tmpreg |= ETH_DMABMR_MB;
    
    if (pDMAConfig->AddressAlignedBeats != DISABLE)
        tmpreg |= ETH_DMABMR_AAB;
    
    if (pDMAConfig->PBL8xMode != DISABLE)
        tmpreg |= ETH_DMABMR_EPM;
    
    if (pDMAConfig->UseSeparatePBL != DISABLE)
        tmpreg |= ETH_DMABMR_USP;
    
    if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_1BEAT)
        tmpreg |= ETH_DMABMR_RDP_0;
    else if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_2BEAT)
        tmpreg |= ETH_DMABMR_RDP_1;
    else if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_4BEAT)
        tmpreg |= ETH_DMABMR_RDP_2;
    else if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_8BEAT)
        tmpreg |= ETH_DMABMR_RDP_3;
    else if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_16BEAT)
        tmpreg |= ETH_DMABMR_RDP_4;
    else if (pDMAConfig->RxDMABurstLen == ETH_RX_DMA_BURST_LEN_32BEAT)
        tmpreg |= ETH_DMABMR_RDP_5;
    
    if (pDMAConfig->FixedBurst != DISABLE)
        tmpreg |= ETH_DMABMR_FB;
    
    if (pDMAConfig->RxTxPriorityRatio == ETH_RX_TX_PRIORITY_RATIO_1_1)
        tmpreg |= 0;
    else if (pDMAConfig->RxTxPriorityRatio == ETH_RX_TX_PRIORITY_RATIO_2_1)
        tmpreg |= ETH_DMABMR_PM_0;
    else if (pDMAConfig->RxTxPriorityRatio == ETH_RX_TX_PRIORITY_RATIO_3_1)
        tmpreg |= ETH_DMABMR_PM_1;
    else if (pDMAConfig->RxTxPriorityRatio == ETH_RX_TX_PRIORITY_RATIO_4_1)
        tmpreg |= ETH_DMABMR_PM_1 | ETH_DMABMR_PM_0;
    
    if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_1BEAT)
        tmpreg |= ETH_DMABMR_PBL_0;
    else if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_2BEAT)
        tmpreg |= ETH_DMABMR_PBL_1;
    else if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_4BEAT)
        tmpreg |= ETH_DMABMR_PBL_2;
    else if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_8BEAT)
        tmpreg |= ETH_DMABMR_PBL_3;
    else if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_16BEAT)
        tmpreg |= ETH_DMABMR_PBL_4;
    else if (pDMAConfig->TxDMABurstLen == ETH_TX_DMA_BURST_LEN_32BEAT)
        tmpreg |= ETH_DMABMR_PBL_5;
    
    if (pDMAConfig->EnhancedDescriptorFormat != DISABLE)
        tmpreg |= ETH_DMABMR_EDFE;
    
    tmpreg |= (pDMAConfig->DescriptorSkipLen << ETH_DMABMR_DSL_Pos) & ETH_DMABMR_DSL;
    
    if (pDMAConfig->DMAArbitration != ETH_DMA_ARBITRATION_ROUNDROBIN)
        tmpreg |= ETH_DMABMR_DA;
    else
        tmpreg |= 0;
    
    heth->Instance->DMABMR = (uint32_t)tmpreg;
    
    return HAL_OK; 
}

/******************************************************************************
*@brief : Configuration the DMA parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_DMADefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitDMADefaultParamter(heth);
    
    return HAL_ETH_DMAConfig(heth);
}

//  ****************************************************************************
//  
//                              All Filter
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization all filter parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitFilterDefaultParamter(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitAddrFilterDefaultParamter(heth);
    HAL_ETH_InitVLANFilterDefaultParamter(heth);
    HAL_ETH_InitL3L4FilterDefaultParamter(heth);
    heth->FilterConfig.DropNonTCPUDP = DISABLE;
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the all filter parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetFilterParameter(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);

    if (HAL_ETH_GetAddrFilterParameter(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (HAL_ETH_GetVLANFilterParameter(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (HAL_ETH_GetL3L4FilterParameter(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (heth->Instance->MACFFR & ETH_MACFFR_DNTU)
        heth->FilterConfig.DropNonTCPUDP = ENABLE;
    else
        heth->FilterConfig.DropNonTCPUDP = DISABLE;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the all filter parameter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_FilterConfig(ETH_HandleTypeDef *heth)
{
    assert_param(IS_FUNCTIONAL_STATE(heth->FilterConfig.DropNonTCPUDP));
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (HAL_ETH_AddrFilterConfig(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (HAL_ETH_VLANFilterConfig(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (HAL_ETH_L3L4FilterConfig(heth) != HAL_OK)
        return (HAL_ERROR);
    
    if (heth->FilterConfig.DropNonTCPUDP == DISABLE)
        heth->Instance->MACFFR &= ~ETH_MACFFR_DNTU;
    else
        heth->Instance->MACFFR |= ETH_MACFFR_DNTU;
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the all filter parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_FilterDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitFilterDefaultParamter(heth);
    
    return HAL_ETH_FilterConfig(heth);
}

//  ****************************************************************************
//  
//                              Address Filter
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization address filter parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitAddrFilterDefaultParamter(ETH_HandleTypeDef *heth)
{
    ETH_AddrFilterTypeDef *pAddrFilterConfig = &heth->FilterConfig.Addr;
    
    // address filter
    pAddrFilterConfig->Addr[0].Enable = DISABLE;
    pAddrFilterConfig->Addr[0].GroupFilter = DISABLE;
    pAddrFilterConfig->Addr[0].PerfectSourAddr = DISABLE;
    pAddrFilterConfig->Addr[0].GroupMask = 0;
    pAddrFilterConfig->Addr[0].Addr[0] = 0xFFU;
    pAddrFilterConfig->Addr[0].Addr[1] = 0xFFU;
    pAddrFilterConfig->Addr[0].Addr[2] = 0xFFU;
    pAddrFilterConfig->Addr[0].Addr[3] = 0xFFU;
    pAddrFilterConfig->Addr[0].Addr[4] = 0xFFU;
    pAddrFilterConfig->Addr[0].Addr[5] = 0xFFU;
    pAddrFilterConfig->Addr[1].Enable = DISABLE;
    pAddrFilterConfig->Addr[1].GroupFilter = DISABLE;
    pAddrFilterConfig->Addr[1].PerfectSourAddr = DISABLE;
    pAddrFilterConfig->Addr[1].GroupMask = 0;
    pAddrFilterConfig->Addr[1].Addr[0] = 0;
    pAddrFilterConfig->Addr[1].Addr[1] = 0;
    pAddrFilterConfig->Addr[1].Addr[2] = 0;
    pAddrFilterConfig->Addr[1].Addr[3] = 0;
    pAddrFilterConfig->Addr[1].Addr[4] = 0;
    pAddrFilterConfig->Addr[1].Addr[5] = 0;
    pAddrFilterConfig->Addr[2].Enable = DISABLE;
    pAddrFilterConfig->Addr[2].GroupFilter = DISABLE;
    pAddrFilterConfig->Addr[2].PerfectSourAddr = DISABLE;
    pAddrFilterConfig->Addr[2].GroupMask = 0;
    pAddrFilterConfig->Addr[2].Addr[0] = 0;
    pAddrFilterConfig->Addr[2].Addr[1] = 0;
    pAddrFilterConfig->Addr[2].Addr[2] = 0;
    pAddrFilterConfig->Addr[2].Addr[3] = 0;
    pAddrFilterConfig->Addr[2].Addr[4] = 0;
    pAddrFilterConfig->Addr[2].Addr[5] = 0;
    pAddrFilterConfig->ReceiveAll = DISABLE;
    pAddrFilterConfig->HashPerfectFilter = DISABLE;
    pAddrFilterConfig->SourceAddrFilter = DISABLE;
    pAddrFilterConfig->SourceAddrInverseFilter = DISABLE;
    pAddrFilterConfig->PassControlPackets = ETH_PASS_CONTROL_BLOCK_ALL;
    pAddrFilterConfig->BroadcastFramesReception = ENABLE;
    pAddrFilterConfig->PassAllMulticast = DISABLE;
    pAddrFilterConfig->DestAddrInverseFilter = DISABLE;
    pAddrFilterConfig->HushMulticastFramesFilter = DISABLE;
    pAddrFilterConfig->HashUnicastFramesFilter = DISABLE;
    pAddrFilterConfig->PromiscuousMode = DISABLE;
    pAddrFilterConfig->HashTable[0] = 0;
    pAddrFilterConfig->HashTable[1] = 0;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the address filter parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetAddrFilterParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_AddrFilterTypeDef *pAddrFilterConfig = &heth->FilterConfig.Addr;

    if (heth == NULL)
        return (HAL_ERROR);

    memset((void *)pAddrFilterConfig, 0, sizeof(ETH_AddrFilterTypeDef));
    
    // MACA1HR
    tmpreg = heth->Instance->MACA1HR;
    
    if (tmpreg & ETH_MACA1HR_AE)
        pAddrFilterConfig->Addr[0].Enable = ENABLE;
    else
        pAddrFilterConfig->Addr[0].Enable = DISABLE;
    
    if (tmpreg & ETH_MACA1HR_MBC)
    {
        pAddrFilterConfig->Addr[0].GroupFilter = ENABLE;
        pAddrFilterConfig->Addr[0].GroupMask = (tmpreg & ETH_MACA1HR_MBC) >> ETH_MACA1HR_MBC_Pos;
    }
    else
    {
        pAddrFilterConfig->Addr[0].GroupFilter = DISABLE;
        pAddrFilterConfig->Addr[0].GroupMask = 0;
    }
    
    if (tmpreg & ETH_MACA1HR_SA)
        pAddrFilterConfig->Addr[0].PerfectSourAddr = ENABLE;
    else
        pAddrFilterConfig->Addr[0].PerfectSourAddr = DISABLE;
    
    pAddrFilterConfig->Addr[0].Addr[5] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[0].Addr[4] = (tmpreg >> 0) & 0xff;
    
    // MACA1LR
    tmpreg = heth->Instance->MACA1LR;
    pAddrFilterConfig->Addr[0].Addr[3] = (tmpreg >> 24) & 0xff;
    pAddrFilterConfig->Addr[0].Addr[2] = (tmpreg >> 16) & 0xff;
    pAddrFilterConfig->Addr[0].Addr[1] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[0].Addr[0] = (tmpreg >> 0) & 0xff;
    
    // MACA2HR
    tmpreg = heth->Instance->MACA2HR;
    
    if (tmpreg & ETH_MACA1HR_AE)
        pAddrFilterConfig->Addr[1].Enable = ENABLE;
    else
        pAddrFilterConfig->Addr[1].Enable = DISABLE;
    
    if (tmpreg & ETH_MACA1HR_MBC)
    {
        pAddrFilterConfig->Addr[1].GroupFilter = ENABLE;
        pAddrFilterConfig->Addr[1].GroupMask = (tmpreg & ETH_MACA1HR_MBC) >> ETH_MACA1HR_MBC_Pos;
    }
    else
    {
        pAddrFilterConfig->Addr[1].GroupFilter = DISABLE;
        pAddrFilterConfig->Addr[1].GroupMask = 0;
    }
    
    if (tmpreg & ETH_MACA1HR_SA)
        pAddrFilterConfig->Addr[1].PerfectSourAddr = ENABLE;
    else
        pAddrFilterConfig->Addr[1].PerfectSourAddr = DISABLE;
    
    pAddrFilterConfig->Addr[1].Addr[5] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[1].Addr[4] = (tmpreg >> 0) & 0xff;
    
    // MACA2LR
    tmpreg = heth->Instance->MACA2LR;
    pAddrFilterConfig->Addr[1].Addr[3] = (tmpreg >> 24) & 0xff;
    pAddrFilterConfig->Addr[1].Addr[2] = (tmpreg >> 16) & 0xff;
    pAddrFilterConfig->Addr[1].Addr[1] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[1].Addr[0] = (tmpreg >> 0) & 0xff;
    
    // MACA3HR
    tmpreg = heth->Instance->MACA3HR;
    
    if (tmpreg & ETH_MACA1HR_AE)
        pAddrFilterConfig->Addr[2].Enable = ENABLE;
    else
        pAddrFilterConfig->Addr[2].Enable = DISABLE;
    
    if (tmpreg & ETH_MACA1HR_MBC)
    {
        pAddrFilterConfig->Addr[2].GroupFilter = ENABLE;
        pAddrFilterConfig->Addr[2].GroupMask = (tmpreg & ETH_MACA1HR_MBC) >> ETH_MACA1HR_MBC_Pos;
    }
    else
    {
        pAddrFilterConfig->Addr[2].GroupFilter = DISABLE;
        pAddrFilterConfig->Addr[2].GroupMask = 0;
    }
    
    if (tmpreg & ETH_MACA1HR_SA)
        pAddrFilterConfig->Addr[2].PerfectSourAddr = ENABLE;
    else
        pAddrFilterConfig->Addr[2].PerfectSourAddr = DISABLE;
    
    pAddrFilterConfig->Addr[2].Addr[5] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[2].Addr[4] = (tmpreg >> 0) & 0xff;
    
    // MACA3LR
    tmpreg = heth->Instance->MACA3LR;
    pAddrFilterConfig->Addr[2].Addr[3] = (tmpreg >> 24) & 0xff;
    pAddrFilterConfig->Addr[2].Addr[2] = (tmpreg >> 16) & 0xff;
    pAddrFilterConfig->Addr[2].Addr[1] = (tmpreg >> 8) & 0xff;
    pAddrFilterConfig->Addr[2].Addr[0] = (tmpreg >> 0) & 0xff;
    
    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    
    if (tmpreg & ETH_MACFFR_RA)
        pAddrFilterConfig->ReceiveAll = ENABLE;
    else
        pAddrFilterConfig->ReceiveAll = DISABLE;
    
    if (tmpreg & ETH_MACFFR_HPF)
        pAddrFilterConfig->HashPerfectFilter = ENABLE;
    else
        pAddrFilterConfig->HashPerfectFilter = DISABLE;
    
    if (tmpreg & ETH_MACFFR_SAF)
        pAddrFilterConfig->SourceAddrFilter = ENABLE;
    else
        pAddrFilterConfig->SourceAddrFilter = DISABLE;
    
    if (tmpreg & ETH_MACFFR_SAIF)
        pAddrFilterConfig->SourceAddrInverseFilter = ENABLE;
    else
        pAddrFilterConfig->SourceAddrInverseFilter = DISABLE;
    
    if ((tmpreg & ETH_MACFFR_PCF) == 0)
        pAddrFilterConfig->PassControlPackets = ETH_PASS_CONTROL_BLOCK_ALL;
    else if ((tmpreg & ETH_MACFFR_PCF) == ETH_MACFFR_PCF_0)
        pAddrFilterConfig->PassControlPackets = ETH_PASS_CONTROL_FORWARD_ALL_EXCEPT_PAUSE_FRAME;
    else if ((tmpreg & ETH_MACFFR_PCF) == ETH_MACFFR_PCF_1)
        pAddrFilterConfig->PassControlPackets = ETH_PASS_CONTROL_FORWARD_ALL;
    else if ((tmpreg & ETH_MACFFR_PCF) == (ETH_MACFFR_PCF_1 | ETH_MACFFR_PCF_0))
        pAddrFilterConfig->PassControlPackets = ETH_PASS_CONTROL_FORWARD_PASSED_ADDR_FILTER;
    
    if (tmpreg & ETH_MACFFR_DBF)
        pAddrFilterConfig->BroadcastFramesReception = DISABLE;
    else
        pAddrFilterConfig->BroadcastFramesReception = ENABLE;
    
    if (tmpreg & ETH_MACFFR_PAM)
        pAddrFilterConfig->PassAllMulticast = ENABLE;
    else
        pAddrFilterConfig->PassAllMulticast = DISABLE;
    
    if (tmpreg & ETH_MACFFR_DAIF)
        pAddrFilterConfig->DestAddrInverseFilter = ENABLE;
    else
        pAddrFilterConfig->DestAddrInverseFilter = DISABLE;
    
    if (tmpreg & ETH_MACFFR_HMC)
        pAddrFilterConfig->HushMulticastFramesFilter = ENABLE;
    else
        pAddrFilterConfig->HushMulticastFramesFilter = DISABLE;
    
    if (tmpreg & ETH_MACFFR_HUC)
        pAddrFilterConfig->HashUnicastFramesFilter = ENABLE;
    else
        pAddrFilterConfig->HashUnicastFramesFilter = DISABLE;
    
    if (tmpreg & ETH_MACFFR_PR)
        pAddrFilterConfig->PromiscuousMode = ENABLE;
    else
        pAddrFilterConfig->PromiscuousMode = DISABLE;
    
    // MACHTHR
    pAddrFilterConfig->HashTable[1] = heth->Instance->MACHTHR;
    
    // MACHTLR
    pAddrFilterConfig->HashTable[0] = heth->Instance->MACHTLR;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the address filter parameter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_AddrFilterConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_AddrFilterTypeDef *pAddrFilterConfig = &heth->FilterConfig.Addr;

    if (heth == NULL)
        return (HAL_ERROR);
    
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[0].Enable));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[1].Enable));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[2].Enable));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->ReceiveAll));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->HashPerfectFilter));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->SourceAddrFilter));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->SourceAddrInverseFilter));
    assert_param(IS_ETH_PASS_CONTROL_PACKETS(pAddrFilterConfig->PassControlPackets));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->BroadcastFramesReception));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->PassAllMulticast));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->DestAddrInverseFilter));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->HushMulticastFramesFilter));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->HashUnicastFramesFilter));
    assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->PromiscuousMode));

    // MACA1HR
    if (pAddrFilterConfig->Addr[0].Enable == DISABLE)
    {
        heth->Instance->MACA1HR = 0;
        heth->Instance->MACA1LR = 0;
    }
    else
    {
        assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[0].GroupFilter));
        
        tmpreg = ETH_MACA1HR_AE;
        if (pAddrFilterConfig->Addr[0].GroupFilter == DISABLE)
        {
            assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[0].PerfectSourAddr));
            
            if (pAddrFilterConfig->Addr[0].PerfectSourAddr != DISABLE)
                tmpreg |= ETH_MACA1HR_SA;
        }
        else
        {
            assert_param(IS_ETH_ADDR_FILTER_GROUP_MASK(pAddrFilterConfig->Addr[0].GroupMask));
            
            tmpreg |= pAddrFilterConfig->Addr[0].GroupMask << ETH_MACA1HR_MBC_Pos;
        }
        
        heth->Instance->MACA1HR = tmpreg | \
                                  ((uint32_t)pAddrFilterConfig->Addr[0].Addr[5] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[0].Addr[4] << 0);
        
        // MACA1LR
        heth->Instance->MACA1LR = ((uint32_t)pAddrFilterConfig->Addr[0].Addr[3] << 24) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[0].Addr[2] << 16) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[0].Addr[1] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[0].Addr[0] << 0);
    }
    
    // MACA1HR
    if (pAddrFilterConfig->Addr[1].Enable == DISABLE)
    {
        heth->Instance->MACA2HR = 0;
        heth->Instance->MACA2LR = 0;
    }
    else
    {
        assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[1].GroupFilter));
        
        tmpreg = ETH_MACA2HR_AE;
        if (pAddrFilterConfig->Addr[1].GroupFilter == DISABLE)
        {
            assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[1].PerfectSourAddr));
            
            if (pAddrFilterConfig->Addr[1].PerfectSourAddr != DISABLE)
                tmpreg |= ETH_MACA2HR_SA;
        }
        else
        {
            assert_param(IS_ETH_ADDR_FILTER_GROUP_MASK(pAddrFilterConfig->Addr[1].GroupMask));
            
            tmpreg |= pAddrFilterConfig->Addr[1].GroupMask << ETH_MACA2HR_MBC_Pos;
        }
        
        heth->Instance->MACA2HR = tmpreg | \
                                  ((uint32_t)pAddrFilterConfig->Addr[1].Addr[5] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[1].Addr[4] << 0);
        
        // MACA2LR
        heth->Instance->MACA2LR = ((uint32_t)pAddrFilterConfig->Addr[1].Addr[3] << 24) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[1].Addr[2] << 16) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[1].Addr[1] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[1].Addr[0] << 0);
    }
    
    // MACA3HR
    if (pAddrFilterConfig->Addr[2].Enable == DISABLE)
    {
        heth->Instance->MACA3HR = 0;
        heth->Instance->MACA3LR = 0;
    }
    else
    {
        assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[2].GroupFilter));
        
        tmpreg = ETH_MACA3HR_AE;
        if (pAddrFilterConfig->Addr[2].GroupFilter == DISABLE)
        {
            assert_param(IS_FUNCTIONAL_STATE(pAddrFilterConfig->Addr[2].PerfectSourAddr));
            
            if (pAddrFilterConfig->Addr[2].PerfectSourAddr != DISABLE)
                tmpreg |= ETH_MACA3HR_SA;
        }
        else
        {
            assert_param(IS_ETH_ADDR_FILTER_GROUP_MASK(pAddrFilterConfig->Addr[2].GroupMask));
            
            tmpreg |= pAddrFilterConfig->Addr[2].GroupMask << ETH_MACA3HR_MBC_Pos;
        }
        
        heth->Instance->MACA3HR = tmpreg | \
                                  ((uint32_t)pAddrFilterConfig->Addr[2].Addr[5] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[2].Addr[4] << 0);
        
        // MACA3LR
        heth->Instance->MACA3LR = ((uint32_t)pAddrFilterConfig->Addr[2].Addr[3] << 24) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[2].Addr[2] << 16) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[2].Addr[1] << 8) | \
                                  ((uint32_t)pAddrFilterConfig->Addr[2].Addr[0] << 0);
    }
    
    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    tmpreg &= ~(ETH_MACFFR_RA | ETH_MACFFR_HPF | ETH_MACFFR_SAF | ETH_MACFFR_PCF | ETH_MACFFR_DBF | \
                ETH_MACFFR_PAM | ETH_MACFFR_DAIF | ETH_MACFFR_HMC | ETH_MACFFR_HUC | ETH_MACFFR_PR);

    if (pAddrFilterConfig->ReceiveAll != DISABLE)
        tmpreg |= ETH_MACFFR_RA;
    
    if (pAddrFilterConfig->HashPerfectFilter != DISABLE)
        tmpreg |= ETH_MACFFR_HPF;
    
    if (pAddrFilterConfig->SourceAddrFilter != DISABLE)
        tmpreg |= ETH_MACFFR_SAF;
    
    if (pAddrFilterConfig->SourceAddrInverseFilter != DISABLE)
        tmpreg |= ETH_MACFFR_SAIF;
    
    if (pAddrFilterConfig->PassControlPackets == ETH_PASS_CONTROL_BLOCK_ALL)
        tmpreg |= 0U;
    else if (pAddrFilterConfig->PassControlPackets == ETH_PASS_CONTROL_FORWARD_ALL_EXCEPT_PAUSE_FRAME)
        tmpreg |= ETH_MACFFR_PCF_0;
    else if (pAddrFilterConfig->PassControlPackets == ETH_PASS_CONTROL_FORWARD_ALL)
        tmpreg |= ETH_MACFFR_PCF_1;
    else if (pAddrFilterConfig->PassControlPackets == ETH_PASS_CONTROL_FORWARD_PASSED_ADDR_FILTER)
        tmpreg |= ETH_MACFFR_PCF_1 | ETH_MACFFR_PCF_0;
    
    if (pAddrFilterConfig->BroadcastFramesReception == DISABLE)
        tmpreg |= ETH_MACFFR_DBF;
    
    if (pAddrFilterConfig->PassAllMulticast != DISABLE)
        tmpreg |= ETH_MACFFR_PAM;
    
    if (pAddrFilterConfig->DestAddrInverseFilter != DISABLE)
        tmpreg |= ETH_MACFFR_DAIF;
    
    if (pAddrFilterConfig->HushMulticastFramesFilter != DISABLE)
        tmpreg |= ETH_MACFFR_HMC;
    
    if (pAddrFilterConfig->HashUnicastFramesFilter != DISABLE)
        tmpreg |= ETH_MACFFR_HUC;
    
    if (pAddrFilterConfig->PromiscuousMode != DISABLE)
        tmpreg |= ETH_MACFFR_PR;
    
    heth->Instance->MACFFR = tmpreg;
    
    // MACHTHR
    heth->Instance->MACHTHR = (uint32_t)pAddrFilterConfig->HashTable[1];

    // MACHTLR
    heth->Instance->MACHTLR = (uint32_t)pAddrFilterConfig->HashTable[0];
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the address filter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_AddrFilterDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitAddrFilterDefaultParamter(heth);
    
    return HAL_ETH_AddrFilterConfig(heth);
}

//  ****************************************************************************
//  
//                              VLAN Filter
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization VLAN filter parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitVLANFilterDefaultParamter(ETH_HandleTypeDef *heth)
{
    ETH_VLANFilterTypeDef *pVLANFilterConfig = &heth->FilterConfig.VLAN;
    
    // VLAN filter
    pVLANFilterConfig->Enable = DISABLE;
    pVLANFilterConfig->Hash = DISABLE;
    pVLANFilterConfig->SVLAN = DISABLE;
    pVLANFilterConfig->InverseMatch = DISABLE;
    pVLANFilterConfig->Comparison12Bits = DISABLE;
    pVLANFilterConfig->Tag = 0;
    pVLANFilterConfig->HashTable = 0;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the VLAN filter parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetVLANFilterParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_VLANFilterTypeDef *pVLANFilterConfig = &heth->FilterConfig.VLAN;

    if (heth == NULL)
        return (HAL_ERROR);

    memset((void *)pVLANFilterConfig, 0, sizeof(ETH_VLANFilterTypeDef));
    
    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    
    if (tmpreg & ETH_MACFFR_VTFE)
        pVLANFilterConfig->Enable = ENABLE;
    else
        pVLANFilterConfig->Enable = DISABLE;
    
    // MACVLANTR
    tmpreg = heth->Instance->MACVLANTR;
    
    if (tmpreg & ETH_MACVLANTR_VTHM)
        pVLANFilterConfig->Hash = ENABLE;
    else
        pVLANFilterConfig->Hash = DISABLE;
    
    if (tmpreg & ETH_MACVLANTR_ESVL)
        pVLANFilterConfig->SVLAN = ENABLE;
    else
        pVLANFilterConfig->SVLAN = DISABLE;
    
    if (tmpreg & ETH_MACVLANTR_VTIM)
        pVLANFilterConfig->InverseMatch = ENABLE;
    else
        pVLANFilterConfig->InverseMatch = DISABLE;
    
    if (tmpreg & ETH_MACVLANTR_ETV)
        pVLANFilterConfig->Comparison12Bits = ENABLE;
    else
        pVLANFilterConfig->Comparison12Bits = DISABLE;
    
    pVLANFilterConfig->Tag = (tmpreg & ETH_MACVLANTR_VL) >> ETH_MACVLANTR_VL_Pos;
    
    // MACVHTR 
    pVLANFilterConfig->HashTable = (heth->Instance->MACVHTR & ETH_MACVHTR_CSVL) >> ETH_MACVHTR_CSVL_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the VLAN filter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_VLANFilterConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_VLANFilterTypeDef *pVLANFilterConfig = &heth->FilterConfig.VLAN;

    if (heth == NULL)
        return (HAL_ERROR);
    
    assert_param(IS_FUNCTIONAL_STATE(pVLANFilterConfig->Enable));
    assert_param(IS_FUNCTIONAL_STATE(pVLANFilterConfig->Hash));
    assert_param(IS_FUNCTIONAL_STATE(pVLANFilterConfig->SVLAN));
    assert_param(IS_FUNCTIONAL_STATE(pVLANFilterConfig->InverseMatch));
    assert_param(IS_FUNCTIONAL_STATE(pVLANFilterConfig->Comparison12Bits));
    assert_param(IS_ETH_VLAN_TAG(pVLANFilterConfig->Tag));
    assert_param(IS_ETH_VLAN_HASH_TABLE(pVLANFilterConfig->HashTable));

    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    tmpreg &= ~ETH_MACFFR_VTFE;
    
    if (pVLANFilterConfig->Enable != DISABLE)
        tmpreg |= ETH_MACFFR_VTFE;
    
    heth->Instance->MACFFR = tmpreg;
    
    // MACVLANTR
    tmpreg = 0;
    
    if (pVLANFilterConfig->Hash != DISABLE)
        tmpreg |= ETH_MACVLANTR_VTHM;
    
    if (pVLANFilterConfig->SVLAN != DISABLE)
        tmpreg |= ETH_MACVLANTR_ESVL;
    
    if (pVLANFilterConfig->InverseMatch != DISABLE)
        tmpreg |= ETH_MACVLANTR_VTIM;
    
    if (pVLANFilterConfig->Comparison12Bits != DISABLE)
        tmpreg |= ETH_MACVLANTR_ETV;

    tmpreg |= pVLANFilterConfig->Tag << ETH_MACVLANTR_VL_Pos;
    
    heth->Instance->MACVLANTR = (uint32_t)tmpreg;
    
    // MACVHTR
    heth->Instance->MACVHTR = pVLANFilterConfig->HashTable;
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the VLAN filter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_VLANFilterDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitVLANFilterDefaultParamter(heth);
    
    return HAL_ETH_VLANFilterConfig(heth);
}

//  ****************************************************************************
//  
//                              L3L4 Filter
//  
//  ****************************************************************************

/******************************************************************************
*@brief : Initialization the L3L4 filter parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitL3L4FilterDefaultParamter(ETH_HandleTypeDef *heth)
{
    uint32_t i;
    ETH_L3FilterTypeDef *pL3Config;
    ETH_L4FilterTypeDef *pL4Config;
    
    // Layer3 Layer4 filter
    heth->FilterConfig.L3L4.Enable = DISABLE;
    
    for (i=0; i<2; i++)
    {
        if (i == 0)
            pL3Config = &heth->FilterConfig.L3L4.L3[0];
        else
            pL3Config = &heth->FilterConfig.L3L4.L3[1];
        
        pL3Config->IPV6 = DISABLE;
        pL3Config->IP.IPV4.SourAddrMatchEnable = DISABLE;
        pL3Config->IP.IPV4.SourAddrInverseMatch = DISABLE;
        pL3Config->IP.IPV4.SourAddrMatch = 0;
        pL3Config->IP.IPV4.DestAddrMatchEnable = DISABLE;
        pL3Config->IP.IPV4.DestAddrInverseMatch = DISABLE;
        pL3Config->IP.IPV4.DestAddrMatch = 0;
        pL3Config->IP.IPV4.SourAddr = 0;
        pL3Config->IP.IPV4.DestAddr = 0;
    }
    
    for (i=0; i<2; i++)
    {
        if (i == 0)
            pL4Config = &heth->FilterConfig.L3L4.L4[0];
        else
            pL4Config = &heth->FilterConfig.L3L4.L4[1];
        
        pL4Config->UDP = DISABLE;
        pL4Config->SourPortMatchEnable = DISABLE;
        pL4Config->SourPortInverseMatch = DISABLE;
        pL4Config->DestPortMatchEnable = DISABLE;
        pL4Config->DestPortInverseMatch = DISABLE;
        pL4Config->SourPort = 0;
        pL4Config->DestPort = 0;
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the the L3L4 filter parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetL3L4FilterParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_L3L4FilterTypeDef *pL3L4FilterConfig = &heth->FilterConfig.L3L4;

    if (heth == NULL)
        return (HAL_ERROR);

    memset((void *)pL3L4FilterConfig, 0, sizeof(ETH_L3L4FilterTypeDef));
    
    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    
    if (tmpreg & ETH_MACFFR_IPFE)
        pL3L4FilterConfig->Enable = ENABLE;
    else
        pL3L4FilterConfig->Enable = DISABLE;
    
    // MACL3L4C0R 
    tmpreg = heth->Instance->MACL3L4C0R;
    
    if (tmpreg & ETH_MACL3L4C0R_L4DPIM0)
        pL3L4FilterConfig->L4[0].DestPortInverseMatch = ENABLE;
    else
        pL3L4FilterConfig->L4[0].DestPortInverseMatch = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C0R_L4DPM0)
        pL3L4FilterConfig->L4[0].DestPortMatchEnable = ENABLE;
    else
        pL3L4FilterConfig->L4[0].DestPortMatchEnable = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C0R_L4SPIM0)
        pL3L4FilterConfig->L4[0].SourPortInverseMatch = ENABLE;
    else
        pL3L4FilterConfig->L4[0].SourPortInverseMatch = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C0R_L4SPM0)
        pL3L4FilterConfig->L4[0].SourPortMatchEnable = ENABLE;
    else
        pL3L4FilterConfig->L4[0].SourPortMatchEnable = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C0R_L4PEN0)
        pL3L4FilterConfig->L4[0].UDP = ENABLE;
    else
        pL3L4FilterConfig->L4[0].UDP = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C0R_L3PEN0)
    {
        pL3L4FilterConfig->L3[0].IPV6 = ENABLE;
        
        pL3L4FilterConfig->L3[0].IP.IPV6.AddrMatch = (tmpreg & (ETH_MACL3L4C0R_L3HDBM0_0 | ETH_MACL3L4C0R_L3HDBM0_1 | ETH_MACL3L4C0R_L3HSBM0)) >> ETH_MACL3L4C0R_L3HSBM0_Pos;
        
        if (tmpreg & ETH_MACL3L4C0R_L3DAIM0)
            pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3DAM0)
            pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrMatchEnable = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3SAIM0)
            pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3SAM0)
            pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrMatchEnable = DISABLE;
        
        // MACL3A00R  
        pL3L4FilterConfig->L3[0].IP.IPV6.Addr[0] = heth->Instance->MACL3A00R;
        
        // MACL3A10R  
        pL3L4FilterConfig->L3[0].IP.IPV6.Addr[1] = heth->Instance->MACL3A10R;
        
        // MACL3A20R  
        pL3L4FilterConfig->L3[0].IP.IPV6.Addr[2] = heth->Instance->MACL3A20R;
        
        // MACL3A30R  
        pL3L4FilterConfig->L3[0].IP.IPV6.Addr[3] = heth->Instance->MACL3A30R;
        
    }
    else
    {
        pL3L4FilterConfig->L3[0].IPV6 = DISABLE;
        
        pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatch = (tmpreg & ETH_MACL3L4C0R_L3HDBM0) >> ETH_MACL3L4C0R_L3HDBM0_Pos;
        
        pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatch = (tmpreg & ETH_MACL3L4C0R_L3HSBM0) >> ETH_MACL3L4C0R_L3HSBM0_Pos;
        
        if (tmpreg & ETH_MACL3L4C0R_L3DAIM0)
            pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3DAM0)
            pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatchEnable = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3SAIM0)
            pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C0R_L3SAM0)
            pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatchEnable = DISABLE;
        
        // MACL3A00R 
        pL3L4FilterConfig->L3[0].IP.IPV4.SourAddr = heth->Instance->MACL3A00R;
        
        // MACL3A10R 
        pL3L4FilterConfig->L3[0].IP.IPV4.DestAddr = heth->Instance->MACL3A10R;
        
    }
    
    // MACL4A0R 
    pL3L4FilterConfig->L4[0].DestPort = (heth->Instance->MACL4A0R & ETH_MACL4A0R_L4DP0) >> ETH_MACL4A0R_L4DP0_Pos;
    
    pL3L4FilterConfig->L4[0].SourPort = (heth->Instance->MACL4A0R & ETH_MACL4A0R_L4SP0) >> ETH_MACL4A0R_L4SP0_Pos;
        
    // MACL3L4C1R 
    tmpreg = heth->Instance->MACL3L4C1R;
    
    if (tmpreg & ETH_MACL3L4C1R_L4DPIM1)
        pL3L4FilterConfig->L4[1].DestPortInverseMatch = ENABLE;
    else
        pL3L4FilterConfig->L4[1].DestPortInverseMatch = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C1R_L4DPM1)
        pL3L4FilterConfig->L4[1].DestPortMatchEnable = ENABLE;
    else
        pL3L4FilterConfig->L4[1].DestPortMatchEnable = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C1R_L4SPIM1)
        pL3L4FilterConfig->L4[1].SourPortInverseMatch = ENABLE;
    else
        pL3L4FilterConfig->L4[1].SourPortInverseMatch = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C1R_L4SPM1)
        pL3L4FilterConfig->L4[1].SourPortMatchEnable = ENABLE;
    else
        pL3L4FilterConfig->L4[1].SourPortMatchEnable = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C1R_L4PEN1)
        pL3L4FilterConfig->L4[1].UDP = ENABLE;
    else
        pL3L4FilterConfig->L4[1].UDP = DISABLE;
    
    if (tmpreg & ETH_MACL3L4C1R_L3PEN1)
    {
        pL3L4FilterConfig->L3[1].IPV6 = ENABLE;
        
        pL3L4FilterConfig->L3[1].IP.IPV6.AddrMatch = (tmpreg & (ETH_MACL3L4C1R_L3HDBM1_0 | ETH_MACL3L4C1R_L3HDBM1_1 | ETH_MACL3L4C1R_L3HSBM1)) >> ETH_MACL3L4C1R_L3HSBM1_Pos;
        
        if (tmpreg & ETH_MACL3L4C1R_L3DAIM1)
            pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3DAM1)
            pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrMatchEnable = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3SAIM1)
            pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3SAM1)
            pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrMatchEnable = DISABLE;
        
        // MACL3A01R  
        pL3L4FilterConfig->L3[1].IP.IPV6.Addr[1] = heth->Instance->MACL3A01R;
        
        // MACL3A11R  
        pL3L4FilterConfig->L3[1].IP.IPV6.Addr[1] = heth->Instance->MACL3A11R;
        
        // MACL3A21R  
        pL3L4FilterConfig->L3[1].IP.IPV6.Addr[2] = heth->Instance->MACL3A21R;
        
        // MACL3A31R  
        pL3L4FilterConfig->L3[1].IP.IPV6.Addr[3] = heth->Instance->MACL3A31R;
        
    }
    else
    {
        pL3L4FilterConfig->L3[1].IPV6 = DISABLE;
        
        pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatch = (tmpreg & ETH_MACL3L4C1R_L3HDBM1) >> ETH_MACL3L4C1R_L3HDBM1_Pos;
        
        pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatch = (tmpreg & ETH_MACL3L4C1R_L3HSBM1) >> ETH_MACL3L4C1R_L3HSBM1_Pos;
        
        if (tmpreg & ETH_MACL3L4C1R_L3DAIM1)
            pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3DAM1)
            pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatchEnable = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3SAIM1)
            pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrInverseMatch = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrInverseMatch = DISABLE;
    
        if (tmpreg & ETH_MACL3L4C1R_L3SAM1)
            pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatchEnable = ENABLE;
        else
            pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatchEnable = DISABLE;
        
        // MACL3A01R 
        pL3L4FilterConfig->L3[1].IP.IPV4.SourAddr = heth->Instance->MACL3A01R;
        
        // MACL3A11R 
        pL3L4FilterConfig->L3[1].IP.IPV4.DestAddr = heth->Instance->MACL3A11R;
        
    }
    
    // MACL4A1R 
    pL3L4FilterConfig->L4[1].DestPort = (heth->Instance->MACL4A1R & ETH_MACL4A1R_L4DP1) >> ETH_MACL4A1R_L4DP1_Pos;
    
    pL3L4FilterConfig->L4[1].SourPort = (heth->Instance->MACL4A1R & ETH_MACL4A1R_L4SP1) >> ETH_MACL4A1R_L4SP1_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the L3L4 filter. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_L3L4FilterConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_L3L4FilterTypeDef *pL3L4FilterConfig = &heth->FilterConfig.L3L4;

    if (heth == NULL)
        return (HAL_ERROR);
    
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->Enable));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[0].IPV6));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[1].IPV6));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[0].UDP));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[0].SourPortMatchEnable));
    assert_param(IS_ETH_SOUR_PORT(pL3L4FilterConfig->L4[0].SourPort));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[0].DestPortMatchEnable));
    assert_param(IS_ETH_DEST_PORT(pL3L4FilterConfig->L4[0].DestPort));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[1].UDP));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[1].SourPortMatchEnable));
    assert_param(IS_ETH_SOUR_PORT(pL3L4FilterConfig->L4[1].SourPort));
    assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L4[1].DestPortMatchEnable));
    assert_param(IS_ETH_DEST_PORT(pL3L4FilterConfig->L4[1].DestPort));

    // MACFFR
    tmpreg = heth->Instance->MACFFR;
    
    if (pL3L4FilterConfig->Enable != DISABLE)
        tmpreg |= ETH_MACFFR_IPFE;
    else
        tmpreg &= ~ETH_MACFFR_IPFE;
    
    heth->Instance->MACFFR = tmpreg;
    
    // MACL3L4C0R 
    tmpreg = 0U;
    
    if (pL3L4FilterConfig->L4[0].UDP != DISABLE)
        tmpreg |= ETH_MACL3L4C0R_L4PEN0;
    
    if (pL3L4FilterConfig->L4[0].DestPortMatchEnable != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C0R_L4DPM0;
    
        if (pL3L4FilterConfig->L4[0].DestPortInverseMatch != DISABLE)
            tmpreg |= ETH_MACL3L4C0R_L4DPIM0;
    }
    
    if (pL3L4FilterConfig->L4[0].SourPortMatchEnable != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C0R_L4SPM0;
    
        if (pL3L4FilterConfig->L4[0].SourPortInverseMatch != DISABLE)
            tmpreg |= ETH_MACL3L4C0R_L4SPIM0;
    }
    
    if (pL3L4FilterConfig->L3[0].IPV6 != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C0R_L3PEN0;
        
        assert_param(IS_ETH_IPV6_ADDR_MASK(pL3L4FilterConfig->L3[0].IP.IPV6.AddrMatch));
        
        tmpreg |= pL3L4FilterConfig->L3[0].IP.IPV6.AddrMatch << ETH_MACL3L4C0R_L3HSBM0_Pos;
    
        if (pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C0R_L3DAM0;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C0R_L3DAIM0;
        }
        
        if (pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C0R_L3SAM0;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[0].IP.IPV6.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[0].IP.IPV6.SourAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C0R_L3SAIM0;
        }
        
        heth->Instance->MACL3L4C0R = tmpreg;
        
        // MACL3A00R  
        heth->Instance->MACL3A00R = pL3L4FilterConfig->L3[0].IP.IPV6.Addr[0];
        
        // MACL3A10R  
        heth->Instance->MACL3A10R = pL3L4FilterConfig->L3[0].IP.IPV6.Addr[1];
        
        // MACL3A20R  
        heth->Instance->MACL3A20R = pL3L4FilterConfig->L3[0].IP.IPV6.Addr[2];
        
        // MACL3A30R  
        heth->Instance->MACL3A30R = pL3L4FilterConfig->L3[0].IP.IPV6.Addr[3];
    }
    else
    {
        assert_param(IS_ETH_IPV4_SOUR_ADDR_MASK(pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatch));
        assert_param(IS_ETH_IPV4_DEST_ADDR_MASK(pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatch));
        
        tmpreg |= pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatch << ETH_MACL3L4C0R_L3HDBM0_Pos;
        
        tmpreg |= pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatch << ETH_MACL3L4C0R_L3HSBM0_Pos;
        
        if (pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C0R_L3DAM0;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[0].IP.IPV4.DestAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C0R_L3DAIM0;
        }
        
        if (pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C0R_L3SAM0;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[0].IP.IPV4.SourAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C0R_L3SAIM0;
        }
        
        heth->Instance->MACL3L4C0R = tmpreg;
        
        // MACL3A00R  
        heth->Instance->MACL3A00R = pL3L4FilterConfig->L3[0].IP.IPV4.SourAddr;
        
        // MACL3A10R  
        heth->Instance->MACL3A10R = pL3L4FilterConfig->L3[0].IP.IPV4.DestAddr;
        
        // MACL3A20R  
        heth->Instance->MACL3A20R = 0;
        
        // MACL3A30R  
        heth->Instance->MACL3A30R = 0;
    }
    
    // MACL4A0R 
    tmpreg = 0;
    
    tmpreg |= pL3L4FilterConfig->L4[0].DestPort << ETH_MACL4A0R_L4DP0_Pos;
    
    tmpreg |= pL3L4FilterConfig->L4[0].SourPort << ETH_MACL4A0R_L4SP0_Pos;
    
    heth->Instance->MACL4A0R = tmpreg;
    
    // MACL3L4C1R 
    tmpreg = 0U;
    
    if (pL3L4FilterConfig->L4[1].UDP != DISABLE)
        tmpreg |= ETH_MACL3L4C1R_L4PEN1;
    
    if (pL3L4FilterConfig->L4[1].DestPortMatchEnable != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C1R_L4DPM1;
    
        if (pL3L4FilterConfig->L4[1].DestPortInverseMatch != DISABLE)
            tmpreg |= ETH_MACL3L4C1R_L4DPIM1;
    }
    
    if (pL3L4FilterConfig->L4[1].SourPortMatchEnable != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C1R_L4SPM1;
    
        if (pL3L4FilterConfig->L4[1].SourPortInverseMatch != DISABLE)
            tmpreg |= ETH_MACL3L4C1R_L4SPIM1;
    }
    
    if (pL3L4FilterConfig->L3[1].IPV6 != DISABLE)
    {
        tmpreg |= ETH_MACL3L4C1R_L3PEN1;
        
        assert_param(IS_ETH_IPV6_ADDR_MASK(pL3L4FilterConfig->L3[1].IP.IPV6.AddrMatch));
        
        tmpreg |= pL3L4FilterConfig->L3[1].IP.IPV6.AddrMatch << ETH_MACL3L4C1R_L3HSBM1_Pos;
    
        if (pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C1R_L3DAM1;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C1R_L3DAIM1;
        }
        
        if (pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C1R_L3SAM1;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[1].IP.IPV6.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[1].IP.IPV6.SourAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C1R_L3SAIM1;
        }
        
        heth->Instance->MACL3L4C1R = tmpreg;
        
        // MACL3A01R  
        heth->Instance->MACL3A01R = pL3L4FilterConfig->L3[1].IP.IPV6.Addr[0];
        
        // MACL3A11R  
        heth->Instance->MACL3A11R = pL3L4FilterConfig->L3[1].IP.IPV6.Addr[1];
        
        // MACL3A21R  
        heth->Instance->MACL3A21R = pL3L4FilterConfig->L3[1].IP.IPV6.Addr[2];
        
        // MACL3A31R  
        heth->Instance->MACL3A31R = pL3L4FilterConfig->L3[1].IP.IPV6.Addr[3];
    }
    else
    {
        assert_param(IS_ETH_IPV4_SOUR_ADDR_MASK(pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatch));
        assert_param(IS_ETH_IPV4_DEST_ADDR_MASK(pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatch));
        
        tmpreg |= pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatch << ETH_MACL3L4C1R_L3HDBM1_Pos;
        
        tmpreg |= pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatch << ETH_MACL3L4C1R_L3HSBM1_Pos;
        
        if (pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C1R_L3DAM1;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[1].IP.IPV4.DestAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C1R_L3DAIM1;
        }
        
        if (pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrMatchEnable != DISABLE)
        {
            tmpreg |= ETH_MACL3L4C1R_L3SAM1;
            
            assert_param(IS_FUNCTIONAL_STATE(pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrInverseMatch));
            
            if (pL3L4FilterConfig->L3[1].IP.IPV4.SourAddrInverseMatch != DISABLE)
                tmpreg |= ETH_MACL3L4C1R_L3SAIM1;
        }
        
        heth->Instance->MACL3L4C1R = tmpreg;
        
        // MACL3A01R  
        heth->Instance->MACL3A01R = pL3L4FilterConfig->L3[1].IP.IPV4.SourAddr;
        
        // MACL3A11R  
        heth->Instance->MACL3A11R = pL3L4FilterConfig->L3[1].IP.IPV4.DestAddr;
        
        // MACL3A21R  
        heth->Instance->MACL3A21R = 0;
        
        // MACL3A31R  
        heth->Instance->MACL3A31R = 0;
    }
    
    // MACL4A1R 
    tmpreg = 0;
    
    tmpreg |= pL3L4FilterConfig->L4[1].DestPort << ETH_MACL4A1R_L4DP1_Pos;
    
    tmpreg |= pL3L4FilterConfig->L4[1].SourPort << ETH_MACL4A1R_L4SP1_Pos;
    
    heth->Instance->MACL4A1R = tmpreg;
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the L3L4 filter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_L3L4FilterDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitL3L4FilterDefaultParamter(heth);
    
    return HAL_ETH_L3L4FilterConfig(heth);
}


//	****************************************************************************
//  
//                              VLAN
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization the VLAN parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitVLANDefaultParamter(ETH_HandleTypeDef *heth)
{
    ETH_VLANTypeDef *pVLANConfig = &heth->MACConfig.VLAN;
    
    pVLANConfig->Enable = DISABLE;
    pVLANConfig->Mode = ETH_VLAN_MODE_NONE;
    pVLANConfig->SVLAN = DISABLE;
    pVLANConfig->Tag = 0;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the VLAN parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetVLANParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_VLANTypeDef *pVLANConfig = &heth->MACConfig.VLAN;

    if (heth == NULL)
        return (HAL_ERROR);

    memset((void *)pVLANConfig, 0, sizeof(ETH_VLANTypeDef));
    
    // MACVTIRR
    tmpreg = heth->Instance->MACVTIRR;
    
    if (tmpreg & ETH_MACVTIRR_CSVL)
        pVLANConfig->SVLAN = ENABLE;
    else
        pVLANConfig->SVLAN = DISABLE;
    
    if (tmpreg & ETH_MACVTIRR_VLP)
        pVLANConfig->Enable = ENABLE;
    else
        pVLANConfig->Enable = DISABLE;
    
    pVLANConfig->Mode = (tmpreg & ETH_MACVTIRR_VLC) >> ETH_MACVTIRR_VLC_Pos;
    
    pVLANConfig->Tag = (tmpreg & ETH_MACVTIRR_VLT) >> ETH_MACVTIRR_VLT_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the VLAN. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_VLANConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_VLANTypeDef *pVLANConfig = &heth->MACConfig.VLAN;

    if (heth == NULL)
        return (HAL_ERROR);
    
    assert_param(IS_FUNCTIONAL_STATE(pVLANConfig->Enable));
    assert_param(IS_FUNCTIONAL_STATE(pVLANConfig->SVLAN));
    assert_param(IS_ETH_VLAN_MODE(pVLANConfig->Mode));
    assert_param(IS_ETH_VLAN_TAG(pVLANConfig->Tag));

    // MACVTIRR 
    tmpreg = 0;
    
    if (pVLANConfig->SVLAN != DISABLE)
        tmpreg |= ETH_MACVTIRR_CSVL;
    
    if (pVLANConfig->Enable != DISABLE)
        tmpreg |= ETH_MACVTIRR_VLP;
    
    tmpreg |= pVLANConfig->Mode << ETH_MACVTIRR_VLC_Pos;
    
    tmpreg |= pVLANConfig->Tag << ETH_MACVTIRR_VLT_Pos;
    
    heth->Instance->MACVTIRR = (uint32_t)tmpreg;
    
    return HAL_OK;  
}

/******************************************************************************
*@brief : Configuration the VLAN to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_VLANDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitVLANDefaultParamter(heth);
    
    return HAL_ETH_VLANConfig(heth);
}

//	****************************************************************************
//  
//                              Flow Control
//  
//	****************************************************************************

/******************************************************************************
*@brief : Gets the receive packets flow control busy status.
*@Note    Only applicable in full duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*             the configuration information for ETHERNET module.
*@return: busy status.
*              0:No pause frame were sent.
*              1:The pause packet is being sent.
******************************************************************************/
uint32_t HAL_ETH_GetReceiveFlowCtrlBusyStatus(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (0U);
    
    if (heth->Instance->MACFCR & ETH_MACFCR_FCB_BPA)
        return (1U);
    else
        return (0U);
}

/******************************************************************************
*@brief : Enable receive packets flow control. 
*@Note    Only applicable in full duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : pausetime: pause time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReceiveFlowCtrlEnable(ETH_HandleTypeDef *heth, uint32_t pausetime)
{
    volatile uint32_t tmpreg;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (heth->Instance->MACFCR & ETH_MACFCR_FCB_BPA)
        return HAL_ERROR;
    
    tmpreg = heth->Instance->MACFCR & ~ETH_MACFCR_PT;
    
    tmpreg |= (pausetime << ETH_MACFCR_PT_Pos) & ETH_MACFCR_PT;
    tmpreg |= ETH_MACFCR_TFCE;
    
    heth->Instance->MACFCR = tmpreg | ETH_MACFCR_FCB_BPA;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Disable receive packets flow control. 
*@Note    Only applicable in full duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReceiveFlowCtrlDisable(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (heth->Instance->MACFCR & ETH_MACFCR_FCB_BPA)
        return HAL_ERROR;
    
    tmpreg = heth->Instance->MACFCR & ~ETH_MACFCR_PT;
    
    tmpreg |= ETH_MACFCR_TFCE;
    
    heth->Instance->MACFCR = tmpreg | ETH_MACFCR_FCB_BPA;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Enable the transmit packets flow control.
*@Note    Only applicable in full duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : unicast_pause_frame_detect: ENABLE or DISABLE unicast pause frame detect.
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_TransmitFlowCtrlEnable(ETH_HandleTypeDef *heth, uint32_t unicast_pause_frame_detect)
{
    volatile uint32_t tmpreg;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (heth->Instance->MACFCR & ETH_MACFCR_FCB_BPA)
        return HAL_ERROR;
    
    tmpreg = heth->Instance->MACFCR;
    
    if (unicast_pause_frame_detect)
        tmpreg |= ETH_MACFCR_UPFD;
    else
        tmpreg &= ~ETH_MACFCR_UPFD;
    
    tmpreg |= ETH_MACFCR_RFCE;
    
    heth->Instance->MACFCR = tmpreg;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Disable the transmit packets flow control.
*@Note    Only applicable in full duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_TransmitFlowCtrlDisable(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (heth->Instance->MACFCR & ETH_MACFCR_FCB_BPA)
        return HAL_ERROR;
    
    heth->Instance->MACFCR &= ~(ETH_MACFCR_UPFD | ETH_MACFCR_RFCE);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Enable receive packets back pressure. 
*@Note    Only applicable in half duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReceiveBackPressureEnable(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    heth->Instance->MACFCR |= ETH_MACFCR_TFCE | ETH_MACFCR_FCB_BPA;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Disable receive packets back pressure. 
*@Note    Only applicable in half duplex mode.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReceiveBackPressureDisable(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    heth->Instance->MACFCR &= ~(ETH_MACFCR_TFCE | ETH_MACFCR_FCB_BPA);
    
    return HAL_OK;
}

//	****************************************************************************
//  
//                              MMC
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization MMC parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitMMCDefaultParamter(ETH_HandleTypeDef *heth)
{
    // MMC
    heth->MMCConfig.UpdateForDroppedBroadcast = 0;
    heth->MMCConfig.FullHalfPreset = 0;
    heth->MMCConfig.Preset = 0;
    heth->MMCConfig.ResetOnRead = 0;
    heth->MMCConfig.StopRollover = 0;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the MMC parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetMMCParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_MMCInitTypeDef *pMMCConfig = &heth->MMCConfig;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    memset((void *)pMMCConfig, 0, sizeof(ETH_MMCInitTypeDef));
    tmpreg = heth->Instance->MMCCR;
    
    if (tmpreg & ETH_MMCCR_UCDBC)
    {
        pMMCConfig->UpdateForDroppedBroadcast = 1;
    }
    
    if (tmpreg & ETH_MMCCR_CNTPRSTLVL)
    {
        pMMCConfig->FullHalfPreset = 1;
    }
    
    if (tmpreg & ETH_MMCCR_CNTPRST)
    {
        pMMCConfig->Preset = 1;
    }
    
    if (tmpreg & ETH_MMCCR_RSTONRD)
    {
        pMMCConfig->ResetOnRead = 1;
    }
    
    if (tmpreg & ETH_MMCCR_CNTSTOPRO)
    {
        pMMCConfig->StopRollover = 1;
    }
    
    return (HAL_OK); 
}

/******************************************************************************
*@brief : Configuration the MMC. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MMCConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_MMCInitTypeDef *pMMCConfig = &heth->MMCConfig;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    tmpreg = 0;
    
    if (pMMCConfig->UpdateForDroppedBroadcast)
        SET_BIT(tmpreg, ETH_MMCCR_UCDBC);
    
    if (pMMCConfig->FullHalfPreset)
        SET_BIT(tmpreg, ETH_MMCCR_CNTPRSTLVL);
    
    if (pMMCConfig->Preset)
        SET_BIT(tmpreg, ETH_MMCCR_CNTPRST);
    
    if (pMMCConfig->ResetOnRead)
        SET_BIT(tmpreg, ETH_MMCCR_RSTONRD);
    
    if (pMMCConfig->StopRollover)
        SET_BIT(tmpreg, ETH_MMCCR_CNTSTOPRO);
    
    WRITE_REG(heth->Instance->MMCCR, tmpreg);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the MMC to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MMCDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitMMCDefaultParamter(heth);
    
    return HAL_ETH_MMCConfig(heth);
}

/******************************************************************************
*@brief : Gets the number of frames of various types.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetMMCCounter(ETH_HandleTypeDef *heth)
{
    ETH_MMCCounterTypeDef *pMMCCounter = &heth->MMCCounter;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    pMMCCounter->TxGoodFrameSingleCollision = heth->Instance->MMCTGFSCCR;
    pMMCCounter->TxGoodFrameMultipleCollision = heth->Instance->MMCTGFMCCR;
    pMMCCounter->TxGoodFrame = heth->Instance->MMCTGFCR;
    pMMCCounter->RxCRCErr = heth->Instance->MMCRFCECR;
    pMMCCounter->RxAlignmentErr = heth->Instance->MMCRFAECR;
    pMMCCounter->RxGoodUnicast = heth->Instance->MMCRGUFCR;
    
    return HAL_OK; 
}

/******************************************************************************
*@brief : Enable or Disable freeze MMC counter.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : NewState: the new state of freeze MMC counter.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MMCFreezeCounterEnable(ETH_HandleTypeDef *heth, uint32_t NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    SET_BIT(heth->Instance->MMCCR, ETH_MMCCR_CNTFREEZ);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Enable or Disable freeze MMC counter.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : NewState: the new state of freeze MMC counter.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MMCFreezeCounterDisable(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    CLEAR_BIT(heth->Instance->MMCCR, ETH_MMCCR_CNTFREEZ);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Reset the MMC counter.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_MMCResetCounter(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    SET_BIT(heth->Instance->MMCCR, ETH_MMCCR_CNTRST);
    
    return HAL_OK;
}


//	****************************************************************************
//  
//                              LPI
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization LPI parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitLPIDefaultParamter(ETH_HandleTypeDef *heth)
{
    // MMC
    heth->LPIConfig.TxAuto = 0;
    heth->LPIConfig.TxClockDisable = 0;
    heth->LPIConfig.EntryTime = 100;
    heth->LPIConfig.ExtiTime = 10;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the LPI parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetLPIParameter(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_LPITypeDef *pLPIConfig = &heth->LPIConfig;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    memset((void *)pLPIConfig, 0, sizeof(ETH_LPITypeDef));
    
    tmpreg = heth->Instance->MACLPICSR;
    
    if (tmpreg & ETH_MACLPICSR_LPITXA)
        pLPIConfig->TxAuto = 1;
    
    if (SYSCFG->SYSCR & SYSCFG_SYSCR_ETHMAC_TX_CLKGE)
        pLPIConfig->TxClockDisable = 1;
    
    if (tmpreg & ETH_MACLPICSR_PLSEN)
        pLPIConfig->EntryTime = (heth->Instance->MACLPITCR & ETH_MACLPITCR_LST) >> ETH_MACLPITCR_LST_Pos;
    
    pLPIConfig->ExtiTime = (heth->Instance->MACLPITCR & ETH_MACLPITCR_TWT) >> ETH_MACLPITCR_TWT_Pos;
    
    return HAL_OK; 
}

/******************************************************************************
*@brief : Configuration the LPI. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_LPIConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_LPITypeDef *pLPIConfig = &heth->LPIConfig;
    
    if (heth == NULL)
        return (HAL_ERROR);
    
    if (pLPIConfig->TxClockDisable)
        SYSCFG->SYSCR |= SYSCFG_SYSCR_ETHMAC_TX_CLKGE;
    else
        SYSCFG->SYSCR &= ~SYSCFG_SYSCR_ETHMAC_TX_CLKGE;
    
    heth->Instance->MACLPITCR = (((uint32_t)pLPIConfig->EntryTime << ETH_MACLPITCR_LST_Pos) & ETH_MACLPITCR_LST) | \
                                (((uint32_t)pLPIConfig->ExtiTime << ETH_MACLPITCR_TWT_Pos) & ETH_MACLPITCR_TWT);
    
    tmpreg = heth->Instance->MACLPICSR & ~(ETH_MACLPICSR_LPITXA | ETH_MACLPICSR_PLSEN);
    
    if (pLPIConfig->TxAuto)
        tmpreg |= ETH_MACLPICSR_LPITXA;
    
    heth->Instance->MACLPICSR = tmpreg;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the LPI to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_LPIDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitLPIDefaultParamter(heth);
    
    return HAL_ETH_LPIConfig(heth);
}

/******************************************************************************
*@brief : Enter LPI. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_EnterLPIMode(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    heth->Instance->MACLPICSR |= ETH_MACLPICSR_LPIEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Exit LPI. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ExitLPIMode(ETH_HandleTypeDef *heth)
{
    if (heth == NULL)
        return (HAL_ERROR);
    
    heth->Instance->MACLPICSR &= ~ETH_MACLPICSR_LPIEN;
    
    return HAL_OK;
}

//	****************************************************************************
//  
//                              PMT
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization the PMT parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitPMTDefaultParamter(ETH_HandleTypeDef *heth)
{
    memset((void *)&heth->PMTConfig, 0, sizeof(ETH_PMTInitTypeDef));
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the PMT parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetPMTParameter(ETH_HandleTypeDef *heth)
{
    uint32_t i;
    uint32_t err;
    volatile uint32_t tmpreg;
    ETH_PMTInitTypeDef *pPMTConfig = &heth->PMTConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    err = 0;
    heth->Instance->MACPMTCSR |= ETH_MACPMTCSR_RWFFPR;
    
    pPMTConfig->Filter[0].Mask = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[1].Mask = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[2].Mask = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[3].Mask = heth->Instance->MACRWUFF;
    
    tmpreg = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[0].Cmd = tmpreg & 0x0f;
    pPMTConfig->Filter[1].Cmd = (tmpreg >> 8) & 0x0f;
    pPMTConfig->Filter[2].Cmd = (tmpreg >> 16) & 0x0f;
    pPMTConfig->Filter[3].Cmd = (tmpreg >> 24) & 0x0f;
    
    tmpreg = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[0].Offset = tmpreg & 0xff;
    pPMTConfig->Filter[1].Offset = (tmpreg >> 8) & 0xff;
    pPMTConfig->Filter[2].Offset = (tmpreg >> 16) & 0xff;
    pPMTConfig->Filter[3].Offset = (tmpreg >> 24) & 0xff;
    
    tmpreg = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[0].CRC16 = tmpreg & 0xffff;
    pPMTConfig->Filter[1].CRC16 = (tmpreg >> 16) & 0xffff;
    
    tmpreg = heth->Instance->MACRWUFF;
    pPMTConfig->Filter[2].CRC16 = tmpreg & 0xffff;
    pPMTConfig->Filter[3].CRC16 = (tmpreg >> 16) & 0xffff;
    
    if (heth->Instance->MACPMTCSR & ETH_MACPMTCSR_GU)
        pPMTConfig->GlobalUnicast = 1;
    else
        pPMTConfig->GlobalUnicast = 0;

    
    for (i=0; i<8; i++)
    {
        tmpreg = heth->Instance->MACRWUFF;
    }
    
    for (i=0; i<4; i++)
    {
        if (pPMTConfig->Filter[i].Mask & BIT31)
        {
            err = 1;
            break;
        }
        if (pPMTConfig->Filter[i].Offset < 12)
        {
            err = 1;
            break;
        }
    }
    
    if (err == 0)
        return HAL_OK;
    else
        return HAL_ERROR;
}

/******************************************************************************
*@brief : Configuration the PMT. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PMTConfig(ETH_HandleTypeDef *heth)
{
    uint32_t i;
    uint32_t tmpreg;
    ETH_PMTInitTypeDef *pPMTConfig = &heth->PMTConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    heth->Instance->MACPMTCSR |= ETH_MACPMTCSR_RWFFPR;
    
    for (i=0; i<4; i++)
    {
        heth->Instance->MACRWUFF = pPMTConfig->Filter[i].Mask;
    }
    
    tmpreg = pPMTConfig->Filter[0].Cmd & 0x0f;
    tmpreg |= (pPMTConfig->Filter[1].Cmd & 0x0f) << 8;
    tmpreg |= (pPMTConfig->Filter[2].Cmd & 0x0f) << 16;
    tmpreg |= (pPMTConfig->Filter[3].Cmd & 0x0f) << 24;
    heth->Instance->MACRWUFF = tmpreg;
    
    tmpreg = pPMTConfig->Filter[0].Offset & 0xff;
    tmpreg |= (pPMTConfig->Filter[1].Offset & 0xff) << 8;
    tmpreg |= (pPMTConfig->Filter[2].Offset & 0xff) << 16;
    tmpreg |= (pPMTConfig->Filter[3].Offset & 0xff) << 24;
    heth->Instance->MACRWUFF = tmpreg;
    
    tmpreg = pPMTConfig->Filter[0].CRC16 & 0xffff;
    tmpreg |= (pPMTConfig->Filter[1].CRC16 & 0xffff) << 16;
    heth->Instance->MACRWUFF = tmpreg;
    
    tmpreg = pPMTConfig->Filter[2].CRC16 & 0xffff;
    tmpreg |= (pPMTConfig->Filter[3].CRC16 & 0xffff) << 16;
    heth->Instance->MACRWUFF = tmpreg;
    
    if (pPMTConfig->GlobalUnicast)
    {
        heth->Instance->MACPMTCSR |= BIT9;
    }
    else
    {
        heth->Instance->MACPMTCSR &= ~BIT9;
    }
    
    return (HAL_OK); 
}

/******************************************************************************
*@brief : Configuration the PMT to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PMTDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitPMTDefaultParamter(heth);
    
    return HAL_ETH_PMTConfig(heth);
}

/******************************************************************************
*@brief : Configuration the LPI to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_EnterPowerDownMode(ETH_HandleTypeDef *heth, uint32_t WakeupMode, uint32_t WakeupFrameEnable, uint32_t MagicPacketEnable)
{
    uint32_t timeout;
    
    if ((WakeupFrameEnable == 0) && (MagicPacketEnable == 0))
        return (HAL_ERROR);
    
    
    /* Disable the DMA transmission */
    heth->Instance->DMAOMR &= ~ETH_DMAOMR_ST;
    
    timeout = 0xffffff;
    while (heth->Instance->DMASR & ETH_DMASR_TS)
    {
        if (--timeout == 0)
            return (HAL_TIMEOUT);
    }
    
    /* Disable the MAC transmission */
    heth->Instance->MACCR &= ~ETH_MACCR_TE;
    
    /* Disable the MAC reception */
    heth->Instance->MACCR &= ~ETH_MACCR_RE;
    
    timeout = 0xffffff;
    while ((heth->Instance->MACDBGR & ETH_MACDBGR_RFFL) != 0)
    {
        if (--timeout == 0)
            return (HAL_TIMEOUT);
    }
    
    /* Disable the DMA reception */
    heth->Instance->DMAOMR &= ~ETH_DMAOMR_SR;
    
    __HAL_RCC_EXTI_CLK_ENABLE();
    
    EXTI->RTENR2 &= ~BIT1;
    EXTI->FTENR2 &= ~BIT1;
    EXTI->PDR2 = BIT1;
    if (WakeupMode & BIT0)
        EXTI->IENR2 |= BIT1;
    else
        EXTI->IENR2 &= ~BIT1;
    if (WakeupMode & BIT1)
        EXTI->EENR2 |= BIT1;
    else
        EXTI->EENR2 &= ~BIT1;
    EXTI->RTENR2 |= BIT1;
    
    if (WakeupMode)
        heth->Instance->MACIMR &= ~ETH_MACIMR_PIM;
    else
        heth->Instance->MACIMR |= ETH_MACIMR_PIM;
    
    if (WakeupFrameEnable)
        heth->Instance->MACPMTCSR |= ETH_MACPMTCSR_WFE;
    else
        heth->Instance->MACPMTCSR &= ~ETH_MACPMTCSR_WFE;
    
    if (MagicPacketEnable)
        heth->Instance->MACPMTCSR |= ETH_MACPMTCSR_MPE;
    else
        heth->Instance->MACPMTCSR &= ~ETH_MACPMTCSR_MPE;
    
    heth->Instance->MACPMTCSR |= ETH_MACPMTCSR_PD;
    
    heth->Instance->MACCR |= ETH_MACCR_RE;
    
    return (HAL_OK);
}


//	****************************************************************************
//  
//                              PTP
//  
//	****************************************************************************

/******************************************************************************
*@brief : Initialization PTP parameter to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_InitPTPDefaultParamter(ETH_HandleTypeDef *heth)
{
    memset((void *)&heth->PTPConfig, 0, sizeof(ETH_PTPInitTypeDef));
    
    heth->PTPConfig.IPV4Message = ENABLE;
    heth->PTPConfig.DigitalRollover = ENABLE;
    heth->PTPConfig.FineUpdate = ENABLE;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the PTP parameter value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_GetPTPParameter(ETH_HandleTypeDef *heth)
{
    uint32_t err;
    volatile uint32_t tmpreg;
    ETH_PTPInitTypeDef *pPTPConfig = &heth->PTPConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    err = 0;
    memset((void *)pPTPConfig, 0, sizeof(ETH_PTPInitTypeDef));
    
    // PTPTSCR
    tmpreg = heth->Instance->PTPTSCR;
    
    if (tmpreg & ETH_PTPTSCR_ASEN3)
        pPTPConfig->AuxiliarySnapshot |= BIT3;
    if (tmpreg & ETH_PTPTSCR_ASEN2)
        pPTPConfig->AuxiliarySnapshot |= BIT2;
    if (tmpreg & ETH_PTPTSCR_ASEN2)
        pPTPConfig->AuxiliarySnapshot |= BIT3;
    if (tmpreg & ETH_PTPTSCR_ASEN2)
        pPTPConfig->AuxiliarySnapshot |= BIT1;
    
    if (tmpreg & ETH_PTPTSCR_EMAFPFF)
        pPTPConfig->MACAddrFilter = ENABLE;
    
    pPTPConfig->Clock = (tmpreg & ETH_PTPTSCR_SPPFTS) >> ETH_PTPTSCR_SPPFTS_Pos;
    
    if (tmpreg & ETH_PTPTSCR_ESFMRTM)
        pPTPConfig->MasterMessage = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_ETSFEM)
        pPTPConfig->EventMessage = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_EPPFSIP4U)
        pPTPConfig->IPV4Message = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_EPPFSIP6U)
        pPTPConfig->IPV6Message = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_EPPEF)
        pPTPConfig->EthernetFrame = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_TSPTPPSV2E)
        pPTPConfig->Ver2 = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_TSR)
        pPTPConfig->DigitalRollover = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_TSARFE)
        pPTPConfig->AllFrames = ENABLE;
    
    if (tmpreg & ETH_PTPTSCR_TFCU)
        pPTPConfig->FineUpdate = ENABLE;
    
    // PTPPPSCR
    tmpreg = heth->Instance->PTPPPSCR ;
    
    if ((tmpreg & ETH_PTPPPSCR_TRGTMODSEL) == ETH_PTPPPSCR_TRGTMODSEL_0)
        err = 1;
    else
        pPTPConfig->PPS.TargetTime = (tmpreg & ETH_PTPPPSCR_TRGTMODSEL) >> ETH_PTPPPSCR_TRGTMODSEL_Pos;
    
    if (tmpreg & ETH_PTPPPSCR_PPSEN)
    {
        pPTPConfig->PPS.OutputMode = ETH_PPS_OUTPUT_MODE_CMD;
        pPTPConfig->PPS.Output = (tmpreg & ETH_PTPPPSCR_PPSCMD) >> ETH_PTPPPSCR_PPSCMD_Pos;
    }
    else
    {
        pPTPConfig->PPS.OutputMode = ETH_PPS_OUTPUT_MODE_PULSE;
        pPTPConfig->PPS.Output = (tmpreg & ETH_PTPPPSCR_PPSCTRL) >> ETH_PTPPPSCR_PPSCTRL_Pos;
    }
    
    // PTPPPSIR
     pPTPConfig->PPS.SignalInterval = heth->Instance->PTPPPSIR;
    
    // PTPPPSWR
     pPTPConfig->PPS.SignalWidth = heth->Instance->PTPPPSWR;
    
    if (err == 0)
        return HAL_OK;
    else
        return HAL_ERROR;
}

/******************************************************************************
*@brief : Configuration the PTP.
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPConfig(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tmpreg;
    ETH_PTPInitTypeDef *pPTPConfig = &heth->PTPConfig;

    if (heth == NULL)
        return (HAL_ERROR);
    
    // PTPTSCR
    tmpreg = heth->Instance->PTPTSCR & ~0x1E07FF02U;
    
    if (pPTPConfig->AuxiliarySnapshot & BIT3)
        tmpreg |= ETH_PTPTSCR_ASEN3;
    if (pPTPConfig->AuxiliarySnapshot & BIT2)
        tmpreg |= ETH_PTPTSCR_ASEN2;
    if (pPTPConfig->AuxiliarySnapshot & BIT1)
        tmpreg |= ETH_PTPTSCR_ASEN1;
    if (pPTPConfig->AuxiliarySnapshot & BIT0)
        tmpreg |= ETH_PTPTSCR_ASEN0;
    
    if (pPTPConfig->MACAddrFilter != DISABLE)
        tmpreg |= ETH_PTPTSCR_EMAFPFF;
    
    tmpreg |= ((uint32_t)pPTPConfig->Clock << ETH_PTPTSCR_SPPFTS_Pos) & ETH_PTPTSCR_SPPFTS;
    
    if (pPTPConfig->MasterMessage != DISABLE)
        tmpreg |= ETH_PTPTSCR_ESFMRTM;
    
    if (pPTPConfig->EventMessage != DISABLE)
        tmpreg |= ETH_PTPTSCR_ETSFEM;
    
    if (pPTPConfig->IPV4Message != DISABLE)
        tmpreg |= ETH_PTPTSCR_EPPFSIP4U;
    
    if (pPTPConfig->IPV6Message != DISABLE)
        tmpreg |= ETH_PTPTSCR_EPPFSIP6U;
    
    if (pPTPConfig->EthernetFrame != DISABLE)
        tmpreg |= ETH_PTPTSCR_EPPEF;
    
    if (pPTPConfig->Ver2 != DISABLE)
        tmpreg |= ETH_PTPTSCR_TSPTPPSV2E;
    
    if (pPTPConfig->DigitalRollover != DISABLE)
    {
        tmpreg |= ETH_PTPTSCR_TSR;
        ETH->PTPSSIR = 20u;
    }
    else
    {
        ETH->PTPSSIR = 43u;
    }
    
    if (pPTPConfig->AllFrames != DISABLE)
        tmpreg |= ETH_PTPTSCR_TSARFE;
    
    if (pPTPConfig->FineUpdate != DISABLE)
        tmpreg |= ETH_PTPTSCR_TFCU;
    
    heth->Instance->PTPTSCR = tmpreg;
    
    // PTPTSCR
    tmpreg = heth->Instance->PTPPPSCR & ~0x0000007FU;
    
    tmpreg |= ((uint32_t)pPTPConfig->PPS.TargetTime << ETH_PTPPPSCR_TRGTMODSEL_Pos) & ETH_PTPPPSCR_TRGTMODSEL;
    
    if (pPTPConfig->PPS.OutputMode == ETH_PPS_OUTPUT_MODE_CMD)
    {
        tmpreg |= ETH_PTPPPSCR_PPSEN;
        tmpreg |= ((uint32_t)pPTPConfig->PPS.Output << ETH_PTPPPSCR_PPSCMD_Pos) & ETH_PTPPPSCR_PPSCMD;
    }
    else
    {
        tmpreg |= ((uint32_t)pPTPConfig->PPS.Output << ETH_PTPPPSCR_PPSCTRL_Pos) & ETH_PTPPPSCR_PPSCTRL;
    }
    
    // PTPPPSIR
     heth->Instance->PTPPPSIR = pPTPConfig->PPS.SignalInterval;
    
    // PTPPPSWR
     heth->Instance->PTPPPSWR = pPTPConfig->PPS.SignalWidth;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configuration the PTP to the default value. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPDefaultConfig(ETH_HandleTypeDef *heth)
{
    HAL_ETH_InitPTPDefaultParamter(heth);
    
    return HAL_ETH_PTPConfig(heth);
}

/******************************************************************************
*@brief : Start PTP. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: Initial system time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPStart(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    uint32_t timeout;
    
    if (timestamp->nsec & BIT31)
        return HAL_ERROR;
    
    heth->Instance->MACIMR |= ETH_MACIMR_TIM;
    
    heth->Instance->PTPTSCR |= ETH_PTPTSCR_TE;
    
    if (heth->PTPConfig.DigitalRollover)
        heth->Instance->PTPSSIR  = 20;
    else
        heth->Instance->PTPSSIR  = 43;
    
    if (heth->PTPConfig.FineUpdate)
    {
        heth->Instance->PTPTSAR = (uint32_t)((double)4294967296 * (double)50000000 / (double)(HAL_RCC_GetHCLKFreq()));
        
        timeout = ETH_PTP_TIMEOUT;
        while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
        {
            if (--timeout == 0)
                return HAL_ERROR;
        }
        
        heth->Instance->PTPTSCR |= ETH_PTPTSCR_TARU;
        
        timeout = ETH_PTP_TIMEOUT;
        while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
        {
            if (--timeout == 0)
                return HAL_ERROR;
        }
        
        heth->Instance->PTPTSCR |= ETH_PTPTSCR_TFCU;
    }
    else
    {
        heth->Instance->PTPTSCR &= (~(uint32_t)ETH_PTPTSCR_TFCU);
    }
    
    heth->Instance->PTPTSHUR = timestamp->sec;
    if (heth->PTPConfig.DigitalRollover)
        heth->Instance->PTPTSLUR = timestamp->nsec;
    else
        heth->Instance->PTPTSLUR = (uint32_t)((uint64_t)timestamp->nsec * 0x80000000 / 1000000000);
    
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TI)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    heth->Instance->PTPTSCR |= ETH_PTPTSCR_TI;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TI)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Stop PTP. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPStop(ETH_HandleTypeDef *heth)
{
    heth->Instance->MACIMR |= ETH_MACIMR_TIM;
    
    heth->Instance->PTPTSCR &= ~ETH_PTPTSCR_TITE;
    
    heth->Instance->PTPTSCR &= ~ETH_PTPTSCR_TE;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Adjust the clock frequency. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPAdjFreq(ETH_HandleTypeDef *heth, int32_t freq)
{
    uint32_t temp;
    uint32_t sysclk;
    uint32_t timeout;
    
    sysclk = HAL_RCC_GetHCLKFreq();
    
    if (freq >= 0)
    {
        sysclk += (uint32_t)freq;
    }
    else
    {
        temp = (uint32_t)(0 - freq);
        sysclk -= temp;
    }
     
    heth->Instance->PTPTSAR = (uint32_t)((double)4294967296 * (double)50000000 / (double)sysclk);
        
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    heth->Instance->PTPTSCR |= ETH_PTPTSCR_TARU;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Adjust the clock frequency. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPUpdateOffset(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    uint32_t sign;
    uint32_t addend;
    uint32_t timeout;

    sign = 0;
    if (timestamp->nsec & ETH_PTPTSLUR_AST)
    {
        sign = ETH_PTPTSLUR_AST;
        timestamp->nsec &= ~ETH_PTPTSLUR_AST;
    }

    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TI)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    /* read old addend register value*/
    addend = ETH->PTPTSAR;

    heth->Instance->PTPTSHUR = timestamp->sec;
    if (heth->PTPConfig.DigitalRollover)
        heth->Instance->PTPTSLUR = sign | (uint32_t)timestamp->nsec;
    else
        heth->Instance->PTPTSLUR = sign | (uint32_t)((uint64_t)timestamp->nsec * (uint64_t)0x80000000 / (uint64_t)1000000000);
    
    heth->Instance->PTPTSCR |= ETH_PTPTSCR_TU;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }

    // update
    ETH->PTPTSAR = addend;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    heth->Instance->PTPTSCR |= ETH_PTPTSCR_TARU;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TARU)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Calibration time. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: calibration time value.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPTimeSetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    uint32_t sign;
    uint32_t timeout;

    sign = 0;
    if (timestamp->nsec & ETH_PTPTSLUR_AST)
    {
        sign = ETH_PTPTSLUR_AST;
        timestamp->nsec &= ~ETH_PTPTSLUR_AST;
    }
    
    ETH->PTPTSHUR = timestamp->sec;
    if (ETH->PTPTSCR & ETH_PTPTSCR_TSR)
        ETH->PTPTSLUR = sign | timestamp->nsec;
    else
        ETH->PTPTSLUR = sign | (uint32_t)((uint64_t)timestamp->nsec * (uint64_t)0x80000000 / (uint64_t)1000000000);
    
    ETH->PTPTSCR |= ETH_PTPTSCR_TI;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTSCR & ETH_PTPTSCR_TI)
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the system time. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: return to system time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPGetSystemTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    if (heth->Instance->PTPTSCR & ETH_PTPTSCR_TSR)
        timestamp->nsec = (uint32_t)ETH->PTPTSLR;
    else
        timestamp->nsec = (uint32_t)(((uint64_t)ETH->PTPTSLR * (uint64_t)1000000000) >> 31U);
    timestamp->sec = ETH->PTPTSHR;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the target time. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: return to target time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPGetTargetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    uint32_t timeout;
    
    timeout = ETH_PTP_TIMEOUT;
    while (heth->Instance->PTPTTLR & ETH_PTPTTLR_TTRB )
    {
        if (--timeout == 0)
            return HAL_ERROR;
    }
    
    if (heth->Instance->PTPTSCR & ETH_PTPTSCR_TSR)
        timestamp->nsec = (uint32_t)ETH->PTPTTLR;
    else
        timestamp->nsec = (uint32_t)(((uint64_t)ETH->PTPTTLR * (uint64_t)1000000000) >> 31);
    timestamp->sec = ETH->PTPTTHR ;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Set the target time. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: return to target time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPSetTargetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    if (timestamp->nsec & ETH_PTPTSLUR_AST)
        return HAL_ERROR;
    
    heth->Instance->PTPTTHR = timestamp->sec;
    if (heth->Instance->PTPTSCR & ETH_PTPTSCR_TSR)
        heth->Instance->PTPTTLR = (uint32_t)timestamp->nsec;
    else
        heth->Instance->PTPTTLR = (uint32_t)((uint64_t)timestamp->nsec * (uint64_t)0x80000000 / (uint64_t)1000000000);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the auxiliary timestamp. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : timestamp: return to target time.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_PTPGetAuxiliaryTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp)
{
    if (heth->Instance->PTPTSCR & ETH_PTPTSCR_TSR)
        timestamp->nsec = (uint32_t)ETH->PTPATSNR;
    else
        timestamp->nsec = (uint32_t)(((uint64_t)ETH->PTPATSNR * (uint64_t)1000000000) >> 31U);
    timestamp->sec = ETH->PTPATSSR;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Gets the PTP status. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : status: return to the PTP status.
*@return: HAL status.
******************************************************************************/
uint32_t HAL_ETH_PTPGetStatus(ETH_HandleTypeDef *heth, ETH_PTPStatusTypeDef *status)
{
    volatile uint32_t tempreg;
    
    tempreg = heth->Instance->PTPTSSR;
    
    status->SnapshotsNbr = (tempreg & ETH_PTPTSSR_ATSNS) >> ETH_PTPTSSR_ATSNS_Pos;
    
    if (tempreg & ETH_PTPTSSR_ATSTM)
        status->TriggerMissed = ENABLE;
    else
        status->TriggerMissed = DISABLE;
    
    status->TriggerFlag = (tempreg & ETH_PTPTSSR_ATSTI) >> ETH_PTPTSSR_ATSTI_Pos;
    
    return HAL_OK;
}


// ****************************************************************************
//  
//                              SMI
//  
// ****************************************************************************

/******************************************************************************
*@brief : Configuration the SMI. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ConfigSMI(ETH_HandleTypeDef *heth)
{
    volatile uint32_t tempreg;
    uint32_t hclk;
    
    //SMI????
    tempreg = heth->Instance->MACMIIAR;
    
    tempreg &= ~ETH_MACMIIAR_CR;

    hclk = HAL_RCC_GetHCLKFreq();

    if (hclk < 20000000)
        return (HAL_ERROR);
    else if (hclk < 35000000)
        tempreg |= ETH_MACMIIAR_CR_1;
    else if (hclk <= 60000000)
        tempreg |= ETH_MACMIIAR_CR_1 | ETH_MACMIIAR_CR_0;
    else if (hclk < 100000000)
        tempreg |= 0;
    else if (hclk < 150000000)
        tempreg |= ETH_MACMIIAR_CR_0;
    else if (hclk < 250000000)
        tempreg |= ETH_MACMIIAR_CR_2;
    else if (hclk < 300000000)
        tempreg |= ETH_MACMIIAR_CR_2 | ETH_MACMIIAR_CR_0;
    else
        return (HAL_ERROR);
    
    heth->Instance->MACMIIAR = (uint32_t)tempreg;
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Reads a PHY register
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : PHYAddr: PHY address.
*@param : PHYReg: PHY register address.
*@param : RegValue: PHY register value.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_ReadPHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg, uint32_t *RegValue)
{
    volatile uint32_t tmpreg;     
    uint32_t tickstart = 0;

    if (READ_BIT(heth->Instance->MACMIIAR, ETH_MACMIIAR_MB))
        return HAL_BUSY;
    
    WRITE_REG(tmpreg, heth->Instance->MACMIIAR);
    
    MODIFY_REG(tmpreg, ETH_MACMIIAR_PA, (PHYAddr << ETH_MACMIIAR_PA_Pos));
    MODIFY_REG(tmpreg, ETH_MACMIIAR_MR, (PHYReg << ETH_MACMIIAR_MR_Pos));
    CLEAR_BIT(tmpreg, ETH_MACMIIAR_MW);
    SET_BIT(tmpreg, ETH_MACMIIAR_MB);
    WRITE_REG(heth->Instance->MACMIIAR, tmpreg);
    
    // ?????
    tickstart = HAL_GetTick();
    while(READ_BIT(heth->Instance->MACMIIAR, ETH_MACMIIAR_MB))
    {
        if((HAL_GetTick() - tickstart ) > ETH_TIMEOUT_PHY_WRITE)
            return HAL_TIMEOUT;
    }

    if (RegValue)
    {
        *RegValue = (uint16_t)(heth->Instance->MACMIIDR);
    }
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Writes to a PHY register
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : PHYAddr: PHY address.
*@param : PHYReg: PHY register address.
*@param : RegValue: RegValue the value to write.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_WritePHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg, uint32_t RegValue)
{
    volatile uint32_t tmpreg = 0;
    uint32_t tickstart = 0;

    if (READ_BIT(heth->Instance->MACMIIAR, ETH_MACMIIAR_MB))
        return HAL_BUSY;
    
    WRITE_REG(tmpreg, heth->Instance->MACMIIAR);
    
    MODIFY_REG(tmpreg, ETH_MACMIIAR_PA, (PHYAddr << ETH_MACMIIAR_PA_Pos));
    MODIFY_REG(tmpreg, ETH_MACMIIAR_MR, (PHYReg << ETH_MACMIIAR_MR_Pos));
    SET_BIT(tmpreg, ETH_MACMIIAR_MW);
    SET_BIT(tmpreg, ETH_MACMIIAR_MB);
    
    WRITE_REG(heth->Instance->MACMIIDR, RegValue);
    
    WRITE_REG(heth->Instance->MACMIIAR, tmpreg);
    
    //??????
    tickstart = HAL_GetTick();
    while(READ_BIT(heth->Instance->MACMIIAR, ETH_MACMIIAR_MB))
    {
        if((HAL_GetTick() - tickstart ) >= ETH_TIMEOUT_PHY_READ)
            return HAL_TIMEOUT;
    }

    return HAL_OK; 
}

//	****************************************************************************
//  
//                              Rx Clock Delay
//  
//	****************************************************************************

/******************************************************************************
*@brief : Configuration the Ethernet Rx clock delay. 
*@param : heth: pointer to a ETH_HandleTypeDef structure that contains
*          the configuration information for ETHERNET module.
*@param : uint: Specifies the number of delay steps per unit delay.
*@param : len: Specifies the phase of the output clock.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_ETH_RxClockDelayConfig(ETH_HandleTypeDef *heth, uint32_t uint, uint32_t len)
{
    UNUSED(heth);
    
    SYSCFG->SYSCR |= SYSCFG_SYSCR_ETHMAC_RX_DLYSEL;
    ETH_DLYB->CR = DLYB_CR_DEN;
    ETH_DLYB->CR |= DLYB_CR_SEN;
    ETH_DLYB->CFGR = ((uint << DLYB_CFGR_UNIT_Pos) & DLYB_CFGR_UNIT_Msk) | ((len << DLYB_CFGR_SEL_Pos) & DLYB_CFGR_SEL_Msk);
    HAL_SimpleDelay(10);
    ETH_DLYB->CR &= ~DLYB_CR_SEN;
    
    return HAL_OK;
}








