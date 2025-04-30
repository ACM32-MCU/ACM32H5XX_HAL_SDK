/******************************************************************************
*@file  : bsp_eth.h
*@brief : 
******************************************************************************/

#include "bsp_eth.h"
#include "bsp_lan8720.h"

ETH_HandleTypeDef ETH_Handle;

ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT];
uint8_t TX_Buff[ETH_TX_DESC_CNT][ETH_TX_BUFF_SIZE];
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT];
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFF_SIZE];

ETH_BuffTypeDef TxBuffTab[ETH_TX_DESC_CNT];
ETH_BuffTypeDef RxBuffTab[ETH_RX_DESC_CNT];

uint8_t ETH_MAC_ADDR[6] = {0x00, 0xAB, 0xCD, 0xEF, 0x07, 0x37};

HAL_StatusTypeDef bsp_eth_init(void)
{
    HAL_ETH_InitDefaultParamter(&ETH_Handle);

    //printfS("init\r\n");
    memcpy((void *)ETH_Handle.Init.MACAddr, (void *)ETH_MAC_ADDR, 6);
    
    ETH_Handle.Instance                     = ETH;
    ETH_Handle.Init.MediaInterface          = ETH_MEDIA_INTERFACE_RMII;
    ETH_Handle.Init.AutoNegotiation         = ENABLE;
    ETH_Handle.Init.Speed                   = ETH_SPEED_100M;
    ETH_Handle.Init.DuplexMode              = ETH_MODE_FULL_DUPLEX;
    ETH_Handle.Init.AutoCRC                 = ENABLE;
    ETH_Handle.Init.AutoPad                 = ENABLE;
    ETH_Handle.Init.AutoChecksumInsertion   = ETH_CHECKSUM_INSERTION_CTRL_IP_HEAD_PAYLOAD;
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
        printfS("ETH initialization failed.\r\n");
        return HAL_ERROR;
    }
    
	if (bsp_lan8720_init(&ETH_Handle) != HAL_OK)
	{
        printfS("LAN8720 initialization failed.\r\n");
        return HAL_ERROR;
	}

    HAL_ETH_Start (&ETH_Handle);
    return HAL_OK;
}
