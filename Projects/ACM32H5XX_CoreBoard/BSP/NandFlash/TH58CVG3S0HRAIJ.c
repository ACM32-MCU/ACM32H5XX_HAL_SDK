/******************************************************************************
*@file  : IC_W25Qxx.c
*@brief : W25Qxx IC driver.
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "TH58CVG3S0HRAIJ.h"

static SPI_HandleTypeDef SPI_Handle_Nand;

/******************************************************************************
*@brief : return the hspi handle for handling NorFlash
*         
*@param : None
*@return: SPI_HandleTypeDef
******************************************************************************/
SPI_HandleTypeDef *SPI_NandFlash_Gethspi(void)
{
    return &SPI_Handle_Nand;
}

/******************************************************************************
*@brief : SPI initialization for handling SPI NorFlash. 
*         
*@param : SPIx: Specify which SPI MEM instance is used.
*@param : clk_div: SPI clk division
*@return: HAL_SPI_NandFLASH_Status
******************************************************************************/
HAL_SPI_NandFLASH_Status SPI_NandFlash_Init(SPI_TypeDef* SPIx, uint32_t clk_div)
{
    GPIO_InitTypeDef	gpio_cfg = {0};
    
    assert_param(IS_SPI_MEM_INSTANCE(SPIx));

	SPI_Handle_Nand.Instance				    = SPIx;
	SPI_Handle_Nand.Init.SPI_Mode		        = SPI_MODE_MASTER;
	SPI_Handle_Nand.Init.SPI_Work_Mode	        = SPI_WORK_MODE_0;
	SPI_Handle_Nand.Init.X_Mode 			    = SPI_4X_MODE;
	SPI_Handle_Nand.Init.First_Bit		        = SPI_FIRSTBIT_MSB;
	SPI_Handle_Nand.Init.BaudRate_Prescaler     = clk_div;
    SPI_Handle_Nand.Init.Master_SShift          = (clk_div == SPI_BAUDRATE_PRESCALER_2 ) ? \
                                                    SPI_MASTER_SSHIFT_1_HCLK : SPI_MASTER_SSHIFT_NONE;

    SPI_Handle_Nand.CSx                         = SPI_CS_CS0;

	HAL_SPI_Init(&SPI_Handle_Nand);    

    __SPI_SET_1X_MODE();
       
    return ERR_NORFLASH_NONE;
}

void ic_th58cvg3_write_enable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = NAND_WRITE_ENABLE;

    __SPI_Write_Data(lu8_DataBuffer, 1);
}

void Iic_th58cvg3_write_disable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = NAND_WRITE_DISABLE;

    __SPI_Write_Data(lu8_DataBuffer, 1);

}

uint16_t ic_th58cvg3_readid(void)
{
    uint8_t lu8_DataBuffer[4] = {0,};
    
    lu8_DataBuffer[0] = NAND_READ_ID;
    lu8_DataBuffer[1] = 0;

    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 2);
    
    __SPI_Read_Data(lu8_DataBuffer, 3);    
    
    return ((uint16_t)lu8_DataBuffer[0] << 8 | (uint16_t)lu8_DataBuffer[1]);
}

bool ic_th58cvg3_block_unlock(void)
{
    uint8_t tmp;
    tmp = ic_th58cvg3_get_feature(0xA0);
    
    tmp &= ~0x38;
    if(ic_th58cvg3_set_feature(0xA0, tmp))
        return true;
    else return false;        
}

bool ic_th58cvg3_erase_block(uint32_t row_addr)
{
    uint8_t tmp[4];
    uint8_t status = 0;
    
    tmp[0] = NAND_BLOCK_ERASE;
    tmp[1] = (uint8_t)(row_addr >> 16 & 0x03);
    tmp[2] = (uint8_t)(row_addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(row_addr >> 0  & 0xFF);

    ic_th58cvg3_write_enable();
    
    __SPI_Write_Data(tmp, 4);

    HAL_Delay(100);
    
    while((status = ic_th58cvg3_get_feature(0xC0)) & 1){};
    
    if(status & 0x4)
        return false;
    else return true;
}


bool ic_th58cvg3_program(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size)
{
    uint8_t tmp[4];
    uint8_t status = 0;
    
    tmp[0] = NAND_PROGRAM_LOAD_1x;
    tmp[1] = (uint8_t)(col_addr >> 8 & 0x1F);
    tmp[2] = (uint8_t)(col_addr >> 0  & 0xFF);


    ic_th58cvg3_write_enable();
    
    __SPI_Write_Data_KeepCS(tmp, 3);
    
    __SPI_Write_Data(buf, size);
    
    tmp[0] = NAND_PROGRAM_EXECUTE;
    tmp[1] = (uint8_t)(row_addr >> 16 & 0x03);
    tmp[2] = (uint8_t)(row_addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(row_addr >> 0  & 0xFF);
    
    __SPI_Write_Data(tmp, 4);
    
    while((status = ic_th58cvg3_get_feature(0xC0)) & 1){ HAL_Delay(1); };
    
    if(status & 0x8)
        return false;
    else return true;
}


bool ic_th58cvg3_enable_4x_mode(void)
{
    uint8_t tmp;

    tmp = ic_th58cvg3_get_feature(0xB0);
    
    tmp |= 1;
    
    ic_th58cvg3_write_enable();
    
    if(ic_th58cvg3_set_feature(0xB0, tmp))
        return true;
    else return false;    
}

bool ic_th58cvg3_disable_4x_mode(void)
{
    uint8_t tmp;
    
    tmp = ic_th58cvg3_get_feature(0xB0);
    
    tmp &= ~1;
    
    ic_th58cvg3_write_enable();
    
    if(ic_th58cvg3_set_feature(0xB0, tmp))
        return true;
    else return false;    
}

bool ic_th58cvg3_program_4x(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size)
{
    uint8_t tmp[4];
    uint8_t status = 0;
    
    tmp[0] = NAND_PROGRAM_LOAD_4x;
    tmp[1] = (uint8_t)(col_addr >> 8 & 0x1F);
    tmp[2] = (uint8_t)(col_addr >> 0  & 0xFF);


    ic_th58cvg3_write_enable();   

    __SPI_Write_Data_KeepCS(tmp, 3);
    
    __SPI_SET_4X_MODE();
    
    __SPI_Write_Data(buf, size);
    
    __SPI_SET_1X_MODE();
    
    tmp[0] = NAND_PROGRAM_EXECUTE;
    tmp[1] = (uint8_t)(row_addr >> 16 & 0x03);
    tmp[2] = (uint8_t)(row_addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(row_addr >> 0  & 0xFF);
    
    __SPI_Write_Data(tmp, 4);
    
    while((status = ic_th58cvg3_get_feature(0xC0)) & 1){ HAL_Delay(1); };
    
    if(status & 0x8)
        return false;
    else return true;
}


void ic_th58cvg3_read(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size)
{
    uint8_t tmp[4];
    uint8_t status = 0;
    
    tmp[0] = NAND_READ_CELL_ARRAY;
    tmp[1] = (uint8_t)(row_addr >> 16 & 0x03);
    tmp[2] = (uint8_t)(row_addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(row_addr >> 0  & 0xFF);
    __SPI_Write_Data(tmp, 4);
    
    while((status = ic_th58cvg3_get_feature(0xC0)) & 1){ HAL_Delay(1); };
    
    
    tmp[0] = NAND_READ_BUFFER_1x;
    tmp[1] = (uint8_t)(col_addr >> 8 & 0x1F);
    tmp[2] = (uint8_t)(col_addr >> 0  & 0xFF);
    tmp[3] = 0;//1 dummy byte
    
    __SPI_Write_Data_KeepCS(tmp, 4);
   
    __SPI_Read_Data(buf, size);
    
   
}

void ic_th58cvg3_read_4x(uint16_t row_addr, uint32_t col_addr, uint8_t *buf, uint32_t size)
{
    uint8_t tmp[4];
    uint8_t status = 0;
    
    tmp[0] = NAND_READ_CELL_ARRAY;
    tmp[1] = (uint8_t)(row_addr >> 16 & 0x03);
    tmp[2] = (uint8_t)(row_addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(row_addr >> 0  & 0xFF);
    __SPI_Write_Data(tmp, 4);
    
    while((status = ic_th58cvg3_get_feature(0xC0)) & 1){ HAL_Delay(1); };
    
    
    tmp[0] = NAND_READ_BUFFER_4x;
    tmp[1] = (uint8_t)(col_addr >> 8 & 0x1F);
    tmp[2] = (uint8_t)(col_addr >> 0  & 0xFF);
    tmp[3] = 0;//1 dummy byte
    
    __SPI_Write_Data_KeepCS(tmp, 4);
    
    __SPI_SET_4X_MODE();
   
    __SPI_Read_Data(buf, size);
    
   __SPI_SET_1X_MODE();
}

uint8_t ic_th58cvg3_get_feature(uint8_t feature_addr)
{
    uint8_t tmp[2];
    
    tmp[0] = NAND_GET_FEATURE;
    tmp[1] = feature_addr;
    
    __SPI_Write_Data_KeepCS(tmp, 2);
    
    __SPI_Read_Data(tmp, 1);

    return tmp[0];
}

bool ic_th58cvg3_set_feature(uint8_t feature_addr, uint8_t value)
{
    uint8_t rd_tmp;
    uint8_t tmp[3];
    
    tmp[0] = NAND_SET_FEATURE;
    tmp[1] = feature_addr;
    tmp[2] = value;
    
    __SPI_Write_Data(tmp, 3);
    
    rd_tmp = ic_th58cvg3_get_feature(feature_addr);
    
    if(rd_tmp == value)
        return true;
    else return false;
  
}

