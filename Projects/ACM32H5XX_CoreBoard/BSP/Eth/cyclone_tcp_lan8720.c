
#include "core/net.h"
#include "cyclone_tcp_lan8720.h"
#include "acm32h5xx_coreboard.h"

//Underlying network interface
static NetInterface *nicDriverInterface;

#if (DATA_ACCELERATE_ENABLE == 1)
#define NO_CACHE  __attribute__((section("NO_CACHE")))
#else
#define NO_CACHE
#endif

ETH_HandleTypeDef ETH_Handle;
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT] NO_CACHE;
uint8_t TX_Buff[ETH_TX_DESC_CNT][ETH_TX_BUFF_SIZE] NO_CACHE;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT] NO_CACHE;
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFF_SIZE] NO_CACHE;

ETH_BuffTypeDef TxBuffTab[ETH_TX_DESC_CNT] NO_CACHE;
ETH_BuffTypeDef RxBuffTab[ETH_RX_DESC_CNT] NO_CACHE;

/**
 * @brief ACM32H5 Ethernet MAC driver
 **/

const NicDriver acm32h5xxEthDriver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   BSP_ETH_Init,
   BSP_ETH_Tick,
   BSP_ETH_EnableIrq,
   BSP_ETH_DisableIrq,
   BSP_ETH_EventHandler,
   BSP_ETH_SendPacket,
   BSP_ETH_UpdateMacAddrFilter,
   BSP_ETH_UpdateMacConfig,
   BSP_ETH_WritePhyReg,
   BSP_ETH_ReadPhyReg,
   TRUE,
   TRUE,
   TRUE,
   TRUE
};


/**
 * @brief STM32H7 Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t BSP_ETH_Init(NetInterface *interface)
{
    error_t error;

    //Save underlying network interface
    nicDriverInterface = interface;

    HAL_ETH_InitDefaultParamter(&ETH_Handle);

    //printfS("init\r\n");
    memcpy((void *)ETH_Handle.Init.MACAddr, (void *)interface->macAddr.b, 6);
    
    ETH_Handle.Instance                     = ETH;
    ETH_Handle.Init.MediaInterface          = ETH_MEDIA_INTERFACE_RMII;
    ETH_Handle.Init.AutoNegotiation         = ENABLE;
    ETH_Handle.Init.Speed                   = ETH_SPEED_100M;
    ETH_Handle.Init.DuplexMode              = ETH_MODE_FULL_DUPLEX;
    ETH_Handle.Init.AutoCRC                 = ENABLE;
    ETH_Handle.Init.AutoPad                 = ENABLE;
    ETH_Handle.Init.AutoChecksumInsertion   = ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD;
    ETH_Handle.Init.PhyAddress              = ETH_PHY_ADDR;
    ETH_Handle.Init.TxBuff                  = TX_Buff[0];
    ETH_Handle.Init.TxBuffNbr               = ETH_TX_DESC_CNT;
    ETH_Handle.Init.TxBuffLen               = ETH_TX_BUFF_SIZE;
    ETH_Handle.Init.RxBuff                  = Rx_Buff[0];
    ETH_Handle.Init.RxBuffNbr               = ETH_RX_DESC_CNT;
    ETH_Handle.Init.RxBuffLen               = ETH_RX_BUFF_SIZE;
    ETH_Handle.Init.TxDesc                  = DMATxDscrTab;
    ETH_Handle.Init.RxDesc                  = DMARxDscrTab;
    ETH_Handle.Init.TxDescNbr               = ETH_TX_DESC_CNT;
    ETH_Handle.Init.RxDescNbr               = ETH_RX_DESC_CNT;
    ETH_Handle.Init.TxDescListMode          = ETH_DESC_LIST_MODE_LIST;
    ETH_Handle.Init.RxDescListMode          = ETH_DESC_LIST_MODE_LIST;
    ETH_Handle.Init.TxBuffTab               = TxBuffTab;
    ETH_Handle.Init.TxBuffNodeNbr           = ETH_TX_DESC_CNT; 
    ETH_Handle.Init.RxBuffTab               = RxBuffTab;
    ETH_Handle.Init.RxBuffNodeNbr           = ETH_RX_DESC_CNT; 
    ETH_Handle.Init.Delay.Uint              = 2; 
    ETH_Handle.Init.Delay.Len               = 5; 
    
    #if (ETH_VLAN_SUPPORT == ENABLED)
    if (interface->vlanId)
    {
        if (((interface->vlanId & 0xfffu) != 0u) && ((interface->vlanId & 0xfffu) != 4095u))
        {
            ETH_Handle.MACConfig.VLAN.Enable = ENABLE;
            ETH_Handle.MACConfig.VLAN.Mode = ETH_VLAN_MODE_REPLACE;
            ETH_Handle.MACConfig.VLAN.Tag = interface->vlanId;
        }
    }
    #endif
    
    ETH_Handle.IT.TxComplete    = ENABLE;
    ETH_Handle.IT.RxComplete    = ENABLE;
    ETH_Handle.IT.Error         = DISABLE;
    ETH_Handle.IT.MMCTx         = DISABLE;
    ETH_Handle.IT.MMCRx         = DISABLE;
    ETH_Handle.IT.LPI           = DISABLE;
    ETH_Handle.IT.PTP           = DISABLE;
    ETH_Handle.IT.PMT           = DISABLE;
    
    if (HAL_ETH_Init(&ETH_Handle) != HAL_OK)
    {
        return ERROR_FAILURE;
    }
    
    if(interface->phyDriver != NULL)
        error = interface->phyDriver->init(interface);
    else
        error = ERROR_FAILURE;

    if(error)
        return error;

    BSP_ETH_UpdateMacAddrFilter(interface);

    HAL_ETH_Start (&ETH_Handle);
        
    osSetEvent(&interface->nicTxEvent);

    return NO_ERROR;
}

/**
 * @brief STM32H7 Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to handle periodic
 * operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void BSP_ETH_Tick(NetInterface *interface)
{
    static uint32_t state = 0xff;
    
    //Valid Ethernet PHY or switch driver?
    if(interface->phyDriver != NULL)
    {
        //Handle periodic operations
        interface->phyDriver->tick(interface);
    }
    else if(interface->switchDriver != NULL)
    {
        //Handle periodic operations
        interface->switchDriver->tick(interface);
    }
    else
    {
        //Just for sanity
    }
    
    if (state != (uint32_t)interface->linkState)
    {
        state = interface->linkState;
    }
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void BSP_ETH_EnableIrq(NetInterface *interface)
{
    UNUSED(interface);
    
    //Enable Ethernet MAC interrupts
    NVIC_EnableIRQ(ETH_IRQn);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void BSP_ETH_DisableIrq(NetInterface *interface)
{
    UNUSED(interface);
    
    //Disable Ethernet MAC interrupts
    NVIC_DisableIRQ(ETH_IRQn);
}

volatile uint32_t g_send_buf_idle_num = 32;

/* HAL callback: Tx transfer completed (frame sent) */
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
    
    osSetEventFromIsr(&nicDriverInterface->nicTxEvent);
}

/* HAL callback: Rx transfer completed (frame received) */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
    
    //Set event flag
    nicDriverInterface->nicEvent = TRUE;
    //Notify the TCP/IP stack of the event
    osSetEventFromIsr(&netEvent);
}


/**
 * @brief Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void BSP_ETH_EventHandler(NetInterface *interface)
{
    error_t error;

    //Process all pending packets
    do
    {
        //Read incoming packet
        error = BSP_ETH_ReceivePacket(interface);

        //No more data in the receive buffer?
    } while(error != ERROR_BUFFER_EMPTY);
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @param[in] ancillary Additional options passed to the stack along with
 *   the packet
 * @return Error code
 **/
error_t BSP_ETH_SendPacket(NetInterface *interface, const NetBuffer *src_buff, size_t src_offset, NetTxAncillary *ancillary)
{
    uint32_t i;
    uint32_t len;
    uint32_t temp_len;
    uint32_t checksum;
    uint8_t *buff;
    ETH_DMADescTypeDef *desc;
    static uint32_t desc_index = 0;
    
    UNUSED(ancillary);
    
    desc = &DMATxDscrTab[desc_index];

    while (READ_BIT(desc->DESC0, ETH_DMA_TX_DESC0_OWN));
    
    if (netBufferGetLength(src_buff) > ETH_TX_BUFF_SIZE)
        return ERROR_INVALID_LENGTH;
    
    checksum = ETH_Handle.Init.AutoChecksumInsertion;
    buff = (uint8_t *)desc->Buff1;
    
    len = 0;
    for (i=0; i<src_buff->chunkCount; i++)
    {
        if (src_offset)
        {
            if (src_offset >= src_buff->chunk[i].length)
            {
                src_offset -= src_buff->chunk[i].length;
                continue;
            }
            else
            {
                len = src_buff->chunk[i].length - src_offset;
                memcpy((void *)buff, (void *)(src_buff->chunk[i].address + src_offset), len);
                
                buff += len;
                src_offset = 0;
            }
        }
        else
        {
            temp_len = src_buff->chunk[i].length;
            memcpy((void *)buff, (void *)src_buff->chunk[i].address, temp_len);
            
            buff += temp_len;
            len += temp_len;
        }
    }
    
    SET_BIT(ETH->DMAIER , ETH_DMAIER_NISE | ETH_DMAIER_TIE);
    WRITE_REG(desc->DESC0, ETH_DMA_TX_DESC0_IC | ETH_DMA_TX_DESC0_FS | ETH_DMA_TX_DESC0_LS | ETH_DMA_TX_DESC0_TCH);
    if (checksum == ETH_CHECKSUM_INSERTION_CTRL_DISABLE)
    {
    }
    else if (checksum == ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD)
    {
        MODIFY_REG(desc->DESC0, ETH_DMA_TX_DESC0_CIC, ETH_DMA_TX_DESC0_CIC_0);
    }
    else if (checksum == ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD_PAYLOAD)
    {
        MODIFY_REG(desc->DESC0, ETH_DMA_TX_DESC0_CIC, ETH_DMA_TX_DESC0_CIC_1);
    }
    else if (checksum == ETH_CHECKSUM_INSERTION_CTRL_FULL)
    {
        WRITE_REG(desc->DESC0, ETH_DMA_TX_DESC0_CIC);
    }
    
    
    WRITE_REG(desc->DESC1, len  << ETH_DMA_TX_DESC1_TBS1_Pos);
    WRITE_REG(desc->DESC2, desc->Buff1);
    
    SET_BIT(desc->DESC0, ETH_DMA_TX_DESC0_OWN);
    
    ETH->DMASR = ETH_DMASR_TBUS;
    ETH->DMATPDR = 0;
    
    desc_index++;
    if (desc_index >= ETH_TX_DESC_CNT)
        desc_index = 0;
    
    desc = &DMATxDscrTab[desc_index];
    
    if (READ_BIT(desc->DESC0, ETH_DMA_TX_DESC0_OWN) == 0)
    {
        osSetEvent(&interface->nicTxEvent);
    }
    
    return NO_ERROR;
}

error_t BSP_ETH_ReceivePacket(NetInterface *interface)
{
    uint8_t *buff;
    uint32_t len;
    NetRxAncillary ancillary;
    ETH_DMADescTypeDef *desc;
    static uint32_t desc_index = 0;
    
    desc = &DMARxDscrTab[desc_index];
    
    if (READ_BIT(desc->DESC0, ETH_DMA_RX_DESC0_OWN))
        return ERROR_BUFFER_EMPTY;
        
    desc_index++;
    if (desc_index >= ETH_RX_DESC_CNT)
        desc_index = 0;
    
    if ((READ_REG(desc->DESC0) & (ETH_DMA_RX_DESC0_FS | ETH_DMA_RX_DESC0_LS | ETH_DMA_RX_DESC0_ES)) != (ETH_DMA_RX_DESC0_FS | ETH_DMA_RX_DESC0_LS))
    {
        WRITE_REG(desc->DESC2, desc->Buff1);
        WRITE_REG(desc->DESC3, desc->Buff2);
        SET_BIT(desc->DESC0, ETH_DMA_RX_DESC0_OWN);
        
        ETH->DMASR = ETH_DMASR_RBUS;
        ETH->DMARPDR = 0;
    
        return ERROR_BUFFER_EMPTY;
    }
    
    buff = (uint8_t *)desc->Buff1;
    len = (READ_REG(desc->DESC0) & ETH_DMA_RX_DESC0_FL) >> ETH_DMA_RX_DESC0_FL_Pos;
    
    ancillary = NET_DEFAULT_RX_ANCILLARY;
    nicProcessPacket(interface, buff, len, &ancillary);
    
    WRITE_REG(desc->DESC2, desc->Buff1);
    WRITE_REG(desc->DESC3, desc->Buff2);
    SET_BIT(desc->DESC0, ETH_DMA_RX_DESC0_OWN);
    
    ETH->DMASR = ETH_DMASR_RBUS;
    ETH->DMARPDR = 0;
    
    return NO_ERROR;
}

/**
 * @brief Configure MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t BSP_ETH_UpdateMacAddrFilter(NetInterface *interface)
{
    uint_t i;
    uint_t k;
    uint32_t crc;
    MacFilterEntry *entry;

    HAL_ETH_InitAddrFilterDefaultParamter(&ETH_Handle);
    
    //Promiscuous mode?
    if(interface->promiscuous)
    {
        //Pass all incoming frames regardless of their destination address
        ETH_Handle.FilterConfig.Addr.PromiscuousMode = ENABLE;
    }
    else
    {
        //The MAC address filter contains the list of MAC addresses to accept
        //when receiving an Ethernet frame
        for(i = 0; i < MAC_ADDR_FILTER_SIZE; i++)
        {
            //Point to the current entry
            entry = &interface->macAddrFilter[i];

            //Valid entry?
            if(entry->refCount > 0)
            {
                //Multicast address?
                if(macIsMulticastAddr(&entry->addr))
                {
                    //Compute CRC over the current MAC address
                    crc = BSP_ETH_CalcCrc(&entry->addr, sizeof(MacAddr));

                    //The upper 6 bits in the CRC register are used to index the
                    //contents of the hash table
                    k = (crc >> 26) & 0x3F;

                    //Update hash table contents
                    ETH_Handle.FilterConfig.Addr.HashTable[k / 32] |= (1 << (k % 32));
                }
                else
                {
                    //Up to 3 additional MAC addresses can be specified
                    if(i < 3)
                    {
                        //Save the unicast address
                        ETH_Handle.FilterConfig.Addr.Addr[i].Enable = ENABLE;
                        ETH_Handle.FilterConfig.Addr.Addr[i].GroupFilter = DISABLE;
                        ETH_Handle.FilterConfig.Addr.Addr[i].PerfectSourAddr = DISABLE;
                        memcpy((void *)ETH_Handle.FilterConfig.Addr.Addr[i].Addr, (void *)entry->addr.b, 6);
                    }
                }
            }
        }

        //Check whether frames with a multicast destination address should be
        //accepted
        if(interface->acceptAllMulticast)
        {
            //Configure the receive filter
             ETH_Handle.FilterConfig.Addr.HashPerfectFilter = ENABLE;
             ETH_Handle.FilterConfig.Addr.PassAllMulticast = ENABLE;
        }
        else
        {
            //Configure the receive filter
             ETH_Handle.FilterConfig.Addr.HashPerfectFilter = ENABLE;
             ETH_Handle.FilterConfig.Addr.HushMulticastFramesFilter = ENABLE;
        }
    }
    HAL_ETH_AddrFilterConfig(&ETH_Handle);
    
    
    #if (ETH_VLAN_SUPPORT == ENABLED)
    
    HAL_ETH_InitVLANFilterDefaultParamter(&ETH_Handle);
    
    if (interface->vlanId)
    {
        if (((interface->vlanId & 0xfffu) != 0u) && ((interface->vlanId & 0xfffu) != 4095u))
        {
            ETH_Handle.FilterConfig.VLAN.Enable = ENABLE;
            ETH_Handle.FilterConfig.VLAN.Tag = interface->vlanId;
        }
    }
    
    HAL_ETH_SetVLANFilterConfig(&ETH_Handle);
    
    #endif
    
    //Successful processing
    return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t BSP_ETH_UpdateMacConfig(NetInterface *interface)
{
   uint32_t config;

   //Read current MAC configuration
   config = ETH->MACCR;

   //10BASE-T or 100BASE-TX operation mode?
   if(interface->linkSpeed == NIC_LINK_SPEED_100MBPS)
   {
      config |= ETH_MACCR_FES;
   }
   else
   {
      config &= ~ETH_MACCR_FES;
   }

   //Half-duplex or full-duplex mode?
   if(interface->duplexMode == NIC_FULL_DUPLEX_MODE)
   {
      config |= ETH_MACCR_DM;
   }
   else
   {
      config &= ~ETH_MACCR_DM;
   }

   //Update MAC configuration register
   ETH->MACCR = config;

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] opcode Access type (2 bits)
 * @param[in] phyAddr PHY address (5 bits)
 * @param[in] regAddr Register address (5 bits)
 * @param[in] data Register value
 **/

void BSP_ETH_WritePhyReg(uint8_t opcode, uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    UNUSED(phyAddr);
    
   //Valid opcode?
   if(opcode == SMI_OPCODE_WRITE)
   {
       HAL_ETH_WritePHYRegister(&ETH_Handle, ETH_Handle.Init.PhyAddress, regAddr, data);
   }
   else
   {
      //The MAC peripheral only supports standard Clause 22 opcodes
   }
}


/**
 * @brief Read PHY register
 * @param[in] opcode Access type (2 bits)
 * @param[in] phyAddr PHY address (5 bits)
 * @param[in] regAddr Register address (5 bits)
 * @return Register value
 **/

uint16_t BSP_ETH_ReadPhyReg(uint8_t opcode, uint8_t phyAddr, uint8_t regAddr)
{
   uint32_t data;

    UNUSED(phyAddr);
    
   //Valid opcode?
   if(opcode == SMI_OPCODE_READ)
   {
       
       HAL_ETH_ReadPHYRegister(&ETH_Handle, ETH_Handle.Init.PhyAddress, regAddr, &data);
   }
   else
   {
      //The MAC peripheral only supports standard Clause 22 opcodes
      data = 0;
   }

   //Return the value of the PHY register
   return data;
}


/**
 * @brief CRC calculation
 * @param[in] data Pointer to the data over which to calculate the CRC
 * @param[in] length Number of bytes to process
 * @return Resulting CRC value
 **/

uint32_t BSP_ETH_CalcCrc(const void *data, size_t length)
{
   uint_t i;
   uint_t j;
   uint32_t crc;
   const uint8_t *p;

   //Point to the data over which to calculate the CRC
   p = (uint8_t *) data;
   //CRC preset value
   crc = 0xFFFFFFFF;

   //Loop through data
   for(i = 0; i < length; i++)
   {
      //The message is processed bit by bit
      for(j = 0; j < 8; j++)
      {
         //Update CRC value
         if((((crc >> 31) ^ (p[i] >> j)) & 0x01) != 0)
         {
            crc = (crc << 1) ^ 0x04C11DB7;
         }
         else
         {
            crc = crc << 1;
         }
      }
   }

   //Return CRC value
   return ~crc;
}
