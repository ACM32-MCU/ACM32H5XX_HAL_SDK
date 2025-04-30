#ifndef __DRV_OSPI_H__
#define __DRV_OSPI_H__

#include  "hal.h" 

void OSPI_DelayBlockConfig(DLYB_TypeDef * ospi_dlyb, uint32_t Timeout);

void OSPI_DelayBlockSelConfig(DLYB_TypeDef * ospi_dlyb, uint8_t sel);


HAL_StatusTypeDef OSPI_TwoTransmits_Recieve_ByHalfWord(OSPI_HandleTypeDef *hospi, uint16_t *pTxCA, uint32_t TxCASize,uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint16_t *pRxData, uint32_t RXSize, uint32_t Timeout);
                                                
HAL_StatusTypeDef OSPI_TwoTransmits_Recieve_ByWord(OSPI_HandleTypeDef *hospi, uint32_t *pTxCA, uint32_t TxCASize,uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint32_t *pRxData, uint32_t RXSize, uint32_t Timeout);

HAL_StatusTypeDef OSPI_ThreeTransmits_ByHalfWord(OSPI_HandleTypeDef *hospi, uint16_t *pTxCA, uint32_t TxCASize, uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint16_t *pTxData, uint32_t TxSize, uint32_t Timeout);

HAL_StatusTypeDef OSPI_ThreeTransmits_ByWord(OSPI_HandleTypeDef *hospi, uint32_t *pTxCA, uint32_t TxCASize,uint16_t TxDummyData, \
                                                uint32_t TxLC0Size, uint32_t TxLC1Size, uint32_t *pTxData, uint32_t TxSize, uint32_t Timeout);                                                



void disable_all_irq(void);

void enable_all_irq(void);


#endif