
#ifndef __IC_APS6404M_H__
#define __IC_APS6404M_H__

#include "stdint.h"
#include "stdbool.h"

#include "hal_spi.h"
#include "hal_gpio.h" 


#ifdef HAL_DMA_MODULE_ENABLED 
#define DMA_DATA_FLOW_P2M                       (DMA_DATAFLOW_P2M)
#define DMA_SOURCE_ADDR_INCREASE_DISABLE        (DMA_SRCINC_DISABLE) 
#define DMA_DST_ADDR_INCREASE_ENABLE            (DMA_DEST_INC)
#define DMA_SRC_WIDTH_BYTE                      (DMA_SRCWIDTH_BYTE)         
#define DMA_DST_WIDTH_BYTE                      (DMA_DESTWIDTH_BYTE)

#define DMA_DATA_FLOW_M2P                       (DMA_DATAFLOW_M2P)
#define DMA_SOURCE_ADDR_INCREASE_ENABLE         (DMA_SRC_INC)
#define DMA_DST_ADDR_INCREASE_DISABLE           (DMA_DESTINC_DISABLE) 
#endif

#define PSRAM __attribute__((section("PSRAM")))

typedef enum
{
    ERR_PSRAM_NONE = 0,
    ERR_PSRAM_DMANOTCFGED,
}HAL_SPI_PSRAM_Status;

typedef enum
{
    PSRAM_SPI_MODE,
    PSRAM_QPI_MODE
}psram_mode_enum;

typedef enum
{
    SPI_CS_HW = 0,
    SPI_CS_SW = 1,    
}spi_cs_mode_enum;

typedef struct _psram_info
{
    SPI_HandleTypeDef hspi;     /* SPI handle for PSRAM interface */
#ifdef HAL_DMA_MODULE_ENABLED    
    DMA_HandleTypeDef hDMATx;   /* DMA handle for spi dma write */
    DMA_HandleTypeDef hDMARx;   /* DMA handle for spi dma read */
#endif
    GPIO_TypeDef *CS_Port;
    uint32_t CS_Pin;
    uint32_t CS_Pin_AF;
    uint32_t psram_state;       /* PSRAM POR, RESET state, etc */
    psram_mode_enum mode;       /* PSRAM is currently in spi mode or in qpi mode */
    bool flag_dma_cfged;        /* whether SPI DMA is configured or not */
}psram_info_t;

#define APS6404M_CMD_RD							0x03
#define APS6404M_CMD_FAST_RD					0x0B
#define APS6404M_CMD_WRAP_RD					0x8B	//wrapped read
#define APS6404M_CMD_FAST_QRD					0xEB	//fast quad read
#define APS6404M_CMD_WR							0x02
#define APS6404M_CMD_WRAP_WR					0x8B	//wrapped write
#define APS6404M_CMD_QWR						0x38	//quad write
#define APS6404M_CMD_MOD_REG_RD					0xB5	//mode register read
#define APS6404M_CMD_MOD_REG_WR					0xB1	//mode register write
#define APS6404M_CMD_ENTER_QPI_MODE				0x35	//enter quad mode
#define APS6404M_CMD_EXIT_QPI_MODE			    0xF5	//exit quad mode
#define APS6404M_CMD_RST_EN						0x66	//reset enable
#define APS6404M_CMD_RST						0x99	//reset
#define APS6404M_CMD_BUR_LEN_TOG				0xC0	//burst length toggle
#define APS6404M_CMD_RD_ID						0x9F	//read ID


/* paltform dependent */
#define SPI_PSRAM_DMA_REQ_TX                    (DMA1_REQ_SPI8_TX)
#define SPI_PSRAM_DMA_REQ_RX                    (DMA1_REQ_SPI8_RX)
///* paltform dependent */
//#define SPI_PSRAM_CS_PORT						GPIOM
//#define SPI_PSRAM_CS_PIN						GPIO_PIN_1
//#define SPI_PSRAM_CS_FUNC                       GPIO_FUNCTION_15

#define SPI_PSRAM_CS_H()						HAL_GPIO_WritePin(psram_info.CS_Port, psram_info.CS_Pin, GPIO_PIN_SET)
#define SPI_PSRAM_CS_L()						HAL_GPIO_WritePin(psram_info.CS_Port, psram_info.CS_Pin, GPIO_PIN_RESET)


#define SPI_PSRAM_DMA_DISABLE()                 psram_info.hspi.Instance->RX_CTL &= ~(SPI_RX_CTL_DMA_REQ_EN | SPI_TX_CTL_DMA_REQ_EN)

#define __SPI_PSRAM_Read_Data(buf, size)        HAL_SPI_Receive(&psram_info.hspi, buf, size, 0)

#define __SPI_PSRAM_Write_Data(buf, size)       HAL_SPI_Transmit(&psram_info.hspi, buf, size, 0)

#define __SPI_PSRAM_SET_1X_MODE()               HAL_SPI_Wire_Config(&psram_info.hspi, SPI_1X_MODE)
#define __SPI_PSRAM_SET_2X_MODE()               HAL_SPI_Wire_Config(&psram_info.hspi, SPI_2X_MODE)
#define __SPI_PSRAM_SET_4X_MODE()               HAL_SPI_Wire_Config(&psram_info.hspi, SPI_4X_MODE)

#define __SWITCH_TO_FIFO_MODE()                 { psram_info.hspi.Instance->MEMO_ACC.acc_en = 0; SPI_CS_Mode_Cfg(SPI_CS_SW); __SPI_PSRAM_SET_1X_MODE();}
#define __SWITCH_TO_XIP_MODE()                  {SPI_CS_Mode_Cfg(SPI_CS_HW); SPI_PSRAM_DMA_DISABLE(); psram_info.hspi.Instance->MEMO_ACC.acc_en = 1; }



psram_info_t * get_psram_info(void);

void SPI_PSRAM_CS_Pin_select(psram_info_t* info);

#ifdef HAL_DMA_MODULE_ENABLED
HAL_SPI_PSRAM_Status IC_APS6404M_Init(SPI_TypeDef* SPIx, DMA_Channel_TypeDef *dma_rx_ch, DMA_Channel_TypeDef *dma_tx_ch);
#else
HAL_SPI_PSRAM_Status IC_APS6404M_Init(SPI_TypeDef* SPIx);
#endif

uint16_t IC_APS6404M_Read_ID(uint8_t eid[6]);
uint8_t IC_APS6404M_Read_ModeRegister(void);
uint8_t IC_APS6404M_Write_ModeRegister(uint8_t mr_val);

HAL_SPI_PSRAM_Status IC_APS6404M_Reset(void);
HAL_SPI_PSRAM_Status IC_APS6404M_Enter_QPI_Mode(void);
HAL_SPI_PSRAM_Status IC_APS6404M_Exit_QPI_Mode(void);

HAL_SPI_PSRAM_Status IC_APS6404M_Read(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_FastRead(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_Write(uint32_t addr, void *buf,  uint32_t len);

HAL_SPI_PSRAM_Status IC_APS6404M_QPI_Read(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_Write(uint32_t addr, void *buf,  uint32_t len);

#ifdef HAL_DMA_MODULE_ENABLED
void IC_APS6404M_DMA_RW_Cfg(DMA_Channel_TypeDef *dma_rx_ch, DMA_Channel_TypeDef *dma_tx_ch);

HAL_SPI_PSRAM_Status IC_APS6404M_DMA_Read(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_DMA_FastRead(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_DMA_Write(uint32_t addr, void *buf,  uint32_t len);

HAL_SPI_PSRAM_Status IC_APS6404M_QPI_DMA_Read(uint32_t addr, void *buf,  uint32_t len);
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_DMA_Write(uint32_t addr, void *buf,  uint32_t len);
#endif

#endif

