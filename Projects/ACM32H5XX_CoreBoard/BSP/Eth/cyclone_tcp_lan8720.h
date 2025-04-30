#ifndef __ACM32H5XX_ETH_DRIVER_FOR_CYCLONE_TCP_H__
#define __ACM32H5XX_ETH_DRIVER_FOR_CYCLONE_TCP_H__

//Dependencies
#include "core/net.h"

#define BSP_ETH_TXEN_GPIO           GPIOB
#define BSP_ETH_TXEN_PIN            GPIO_PIN_11
#define BSP_ETH_TXEN_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_TXEN_PULL           GPIO_PULLUP
#define BSP_ETH_TXEN_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_TXEN_ALTERNATE      GPIO_FUNCTION_11
#define BSP_ETH_TXEN_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_TXD0_GPIO           GPIOB
#define BSP_ETH_TXD0_PIN            GPIO_PIN_12
#define BSP_ETH_TXD0_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_TXD0_PULL           GPIO_PULLUP
#define BSP_ETH_TXD0_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_TXD0_ALTERNATE      GPIO_FUNCTION_11
#define BSP_ETH_TXD0_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_TXD1_GPIO           GPIOB
#define BSP_ETH_TXD1_PIN            GPIO_PIN_13
#define BSP_ETH_TXD1_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_TXD1_PULL           GPIO_PULLUP
#define BSP_ETH_TXD1_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_TXD1_ALTERNATE      GPIO_FUNCTION_11
#define BSP_ETH_TXD1_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_RXD0_GPIO           GPIOC
#define BSP_ETH_RXD0_PIN            GPIO_PIN_4
#define BSP_ETH_RXD0_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_RXD0_PULL           GPIO_PULLUP
#define BSP_ETH_RXD0_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_RXD0_ALTERNATE      GPIO_FUNCTION_9
#define BSP_ETH_RXD0_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_ETH_RXD1_GPIO           GPIOC
#define BSP_ETH_RXD1_PIN            GPIO_PIN_5
#define BSP_ETH_RXD1_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_RXD1_PULL           GPIO_PULLUP
#define BSP_ETH_RXD1_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_RXD1_ALTERNATE      GPIO_FUNCTION_9
#define BSP_ETH_RXD1_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_ETH_CRS_DV_GPIO         GPIOA
#define BSP_ETH_CRS_DV_PIN          GPIO_PIN_7
#define BSP_ETH_CRS_DV_MODE         GPIO_MODE_AF_PP
#define BSP_ETH_CRS_DV_PULL         GPIO_PULLUP
#define BSP_ETH_CRS_DV_DRIVE        GPIO_DRIVE_LEVEL3
#define BSP_ETH_CRS_DV_ALTERNATE    GPIO_FUNCTION_10
#define BSP_ETH_CRS_DV_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_ETH_REFCLK_GPIO         GPIOA
#define BSP_ETH_REFCLK_PIN          GPIO_PIN_1
#define BSP_ETH_REFCLK_MODE         GPIO_MODE_AF_PP
#define BSP_ETH_REFCLK_PULL         GPIO_PULLUP
#define BSP_ETH_REFCLK_DRIVE        GPIO_DRIVE_LEVEL3
#define BSP_ETH_REFCLK_ALTERNATE    GPIO_FUNCTION_11
#define BSP_ETH_REFCLK_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_ETH_MDC_GPIO            GPIOC
#define BSP_ETH_MDC_PIN             GPIO_PIN_1
#define BSP_ETH_MDC_MODE            GPIO_MODE_AF_PP
#define BSP_ETH_MDC_PULL            GPIO_PULLUP
#define BSP_ETH_MDC_DRIVE           GPIO_DRIVE_LEVEL3
#define BSP_ETH_MDC_ALTERNATE       GPIO_FUNCTION_11
#define BSP_ETH_MDC_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_ETH_MDIO_GPIO           GPIOA
#define BSP_ETH_MDIO_PIN            GPIO_PIN_2
#define BSP_ETH_MDIO_MODE           GPIO_MODE_AF_PP
#define BSP_ETH_MDIO_PULL           GPIO_PULLUP
#define BSP_ETH_MDIO_DRIVE          GPIO_DRIVE_LEVEL3
#define BSP_ETH_MDIO_ALTERNATE      GPIO_FUNCTION_11
#define BSP_ETH_MDIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_ETH_RST_GPIO            GPIOG
#define BSP_ETH_RST_PIN             GPIO_PIN_0
#define BSP_ETH_RST_MODE            GPIO_MODE_OUTPUT_PP
#define BSP_ETH_RST_PULL            GPIO_PULLUP
#define BSP_ETH_RST_DRIVE           GPIO_DRIVE_LEVEL3
#define BSP_ETH_RST_ALTERNATE       GPIO_FUNCTION_0
#define BSP_ETH_RST_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()


/* EMAC Media Interface definition */
#define ETH_MII                     ( 0U )

#define ETH_PHY_NUM                 ( 1U )
#define ETH_PHY_ADDR                ( 0U )
#define ETH_TX_DESC_CNT             ( 8 )
#define ETH_TX_BUFF_SIZE            ( ETH_MAX_PACKET_SIZE )
#define ETH_RX_DESC_CNT             ( 8 )
#define ETH_RX_BUFF_SIZE            ( ETH_MAX_PACKET_SIZE )

#define ETH_TX_DESC_LIST_MODE       ( ETH_DESC_LIST_MODE_LIST )
#define ETH_RX_DESC_LIST_MODE       ( ETH_DESC_LIST_MODE_LIST )

extern const NicDriver acm32h5xxEthDriver;

error_t BSP_ETH_Init(NetInterface *interface);
void BSP_ETH_Tick(NetInterface *interface);
void BSP_ETH_EnableIrq(NetInterface *interface);
void BSP_ETH_DisableIrq(NetInterface *interface);
void BSP_ETH_EventHandler(NetInterface *interface);
error_t BSP_ETH_SendPacket(NetInterface *interface, const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);
error_t BSP_ETH_ReceivePacket(NetInterface *interface);
error_t BSP_ETH_UpdateMacAddrFilter(NetInterface *interface);


error_t BSP_ETH_UpdateMacConfig(NetInterface *interface);
void BSP_ETH_WritePhyReg(uint8_t opcode, uint8_t phyAddr, uint8_t regAddr, uint16_t data);
uint16_t BSP_ETH_ReadPhyReg(uint8_t opcode, uint8_t phyAddr, uint8_t regAddr);
uint32_t BSP_ETH_CalcCrc(const void *data, size_t length);

#endif
