#ifndef __BSP_LAN8720_H__
#define __BSP_LAN8720_H__

#include "hal.h"

#define LAN8720_MAX_ADDR							( 31UL )
//#define LAN8720_PHY_ADDRESS							( 0UL )
#define LAN8720_DELAY_CONFIG						( 5UL )
#define LAN8720_TIMEOUT_RESET						( 5UL )
#define LAN8720_TIMEOUT_AUTO_NEGOTIATION			( 5000UL )
#define LAN8720_TIMEOUT_LINKED_STATE				( 5000UL )

typedef enum lan8720_link_typedef_e
{
    LAN8720_LINK_DOWN       = 0UL,
    LAN8720_LINK_AUTO_NEGOTIATION,
    LAN8720_LINK_10M_HALF_DUPLEX,
    LAN8720_LINK_10M_FULL_DUPLEX,
    LAN8720_LINK_100M_HALF_DUPLEX,
    LAN8720_LINK_100M_FULL_DUPLEX,
} lan8720_link_typedef;  

typedef enum lan8720_status_typedef_e
{
    LAN8720_STATUS_OK       = 0x00U,
    LAN8720_STATUS_ERROR    = 0x01U,
    LAN8720_STATUS_BUSY     = 0x02U,
    LAN8720_STATUS_TIMEOUT  = 0x03U
} lan8720_status_typedef;  

#define LAN8720_AUTO_NEGOTIATION_ENABLE			( 1UL )
#define LAN8720_AUTO_NEGOTIATION_DISABLE		( 0UL )

#define LAN8720_SPEED_10M						( 0UL )
#define LAN8720_SPEED_100M						( 1UL )

#define LAN8720_MODE_FULL_DUPLEX				( 1UL )
#define LAN8720_MODE_HALF_DUPLEX				( 0UL )

#define LAN8720_RXPOLLING_MODE					( 0UL )
#define LAN8720_RXINTERRUPT_MODE				( 1UL )

#define LAN8720_MEDIA_INTERFACE_MII				( 0UL )
#define LAN8720_MEDIA_INTERFACE_RMII			( 1UL )


typedef enum lan8720_reg_typedef_e
{
	LAN8720_REG_BCR = 0UL,
	LAN8720_REG_BSR = 1UL,
	LAN8720_REG_PI1R = 2UL,
	LAN8720_REG_PI2R = 3UL,
	LAN8720_REG_ANAR = 4UL,
	LAN8720_REG_ANLPAR = 5UL,
	LAN8720_REG_ANER = 6UL,
	LAN8720_REG_MCSR = 17UL,
	LAN8720_REG_SMR = 18UL,
	LAN8720_REG_SECR = 26UL,
	LAN8720_REG_SESIR = 27UL,
	LAN8720_REG_ISFR = 29UL,
	LAN8720_REG_IMR = 30UL,
	LAN8720_REG_PSCSR = 31UL,
} lan8720_reg_typedef;

#define LAN8720_BCR_RESET							( BIT15 )
#define LAN8720_BCR_LOOP_BACK						( BIT14 )
#define LAN8720_BCR_SPEED							( BIT13 )
#define LAN8720_BCR_AUTO_NEGOTIATION				( BIT12 )
#define LAN8720_BCR_POWER_DOWN						( BIT11 )
#define LAN8720_BCR_ISOLATE							( BIT10 )
#define LAN8720_BCR_RESTART_AUTO_NEGOTIATION		( BIT9 )
#define LAN8720_BCR_DUPLEX_MODE						( BIT8 )
#define LAN8720_BCR_COLLISION_TEST					( BIT7 )


#define LAN8720_BSR_100BASE_T4						( BIT15 )
#define LAN8720_BSR_100BASE_TX_FULL_DUPLEX			( BIT14 )
#define LAN8720_BSR_100BASE_TX_HALF_DUPLEX			( BIT13 )
#define LAN8720_BSR_10BASE_T_FULL_DUPLEX			( BIT12 )
#define LAN8720_BSR_10BASE_T_HALF_DUPLEX			( BIT11 )
#define LAN8720_BSR_100BASE_T2_FULL_DUPLEX			( BIT10 )
#define LAN8720_BSR_100BASE_T2_HALF_DUPLEX			( BIT9 )
#define LAN8720_BSR_EXTENDED_STATUS					( BIT8 )
#define LAN8720_BSR_AUTO_NEGOTIATION_COMPLETE		( BIT5 )
#define LAN8720_BSR_REMOTE_FAULT					( BIT4 )
#define LAN8720_BSR_AUTO_NEGOTIATION_ABILITY		( BIT3 )
#define LAN8720_BSR_LINK_STATUS						( BIT2 )
#define LAN8720_BSR_JABBER_DETECT					( BIT1 )
#define LAN8720_BSR_EXTENDED_CAPABILITIES			( BIT0 )


#define LAN8720_ANAR_REMOTE_FAULT					( BIT13 )
#define LAN8720_ANAR_ASYMMETRIC_PAUSE				( BIT11 )
#define LAN8720_ANAR_SYMMETRIC_PAUSE				( BIT10 )
#define LAN8720_ANAR_100BASE_TX_FULL_DUPLEX			( BIT8 )
#define LAN8720_ANAR_100BASE_TX						( BIT7 )
#define LAN8720_ANAR_10BASE_T_FULL_DUPLEX			( BIT6 )
#define LAN8720_ANAR_10BASE_T						( BIT5 )

#define LAN8720_ANLPAR_PAUSE						( BIT10 )
#define LAN8720_ANLPAR_100BASE_T4					( BIT9 )
#define LAN8720_ANLPAR_100BASE_TX_FULL_DUPLEX		( BIT8 )
#define LAN8720_ANLPAR_100BASE_TX					( BIT7 )
#define LAN8720_ANLPAR_10BASE_T_FULL_DUPLEX			( BIT6 )
#define LAN8720_ANLPAR_10BASE_T						( BIT5 )


#define LAN8720_ANER_AUTO_NEGOTIATION_ABLE			( BIT0 )

#define LAN8720_MCSR_ENERGY_DETECT_POWER_DOWN		( BIT13 )
#define LAN8720_ANLPAR_FAR_LOOP_BACK				( BIT9 )
#define LAN8720_ANLPAR_ALTERNATE_INTERRUPT_MODE		( BIT6 )
#define LAN8720_ANLPAR_ENERGY_DETECT_POWER_ON		( BIT1 )

#define LAN8720_SMR_MII_MODE						( BIT14 )
#define LAN8720_SMR_MODE							( BIT7 | BIT6 | BIT5 )
#define LAN8720_SMR_PHY_ADDR						( BIT4 | BIT3 | BIT2 | BIT1 | BIT0 )

#define LAN8720_PSCSR_AUTO_NEGOTIATION_STATUS		( BIT12 )
#define LAN8720_PSCSR_ENABLE_4B5B					( BIT6 )
#define LAN8720_PSCSR_SPEED_STATUS					( BIT4 | BIT3 | BIT2 )
#define LAN8720_PSCSR_SPEED_STATUS_10M_HALF_DUPLEX	( BIT2 )
#define LAN8720_PSCSR_SPEED_STATUS_10M_FULL_DUPLEX	( BIT4 | BIT2 )
#define LAN8720_PSCSR_SPEED_STATUS_100M_HALF_DUPLEX	( BIT3 )
#define LAN8720_PSCSR_SPEED_STATUS_100M_FULL_DUPLEX	( BIT4 | BIT3 )






HAL_StatusTypeDef bsp_lan8720_init(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_deinit(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_soft_reset(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_power_down_enable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_power_down_disable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_get_media_interface(ETH_HandleTypeDef *heth, uint32_t *media_interface);
HAL_StatusTypeDef bsp_lan8720_auto_negotiation_enable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_auto_negotiation_disable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_get_link_state(ETH_HandleTypeDef *heth, uint32_t *auto_negotiation, uint32_t *state);
HAL_StatusTypeDef bsp_lan8720_set_link_state(ETH_HandleTypeDef *heth, uint32_t auto_negotiation, uint32_t state);
HAL_StatusTypeDef bsp_lan8720_loop_back_enable(ETH_HandleTypeDef *heth);
HAL_StatusTypeDef bsp_lan8720_loop_back_disable(ETH_HandleTypeDef *heth);




#endif

