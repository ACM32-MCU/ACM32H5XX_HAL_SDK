/*
  ******************************************************************************
  * @file   IC_W25Qxx.h
  * @version V1.0.0
  * @date    2020
  * @brief   HAL Config header file.
  ******************************************************************************
*/
#ifndef __IC_TH58CVG3S0HRAIJ_H__
#define __IC_TH58CVG3S0HRAIJ_H__

#include "stdint.h"
#include "stdbool.h"

#include "hal_spi.h"
#include "hal_gpio.h" 

typedef enum
{
    ERR_NORFLASH_NONE = 0,
    ERR_NORFLASH_DMANOTCFGED,
}HAL_SPI_NandFLASH_Status;

#define __SPI_SET_1X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nand, SPI_1X_MODE)
#define __SPI_SET_2X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nand, SPI_2X_MODE)
#define __SPI_SET_4X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nand, SPI_4X_MODE)

#define __SPI_CS_Release()                      
#define __SPI_CS_Select()                       

#define __SPI_Read_Data(buf, size)              HAL_SPI_Receive(&SPI_Handle_Nand, buf, size, 0)
#define __SPI_Read_Data_KeepCS(buf, size)       HAL_SPI_ReceiveKeepCS(&SPI_Handle_Nand, buf, size, 0)
#define __SPI_Write_Data(buf, size)             HAL_SPI_Transmit(&SPI_Handle_Nand, buf, size, 0)
#define __SPI_Write_Data_KeepCS(buf, size)      HAL_SPI_TransmitKeepCS(&SPI_Handle_Nand, buf, size, 0)


/**
  * @brief  W25Qxx Command Descriptions
  */
#define NAND_READ_ID                            (0x9F)
#define NAND_WRITE_ENABLE                       (0x06)
#define NAND_GET_FEATURE                        (0x0F)
#define NAND_SET_FEATURE                        (0x1F)
#define NAND_WRITE_DISABLE                      (0x04)
#define NAND_BLOCK_ERASE                        (0xD8)
#define NAND_PROGRAM_LOAD_1x                    (0x02)
#define NAND_PROGRAM_LOAD_4x                    (0x32)
#define NAND_PROGRAM_EXECUTE                    (0x10)
#define NAND_PROGRAM_LOAD_RANDOM_1x             (0x84)
#define NAND_PROGRAM_LOAD_RANDOM_4x             (0x34)
#define NAND_READ_CELL_ARRAY                    (0x13)
#define NAND_READ_BUFFER_1x                     (0x03)
#define NAND_READ_BUFFER_2x                     (0x3B)
#define NAND_READ_BUFFER_4x                     (0x6B)


SPI_HandleTypeDef *SPI_NorFlash_Gethspi(void);
HAL_SPI_NandFLASH_Status SPI_NandFlash_Init(SPI_TypeDef* SPIx, uint32_t clk_div);
uint16_t ic_th58cvg3_readid(void);
uint8_t ic_th58cvg3_get_feature(uint8_t feature_addr);
bool ic_th58cvg3_set_feature(uint8_t feature_addr, uint8_t value);
bool ic_th58cvg3_block_unlock(void);
bool ic_th58cvg3_erase_block(uint32_t block_addr);
bool ic_th58cvg3_program(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size);
bool ic_th58cvg3_program_4x(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size);
void ic_th58cvg3_read(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size);
void ic_th58cvg3_read_4x(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size);
bool ic_th58cvg3_enable_4x_mode(void);
bool ic_th58cvg3_disable_4x_mode(void);

#endif
