#ifndef __NORFLASH_H__
#define __NORFLASH_H__

#include "app.h"




#define HAL_MAX_DELAY      0xFFFFFFFFU



#define BLOCKERASETIMEOUT    ((uint32_t)0x000A0000)
#define CHIPERASETIMEOUT     ((uint32_t)0x30000000) 
#define PROGRAMTIMEOUT       ((uint32_t)0x00001400)

#define CMD_ADDR_SHIFT              (0x00)
#define QUERY_ADDR_SHIFT            (0x01)
#define WRITE_READ_SHIFT            (0x00)

#define NOR_ADDR_SHIFT(ADDRESS,SHIFT)                (((FMC_BANK_NOR) + (ADDRESS << SHIFT)))
#define NOR_WRITE(ADDRESS, DATA)                     (*(__IO uint16_t *)((ADDRESS)) = (DATA))
#define NOR_READ_ADDR_SHIFT(ADDRESS,SHIFT)           (((FMC_BANK_NOR) + (ADDRESS << SHIFT)))

/* Private macro -------------------------------------------------------------*/

 
/* Mask on NOR STATUS REGISTER */

#define NOR_MASK_STATUS_DQ6                   (uint16_t)0x0040
#define NOR_MASK_STATUS_DQ7                   (uint16_t)0x0080

typedef enum
{
    FMC_NOR_STATUS_SUCCESS  = 0U,
    FMC_NOR_STATUS_ONGOING,
    FMC_NOR_STATUS_ERROR,
    FMC_NOR_STATUS_ERASE_ERROR,
    FMC_NOR_STATUS_PROGRAM_ERROR,
    FMC_NOR_STATUS_TIMEOUT
}FMC_NOR_StatusTypeDef;

typedef struct
{
    uint16_t Manufacturer_Code; 
    uint16_t Device_Code;        
    uint8_t CFI_Query_String[11]; 
    uint8_t Extended_Query_String[14];      
} NOR_IDTypeDef;



/* Constants to define address to set to write a command */
#define NOR_CMD_ADDRESS_FIRST                 (uint16_t)0x0555
#define NOR_CMD_ADDRESS_FIRST_CFI             (uint16_t)0x0055
#define NOR_CMD_ADDRESS_SECOND                (uint16_t)0x02AA
#define NOR_CMD_ADDRESS_THIRD                 (uint16_t)0x0555
#define NOR_CMD_ADDRESS_FOURTH                (uint16_t)0x0555
#define NOR_CMD_ADDRESS_FIFTH                 (uint16_t)0x02AA
#define NOR_CMD_ADDRESS_SIXTH                 (uint16_t)0x0555

/* Constants to define data to program a command */
#define NOR_CMD_DATA_READ_RESET               (uint16_t)0x00FF
#define NOR_CMD_DATA_READ_STAUTS              (uint16_t)0x0070  
#define NOR_CMD_DATA_FIRST                    (uint16_t)0x00AA
#define NOR_CMD_DATA_SECOND                   (uint16_t)0x0055

#define NOR_CMD_DATA_READ_INFO                (uint16_t)0x0090
#define NOR_CMD_DATA_PROGRAM                  (uint16_t)0x0040
#define NOR_CMD_DATA_PROGRAM_1                (uint16_t)0x0010
 

#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_THIRD   (uint16_t)0x0080
#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_FOURTH  (uint16_t)0x00AA
#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_FIFTH   (uint16_t)0x0055
#define NOR_CMD_DATA_CHIP_ERASE               (uint16_t)0x0010
#define NOR_CMD_DATA_CFI                      (uint16_t)0x0098

#define NOR_CMD_DATA_BUFFER_AND_PROG          (uint8_t)0x00E8
#define NOR_CMD_DATA_BUFFER_AND_PROG_CONFIRM  (uint8_t)0x00D0
#define NOR_CMD_DATA_BLOCK_ERASE              (uint8_t)0x0020
#define NOR_CMD_DATA_BLOCK_ERASE_CONFIRM      (uint8_t)0x00D0

/* Mask on NOR STATUS REGISTER */
#define NOR_MASK_STATUS_DQ1                   (uint16_t)0x0001
#define NOR_MASK_STATUS_DQ2                   (uint16_t)0x0002
#define NOR_MASK_STATUS_DQ3                   (uint16_t)0x0004
#define NOR_MASK_STATUS_DQ4                   (uint16_t)0x0008
#define NOR_MASK_STATUS_DQ5                   (uint16_t)0x0020
#define NOR_MASK_STATUS_DQ6                   (uint16_t)0x0040
#define NOR_MASK_STATUS_DQ7                   (uint16_t)0x0080

/* NOR device IDs addresses */
#define MANUFACTURER_CODE_ADDR                ((uint16_t)0x0000)
#define DEVICE_CODE_ADDR                      ((uint16_t)0x0001)

/* NOR CFI Query Identification String  addresses */
#define CFI_QUERY_STRING_ADDR                 ((uint16_t)0x0010)

/* Primary Vendor-Specific Extended Query addresses */
#define EXTENDED_QUERY_STRING_ADDR            ((uint16_t)0x0031)


/* NOR operation wait timeout */
#define NOR_TMEOUT                            ((uint16_t)0xFFFF)


                                      

void FMC_Norflash_Test(void);




#endif