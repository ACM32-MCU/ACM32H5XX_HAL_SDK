/******************************************************************************
*@file  : hal_eth.h
*@brief : Header file of ETH HAL module.
******************************************************************************/

#ifndef __HAL_ETH_H__
#define __HAL_ETH_H__

#include "acm32h5xx_hal_conf.h"


/** 
  * @brief ETH Buffers List structure definition
  */

typedef struct __ETH_BufferTypeDef
{
  uint8_t *Buff;
  uint32_t Len;
  struct __ETH_BufferTypeDef *next;
} __attribute__((aligned(4))) ETH_BuffTypeDef;

/** 
  * @brief ETH DMA Descriptor structure definition
  */

typedef struct  
{
    __IO uint32_t DESC0;
    __IO uint32_t DESC1;
    __IO uint32_t DESC2;
    __IO uint32_t DESC3;
    __IO uint32_t DESC4;
    __IO uint32_t DESC5;
    __IO uint32_t DESC6;
    __IO uint32_t DESC7;
    __IO uint32_t Buff1;                /* used to store tx/rx buffer 1 address */
    __IO uint32_t Buff2;                /* used to store tx/rx buffer 2 address */
    
} __attribute__((aligned(4))) ETH_DMADescTypeDef;


/** 
  * @brief  DMA Transmit Descriptors Wrapper structure definition
  */

typedef struct
{
    ETH_DMADescTypeDef *TxDescTab;      /*<! Tx DMA descriptors addresses                           */
    uint32_t CurTxDesc;                 /*<! Current Tx descriptor index for packet transmission    */
    
} __attribute__((aligned(4))) ETH_TxDescListTypeDef;

/** 
 * @brief  DMA Receive Descriptors Wrapper structure definition
 */

typedef struct
{
    ETH_DMADescTypeDef *RxDescTab;      /*<! Rx DMA descriptors addresses.                      */
    uint32_t CurRxDesc;                 /*<! Current Rx descriptor, ready for next reception.   */
    uint32_t FirstRxDesc;               /*<! First descriptor of last received packet.          */
    uint32_t RxDescNbr;                 /*<! Number of descriptors of last received packet.     */
    
} __attribute__((aligned(4))) ETH_RxDescListTypeDef;

/** 
 * @brief  ETH Timestamp structure definition
 */

typedef struct
{
    uint32_t sec;
    uint32_t nsec;
} __attribute__((aligned(4))) ETH_TimestampTypeDef;

/** 
 * @brief  ETH TX status structure definition
 */

typedef struct
{
    uint32_t Status0;                   /*!< TDES0 contains the transmitted frame status and the descriptor ownership information.          */
    uint32_t TimestampValid;            /*!< Specifies whether the timestamp is valid.                                                      */
    ETH_TimestampTypeDef Timestamp;     /*!< Transmit frame timestamp                                                                       */
} __attribute__((aligned(4))) ETH_TxStatusTypeDef;

/** 
 * @brief  ETH RX status structure definition
 */

typedef struct
{
    uint32_t Status0;                   /*!< TDES0 contains the Received frame status and the descriptor ownership information.             */
    uint32_t Status4;                   /*!< TDES4 contains the Received frame status.                                                      */
    uint32_t TimestampValid;            /*!< Specifies whether the timestamp is valid.                                                      */
    ETH_TimestampTypeDef Timestamp;     /*!< Transmit frame timestamp                                                                       */
} __attribute__((aligned(4))) ETH_RxStatusTypeDef;


/** @defgroup ETH_DLYB_Clock_Source
  * @{
  */ 

#define ETH_DLYB_CLOCK_SOURCE_HCLK      ( 0 )
#define ETH_DLYB_CLOCK_SOURCE_PHY_RX    ( 1 )

/**
  * @}
  */

/** 
 * @brief  ETH rx clock delay structure definition
 */

typedef struct
{
    uint32_t ClockSource;
    uint32_t Uint;
    uint32_t Len;
    
}ETH_DelayTypeDef;

/** 
  * @brief  ETH Init Structure definition  
  */

/** 
 * @brief  ETH TX Config structure definition
 */

typedef struct
{
    uint8_t  MACAddr[6];                        /*!< MAC Address of used Hardware: must be pointer on an array of 6 bytes   */
    uint8_t  Rsv1[2];
    uint32_t MediaInterface;                    /*!< Selects the media interface.
                                                     This parameter can be a value of @ref ETH_Media_Interfaces             */
    uint32_t AutoNegotiation;                   /*!< Selects or not the AutoNegotiation mode for the external PHY
                                                     The AutoNegotiation allows an automatic setting of the Speed 
                                                     (10/100Mbps) and the mode (half/full-duplex).
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t Speed;                             /*!< Sets the Ethernet speed: 10/100 Mbps.
                                                     This parameter can be a value of @ref ETH_Speed                        */
    uint32_t DuplexMode;                        /*!< Selects the MAC duplex mode: Half-Duplex or Full-Duplex mode
                                                     This parameter can be a value of @ref ETH_Duplex_Mode                  */
    uint32_t AutoCRC;                           /*!< Select or not the Automatic MAC CRC Stripping.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t AutoPad;                           /*!< Select or not the Automatic MAC Pad Stripping.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t AutoChecksumInsertion;             /*!< Selects the Checksum Insertion Control.
                                                     This parameter can be a value of @ref ETH_Checksum_Insertion           */
    uint32_t PhyAddress;                        /*!< PHY port address                                                       */
    uint8_t  *TxBuff;                           /*!< Pointer to the tx buffer                                               */
    uint32_t TxBuffNbr;                         /*!< Provides the number of tx buffers                                      */
    uint32_t TxBuffLen;                         /*!< Provides the length of tx buffers size                                 */
    uint8_t  *RxBuff;                           /*!< Pointer to the rx buffer                                               */
    uint32_t RxBuffNbr;                         /*!< Provides the number of rx buffers                                      */
    uint32_t RxBuffLen;                         /*!< Provides the length of rx buffers size                                 */
    ETH_DMADescTypeDef *TxDesc;                 /*!< Provides the address of the first DMA Tx descriptor in the list        */
    ETH_DMADescTypeDef *RxDesc;                 /*!< Provides the address of the first DMA Rx descriptor in the list        */
    uint32_t TxDescNbr;                         /*!< TX Descriptor number                                                   */
    uint32_t RxDescNbr;                         /*!< TX Descriptor number                                                   */
    uint32_t TxDescListMode;                    /*!< TX Descriptor Structure
                                                     This parameter can be a value of @ref ETH_Desc_List_Mode               */
    uint32_t RxDescListMode;                    /*!< RX Descriptor Structure
                                                     This parameter can be a value of @ref ETH_Desc_List_Mode               */
    ETH_BuffTypeDef *TxBuffTab;                 /*!< tx descriptor wrapper: holds all Tx descriptors list addresses         */
    ETH_BuffTypeDef *RxBuffTab;                 /*!< rx descriptor wrapper: holds all Tx descriptors list addresses         */
    uint32_t TxBuffNodeNbr;                     /*!< Provides the number of the tx buffer node                              */
    uint32_t RxBuffNodeNbr;                     /*!< Provides the number of the tx buffer node                              */
    ETH_DelayTypeDef Delay;                     /*!< rx clock delay                                                         */
    
} __attribute__((aligned(4))) ETH_InitTypeDef;

/** 
 * @brief  ETH interrupt structure definition
 */

typedef struct
{
    uint32_t     TxComplete;
    uint32_t     RxComplete;
    uint32_t     Error;
    uint32_t     MMCTx;
    uint32_t     MMCRx;
    uint32_t     LPI;
    uint32_t     PTP;
    uint32_t     PMT;
    
} __attribute__((aligned(4))) ETH_ITTypeDef;

/** 
 * @brief  ETH MAC address filter structure definition
 */

typedef struct
{
    uint32_t Enable;                            /*!< MAC address filter. This parameter can be ENABLE or DISABLE                        */
    uint32_t GroupFilter;                       /*!< Specifies whether the group filter matches.
                                                     This parameter can be ENABLE or DISABLE.
                                                     ENABLE: group filter matches.
                                                     DISABLE: Perfect filter matches.                                                   */
    uint32_t PerfectSourAddr;                   /*!< Specify the source address for perfect filter matching.                            */
    uint32_t GroupMask;                         /*!< Specify the mask control bits for comparison of each of the MAC Address bytes.     */
    uint8_t Addr[6];                            /*!< Specify the MAC address.                                                           */
    uint8_t Rsv1[2];
} __attribute__((aligned(4))) ETH_MACAddrTypeDef;

/** 
 * @brief  ETH MAC address filter structure definition
 */

typedef struct
{
    ETH_MACAddrTypeDef Addr[3];                 /*!< Specify the MAC address filter.                                    */
    
    uint32_t ReceiveAll;                        /*!< Selects or not all frames reception by the MAC (No filtering).
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t HashPerfectFilter;                 /*!< Selects or not the hash perfect filter.
                                                     This parameter can be ENABLE or DISABLE.
                                                     ENABLE: perfect filter or hash filter.
                                                     DISABLE: only hash filter.                                         */
    uint32_t SourceAddrFilter;                  /*!< Selects or not the source address filter.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t SourceAddrInverseFilter;           /*!< Selects or not the source address inverse filter.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t PassControlPackets;                /*!< Sets the forwarding mode of the control frames
                                                     This parameter can be a value of @ref ETH_Pass_Control_Frames      */ 
    uint32_t BroadcastFramesReception;          /*!< Selects or not the reception of Broadcast Frames.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t PassAllMulticast;                  /*!< Selects or not the pass all multicast.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t DestAddrInverseFilter;             /*!< Selects or not the destination address inverse filter.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t HushMulticastFramesFilter;         /*!< Selects or not the hash unicast frames filter.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t HashUnicastFramesFilter;           /*!< Selects or not the hash multicast frames filter.
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t PromiscuousMode;                   /*!< Selects or not the promiscuous mode
                                                     This parameter can be ENABLE or DISABLE.                           */
    uint32_t HashTable[2];                      /*!< This field holds the hash table.
                                                     This parameter must be a number between Min_Data = 0x0 and Max_Data = 0xFFFFFFFFU  */
    
} __attribute__((aligned(4))) ETH_AddrFilterTypeDef;

/** 
 * @brief  ETH MAC VLAN filter structure definition
 */

typedef struct
{
    uint32_t Enable;                            /*!< Select or not enable VLAN Filter.
                                                     This parameter can be ENABLE or DISABLE.                                       */
    uint32_t Hash;                              /*!< Select or not enable SVLAN.
                                                     This parameter can be ENABLE or DISABLE.                                       */
    uint32_t SVLAN;                             /*!< Select or not enable SVLAN.
                                                     This parameter can be ENABLE or DISABLE.                                       */
    uint32_t InverseMatch;                      /*!< Selects or not the VLAN inverse match.
                                                     This parameter can be ENABLE or DISABLE.                                       */
    uint32_t Comparison12Bits;                  /*!< Selects or not the 12-bit VLAN identifier for comparison and filtering.
                                                     This parameter can be ENABLE or DISABLE.
                                                     ENABLE: 12-bit VLAN identifier.
                                                     DISABLE: the complete 16-bit VLAN tag.                                         */
    uint32_t Tag;                               /*!< This field holds the VLAN Tag Identifier for Receive Packets.
                                                     This parameter must be a number between Min_Data = 0x0 and Max_Data = 0xFFFFU  */
    uint32_t HashTable;                         /*!< This field holds the VLAN hash table.
                                                     This parameter must be a number between Min_Data = 0x0 and Max_Data = 0xFFFFU  */
} __attribute__((aligned(4))) ETH_VLANFilterTypeDef;
            
/** 
 * @brief  ETH MAC Layer 3 filter structure definition
 */

typedef struct
{
    uint32_t IPV6;                              /*!< Select or not enable the layer 3 IP source address or destination
                                                     address filter in the IPV6 protocol.
                                                     This parameter can be ENABLE or DISABLE.
                                                     ENABLE: in IPV6 protocol.
                                                     DISABLE: in IPV4 protocol.                                             */
    union
    {
        struct
        {
            uint32_t SourAddrMatchEnable;       /*!< Select or not enable layer 3 IPV4 source address match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t SourAddrInverseMatch;      /*!< Select or not enable layer 3 IPV4 source address inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t SourAddrMatch;             /*!< This field holds the Layer 3 IPV4 source address match.
                                                     This parameter can be 0 to 63.                                         */
            uint32_t DestAddrMatchEnable;       /*!< Select or not enable layer 3 IPV4 destination address match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t DestAddrInverseMatch;      /*!< Select or not enable layer 3 IPV4 destination address inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t DestAddrMatch;             /*!< This field holds the Layer 3 IPV4 destination address match.
                                                     This parameter can be 0 to 63.                                         */
            uint32_t SourAddr;                  /*!< This field holds the Layer 3 IPV4 source address.                      */
            uint32_t DestAddr;                  /*!< This field holds the Layer 3 IPV4 destination address.                 */
        }IPV4;
        
        struct
        {
            uint32_t SourAddrMatchEnable;       /*!< Select or not enable layer 3 IPV6 source address match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t SourAddrInverseMatch;      /*!< Select or not enable layer 3 IPV6 source address inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t DestAddrMatchEnable;       /*!< Select or not enable layer 3 IPV6 destination address match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t DestAddrInverseMatch;      /*!< Select or not enable layer 3 IPV6 destination address inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
            uint32_t AddrMatch;                 /*!< This field holds the Layer 3 IPV6 destination address match.
                                                     This parameter can be 0 to 127.                                        */
            uint32_t Addr[4];                   /*!< This field holds the Layer 3 IPV6 address.                             */
            
        }IPV6;
    }IP;
    
} __attribute__((aligned(4))) ETH_L3FilterTypeDef;

/** 
 * @brief  ETH MAC Layer 4 filter structure definition
 */

typedef struct
{
    uint32_t UDP;                               /*!< Select or not enable the layer 4 source port number or destination
                                                     port number filter in UDP protocol.
                                                     This parameter can be ENABLE or DISABLE.
                                                     ENABLE: in UDP protocol..
                                                     DISABLE: in TCP protocol.                                              */
    uint32_t SourPortMatchEnable;               /*!< Select or not enable layer 4 source port number match.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t SourPortInverseMatch;              /*!< Select or not enable layer 4 source port number inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t DestPortMatchEnable;               /*!< Select or not enable layer 4 destination port number match.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t DestPortInverseMatch;              /*!< Select or not enable layer 4 destination port number inverse match.
                                                     This parameter can be ENABLE or DISABLE.                               */
    uint32_t SourPort;                          /*!< This field holds the Layer 4 source port number.                       */
    uint32_t DestPort;                          /*!< This field holds the Layer 4 destination port number.                  */
    
} __attribute__((aligned(4))) ETH_L4FilterTypeDef;

/** 
 * @brief  ETH MAC Layer 3 and Layer 4 filter structure definition
 */

typedef struct
{
    uint32_t Enable;                            /*!< Select or not enable L3L4 Filter.
                                                     This parameter can be ENABLE or DISABLE.                       */
    ETH_L3FilterTypeDef L3[2];                  /*!< This field holds the Layer 3 filter configuration values.      */
    ETH_L4FilterTypeDef L4[2];                  /*!< This field holds the Layer 4 filter configuration values.      */
} __attribute__((aligned(4))) ETH_L3L4FilterTypeDef;

/** 
 * @brief  ETH MAC filter structure definition
 */

typedef struct
{
    ETH_AddrFilterTypeDef Addr;                 /*!< This field holds the address filter configuration values.      */
    ETH_VLANFilterTypeDef VLAN;                 /*!< This field holds the VLAN filter configuration values.         */
    ETH_L3L4FilterTypeDef L3L4;                 /*!< This field holds the L3L4 filter configuration values.         */  
    uint32_t DropNonTCPUDP;                     /*!< Selects or not the Dropping of not TCP/UDP over IP packets.
                                                     This parameter can be ENABLE or DISABLE.                       */
} __attribute__((aligned(4))) ETH_FilterTypeDef;


/** 
 * @brief  ETH VLAN Configuration Structure definition  
 */

typedef struct
{                               
    uint32_t Enable;                            /*!< Select or not use delete, insert or replace VLAN.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t Mode;                              /*!< Specifies how the VLAN Tag Control in Transmit Packets.
                                                     This parameter can be a value of @ref ETH_VLAN_Mode            */
    uint32_t SVLAN;                             /*!< Specifies or not insert or replace the S-VLAN type.
                                                     When enabled, insert or replace the S-VLAN type.
                                                     When disabled, insert or replace the C-VLAN type.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t Tag;                               /*!< This field holds the VLAN Tag for Transmit Packets.            */
} __attribute__((aligned(4))) ETH_VLANTypeDef;

 /** 
  * @brief  ETH MAC Configuration Structure definition  
  */

/** 
 * @brief  ETH TX Config structure definition
 */

typedef struct
{
    uint32_t SourceAddrControl;                 /*!< Specifies how the source address insertion or replacement 
                                                     control.
                                                     This parameter can be a value of @ref ETH_Source_Addr_Mode     */
    uint32_t Support2KPacket;                   /*!< Select or not IEEE 802.3as Support for 2K Packets.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t CRCStripTypePacket;                /*!< Select or not CRC stripping for type frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t Watchdog;                          /*!< Select or not enable Watch dog.
                                                     When enabled, the MAC allows the received frame not to exceed 
                                                     2048 bytes.
                                                     When disabled, the MAC allows the received frame not to exceed 
                                                     16384 bytes.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t WatchdogTimeout;                   /*!< This field holds the Watch dog timeout.
                                                     This parameter can be 1523 to 16383.                          */
    uint32_t Jabber;                            /*!< Selects or not Jabber timer.
                                                     When enabled, the MAC allows no more then 2048 bytes to be sent.
                                                     When disabled, the MAC can send up to 16384 bytes.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t JumboPacket;                       /*!< Selects or not Jumbo Packet Enable.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t InterFrameGap;                     /*!< Selects the minimum IFG between frames during transmission.
                                                     This parameter can be a value of @ref RCC_Interframe_Gap */
    uint32_t CarrierSense;                      /*!< Selects or not the Carrier Sense.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ReceiveOwn;                        /*!< Selects or not the ReceiveOwn,
                                                     ReceiveOwn allows the reception of frames when the TX_EN signal 
                                                     is asserted in Half-Duplex mode.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t LoopbackMode;                      /*!< Selects or not the internal MAC MII Loopback mode.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ChecksumOffload;                   /*!< Selects or not the IPv4 checksum checking for received frame 
                                                     payloads' TCP/UDP/ICMP headers.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t RetryTransmission;                 /*!< Selects or not the MAC attempt retries transmission, based on 
                                                     the settings of BL, when a collision occurs (Half-Duplex mode).
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t AutoPadCRCStrip;                   /*!< Selects or not the Automatic MAC Pad/CRC Stripping.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t BackOffLimit;                      /*!< Selects the BackOff limit value.
                                                     This parameter can be a value of @ref ETH_Back_Off_Limit       */
    uint32_t DeferralCheck;                     /*!< Selects or not the deferral check function (Half-Duplex mode).
                                                     This parameter can be ENABLE or DISABLE.                       */ 	
    uint32_t PreambleLen;                       /*!< Selects the preamble length for transmit packets.
                                                     This parameter can be a value of @ref ETH_Preamble_Length      */
    ETH_VLANTypeDef VLAN;                       /*!< Specify VLAN configuration parameters                          */
    
} __attribute__((aligned(4))) ETH_MACInitTypeDef;

/** 
  * @brief  ETH DMA Configuration Structure definition  
  */

typedef struct
{
    uint32_t DropTCPIPChecksumErrorFrame;       /*!< Selects or not the dropping of TCP/IP checksum error frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ReceiveStoreForward;               /*!< Selects or not the receive store and forward.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t FlushReceivedFrame;                /*!< Selects or not the flushing of received frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t TransmitStoreForward;              /*!< Selects or not the transmit store and forwards.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t TransmitThresholdControl;          /*!< This field holds the transmit threshold control.
                                                     This parameter can be a value of @ref ETH_Transmit_Threshold_Control.  */
    uint32_t ForwardErrorFrames;                /*!< Selects or not the forward error frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ForwardUndersizedGoodFrames;       /*!< Selects or not the forward undersized good frame.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ReceiveThresholdControl;           /*!< This field holds the receive threshold control.
                                                     This parameter can be a value of @ref ETH_Recvived_Threshold_Control.  */
    uint32_t SecondFrameOperate;                /*!< Selects or not the operate on second frame.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t MixedBurst;                        /*!< Selects or not the mixed burst.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t AddressAlignedBeats;               /*!< Selects or not the Address Aligned Beats.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t PBL8xMode;                         /*!< Selects or not the 8xPBL mode.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t UseSeparatePBL;                    /*!< Selects or not the use separate PBL.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t RxDMABurstLen;                     /*!< Indicates the maximum number of beats to be transferred in 
                                                     one Rx DMA transaction.
                                                     This parameter can be a value of @ref ETH_RX_DMA_Burst_Length  */
    uint32_t FixedBurst;                        /*!< Selects or not the fixed burst.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t RxTxPriorityRatio;                 /*!< Indicates the rx tx priority ratio.
                                                     This parameter can be a value of @ref ETH_RX_TX_Priority_ratio */
    uint32_t TxDMABurstLen;                     /*!< Indicates the maximum number of beats to be transferred in one 
                                                     Tx DMA transaction.
                                                     This parameter can be a value of @ref ETH_RX_DMA_Burst_Length  */
    uint32_t EnhancedDescriptorFormat;          /*!< Selects or not the enhanced descriptor format.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t DescriptorSkipLen;                 /*!< Specifies the number of word to skip between two unchained 
                                                     descriptors (Ring mode)
                                                     This parameter can be 0 to 31                                  */
    uint32_t DMAArbitration;                    /*!< Indicates the DMA arbitration.
                                                     This parameter can be a value of @ref ETH_DMA_Arbitration      */
} __attribute__((aligned(4))) ETH_DMAInitTypeDef;


/** 
  * @brief  ETH MMC Init Structure definition
  */

typedef struct
{
    uint32_t UpdateForDroppedBroadcast;         /*!< Selects or not the update MMC counters for dropped broadcast 
                                                     packets.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t FullHalfPreset;                    /*!< Selects or not the full-half preset.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t Preset;                            /*!< Selects or not the counters preset.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t ResetOnRead;                       /*!< Selects or not reset on read.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t StopRollover;                      /*!< Selects or not counters reset.
                                                     This parameter can be ENABLE or DISABLE.                       */
} __attribute__((aligned(4))) ETH_MMCInitTypeDef;

/** 
  * @brief  ETH MMC Counter Structure definition
  */

typedef struct
{
    uint32_t TxGoodFrameSingleCollision;        /*!< Indicates the number of good frames sent after a single collision.     */
    uint32_t TxGoodFrameMultipleCollision;      /*!< Indicates the number of good frames sent after multiple  collisions.   */
    uint32_t TxGoodFrame;                       /*!< Indicates the number of good frames sent.                              */
    uint32_t RxCRCErr;                          /*!< Indicates the number of frames received with CRC errors.               */
    uint32_t RxAlignmentErr;                    /*!< Indicates the number of frames received with alignment errors.         */
    uint32_t RxGoodUnicast;                     /*!< Indicates the number of good unicast frames received.                  */
} __attribute__((aligned(4))) ETH_MMCCounterTypeDef;

/** 
 * @brief  ETH PMT filter structure definition
 */

typedef struct
{
    uint32_t Mask;                              /*!< This field holds the filter x byte mask.                               */
    uint32_t Cmd;                               /*!< This field holds the filter x command.                                 */
    uint32_t Offset;                            /*!< This field holds the offset of the frame to be detected by filter x.   */
    uint32_t CRC16;                             /*!< This field holds the the CRC_16 value calculated according to 
                                                     the pattern and the byte mask.                                         */
} __attribute__((aligned(4))) ETH_PMTFilterTypeDef;

/** 
 * @brief  ETH PMT Init Structure definition
 */

typedef struct
{
    ETH_PMTFilterTypeDef Filter[4];             /*!< Indicates the PMT filter structure.                                    */
    uint32_t GlobalUnicast;                     /*!< Selects or not global unicast.
                                                     This parameter can be ENABLE or DISABLE.                               */
} __attribute__((aligned(4))) ETH_PMTInitTypeDef;

/** 
 * @brief  ETH PPS Init Structure definition
 */

typedef struct
{
    uint32_t TargetTime;                        /*!< Indicates the Target Time Register Mode for PPS Output.
                                                     This parameter can be a value of @ref ETH_PPS_Target_Time      */
    uint32_t OutputMode;                        /*!< Indicates the flexible PPS output mode.
                                                     This parameter can be a value of @ref ETH_PPS_Output_Mode      */
    uint32_t Output;                            /*!< Indicates the PPS output.
                                                     When ETH_PPS_OUTPUT_MODE_PULSE, This parameter can be a 
                                                     value of @ref ETH_PPS_Output_PULSE.
                                                     When ETH_PPS_OUTPUT_MODE_CMD, This parameter can be a 
                                                     value of @ref ETH_PPS_Output_CMD.                              */
    uint32_t SignalInterval;                    /*!< Indicates the PPS output signal interval.                      */
    uint32_t SignalWidth;                       /*!< Indicates the PPS output signal width.                         */
} __attribute__((aligned(4))) ETH_PPSInitTypeDef;

/** 
 * @brief  ETH PPS Init Structure definition
 */

typedef struct
{
    uint32_t AuxiliarySnapshot;                 /*!< Selects or not enable Auxiliary Snapshot.
                                                     bit0: enable Auxiliary Snapshot 0.
                                                     bit1: enable Auxiliary Snapshot 1.
                                                     bit2: enable Auxiliary Snapshot 2.
                                                     bit3: enable Auxiliary Snapshot 3.                             */
    uint32_t Clock;                             /*!< Indicates the PTP packets for taking snapshot.
                                                     This parameter can be a value of @ref ETH_PTP_Clock            */
    uint32_t AllFrames;                         /*!< Selects or not timestamp snapshot for all received frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t EthernetFrame;                     /*!< Selects or not processing of PTP over ethernet frames.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t MACAddrFilter;                     /*!< Selects or not MAC address for PTP frame filtering.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t MasterMessage;                     /*!< Selects or not snapshot for message relevant to master.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t EventMessage;                      /*!< Selects or not timestamp snapshot for event messages.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t IPV4Message;                       /*!< Selects or not processing of PTP frames sent over IPv4-UDP.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t IPV6Message;                       /*!< Selects or not processing of PTP frames sent over IPv6-UDP.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t Ver2;                              /*!< Selects or not timestamp PTP packet snooping for version2 format.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t DigitalRollover;                   /*!< Selects or not timestamp subsecond rollover: digital or binary rollover control.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t FineUpdate;                        /*!< Selects or not Timestamp fin update. 
                                                     When ENABLE, Timestamp fin update.
                                                     When DISABLE, Timestamp coarse update.
                                                     This parameter can be ENABLE or DISABLE.                       */
    
    ETH_PPSInitTypeDef      PPS;                /*!< Indicates the PPS structure.                                   */
} __attribute__((aligned(4))) ETH_PTPInitTypeDef;

/** 
 * @brief  ETH PTP Timestamp Status Structure definition
 */

typedef struct
{
    uint32_t SnapshotsNbr;                      /*!< Indicates the number of auxiliary timestamp snapshots.
                                                     This parameter can be 0 and 4.                                 */
    uint32_t TriggerMissed;                     /*!< Indicates the auxiliary timestamp snapshot trigger missed.     */
    uint32_t TriggerFlag;                       /*!< Indicates the auxiliary timestamp snapshot trigger identifier.
                                                     bit0: auxiliary trigger 0.
                                                     bit1: auxiliary trigger 0.
                                                     bit2: auxiliary trigger 0.
                                                     bit3: auxiliary trigger 0.                                     */
} __attribute__((aligned(4))) ETH_PTPStatusTypeDef;

/** 
 * @brief  ETH LPI Structure definition
 */

typedef struct
{
    uint32_t TxAuto;                            /*!< Selects or not LPI Tx Automate.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t TxClockDisable;                    /*!< Selects or not disable tx clock.
                                                     This parameter can be ENABLE or DISABLE.                       */
    uint32_t EntryTime;                         /*!< Specifies the minimum time (in milliseconds) that the link 
                                                     state from the PHY should last before the LPI mode can be 
                                                     sent to the PHY side..
                                                     This parameter can be 0 to 1023.                               */
    uint32_t ExtiTime;                          /*!< Specifies the minimum time (in microseconds) after the MAC 
                                                     stops sending LPI mode to the PHY to resume normal transmission.
                                                     This parameter can be 0 to 65535.                              */
} __attribute__((aligned(4))) ETH_LPITypeDef;

/** 
  * @brief  ETH Handle Structure definition  
  */
  
typedef struct
{
    ETH_TypeDef             *Instance;          /*!< Register base address       */
    ETH_InitTypeDef         Init;               /*!< Ethernet Init Configuration */
    ETH_MACInitTypeDef      MACConfig;          /*!< MAC Configuration */
    ETH_DMAInitTypeDef      DMAConfig;          /*!< DMA Configuration */
    ETH_FilterTypeDef       FilterConfig;       /*!< Filter Configuration */
    ETH_MMCInitTypeDef      MMCConfig;          /*!< MMC Configuration */
    ETH_MMCCounterTypeDef   MMCCounter;         /*!< MMC Counter */
    ETH_LPITypeDef          LPIConfig;          /*!< LPI Configuration */
    ETH_PMTInitTypeDef      PMTConfig;          /*!< PMT Configuration */
    ETH_PTPInitTypeDef      PTPConfig;          /*!< PTP Configuration */
    ETH_ITTypeDef           IT;                 /*!< IT Configuration */
    ETH_TxDescListTypeDef   TxDescList;         /*!< TX descriptors list */
    ETH_RxDescListTypeDef   RxDescList;         /*!< RX descriptors list */
} __attribute__((aligned(4))) ETH_HandleTypeDef;


/** @brief ETH PTP timeout
  */
  
#define ETH_TIMEOUT_SWRESET                                 ( 500U )
#define ETH_TIMEOUT_LINKED_STATE                            ( 5000U )
#define ETH_TIMEOUT_AUTONEGO_COMPLETED                      ( 5000U )
#define ETH_TIMEOUT_PHY_READ                                ( 0xFFFFU )
#define ETH_TIMEOUT_PHY_WRITE                               ( 0xFFFFU )
#define ETH_PTP_TIMEOUT                                     ( 0xFFFFU )
#define ETH_FLOWCTRL_TIMEOUT                                ( 0xFFFFU )

/** 
  * @brief ETH_Buffs_setting ETH Buffs setting
  */
  
#define ETH_MAX_PACKET_SIZE                                 ( 1528U )       /*!< ETH_HEADER + ETH_EXTRA + ETH_VLAN_TAG * 2 + ETH_MAX_ETH_PAYLOAD + ETH_CRC  */
#define ETH_HEADER                                          ( 14U )         /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type                      */
#define ETH_CRC                                             ( 4U )          /*!< Ethernet CRC                                                               */
#define ETH_EXTRA                                           ( 2U )          /*!< Extra bytes in some cases                                                  */   
#define ETH_VLAN_TAG                                        ( 4U )          /*!< optional 802.1q VLAN Tag                                                   */
#define ETH_MIN_ETH_PAYLOAD                                 ( 46U )         /*!< Minimum Ethernet payload size                                              */
#define ETH_MAX_ETH_PAYLOAD                                 ( 1500U )       /*!< Maximum Ethernet payload size                                              */
#define ETH_JUMBO_FRAME_PAYLOAD                             ( 9000U )       /*!< Jumbo frame payload size                                                   */      


/** @brief ETH DMA TX Descriptors
  */
  
#define ETH_DMA_TX_DESC0_OWN                                ( BIT31 )
#define ETH_DMA_TX_DESC0_IC                                 ( BIT30 )
#define ETH_DMA_TX_DESC0_LS                                 ( BIT29 )
#define ETH_DMA_TX_DESC0_FS                                 ( BIT28 )
#define ETH_DMA_TX_DESC0_DC                                 ( BIT27 )
#define ETH_DMA_TX_DESC0_DP                                 ( BIT26 )
#define ETH_DMA_TX_DESC0_TTSE                               ( BIT25 )
#define ETH_DMA_TX_DESC0_CIC_Pos                            ( 22U )
#define ETH_DMA_TX_DESC0_CIC_Msk                            ( 0x3UL << ETH_DMA_TX_DESC0_CIC_Pos )
#define ETH_DMA_TX_DESC0_CIC                                ( ETH_DMA_TX_DESC0_CIC_Msk )
#define ETH_DMA_TX_DESC0_CIC_0                              ( 0x1UL << ETH_DMA_TX_DESC0_CIC_Pos )
#define ETH_DMA_TX_DESC0_CIC_1                              ( 0x2UL << ETH_DMA_TX_DESC0_CIC_Pos )
#define ETH_DMA_TX_DESC0_TER                                ( BIT21 )
#define ETH_DMA_TX_DESC0_TCH                                ( BIT20 )
#define ETH_DMA_TX_DESC0_TTSS                               ( BIT17 )
#define ETH_DMA_TX_DESC0_IHE                                ( BIT16 )
#define ETH_DMA_TX_DESC0_ES                                 ( BIT15 )
#define ETH_DMA_TX_DESC0_JT                                 ( BIT14 )
#define ETH_DMA_TX_DESC0_FF                                 ( BIT13 )
#define ETH_DMA_TX_DESC0_IPE                                ( BIT12 )
#define ETH_DMA_TX_DESC0_LCA                                ( BIT11 )
#define ETH_DMA_TX_DESC0_NC                                 ( BIT10 )
#define ETH_DMA_TX_DESC0_LCO                                ( BIT9 )
#define ETH_DMA_TX_DESC0_EC                                 ( BIT8 )
#define ETH_DMA_TX_DESC0_VF                                 ( BIT7 )
#define ETH_DMA_TX_DESC0_CC                                 ( BIT6 | BIT5 | BIT4 | BIT3 )
#define ETH_DMA_TX_DESC0_ED                                 ( BIT2 )
#define ETH_DMA_TX_DESC0_UF                                 ( BIT1 )
#define ETH_DMA_TX_DESC0_DB                                 ( BIT0 )


#define ETH_DMA_TX_DESC1_TBS2_Pos                           ( 16U )
#define ETH_DMA_TX_DESC1_TBS2                               ( 0x1fff0000U )
#define ETH_DMA_TX_DESC1_TBS1_Pos                           ( 0U )
#define ETH_DMA_TX_DESC1_TBS1                               ( 0x00001fffU )

/** @brief ETH DMA TX Descriptors
  */
  
#define ETH_DMA_RX_DESC0_OWN                                ( BIT31 )
#define ETH_DMA_RX_DESC0_AFM                                ( BIT30 )
#define ETH_DMA_RX_DESC0_FL_Pos                             ( 16U )
#define ETH_DMA_RX_DESC0_FL                                 ( 0x3FFF0000U )
#define ETH_DMA_RX_DESC0_ES                                 ( BIT15 )
#define ETH_DMA_RX_DESC0_DE                                 ( BIT14 )
#define ETH_DMA_RX_DESC0_SAF                                ( BIT13 )
#define ETH_DMA_RX_DESC0_LE                                 ( BIT12 )
#define ETH_DMA_RX_DESC0_OE                                 ( BIT11 )
#define ETH_DMA_RX_DESC0_VLAN                               ( BIT10 )
#define ETH_DMA_RX_DESC0_FS                                 ( BIT9 )
#define ETH_DMA_RX_DESC0_LS                                 ( BIT8 )
#define ETH_DMA_RX_DESC0_IPHCE                              ( BIT7 )
#define ETH_DMA_RX_DESC0_TSV                                ( BIT7 )
#define ETH_DMA_RX_DESC0_LCO                                ( BIT6 )
#define ETH_DMA_RX_DESC0_FT                                 ( BIT5 )
#define ETH_DMA_RX_DESC0_RWT                                ( BIT4 )
#define ETH_DMA_RX_DESC0_RE                                 ( BIT3 )
#define ETH_DMA_RX_DESC0_DBE                                ( BIT2 )
#define ETH_DMA_RX_DESC0_CE                                 ( BIT1 )
#define ETH_DMA_RX_DESC0_PCE_ESA                            ( BIT0 )

#define ETH_DMA_RX_DESC0_ERRORS_MASK                        ( BIT14 | BIT11 | BIT7 | BIT6 | BIT4 | BIT3 | BIT1 | BIT0 )

#define ETH_DMA_RX_DESC1_DIC                                ( BIT31 )
#define ETH_DMA_RX_DESC1_RBS2_Pos                           ( 16U )
#define ETH_DMA_RX_DESC1_RBS2                               ( 0x1FFF0000U )
#define ETH_DMA_RX_DESC1_RER                                ( BIT15 )
#define ETH_DMA_RX_DESC1_RCH                                ( BIT14 )
#define ETH_DMA_RX_DESC1_RBS1_Pos                           ( 0U )
#define ETH_DMA_RX_DESC1_RBS1                               ( 0x00001FFFU )

/** @defgroup ETH_Media_Interfaces
  * @{
  */
  
#define ETH_MEDIA_INTERFACE_MII                             ( 0U )
#define ETH_MEDIA_INTERFACE_RMII                            ( 1U )

/**
  * @}
  */
  
/** @defgroup ETH_Speed
  * @{
  */
  
#define ETH_SPEED_10M                                       ( 0U )
#define ETH_SPEED_100M                                      ( 1U )

/**
  * @}
  */
  
/** @defgroup ETH_Duplex_Mode
  * @{
  */
  
#define ETH_MODE_FULL_DUPLEX                                ( 0U )
#define ETH_MODE_HALF_DUPLEX                                ( 1U )

/**
  * @}
  */
  
/** @defgroup ETH_Checksum_Insertion
  * @{
  */
  
#define ETH_CHECKSUM_INSERTION_CTRL_DISABLE                 ( 0U )
#define ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD                 ( 1U )
#define ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD_PAYLOAD         ( 2U )
#define ETH_CHECKSUM_INSERTION_CTRL_FULL                    ( 3U )

/**
  * @}
  */
  
/** @defgroup ETH_Desc_List_Mode
  * @{
  */
  
#define ETH_DESC_LIST_MODE_RING                             ( 0 )
#define ETH_DESC_LIST_MODE_LIST                             ( 1 )

/**
  * @}
  */

/** @defgroup ETH_TX_Mode
  * @{
  */ 

#define ETH_TX_MODE_DATA_COPY                               ( BIT0 )    /*!< data copy, or address import   */
#define ETH_TX_MODE_TIMESTAMP                               ( BIT1 )    /*!< timestamp                      */
#define ETH_TX_MODE_WAIT_TX_COMPLETE                        ( BIT2 )    /*!< wait tx complete               */

/**
  * @}
  */

/** @defgroup ETH_Source_Addr_Mode
  * @{
  */

#define ETH_SOURCE_ADDRESS_DISABLE                          ( 0U )
#define ETH_SOURCE_ADDRESS_INSERT_ADDR0                     ( 1U )
#define ETH_SOURCE_ADDRESS_INSERT_ADDR1                     ( 2U )
#define ETH_SOURCE_ADDRESS_REPLACE_ADDR0                    ( 3U )
#define ETH_SOURCE_ADDRESS_REPLACE_ADDR1                    ( 4U )

/**
  * @}
  */
  
/** @defgroup RCC_Interframe_Gap
  * @{
  */

#define ETH_INTERFRAME_GAP_96BIT                            ( 0U )
#define ETH_INTERFRAME_GAP_88BIT                            ( 1U )
#define ETH_INTERFRAME_GAP_80BIT                            ( 2U )
#define ETH_INTERFRAME_GAP_72BIT                            ( 3U )
#define ETH_INTERFRAME_GAP_64BIT                            ( 4U )
#define ETH_INTERFRAME_GAP_56BIT                            ( 5U )
#define ETH_INTERFRAME_GAP_48BIT                            ( 6U )
#define ETH_INTERFRAME_GAP_40BIT                            ( 7U )

/**
  * @}
  */

/** @defgroup ETH_Back_Off_Limit
  * @{
  */
  
#define ETH_BACK_OFF_LIMIT_10                               ( 0U )
#define ETH_BACK_OFF_LIMIT_8                                ( ETH_MACCR_BL_0 )
#define ETH_BACK_OFF_LIMIT_4                                ( ETH_MACCR_BL_1 )
#define ETH_BACK_OFF_LIMIT_1                                ( ETH_MACCR_BL_1 | ETH_MACCR_BL_0 )

/** @defgroup ETH_Preamble_Length
  * @{
  */
  
#define ETH_PREAMBLE_LEN_7B                                 ( 0U )
#define ETH_PREAMBLE_LEN_5B                                 ( ETH_MACCR_PRELEN_0 )
#define ETH_PREAMBLE_LEN_3B                                 ( ETH_MACCR_PRELEN_1 )

/**
  * @}
  */
  
/** @defgroup ETH_Pass_Control_Frames ETH Pass Control Frames
  * @{
  */ 
  
#define ETH_PASS_CONTROL_BLOCK_ALL                          ( 0U )
#define ETH_PASS_CONTROL_FORWARD_ALL_EXCEPT_PAUSE_FRAME     ( ETH_MACFFR_PCF_0 )
#define ETH_PASS_CONTROL_FORWARD_ALL                        ( ETH_MACFFR_PCF_1 )
#define ETH_PASS_CONTROL_FORWARD_PASSED_ADDR_FILTER         ( ETH_MACFFR_PCF_1 | ETH_MACFFR_PCF_0 )

/**
  * @}
  */
  
#define ETH_PAUSE_LOW_THRESHOLD_MINUS_4                     ( 0U )
#define ETH_PAUSE_LOW_THRESHOLD_MINUS_28                    ( ETH_MACFCR_PLT_0 )
#define ETH_PAUSE_LOW_THRESHOLD_MINUS_144                   ( ETH_MACFCR_PLT_1 )
#define ETH_PAUSE_LOW_THRESHOLD_MINUS_256                   ( ETH_MACFCR_PLT_1 | ETH_MACFCR_PLT_0 )


/** @defgroup ETH_VLAN_Mode
  * @{
  */ 
  
#define ETH_VLAN_MODE_NONE                                  ( 0U )
#define ETH_VLAN_MODE_DELETE                                ( 1U )
#define ETH_VLAN_MODE_INSERT                                ( 2U )
#define ETH_VLAN_MODE_REPLACE                               ( 3U )

/**
  * @}
  */
  
/** @defgroup ETH_Transmit_Threshold_Control
  * @{
  */ 
  
#define ETH_TRANSMIT_THRESHOLD_CONTROL_64BYTES              ( 0U )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_128BYTES             ( ETH_DMAOMR_TTC_0 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_192BYTES             ( ETH_DMAOMR_TTC_1 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_256BYTES             ( ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_40BYTES              ( ETH_DMAOMR_TTC_2 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_32BYTES              ( ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_0 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_24BYTES              ( ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1 )
#define ETH_TRANSMIT_THRESHOLD_CONTROL_16BYTES              ( ETH_DMAOMR_TTC_2 | ETH_DMAOMR_TTC_1 | ETH_DMAOMR_TTC_0 )

/**
  * @}
  */
  
/** @defgroup ETH_Recvived_Threshold_Control
  * @{
  */ 
  
#define ETH_RECEIVED_THRESHOLD_CONTROL_64BYTES              ( 0U )
#define ETH_RECEIVED_THRESHOLD_CONTROL_32BYTES              ( ETH_DMAOMR_RTC_0 )
#define ETH_RECEIVED_THRESHOLD_CONTROL_96BYTES              ( ETH_DMAOMR_RTC_1 )
#define ETH_RECEIVED_THRESHOLD_CONTROL_128BYTES             ( ETH_DMAOMR_RTC_1 | ETH_DMAOMR_RTC_0 )

/**
  * @}
  */
  
/** @defgroup ETH_RX_DMA_Burst_Length
  * @{
  */ 
  
#define ETH_RX_DMA_BURST_LEN_1BEAT                          ( ETH_DMABMR_RDP_0 )
#define ETH_RX_DMA_BURST_LEN_2BEAT                          ( ETH_DMABMR_RDP_1 )
#define ETH_RX_DMA_BURST_LEN_4BEAT                          ( ETH_DMABMR_RDP_2 )
#define ETH_RX_DMA_BURST_LEN_8BEAT                          ( ETH_DMABMR_RDP_3 )
#define ETH_RX_DMA_BURST_LEN_16BEAT                         ( ETH_DMABMR_RDP_4 )
#define ETH_RX_DMA_BURST_LEN_32BEAT                         ( ETH_DMABMR_RDP_5 )

/**
  * @}
  */
  
/** @defgroup ETH_RX_TX_Priority_ratio
  * @{
  */ 
  
#define ETH_RX_TX_PRIORITY_RATIO_1_1                        ( 0U )
#define ETH_RX_TX_PRIORITY_RATIO_2_1                        ( ETH_DMABMR_PM_0 )
#define ETH_RX_TX_PRIORITY_RATIO_3_1                        ( ETH_DMABMR_PM_1 )
#define ETH_RX_TX_PRIORITY_RATIO_4_1                        ( ETH_DMABMR_PM_1 | ETH_DMABMR_PM_0 )

/**
  * @}
  */
  
/** @defgroup ETH_TX_DMA_Burst_Length
  * @{
  */ 
  
#define ETH_TX_DMA_BURST_LEN_1BEAT                          ( ETH_DMABMR_PBL_0 )
#define ETH_TX_DMA_BURST_LEN_2BEAT                          ( ETH_DMABMR_PBL_1 )
#define ETH_TX_DMA_BURST_LEN_4BEAT                          ( ETH_DMABMR_PBL_2 )
#define ETH_TX_DMA_BURST_LEN_8BEAT                          ( ETH_DMABMR_PBL_3 )
#define ETH_TX_DMA_BURST_LEN_16BEAT                         ( ETH_DMABMR_PBL_4 )
#define ETH_TX_DMA_BURST_LEN_32BEAT                         ( ETH_DMABMR_PBL_5 )

/**
  * @}
  */
  
/** @defgroup ETH_DMA_Arbitration
  * @{
  */ 
  
#define ETH_DMA_ARBITRATION_ROUNDROBIN                      ( 0U )
#define ETH_DMA_ARBITRATION_RX_PRIOR_TX                     ( ETH_DMABMR_DA )

/**
  * @}
  */

/** @defgroup ETH_PTP_Clock
  * @{
  */ 
  
#define ETH_PTP_CLOCK_BC                                    ( 0 )
#define ETH_PTP_CLOCK_OC                                    ( 1 )
#define ETH_PTP_CLOCK_E2E                                   ( 2 )
#define ETH_PTP_CLOCK_P2P                                   ( 3 )

/**
  * @}
  */
  
/** @defgroup ETH_PPS_Target_Time
  * @{
  */ 
  
#define ETH_PPS_TARGET_TIME_IT                              ( 0 )
#define ETH_PPS_TARGET_TIME_IT_START_STOP                   ( 2 )
#define ETH_PPS_TARGET_TIME_START_STOP                      ( 3 )

/**
  * @}
  */
  
/** @defgroup ETH_PPS_Output_Mode
  * @{
  */ 
  
#define ETH_PPS_OUTPUT_MODE_PULSE                           ( 0 )
#define ETH_PPS_OUTPUT_MODE_CMD                             ( 1 )

/**
  * @}
  */

/** @defgroup ETH_PPS_Output_PULSE
  * @{
  */ 
  
#define ETH_PPS_OUTPUT_PULSE_BINARY_2Hz                     ( 1 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_4Hz                     ( 2 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_8Hz                     ( 3 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_16Hz                    ( 4 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_32Hz                    ( 5 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_64Hz                    ( 6 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_128Hz                   ( 7 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_256Hz                   ( 8 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_512Hz                   ( 9 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_1024Hz                  ( 10 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_2048Hz                  ( 11 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_4096Hz                  ( 12 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_8192KHz                 ( 13 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_16384Hz                 ( 14 )
#define ETH_PPS_OUTPUT_PULSE_BINARY_32768Hz                 ( 15 )

#define ETH_PPS_OUTPUT_PULSE_DIGIT_1Hz                      ( 1 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_2Hz                      ( 2 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_4Hz                      ( 3 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_8Hz                      ( 4 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_16Hz                     ( 5 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_32Hz                     ( 6 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_64Hz                     ( 7 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_128Hz                    ( 8 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_256Hz                    ( 9 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_512Hz                    ( 10 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_1024Hz                   ( 11 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_2048Hz                   ( 12 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_4096Hz                   ( 13 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_8192KHz                  ( 14 )
#define ETH_PPS_OUTPUT_PULSE_DIGIT_16384Hz                  ( 15 )

/**
  * @}
  */

/** @defgroup ETH_PPS_Output_Cmd
  * @{
  */
  
#define ETH_PPS_OUTPUT_CMD_NONE                             ( 0 )
#define ETH_PPS_OUTPUT_CMD_START_SINGLE_PULSE               ( 1 )
#define ETH_PPS_OUTPUT_CMD_START_PULSE_TRAIN                ( 2 )
#define ETH_PPS_OUTPUT_CMD_CANCEL_START                     ( 3 )
#define ETH_PPS_OUTPUT_CMD_STOP_PULSE_TRAIN_AT_TIME         ( 4 )
#define ETH_PPS_OUTPUT_CMD_STOP_PULSE_TRAIN_IMMEDIATELY     ( 5 )
#define ETH_PPS_OUTPUT_CMD_CANCEL_STOP_PULSE_TRAIN          ( 6 )

/**
  * @}
  */

 /** 
  * @brief  ETH interrupt flag
  */
  
#define ETH_INT_FLAG_TX                                     ( 0 )
#define ETH_INT_FLAG_TX_BUF_UNAVAILABLE                     ( 1 )
#define ETH_INT_FLAG_EARLY_RX                               ( 2 )
#define ETH_INT_FLAG_RX                                     ( 3 )
#define ETH_INT_FLAG_FATAL_BUS_ERR                          ( 4 )
#define ETH_INT_FLAG_EARLY_TX                               ( 5 )
#define ETH_INT_FLAG_RX_WATCHDOG_TIMEOUT                    ( 6 )
#define ETH_INT_FLAG_RX_STOP                                ( 7 )
#define ETH_INT_FLAG_RX_BUF_UNAVAILABLE                     ( 8 )
#define ETH_INT_FLAG_UNDERFLOW                              ( 9 )
#define ETH_INT_FLAG_OVERFLOW                               ( 10 )
#define ETH_INT_FLAG_JABBER                                 ( 11 )
#define ETH_INT_FLAG_TX_STOP                                ( 12 )
#define ETH_INT_FLAG_LPI                                    ( 13 )
#define ETH_INT_FLAG_TIMESTAMP                              ( 14 )
#define ETH_INT_FLAG_PMT                                    ( 15 )
#define ETH_INT_FLAG_MMC                                    ( 16 )
#define ETH_INT_FLAG_MMC_TX                                 ( 17 )
#define ETH_INT_FLAG_MMC_TX_GOOD_FRAME                      ( 18 )
#define ETH_INT_FLAG_MMC_TX_SINGLE_COLLIDION                ( 19 )
#define ETH_INT_FLAG_MMC_TX_MULTIPLE_COLLIDION              ( 20 )
#define ETH_INT_FLAG_MMC_RX                                 ( 21 )
#define ETH_INT_FLAG_MMC_RX_UNICAST                         ( 22 )
#define ETH_INT_FLAG_MMC_RX_ALIGNMENT_ERR                   ( 23 )
#define ETH_INT_FLAG_MMC_RX_CRC_ERR                         ( 24 )
#define ETH_INT_FLAG_MAX                                    ( 25 )

/**
  * @}
  */

 /** 
  * @brief  ETH interrupt
  */
  
#define ETH_INT_TX                                          ( 0 )
#define ETH_INT_TX_BUF_UNAVAILABLE                          ( 1 )
#define ETH_INT_EARLY_RX                                    ( 2 )
#define ETH_INT_RX                                          ( 3 )
#define ETH_INT_FATAL_BUS_ERR                               ( 4 )
#define ETH_INT_EARLY_TX                                    ( 5 )
#define ETH_INT_RX_WATCHDOG_TIMEOUT                         ( 6 )
#define ETH_INT_RX_STOP                                     ( 7 )
#define ETH_INT_RX_BUF_UNAVAILABLE                          ( 8 )
#define ETH_INT_UNDERFLOW                                   ( 9 )
#define ETH_INT_OVERFLOW                                    ( 10 )
#define ETH_INT_JABBER                                      ( 11 )
#define ETH_INT_TX_STOP                                     ( 12 )
#define ETH_INT_LPI                                         ( 13 )
#define ETH_INT_TIMESTAMP                                   ( 14 )
#define ETH_INT_PMT                                         ( 15 )
#define ETH_INT_MMC_TX_GOOD_FRAME                           ( 18 )
#define ETH_INT_MMC_TX_SINGLE_COLLIDION                     ( 19 )
#define ETH_INT_MMC_TX_MULTIPLE_COLLIDION                   ( 20 )
#define ETH_INT_MMC_RX_UNICAST                              ( 22 )
#define ETH_INT_MMC_RX_ALIGNMENT_ERR                        ( 23 )
#define ETH_INT_MMC_RX_CRC_ERR                              ( 24 )
#define ETH_INT_MAX                                         ( 25 )

/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/


/** 
 * @brief  Enables the specified ETHERNET MAC interrupts.
  * @param  __HANDLE__    ETH Handle
  * @param  __INTERRUPT__ specifies the ETHERNET MAC interrupt sources to be
  *   enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg ETH_MAC_IT_TST : Time stamp trigger interrupt 
  *     @arg ETH_MAC_IT_PMT : PMT interrupt 
  * @retval None
  */
#define __HAL_ETH_MAC_ENABLE_IT(__HANDLE__, __INTERRUPT__)      ((__HANDLE__)->Instance->MACIMR |= (__INTERRUPT__))

/**
  * @brief  Disables the specified ETHERNET MAC interrupts.
  * @param  __HANDLE__    ETH Handle
  * @param  __INTERRUPT__ specifies the ETHERNET MAC interrupt sources to be
  *   enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg ETH_MAC_IT_TST : Time stamp trigger interrupt 
  *     @arg ETH_MAC_IT_PMT : PMT interrupt
  * @retval None
  */
#define __HAL_ETH_MAC_DISABLE_IT(__HANDLE__, __INTERRUPT__)     ((__HANDLE__)->Instance->MACIMR &= ~(__INTERRUPT__))
#define __HAL_ETH_MAC_GET_IT(__HANDLE__, __INTERRUPT__)         (((__HANDLE__)->Instance->MACIMR &( __INTERRUPT__)) == ( __INTERRUPT__))

/**
  * @brief  Checks whether the specified ETHERNET MAC flag is set or not.
  * @param  __HANDLE__ ETH Handle
  * @param  __FLAG__ specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg ETH_MAC_FLAG_TST  : Time stamp trigger flag   
  *     @arg ETH_MAC_FLAG_MMCT : MMC transmit flag  
  *     @arg ETH_MAC_FLAG_MMCR : MMC receive flag   
  *     @arg ETH_MAC_FLAG_MMC  : MMC flag  
  *     @arg ETH_MAC_FLAG_PMT  : PMT flag  
  * @retval The state of ETHERNET MAC flag.
  */
#define __HAL_ETH_MAC_GET_FLAG(__HANDLE__, __FLAG__)            (((__HANDLE__)->Instance->MACSR &( __FLAG__)) == ( __FLAG__))

/** 
  * @brief  Enables the specified ETHERNET DMA interrupts.
  * @param  __HANDLE__    ETH Handle
  * @param  __INTERRUPT__ specifies the ETHERNET DMA interrupt sources to be
  *   enabled @ref ETH_DMA_Interrupts
  * @retval None
  */
#define __HAL_ETH_DMA_ENABLE_IT(__HANDLE__, __INTERRUPT__)      ((__HANDLE__)->Instance->DMAIER |= (__INTERRUPT__))

/**
  * @brief  Disables the specified ETHERNET DMA interrupts.
  * @param  __HANDLE__    ETH Handle
  * @param  __INTERRUPT__ specifies the ETHERNET DMA interrupt sources to be
  *   disabled. @ref ETH_DMA_Interrupts
  * @retval None
  */
#define __HAL_ETH_DMA_DISABLE_IT(__HANDLE__, __INTERRUPT__)     ((__HANDLE__)->Instance->DMAIER &= ~(__INTERRUPT__))

#define __HAL_ETH_DMA_GET_IT(__HANDLE__, __INTERRUPT__)         (((__HANDLE__)->Instance->DMAIER & (__INTERRUPT__)) == (__INTERRUPT__))

/**
  * @brief  Clears the ETHERNET DMA IT pending bit.
  * @param  __HANDLE__    ETH Handle
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear. @ref ETH_DMA_Interrupts
  * @retval None
  */
//#define __HAL_ETH_DMA_CLEAR_FLAG(__HANDLE__, __FLAG__)      ((__HANDLE__)->Instance->DMASR =(__FLAG__))

/**
  * @brief  Checks whether the specified ETHERNET DMA flag is set or not.
* @param  __HANDLE__ ETH Handle
  * @param  __FLAG__ specifies the flag to check. @ref ETH_DMA_Flags
  * @retval The new state of ETH_DMA_FLAG (SET or RESET).
  */
#define __HAL_ETH_DMA_GET_FLAG(__HANDLE__, __FLAG__)            (((__HANDLE__)->Instance->DMASR &( __FLAG__)) == ( __FLAG__))

/**
  * @brief  Checks whether the specified ETHERNET DMA flag is set or not.
  * @param  __HANDLE__ ETH Handle
  * @param  __FLAG__ specifies the flag to clear. @ref ETH_DMA_Flags
  * @retval The new state of ETH_DMA_FLAG (SET or RESET).
  */
#define __HAL_ETH_DMA_CLEAR_FLAG(__HANDLE__, __FLAG__)          ((__HANDLE__)->Instance->DMASR = (__FLAG__))





#define IS_ETH_SOURCE_ADDR_CONTROL(para)                    (((para) == ETH_SOURCE_ADDRESS_DISABLE) || \
                                                             ((para) == ETH_SOURCE_ADDRESS_INSERT_ADDR0) || \
                                                             ((para) == ETH_SOURCE_ADDRESS_INSERT_ADDR1) || \
                                                             ((para) == ETH_SOURCE_ADDRESS_REPLACE_ADDR0) || \
                                                             ((para) == ETH_SOURCE_ADDRESS_REPLACE_ADDR1))
  
#define IS_ETH_INTERFRAME_GAP(para)                         (((para) == ETH_INTERFRAME_GAP_96BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_88BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_80BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_72BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_64BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_56BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_48BIT) || \
                                                             ((para) == ETH_INTERFRAME_GAP_40BIT))
  
#define IS_ETH_SPEED(para)                                  (((para) == ETH_SPEED_10M) || \
                                                             ((para) == ETH_SPEED_100M))
                                                             
#define IS_ETH_DUPLEX_MODE(para)                            (((para) == ETH_MODE_FULL_DUPLEX) || \
                                                             ((para) == ETH_MODE_HALF_DUPLEX))
                                                             
#define IS_ETH_MAC_SPEED(para)                              (((para) == ETH_MAC_SPEED_10M) || \
                                                             ((para) == ETH_MAC_SPEED_100M))
  
#define IS_ETH_MODE(para)                                   (((para) == ETH_MODE_HALF_DUPLEX) || \
                                                             ((para) == ETH_MODE_FULL_DUPLEX))
  
#define IS_ETH_BACK_OFF_LIMIT(para)                         (((para) == ETH_BACK_OFF_LIMIT_10) || \
                                                             ((para) == ETH_BACK_OFF_LIMIT_8) || \
                                                             ((para) == ETH_BACK_OFF_LIMIT_4) || \
                                                             ((para) == ETH_BACK_OFF_LIMIT_1))

#define IS_ETH_PREAMBLE_LEN(para)                           (((para) == ETH_PREAMBLE_LEN_7B) || \
                                                             ((para) == ETH_PREAMBLE_LEN_5B) || \
                                                             ((para) == ETH_PREAMBLE_LEN_3B))
  
#define IS_ETH_PASS_CONTROL_PACKETS(para)                   (((para) == ETH_PASS_CONTROL_BLOCK_ALL) || \
                                                             ((para) == ETH_PASS_CONTROL_FORWARD_ALL_EXCEPT_PAUSE_FRAME) || \
                                                             ((para) == ETH_PASS_CONTROL_FORWARD_ALL) || \
                                                             ((para) == ETH_PASS_CONTROL_FORWARD_PASSED_ADDR_FILTER))
  
#define IS_ETH_PAUSE_TIME(para)                             ((para) <= 0xFFFFUL)
  
#define IS_ETH_PAUSE_LOW_THRESHOLD(para)                    (((para) == ETH_PAUSE_LOW_THRESHOLD_MINUS_4) || \
                                                             ((para) == ETH_PAUSE_LOW_THRESHOLD_MINUS_28) || \
                                                             ((para) == ETH_PAUSE_LOW_THRESHOLD_MINUS_144) || \
                                                             ((para) == ETH_PAUSE_LOW_THRESHOLD_MINUS_256))
  
#define IS_ETH_VLAN_TAG(para)                               ((para) <= 0x0000FFFFUL)
  
  
#define IS_ETH_TRANSMIT_THRESHOLD_CONTROL(para)             (((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_64BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_128BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_192BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_256BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_40BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_32BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_24BYTES) || \
                                                             ((para) == ETH_TRANSMIT_THRESHOLD_CONTROL_16BYTES))

#define IS_ETH_RECEIVED_THRESHOLD_CONTROL(para)             (((para) == ETH_RECEIVED_THRESHOLD_CONTROL_64BYTES) || \
                                                             ((para) == ETH_RECEIVED_THRESHOLD_CONTROL_32BYTES) || \
                                                             ((para) == ETH_RECEIVED_THRESHOLD_CONTROL_96BYTES) || \
                                                             ((para) == ETH_RECEIVED_THRESHOLD_CONTROL_128BYTES))

#define IS_ETH_RX_DMA_BURST_LEN(para)                       (((para) == ETH_RX_DMA_BURST_LEN_1BEAT) || \
                                                             ((para) == ETH_RX_DMA_BURST_LEN_2BEAT) || \
                                                             ((para) == ETH_RX_DMA_BURST_LEN_4BEAT) || \
                                                             ((para) == ETH_RX_DMA_BURST_LEN_8BEAT) || \
                                                             ((para) == ETH_RX_DMA_BURST_LEN_16BEAT) || \
                                                             ((para) == ETH_RX_DMA_BURST_LEN_32BEAT))

#define IS_ETH_RX_TX_PRIORITY_RATIO(para)                   (((para) == ETH_RX_TX_PRIORITY_RATIO_1_1) || \
                                                             ((para) == ETH_RX_TX_PRIORITY_RATIO_2_1) || \
                                                             ((para) == ETH_RX_TX_PRIORITY_RATIO_3_1) || \
                                                             ((para) == ETH_RX_TX_PRIORITY_RATIO_4_1))
  
#define IS_ETH_TX_DMA_BURST_LEN(para)                       (((para) == ETH_TX_DMA_BURST_LEN_1BEAT) || \
                                                             ((para) == ETH_TX_DMA_BURST_LEN_2BEAT) || \
                                                             ((para) == ETH_TX_DMA_BURST_LEN_4BEAT) || \
                                                             ((para) == ETH_TX_DMA_BURST_LEN_8BEAT) || \
                                                             ((para) == ETH_TX_DMA_BURST_LEN_16BEAT) || \
                                                             ((para) == ETH_TX_DMA_BURST_LEN_32BEAT))

#define IS_ETH_DESCRIPTOR_SKIP_LEN(para)                    ((para) <= 31U)

#define IS_ETH_DMA_ARBITRATION(para)                        (((para) == ETH_DMA_ARBITRATION_ROUNDROBIN) || \
                                                             ((para) == ETH_DMA_ARBITRATION_RX_PRIOR_TX))
                                                             
                                                             
                                                             
                                                             
                                                             
#define IS_ETH_ADDR_FILTER_GROUP_MASK(para)                 (((para) & ~(ETH_MACA1HR_MBC >> ETH_MACA1HR_MBC_Pos)) == 0)

#define IS_ETH_VLAN_HASH_TABLE(para)                        ((para) <= 0xFFFFU)

#define IS_ETH_VLAN_MODE(para)                              (((para) == ETH_VLAN_MODE_NONE) || \
                                                             ((para) == ETH_VLAN_MODE_DELETE) || \
                                                             ((para) == ETH_VLAN_MODE_INSERT) || \
                                                             ((para) == ETH_VLAN_MODE_REPLACE))

#define IS_ETH_IPV6_ADDR_MASK(para)                         ((para) <= 127U)

#define IS_ETH_IPV4_SOUR_ADDR_MASK(para)                    ((para) <= 31U)

#define IS_ETH_IPV4_DEST_ADDR_MASK(para)                    ((para) <= 31U)

#define IS_ETH_SOUR_PORT(para)                              ((para) <= 0xFFFFU)

#define IS_ETH_DEST_PORT(para)                              ((para) <= 0xFFFFU)


/* Exported functions --------------------------------------------------------*/

void HAL_ETH_IRQHandler(ETH_HandleTypeDef *heth);
void HAL_ETH_WakeupIRQHandler(ETH_HandleTypeDef *heth);
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_DMAErrorCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_MMCTxCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_MMCRxCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_PMTCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_LPITransmitEntryCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_LPITransmitExitCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_LPIReceiveEntryCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_LPIReceiveExitCallback(ETH_HandleTypeDef *heth);
void HAL_ETH_WakeupCallback(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_Init(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DeInit(ETH_HandleTypeDef *heth);
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth);
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DMATxDescListInit(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DMARxDescListInit(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetTxBuff(ETH_HandleTypeDef *heth, ETH_BuffTypeDef **buff);
HAL_StatusTypeDef HAL_ETH_Transmit(ETH_HandleTypeDef *heth, ETH_BuffTypeDef *buff, uint32_t mode, ETH_TxStatusTypeDef *pStatus);
HAL_StatusTypeDef HAL_ETH_Receive(ETH_HandleTypeDef *heth, ETH_BuffTypeDef **buff, ETH_RxStatusTypeDef *pStatus);
HAL_StatusTypeDef HAL_ETH_ReleaseRxDescriptors(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_Start(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_Stop(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ITConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitSpeedDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetSpeedParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_SpeedConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_SpeedDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitDuplexModeDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetDuplexModeParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DuplexModeConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DuplexModeDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitMACAddrDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetMACAddrParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MACAddrConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MACAddrDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitMACDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetMACParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MACConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MACDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitDMADefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetDMAParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DMAConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_DMADefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitFilterDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetFilterParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_FilterConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_FilterDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitAddrFilterDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetAddrFilterParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_AddrFilterConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_AddrFilterDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitVLANFilterDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetVLANFilterParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_VLANFilterConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_VLANFilterDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitL3L4FilterDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetL3L4FilterParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_L3L4FilterConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_L3L4FilterDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitVLANDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetVLANParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_VLANConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_VLANDefaultConfig(ETH_HandleTypeDef *heth);
uint32_t HAL_ETH_GetReceiveFlowCtrlBusyStatus(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ReceiveFlowCtrlEnable(ETH_HandleTypeDef *heth, uint32_t pausetime);
HAL_StatusTypeDef HAL_ETH_ReceiveFlowCtrlDisable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_TransmitFlowCtrlEnable(ETH_HandleTypeDef *heth, uint32_t unicast_pause_frame_detect);
HAL_StatusTypeDef HAL_ETH_TransmitFlowCtrlDisable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ReceiveBackPressureEnable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ReceiveBackPressureDisable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitMMCDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetMMCParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MMCConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MMCDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetMMCCounter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MMCFreezeCounterEnable(ETH_HandleTypeDef *heth, uint32_t NewState);
HAL_StatusTypeDef HAL_ETH_MMCFreezeCounterDisable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_MMCResetCounter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitLPIDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetLPIParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_LPIConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_LPIDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_EnterLPIMode(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ExitLPIMode(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_InitPMTDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetPMTParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PMTConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PMTDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_EnterPowerDownMode(ETH_HandleTypeDef *heth, uint32_t WakeupMode, uint32_t WakeupFrameEnable, uint32_t MagicPacketEnable);
HAL_StatusTypeDef HAL_ETH_InitPTPDefaultParamter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_GetPTPParameter(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PTPConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PTPDefaultConfig(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PTPStart(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPStop(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_PTPAdjFreq(ETH_HandleTypeDef *heth, int32_t freq);
HAL_StatusTypeDef HAL_ETH_PTPUpdateOffset(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPTimeSetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPGetSystemTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPGetTargetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPSetTargetTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
HAL_StatusTypeDef HAL_ETH_PTPGetAuxiliaryTime(ETH_HandleTypeDef *heth, ETH_TimestampTypeDef *timestamp);
uint32_t HAL_ETH_PTPGetStatus(ETH_HandleTypeDef *heth, ETH_PTPStatusTypeDef *status);
HAL_StatusTypeDef HAL_ETH_ConfigSMI(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef HAL_ETH_ReadPHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg, uint32_t *RegValue);
HAL_StatusTypeDef HAL_ETH_WritePHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg, uint32_t RegValue);
HAL_StatusTypeDef HAL_ETH_RxClockDelayConfig(ETH_HandleTypeDef *heth, uint32_t uint, uint32_t len);


#endif
