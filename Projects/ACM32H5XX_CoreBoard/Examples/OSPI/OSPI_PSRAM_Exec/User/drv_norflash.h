
#ifndef __DRV_NORFLASH_H__
#define __DRV_NORFLASH_H__

#include  "hal.h" 




extern OSPI_HandleTypeDef NORFLASH_Handle;


#define OCTAL_SPI_MODE          (0U)
#define OCTAL_OPI_MODE          (1U)
#define OCTAL_OPI_STR_MODE      (2U)
#define OCTAL_OPI_DTR_MODE      (3U)


/**
  * @brief  NORFLASH Size
  */
#define NORFLASH_PAGE_SIZE      (256U)     // Each Page has 256 Bytes
#define NORFLASH_SECTOR_SIZE    (4096U)    // Each Sector has 4k

/**
  * @brief  NORFLASH Command Descriptions
  */
/* Software Reset Operations */
#define ENABLE_RESET                                        (0x66)
#define RESET                                               (0x99)

/* Read ID Operations */
#define READ_ID                                             (0x9F)
#define READ_UNIQUE_ID                                      (0x4B)
#define READ_PARAMETER                                      (0x5A)
#define READ_MID                                            (0x90)                                                             

/* Read Memory Operation */
#define READ_DATA                                           (0x03)
#define READ_DATA_FAST                                      (0x0B)
#define OCTAL_OUTPUT_FAST_READ                              (0x8B)
#define OCTAL_IO_FAST_READ                                  (0xCB)
#define DUAL_OUTPUT_FAST_READ                               (0x3B)
#define QUAD_OUTPUT_FAST_READ                               (0x6B)
#define DUAL_IO_FAST_READ                                   (0xBB)
#define QUAD_IO_FAST_READ                                   (0xEB)

/* Read Memory Operations with 4-Byte Address */
#define FOUR_BYTE_READ_DATA                                 (0x13)
#define FOUR_BYTE_READ_DATA_FAST                            (0x0C)
#define FOUR_BYTE_OCTAL_OUTPUT_FAST_READ                    (0x7C)
#define FOUR_BYTE_OCTAL_IO_FAST_READ                        (0xCC)
#define FOUR_BYTE_DTR_OCTAL_IO_FAST_READ                    (0xFD)

/* Write Operations */
#define WRITE_ENABLE                                        (0x06)
#define WRITE_DISABLE                                       (0x04)

/* Read Register Operations */
#define READ_STATUS_REGISTER                                (0x05)
#define READ_FLAG_STATUS_REGISTER                           (0x70)
#define READ_EXTENDED_ADDR_REGISTER                         (0xC8)
#define READ_NONVOLATILE_CONFIG_REGISTER                    (0xB5)
#define READ_VOLATILE_CONFIG_REGISTER                       (0x85)

#define READ_STATUS_REGISTER_S15_S08                        (0x35)

/* Write Register Operations */
#define NORFLASH_REG_NULL                                   (0)
#define WRITE_STATUS_REGISTER                               (0x01)
#define WRITE_EXTENDED_ADDR_REGISTER                        (0xC5)
#define VOLATILE_SR_WRITE_ENABLE                            (0x50)
#define WRITE_NONVOLATILE_CONFIG_REGISTER                   (0xB1)
#define WRITE_VOLATILE_CONFIG_REGISTER                      (0x81)

/* Clear Flag Status Register Operation */
#define CLEAR_FLAG_STATUS_REGISTER                          (0x30)

/* Program Operations */
#define PAGE_PROGARM                                        (0x02)
#define DUAL_PAGE_PROGARM                                   (0xA2) 
#define OCTAL_INPUT_FAST_PROGRAM                            (0x82)
#define EXTENDED_OCTAL_INPUT_FAST_PROGRAM                   (0xC2)
#define QUAD_PAGE_PROGRAM                                   (0x32)

/* Program Operations with 4-Byte Address */
#define FOUR_BYTE_PAGE_PROGARM                              (0x12)
#define FOUR_BYTE_OCTAL_INPUT_FAST_PROGRAM                  (0x84)
#define FOUR_BYTE_EXTENDED_OCTAL_INPUT_FAST_PROGRAM         (0x8E)

/* Erase Operations */
#define SECTOR_ERASE                                        (0x20)
#define BLOCK_ERASE_32K                                     (0x52)
#define BLOCK_ERASE_64K                                     (0xD8)
#define CHIP_ERASE                                          (0xC7)

/* Erase Operations with 4-Byte Address */
#define FOUR_BYTE_SECTOR_ERASE                              (0x21)
#define FOUR_BYTE_BLOCK_ERASE_32K                           (0x5C)
#define FOUR_BYTE_BLOCK_ERASE_64K                           (0xDC)

/* Suspend/Resume Operations */
#define SUSPEND_PROGRAM_ERASE                               (0x75)
#define RESUME_PROGRAM_ERASE                                (0x7A)

/* 4-ByteAddress Mode Operations */
#define ENTER_FOUR_BYTE_ADDR_MODE                           (0xB7)
#define EXIT_FOUR_BYTE_ADDR_MODE                            (0xE9)

/* Deep Power-Down Operations */
#define DEEP_POWER_DOWN                                     (0xB9)
#define RELEASE_FORM_DEEP_POWER_DOWN                        (0xAB)

/**
  * @brief  NORFLASH Stauts Register
  */
#define REGISTER_NULL               (0)

#define REGISTER_S07_S00_SRP0       (1 << 7)
#define REGISTER_S07_S00_BP4        (1 << 6)
#define REGISTER_S07_S00_BP3        (1 << 5)
#define REGISTER_S07_S00_BP2        (1 << 4)
#define REGISTER_S07_S00_BP1        (1 << 3)
#define REGISTER_S07_S00_BP0        (1 << 2)
#define REGISTER_S07_S00_WEL        (1 << 1)
#define REGISTER_S07_S00_WIP        (1 << 0)

#define REGISTER_S15_S08_SUS        (1 << 7)
#define REGISTER_S15_S08_CMP        (1 << 6)
#define REGISTER_S15_S08_NULL       (1 << 5)
#define REGISTER_S15_S08_DC         (1 << 4)
#define REGISTER_S15_S08_LB1        (1 << 3)
#define REGISTER_S15_S08_LB0        (1 << 2)
#define REGISTER_S15_S08_QE         (1 << 1)    // Quad Enable
#define REGISTER_S15_S08_SRP1       (1 << 0)


#define REGISTER_FS7_RY_BY          (1 << 7)
#define REGISTER_FS6_SUS_E          (1 << 6)
#define REGISTER_FS5_EE             (1 << 5)
#define REGISTER_FS4_PE             (1 << 4)
#define REGISTER_FS3_RSV            (1 << 3)
#define REGISTER_FS2_SUS_P          (1 << 2)
#define REGISTER_FS1_PROTECTION     (1 << 1)
#define REGISTER_FS0_ADS            (1 << 0)


#define REGISTER_SPI_WITH_DQS       (0xFF)
#define REGISTER_SPI_WO_DQS         (0xDF)
#define REGISTER_DTR_WITH_DQS       (0xE7)
#define REGISTER_DTR_WO_DQS         (0xC7)
#define REGISTER_STR_WITH_DQS       (0xB7)
#define REGISTER_STR_WO_DQS         (0x97)


void __OSPI_CS_Release(void);

void __OSPI_CS_Select(void);

void __OSPI_SET_1X_MODE(void);

void __OSPI_SET_2X_MODE(void);

void __OSPI_SET_4X_MODE(void);

void __OSPI_SET_8X_MODE(void);

void __OSPI_SET_8X_STR_MODE(void);

void __OSPI_SET_8X_DTR_MODE(void);

void __OSPI_SET_FIFO_MODE(uint32_t mode);

void __OSPI_Read_Data(uint8_t *pData, uint32_t size);

void __OSPI_Write_Data(uint8_t *pData, uint32_t size);

void NORFLASH_WriteEnable(void);

void NORFLASH_WriteDisable(void);

void NORFLASH_WaitNotBusy(void);

void INORFLASH_WriteRegister(uint8_t fu8_Register_S7_S0, uint8_t fu8_Register_S15_S08);

uint32_t NORFLASH_Read_Identification(void);

void NORFLASHx_Read_Unique_ID(uint8_t *uid);

uint8_t NORFLASH_Read_Register(uint8_t cmd);

uint8_t NORFLASH_Read_RegisterS15_S08(void);

void NORFLASH_Read_Data(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void INORFLASH_Read_Dual_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Dual_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Quad_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Quad_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Dual(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Quad(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_EraseSector(uint32_t fu32_DataAddress);

void NORFLASH_EraseChip(void);

void NORFLASH_QuadConfig(bool fb_Config);

void NORFLASH_WaitNotInProgress(void);

void NORFLASH_Reset(void);

void NORFLASH_PowerDown(void);

void NORFLASH_Wakeup(void);

void NORFLASH_Read_Unique_ID(uint8_t *uid);

void NorFlash_Program(uint8_t *buff, uint32_t addr, uint32_t len);

void NorFlash_Read(uint8_t *buff, uint32_t addr, uint32_t len);

uint16_t NORFLASH_Read_Manufacture_Device_ID(void);

void NORFLASH_WriteNonVolatile_VolatileStatusRegister(uint8_t cmd, uint8_t regAddr, uint8_t data);

void NORFLASH_Read_Octal_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Octal_Output_STR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Octal_Output_DTR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Octal_IO_STR(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_DTR_Read_Octal_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_DTR_Read_Octal_IO_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Octal(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Extend_Octal(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Extend_Octal_STR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_PageProgram_Extend_Octal_DTR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_WriteEnableFourByteMode(void);

void NORFLASH_WriteDisableFourByteMode(void);

uint8_t NORFLASH_ReadNonVolatile_VolatileStatusRegister(uint8_t cmd, uint8_t regAddr);

void NORFLASH_Reset8xMode(void);

void NORFLASH_Read_Octal_IO_DTR(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Octal_IO_FourByte(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

uint32_t NORFLASH_Read_Identification_x8STR(void);

uint32_t NORFLASH_Read_Identification_x8DTR(void);

uint16_t NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR(uint8_t cmd, uint8_t regAddr);

uint8_t NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8STR(uint8_t cmd, uint8_t regAddr);

void NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8DTR(uint8_t cmd, uint8_t regAddr, uint8_t data);

void NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8STR(uint8_t cmd, uint8_t regAddr, uint8_t data);

void NORFLASH_Reset_8xMode(void);

void NORFLASH_Read_Octal_IO_DTR_OPI(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_Read_Octal_IO_STR_OPI(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);


uint8_t NORFLASH_ReadNonVolatile_VolatileStatusRegister_x8DTR_1(uint8_t cmd, uint8_t regAddr);

void NORFLASH_WriteNonVolatile_VolatileStatusRegister_FourAddr(uint8_t cmd, uint8_t regAddr, uint8_t data);

void NORFLASH_WriteEnable_DTR_OPI(void);

void NORFLASH_WriteDisable_DTR_OPI(void);

void NORFLASH_WriteNonVolatile_VolatileStatusRegister_x8DTR_1(uint8_t cmd, uint8_t regAddr, uint8_t data);

void NORFLASH_WriteEnableFourByteMode_DTR_OPI(void);

void NORFLASH_WriteDisableFourByteMode_DTR_OPI(void);

uint8_t NORFLASH_Read_Register_DTR_OPI(uint8_t cmd);

void NORFLASH_WaitNotInProgress_DTR_OPI(void);

void NORFLASH_PageProgram_Octal_OPI(uint16_t *pu16_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length);

void NORFLASH_WriteEnable_STR_OPI(void);

void NORFLASH_WriteDisable_STR_OPI(void);

void NORFLASH_WriteEnableFourByteMode_STR_OPI(void);

uint8_t NORFLASH_Read_Register_STR_OPI(uint8_t cmd);

void NORFLASH_WaitNotInProgress_STR_OPI(void);

void NORFLASH_ReadSerialFlashDiscoverableParameter(uint32_t addr, uint8_t *pData, uint8_t len);

void NORFLASH_ReadSerialFlashDiscoverableParameter_32BitAddr(uint32_t addr, uint8_t *pData, uint8_t len);


#endif



