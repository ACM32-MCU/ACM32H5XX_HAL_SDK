/******************************************************************************
*@file  : IC_W25Qxx.c
*@brief : W25Qxx IC driver.
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "ic_w25qxx.h"

static SPI_HandleTypeDef SPI_Handle_Nor;

/******************************************************************************
*@brief : return the hspi handle for handling NorFlash
*         
*@param : None
*@return: SPI_HandleTypeDef
******************************************************************************/
SPI_HandleTypeDef *SPI_NorFlash_Gethspi(void)
{
    return &SPI_Handle_Nor;
}

/******************************************************************************
*@brief : Configure SPI Memory map mode 
*         
*@param : hspi: a pointer of SPI_HandleTypeDef structure which contains 
*         the configuration information for the specified SPI.
*@param : MemACCXMode: Xmode. the value can be SPI_1X_MODE, SPI_2X_MODE, SPI_4X_MODE.
*@return: None
******************************************************************************/
static void SPI_NorFlash_MemAccInit(SPI_HandleTypeDef* hspi, uint32_t MemACCXMode)
{
    SPI_MemACCInitTypeDef MemAccInit;
    
    switch(MemACCXMode)
    {
        case SPI_1X_MODE:
        {
            MemAccInit.MemCMD.wr_cmd            = 0;
            MemAccInit.MemCMD.rd_cmd            = 0x0B;
            
            MemAccInit.MemAlterByte             = 0;
            
            MemAccInit.MemACC.data_mode         = DATA_MODE_x1;         //1x mode
            MemAccInit.MemACC.alter_byte_mode   = ALTER_BYTE_MODE_x1;   //交替字节模式			(1线)
            MemAccInit.MemACC.addr_mode         = ADDR_MODE_x1;         //地址模式 1x 2x 4x		(1线)
            MemAccInit.MemACC.instr_mode        = CMD_MODE_x1;          //指令模式 1x 2x 4x		(1线)
            MemAccInit.MemACC.addr_size         = ADDR_SIZE_24;         //地址长度				(3bytes)
            MemAccInit.MemACC.dummy_cycle       = DUMMY_CYCLE_8;        //dummy cycles

            MemAccInit.MemACC.rd_db_en          = 1;                    //dummy cycles for reading
            MemAccInit.MemACC.wr_db_en          = 0;                    //dummy cycles for writing
            MemAccInit.MemACC.alter_byte_size   = 0;                    //交替字节长度:			(1byte)
            MemAccInit.MemACC.rd_ab_en          = 0;                    //读操作交替字节使能位
            MemAccInit.MemACC.wr_ab_en          = 0;                    //写操作交替字节使能位
            MemAccInit.MemACC.instr_once        = 0;                    //仅发送指令一次
            MemAccInit.MemACC.crm_en            = 1;                    //连续读使能
            MemAccInit.MemACC.cs_tout_en        = 1;                    //CS拉低超时时间
            MemAccInit.MemCSTimeout             = 200;//448; 
        }
        break;
        
        case SPI_2X_MODE:
            MemAccInit.MemCMD.wr_cmd            = 0;
            MemAccInit.MemCMD.rd_cmd            = 0xBB;    //Dual I/O Fast Read
            
            MemAccInit.MemAlterByte             = 0xA0;
            
            MemAccInit.MemACC.data_mode         = DATA_MODE_x2;			//2x mode
            MemAccInit.MemACC.alter_byte_mode   = ALTER_BYTE_MODE_x2; 	//交替字节模式	(2线)
            MemAccInit.MemACC.addr_mode         = ADDR_MODE_x2;			//地址模式      (2线)
            MemAccInit.MemACC.instr_mode        = CMD_MODE_x1;			//指令模式      (1线)
            MemAccInit.MemACC.addr_size         = ADDR_SIZE_24;			//地址长度		(3bytes)
            MemAccInit.MemACC.dummy_cycle       = 0;		            //dummy cycles

            MemAccInit.MemACC.rd_db_en          = 0;					//no dummy cycles for reading
            MemAccInit.MemACC.wr_db_en          = 0;					//no dummy cycles for writing
            MemAccInit.MemACC.alter_byte_size   = ALTER_BYTE_SIZE_8;	//交替字节长度:(1byte)
            MemAccInit.MemACC.rd_ab_en          = 1;					//读操作交替字节使能位
            MemAccInit.MemACC.wr_ab_en          = 0;					//写操作交替字节使能位
            MemAccInit.MemACC.instr_once        = 1;					//仅发送指令一次
            MemAccInit.MemACC.crm_en            = 1;				    //CS空闲不拉低
            MemAccInit.MemACC.cs_tout_en        = 1;                    //CS拉低超时时间
            MemAccInit.MemCSTimeout             = 200;//448; 
            break;
        
        case SPI_4X_MODE:
        {
            float period = 0;
            MemAccInit.MemCMD.wr_cmd            = 0x00;
            MemAccInit.MemCMD.rd_cmd            = 0xEB;
            MemAccInit.MemAlterByte             = 0xA0;

            MemAccInit.MemACC.data_mode         = DATA_MODE_x4;		    //4x mode
            MemAccInit.MemACC.alter_byte_mode   = ALTER_BYTE_MODE_x4; 	//交替字节模式			(4线)
            MemAccInit.MemACC.addr_mode         = ADDR_MODE_x4;			//地址模式 1x 2x 4x		(4线)
            MemAccInit.MemACC.instr_mode        = CMD_MODE_x1;			//指令模式 1x 2x 4x		(1线)
            MemAccInit.MemACC.addr_size         = ADDR_SIZE_24;			//地址长度				(3bytes)
            MemAccInit.MemACC.dummy_cycle       = DUMMY_CYCLE_4;		//dummy cycles
            MemAccInit.MemACC.rd_db_en          = 1;				    //Enable read dummy cycles
            MemAccInit.MemACC.wr_db_en          = 0;					//Disable write dummy cycles
            MemAccInit.MemACC.alter_byte_size   = ALTER_BYTE_SIZE_8;	//交替字节长度:			(1byte)
            MemAccInit.MemACC.rd_ab_en          = 1;					//读操作交替字节使能位
            MemAccInit.MemACC.wr_ab_en          = 0;					//写操作交替字节使能位
            MemAccInit.MemACC.instr_once        = 1;					//仅发送指令一次
            MemAccInit.MemACC.crm_en            = 1;					//连续读使能
            MemAccInit.MemACC.cs_tout_en        = 1;
            MemAccInit.MemCSTimeout             = 200;//448;            //CS拉低的超时时间，CS超时后拉高只在crm_en = 1有用,因为crm_en = 1时 CS才会一直拉低
                                                                        //否则CS在当前操作完就拉高了
            
        }
        break;
        
        default:break;
    }
    
    HAL_SPI_MEMACCInit(hspi, &MemAccInit);
}

/******************************************************************************
*@brief : SPI initialization for handling SPI NorFlash. 
*         
*@param : SPIx: Specify which SPI MEM instance is used.
*@param : clk_div: SPI clk division
*@return: HAL_SPI_NORFLASH_Status
******************************************************************************/
HAL_SPI_NORFLASH_Status SPI_NorFlash_Init(SPI_TypeDef* SPIx, uint32_t clk_div)
{
    GPIO_InitTypeDef	gpio_cfg = {0};
    
    assert_param(IS_SPI_MEM_INSTANCE(SPIx));

	SPI_Handle_Nor.Instance				    = SPIx;
	SPI_Handle_Nor.Init.SPI_Mode		    = SPI_MODE_MASTER;
	SPI_Handle_Nor.Init.SPI_Work_Mode	    = SPI_WORK_MODE_0;
	SPI_Handle_Nor.Init.X_Mode 			    = SPI_4X_MODE;
	SPI_Handle_Nor.Init.First_Bit		    = SPI_FIRSTBIT_MSB;
	SPI_Handle_Nor.Init.BaudRate_Prescaler  = clk_div;
    SPI_Handle_Nor.Init.Master_SShift       = (clk_div == SPI_BAUDRATE_PRESCALER_2 ) ? \
                                                SPI_MASTER_SSHIFT_1_HCLK : SPI_MASTER_SSHIFT_NONE;

    SPI_Handle_Nor.CSx                      = SPI_CS_CS0;

	HAL_SPI_Init(&SPI_Handle_Nor);    

    SPI_NorFlash_MemAccInit(&SPI_Handle_Nor, SPI_4X_MODE);
    
    /* For those cmds used 1x mode in FIFO mode, such as readId, chip erase etc.*/
    __SPI_SET_1X_MODE();
       
    return ERR_NORFLASH_NONE;
}

/*********************************************************************************
* function    :  IC_W25Qxx_WriteEnable
* Description :  Write Enable
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteEnable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_ENABLE;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();    
}

/*********************************************************************************
* function   :  IC_W25Qxx_WriteDisable
* Description :  Write Disable
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteDisable(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = WRITE_DISABLE;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_WriteDisable
* Description :  Write Disable
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteRegister(uint8_t fu8_Register_S7_S0, uint8_t fu8_Register_S15_S08)
{
    uint8_t lu8_DataBuffer[3];

    lu8_DataBuffer[0] = WRITE_STATUS_REGISTER;
    lu8_DataBuffer[1] = fu8_Register_S7_S0;
    lu8_DataBuffer[2] = fu8_Register_S15_S08;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 3);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Write register End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_ID
* Description :  Read Manufacture ID and Device ID
* Input       :  
* Output      :  Manufacture ID and Device ID
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint16_t IC_W25Qxx_Read_ID(void)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = READ_DEVICE_ID;
    lu8_DataBuffer[1] = 0;
    lu8_DataBuffer[2] = 0;
    lu8_DataBuffer[3] = 0;
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 4);
    
    /* Recieve Manufacture ID and Device ID */
    __SPI_Read_Data(lu8_DataBuffer, 2);
    /* CS Realse */
    __SPI_CS_Release();
    
    return ((uint16_t)lu8_DataBuffer[0] << 8 | (uint16_t)lu8_DataBuffer[1]);
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_RegisterS07_S00
* Description :  Read Status Register S07 ~ S00
* Input       :  
* Output      :  Status Register S07 ~ S00
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint8_t IC_W25Qxx_Read_RegisterS07_S00(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = READ_STATUS_REGISTER_S07_S00;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 1);
    /* Recieve Status Register S07 ~ S00 */
    __SPI_Read_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
    
    return lu8_DataBuffer[0];
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_RegisterS15_S08
* Description :  Read Status Register S15 ~ S08
* Input       :  
* Output      :  Status Register S15 ~ S08
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint8_t IC_W25Qxx_Read_RegisterS15_S08(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = READ_STATUS_REGISTER_S15_S08;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 1);
    /* Recieve Status Register S15 ~ S08 */
    __SPI_Read_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
    
    return lu8_DataBuffer[0];
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Data
* Description :  Read Data
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Data(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = READ_DATA;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 4);
    /* Recieve Data */
    __SPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Dual_IO
* Description :  Dual Io Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Dual_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = DUAL_IO_FAST_READ;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 1);

    lu8_DataBuffer[0] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0xC8);

    /* Set SPI Work In 2 Wire Mode */
    __SPI_SET_2X_MODE();

    /* Send Address */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 4);
    /* Recieve Data */
    __SPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __SPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Dual_Output
* Description :  Dual Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Dual_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = DUAL_OUTPUT_FAST_READ;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0x00);
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 5);

    /* Set SPI Work In 2 Wire Mode */
    __SPI_SET_2X_MODE();

    /* Recieve Data */
    __SPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __SPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Quad_IO
* Description :  Quad Io Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Quad_IO(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[6];

    lu8_DataBuffer[0] = QUAD_IO_FAST_READ;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 1);

    lu8_DataBuffer[0] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[3] = (0x00);

    lu8_DataBuffer[4] = (0x00);
    lu8_DataBuffer[5] = (0x00);

    /* Set SPI Work In 4 Wire Mode */
    __SPI_SET_4X_MODE();

    /* Send Address */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 6);
    /* Recieve Data */
    __SPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __SPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Quad_Output
* Description :  Quad Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Quad_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[6];

    lu8_DataBuffer[0] = QUAD_OUTPUT_FAST_READ;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);
    lu8_DataBuffer[4] = (0x00);
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 5);

    /* Set SPI Work In 4 Wire Mode */
    __SPI_SET_4X_MODE();

    /* Recieve Data */
    __SPI_Read_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __SPI_SET_1X_MODE();
}

/*********************************************************************************
* function   :  IC_W25Qxx_PageProgram
* Description :  Page Program
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PageProgram(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = PAGE_PROGARM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    IC_W25Qxx_WriteEnable();
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 4);
    /* Send Data */
    __SPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();
    
    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_PageProgram_Quad
* Description :  Quad Page Program 
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PageProgram_Quad(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = QUAD_PAGE_PROGRAM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    IC_W25Qxx_WriteEnable();
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data_KeepCS(lu8_DataBuffer, 4);
//    HAL_SPI_Transmit_IT_KeepCS(&SPI_Handle_Nor, lu8_DataBuffer, 4);
//    HAL_SPI_WaitTxTimeout(&SPI_Handle_Nor, 0);

    /* Set SPI Work In 4 Wire Mode */
    __SPI_SET_4X_MODE();

    /* Send Data */
    __SPI_Write_Data(pu8_Buffer, fu32_Length);
//    HAL_SPI_Transmit_IT(&SPI_Handle_Nor, pu8_Buffer, fu32_Length);
//    HAL_SPI_WaitTxTimeout(&SPI_Handle_Nor, 0);
    /* CS Realse */
    __SPI_CS_Release();

    /* Set SPI Work In 1 Wire Mode */
    __SPI_SET_1X_MODE();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_EraseSector
* Description :  Erease The specific Sector
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_EraseSector(uint32_t fu32_DataAddress)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = SECTOR_ERASE;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    IC_W25Qxx_WriteEnable();
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 4);
    /* CS Realse */
    __SPI_CS_Release();
    
    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_EraseChip
* Description :  Erease The Whole Chip
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_EraseChip(void)
{
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = CHIP_ERASE;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_QuadConfig
* Description :  Quad Function Config
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_QuadConfig(bool fb_Config)
{
    if (fb_Config == true) 
    {
        uint8_t temp = 0;
        /* Set W25Qxx Quad Enable */
        IC_W25Qxx_WriteRegister(REGISTER_NULL, REGISTER_S15_S08_QE);
        temp = IC_W25Qxx_Read_RegisterS15_S08();
        if(temp & 0x2)
            printfS("QE bit is true\r\n");
    }
    else 
    {
        /* Set W25Qxx Quad Disable */
        IC_W25Qxx_WriteRegister(REGISTER_NULL, REGISTER_NULL);
    }
}

/*********************************************************************************
* function   :  IC_W25Qxx_WaitBusy
* Description :  Wait IC Not Busy
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WaitBusy(void)
{   
    /* Wait IC Not Busy */
    while(IC_W25Qxx_Read_RegisterS07_S00() & REGISTER_S07_S00_WIP);
}

/*********************************************************************************
* function   :  IC_W25Qxx_PowerDown
* Description :  
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PowerDown(void)
{ 
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = DEEP_POWER_DOWN;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Wakeup
* Description :  
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Wakeup(void)
{  
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = RELEASE_FORM_DEEP_POWER_DOWN;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Reset
* Description :  W25Qxx Reset
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Reset(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = ENABLE_RESET;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();


    lu8_DataBuffer[0] = RESET;
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

void IC_W25Qxx_CRMReset(void)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = 0xFF;
    lu8_DataBuffer[1] = 0xFF;
    lu8_DataBuffer[2] = 0xFF;
    lu8_DataBuffer[3] = 0xFF;
    
    __SPI_SET_4X_MODE();
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 4);
    /* CS Realse */
    __SPI_CS_Release();
    
    __SPI_SET_1X_MODE();
    HAL_DelayUs(1);
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 4);
    /* CS Realse */
    __SPI_CS_Release();
}

