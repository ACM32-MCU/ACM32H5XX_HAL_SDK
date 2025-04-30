/*
  ******************************************************************************
  * @file   IC_W25Qxx.h
  * @version V1.0.0
  * @date    2020
  * @brief   HAL Config header file.
  ******************************************************************************
*/
#ifndef __IC_W25QXX_H__
#define __IC_W25QXX_H__

#include "stdint.h"
#include "stdbool.h"

#include "hal_spi.h"
#include "hal_gpio.h" 

typedef enum
{
    ERR_NORFLASH_NONE = 0,
    ERR_NORFLASH_DMANOTCFGED,
}HAL_SPI_NORFLASH_Status;

#define __SPI_SET_1X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nor, SPI_1X_MODE)
#define __SPI_SET_2X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nor, SPI_2X_MODE)
#define __SPI_SET_4X_MODE()                     HAL_SPI_Wire_Config(&SPI_Handle_Nor, SPI_4X_MODE)

#define __SPI_CS_Release()                      
#define __SPI_CS_Select()                       

#define __SPI_Read_Data(buf, size)              HAL_SPI_Receive(&SPI_Handle_Nor, buf, size, 0)
#define __SPI_Read_Data_KeepCS(buf, size)       HAL_SPI_ReceiveKeepCS(&SPI_Handle_Nor, buf, size, 0)
#define __SPI_Write_Data(buf, size)             HAL_SPI_Transmit(&SPI_Handle_Nor, buf, size, 0)
#define __SPI_Write_Data_KeepCS(buf, size)      HAL_SPI_TransmitKeepCS(&SPI_Handle_Nor, buf, size, 0)

#define SPI_DMA_DISABLE()                       SPI_NorFlash_Gethspi()->Instance->RX_CTL &= ~(SPI_RX_CTL_DMA_REQ_EN | SPI_TX_CTL_DMA_REQ_EN)
#define __SWITCH_TO_FIFO_MODE()                 {__SPI_MEMACC_DISABLE(SPI_NorFlash_Gethspi()->Instance); IC_W25Qxx_CRMReset();}
#define __SWITCH_TO_XIP_MODE()                  {SPI_DMA_DISABLE(); __SPI_MEMACC_ENABLE(SPI_NorFlash_Gethspi()->Instance); __SPI_MEMACC_INSTRONCE_CLEAR(SPI_NorFlash_Gethspi()->Instance);}

/*********************************************************************************
                          One Block have 32K 

           Block                Setor                 Address Range

                                  17               0x011000 ~ 0x011FFF
                                  16               0x010000 ~ 0x010FFF
                                  15               0x00F000 ~ 0x00FFFF
                                  14               0x00E000 ~ 0x00EFFF
             1                    13               0x00D000 ~ 0x00DFFF
                                  12               0x00C000 ~ 0x00CFFF
                                  11               0x00B000 ~ 0x00BFFF
                                  10               0x00A000 ~ 0x00AFFF
                                  9                0x009000 ~ 0x009FFF

                                  8                0x008000 ~ 0x008FFF
                                  7                0x007000 ~ 0x007FFF
                                  6                0x006000 ~ 0x006FFF
                                  5                0x005000 ~ 0x005FFF
             0                    4                0x004000 ~ 0x004FFF
                                  3                0x003000 ~ 0x003FFF
                                  2                0x002000 ~ 0x002FFF
                                  1                0x001000 ~ 0x001FFF
                                  0                0x000000 ~ 0x000FFF
**********************************************************************************/

/**
  * @brief  W25Qxx Size
  */
#define W25QXX_PAGE_SIZE      (256U)     // Each Page has 256 Bytes
#define W25QXX_SECTOR_SIZE    (4096U)    // Each Sector has 4k

/**
  * @brief  W25Qxx Command Descriptions
  */
#define WRITE_ENABLE                             (0x06)
#define WRITE_DISABLE                            (0x04)
#define READ_STATUS_REGISTER_S07_S00             (0x05)
#define READ_STATUS_REGISTER_S15_S08             (0x35)
#define WRITE_STATUS_REGISTER                    (0x01)
#define WRITE_ENABLE_VOLATILE_STATUS_REGISTER    (0x50)
#define READ_DATA                                (0x03)
#define READ_DATA_FAST                           (0x0B)
#define DUAL_OUTPUT_FAST_READ                    (0x3B)
#define QUAD_OUTPUT_FAST_READ                    (0x6B)
#define DUAL_IO_FAST_READ                        (0xBB)
#define QUAD_IO_FAST_READ                        (0xEB)
#define SET_BURST_WITH_WRAP                      (0x77)
#define PAGE_PROGARM                             (0x02)
#define QUAD_PAGE_PROGRAM                        (0x32)
#define SECTOR_ERASE                             (0x20)
#define BLOCK_ERASE_32K                          (0x52)
#define BLOCK_ERASE_64K                          (0x52)
#define CHIP_ERASE                               (0xC7)
#define READ_DEVICE_ID                           (0x90)
#define READ_ID                                  (0x9F)
#define READ_UNIQUE_ID                           (0x4B)
#define ERASE_SECURITY_REGISTER                  (0x44)
#define PROGRAM_SECURITY_REGISTER                (0x42)
#define READ_SECURITY_REGISTER                   (0x48)
#define ENABLE_RESET                             (0x66)
#define RESET                                    (0x99)
#define PROGRAM_ERASE_SUSPEND                    (0x75)
#define PROGRAM_ERASE_RESUME                     (0x7A)
#define DEEP_POWER_DOWN                          (0xB9)
#define RELEASE_FORM_DEEP_POWER_DOWN             (0xAB)
#define READ_DATA_COMPATIBILITY                  (0x5A)

/**
  * @brief  W25Qxx Stauts Register
  */
#define REGISTER_NULL            (0)

#define REGISTER_S07_S00_SRP0    (1 << 7)
#define REGISTER_S07_S00_BP4     (1 << 6)
#define REGISTER_S07_S00_BP3     (1 << 5)
#define REGISTER_S07_S00_BP2     (1 << 4)
#define REGISTER_S07_S00_BP1     (1 << 3)
#define REGISTER_S07_S00_BP0     (1 << 2)
#define REGISTER_S07_S00_WEL     (1 << 1)
#define REGISTER_S07_S00_WIP     (1 << 0)

#define REGISTER_S15_S08_SUS     (1 << 7)
#define REGISTER_S15_S08_CMP     (1 << 6)
#define REGISTER_S15_S08_NULL    (1 << 5)
#define REGISTER_S15_S08_DC      (1 << 4)
#define REGISTER_S15_S08_LB1     (1 << 3)
#define REGISTER_S15_S08_LB0     (1 << 2)
#define REGISTER_S15_S08_QE      (1 << 1)    // Quad Enable
#define REGISTER_S15_S08_SRP1    (1 << 0)

SPI_HandleTypeDef *SPI_NorFlash_Gethspi(void);

HAL_SPI_NORFLASH_Status SPI_NorFlash_Init(SPI_TypeDef* SPIx, uint32_t clk_div);

HAL_SPI_NORFLASH_Status SPI_NorFlash_Init_testbench(SPI_TypeDef* SPIx, uint32_t mem_xmode, bool instr_once, \
                                                bool crm_en, bool cs_tout, uint32_t cs_timeout);

HAL_SPI_NORFLASH_Status SPI_NorFlash_Init_FIFOOnly(SPI_TypeDef* SPIx);

/* Function : IC_W25Qxx_WriteEnable */
void IC_W25Qxx_WriteEnable(void);

/* Function : IC_W25Qxx_WriteDisable */
void IC_W25Qxx_WriteDisable(void);

/* Function : IC_W25Qxx_WriteRegister */
void IC_W25Qxx_WriteRegister(uint8_t fu8_Register_S7_S0, uint8_t fu8_Register_S15_S08);

/* Function : IC_W25Qxx_Read_ID */
uint16_t IC_W25Qxx_Read_ID(void);

/* Function : IC_W25Qxx_Read_RegisterS07_S00 */
uint8_t IC_W25Qxx_Read_RegisterS07_S00(void);

/* Function : IC_W25Qxx_Read_RegisterS15_S08 */
uint8_t IC_W25Qxx_Read_RegisterS15_S08(void);

/* Function : IC_W25Qxx_Read */
void IC_W25Qxx_Read_Data(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_Read_Dual_IO */
void IC_W25Qxx_Read_Dual_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_Read_Dual_Output */
void IC_W25Qxx_Read_Dual_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_Read_Quad_IO */
void IC_W25Qxx_Read_Quad_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_Read_Quad_Output */
void IC_W25Qxx_Read_Quad_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_PageProgram */
void IC_W25Qxx_PageProgram(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_PageProgram_Quad */
void IC_W25Qxx_PageProgram_Quad(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

/* Function : IC_W25Qxx_EraseSector */
void IC_W25Qxx_EraseSector(uint32_t fu32_DataAddress);

/* Function : IC_W25Qxx_EraseChip */
void IC_W25Qxx_EraseChip(void);

/* Function : IC_W25Qxx_QuadConfig */
void IC_W25Qxx_QuadConfig(bool fb_Config);

/* Function : IC_W25Qxx_WaitBusy */
void IC_W25Qxx_WaitBusy(void);

/* Function : IC_W25Qxx_Reset */
void IC_W25Qxx_Reset(void);

/* Function : IC_W25Qxx_PowerDown */
void IC_W25Qxx_PowerDown(void);

/* Function : IC_W25Qxx_Wakeup */
void IC_W25Qxx_Wakeup(void);

void IC_W25Qxx_CRMReset(void);

#endif
