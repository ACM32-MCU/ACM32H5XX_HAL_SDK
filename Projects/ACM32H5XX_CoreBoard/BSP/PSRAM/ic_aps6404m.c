#include "ic_aps6404m.h"

static psram_info_t psram_info;

/******************************************************************************
*@brief : Change SPI CS pin mode
*
*@param : cs_mode: REF@spi_cs_mode_enum
*@return: None
*@Note  : SPI CS pin is hardware controlled in XIP mode,
*         and software controlled in FIFO mode
******************************************************************************/
static void SPI_CS_Mode_Cfg(spi_cs_mode_enum cs_mode)
{
    GPIO_InitTypeDef    gpio_cfg = {0};

    if (cs_mode == SPI_CS_HW)
    {
        gpio_cfg.Pin       = psram_info.CS_Pin; //SPI_PSRAM_CS_PIN;
        gpio_cfg.Mode      = GPIO_MODE_AF_PP;
        gpio_cfg.Pull      = GPIO_PULLUP;
        gpio_cfg.Alternate = psram_info.CS_Pin_AF; //SPI_PSRAM_CS_FUNC;     //GPIO_FUNCTION_6;

        HAL_GPIO_Init(psram_info.CS_Port, &gpio_cfg);
        SPI_PSRAM_CS_H();
    }
    else if (cs_mode == SPI_CS_SW)
    {
        gpio_cfg.Pin       = psram_info.CS_Pin;
        gpio_cfg.Mode      = GPIO_MODE_OUTPUT_PP;
        gpio_cfg.Pull      = GPIO_PULLUP;
        gpio_cfg.Alternate = GPIO_FUNCTION_0;

        HAL_GPIO_Init(psram_info.CS_Port, &gpio_cfg);
        SPI_PSRAM_CS_H();
    }
}

static void SPI_Mem_inf_init_1x(SPI_TypeDef *SPIx)
{
    SPIx->CMD.wr_cmd = APS6404M_CMD_WR;
    SPIx->CMD.rd_cmd = APS6404M_CMD_FAST_RD;    //APS6404M_CMD_RD;
    SPIx->ALTER_BYTE = 0;

    SPIx->MEMO_ACC.data_mode = DATA_MODE_x1;                //1x mode
    SPIx->MEMO_ACC.alter_byte_mode = ALTER_BYTE_MODE_x1;    //交替字节模式          (1线)
    SPIx->MEMO_ACC.addr_mode = ADDR_MODE_x1;                //地址模式 1x 2x 4x     (1线)
    SPIx->MEMO_ACC.instr_mode = CMD_MODE_x1;                //指令模式 1x 2x 4x     (1线)
    SPIx->MEMO_ACC.addr_size = ADDR_SIZE_24;                //地址长度              (3bytes)
    SPIx->MEMO_ACC.dummy_cycle = DUMMY_CYCLE_8;             //dummy cycles
    /* if rd_cmd is APS6404M_CMD_FAST_RD, set rd_db_en = 1 */
    SPIx->MEMO_ACC.rd_db_en = 1;                            //dummy cycles for reading
    SPIx->MEMO_ACC.wr_db_en = 0;                            //dummy cycles for writing
    SPIx->MEMO_ACC.alter_byte_size = 0;                     //交替字节长度:         (1byte)
    SPIx->MEMO_ACC.rd_ab_en = 0;                            //读操作交替字节使能位
    SPIx->MEMO_ACC.wr_ab_en = 0;                            //写操作交替字节使能位
    SPIx->MEMO_ACC.instr_once = 0;                          //仅发送指令一次
    SPIx->MEMO_ACC.crm_en = 1;                              //连续读使能

//  SPIx->MEMO_ACC.acc_en = 1;                              //存储器访问使能
}

static void SPI_Mem_inf_init_4x(SPI_TypeDef *SPIx)
{
    SPIx->CMD.wr_cmd = APS6404M_CMD_QWR;
    SPIx->CMD.rd_cmd = APS6404M_CMD_FAST_QRD;
    SPIx->ALTER_BYTE = 0x00;

    SPIx->MEMO_ACC.data_mode = DATA_MODE_x4;                //4x mode
    SPIx->MEMO_ACC.alter_byte_mode = ALTER_BYTE_MODE_x4;    //交替字节模式          (4线)
    SPIx->MEMO_ACC.addr_mode = ADDR_MODE_x4;                //地址模式 1x 2x 4x     (4线)
    SPIx->MEMO_ACC.instr_mode = CMD_MODE_x1;                //指令模式 1x 2x 4x     (1线)
    SPIx->MEMO_ACC.addr_size = ADDR_SIZE_24;                //地址长度              (3bytes)
    SPIx->MEMO_ACC.dummy_cycle = DUMMY_CYCLE_6;             //dummy cycles
    SPIx->MEMO_ACC.rd_db_en = 1;                            //dummy cycles for reading
    SPIx->MEMO_ACC.wr_db_en = 0;                            //dummy cycles for writing
    SPIx->MEMO_ACC.alter_byte_size = 0;                     //交替字节长度:         (1byte)
    SPIx->MEMO_ACC.rd_ab_en = 0;                            //读操作交替字节使能位
    SPIx->MEMO_ACC.wr_ab_en = 0;                            //写操作交替字节使能位
    SPIx->MEMO_ACC.instr_once = 0;                          //仅发送指令一次
    SPIx->MEMO_ACC.crm_en = 1;                              //连续读使能

    //SPIx->MEMO_ACC.acc_en = 1;                                //存储器访问使能
}

__weak void SPI_PSRAM_CS_Pin_select(psram_info_t *info)
{
    printfS("Please Select CS Pin in app code\r\n");
    assert_param(0);
}

/******************************************************************************
*@brief : get psram information struct
*
*@param : None
*@return: psram_info_t
******************************************************************************/
psram_info_t *get_psram_info(void)
{
    return &psram_info;
}

#ifdef HAL_DMA_MODULE_ENABLED
/******************************************************************************
*@brief : Configure DMA channels used for PSRAM read and write
*
*@param : dma_rx_ch: DMA channel instance for PSRAM read
*@param : dma_tx_ch: DMA channel instance for PSRAM write
*@return: None
******************************************************************************/
void IC_APS6404M_DMA_RW_Cfg(DMA_Channel_TypeDef *dma_rx_ch, DMA_Channel_TypeDef *dma_tx_ch)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    psram_info.hDMARx.Instance              = dma_rx_ch;
    psram_info.hDMARx.Init.DataFlow         = DMA_DATA_FLOW_P2M;
    psram_info.hDMARx.Init.ReqID            = SPI_PSRAM_DMA_REQ_RX;
    psram_info.hDMARx.Init.SrcIncDec        = DMA_SRCINCDEC_DISABLE;
    psram_info.hDMARx.Init.DestIncDec       = DMA_DESTINCDEC_INC;
    psram_info.hDMARx.Init.SrcWidth         = DMA_SRC_WIDTH_BYTE;
    psram_info.hDMARx.Init.DestWidth        = DMA_DST_WIDTH_BYTE;

    /*-----------------------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set DMA_ITC_Callback, DMA_IE_Callback NULL */
    /*-----------------------------------------------------------------------------------*/
    psram_info.hDMARx.XferCpltCallback = NULL;
    psram_info.hDMARx.XferErrorCallback  = NULL;

    HAL_DMA_Init(&psram_info.hDMARx);

    __HAL_LINK_DMA(psram_info.hspi, HDMA_Rx, psram_info.hDMARx);



    psram_info.hDMATx.Instance              = dma_tx_ch;
    psram_info.hDMATx.Init.DataFlow         = DMA_DATA_FLOW_M2P;
    psram_info.hDMATx.Init.ReqID            = SPI_PSRAM_DMA_REQ_TX;
    psram_info.hDMATx.Init.SrcIncDec        = DMA_SRCINCDEC_INC;
    psram_info.hDMATx.Init.DestIncDec       = DMA_DESTINCDEC_DISABLE;
    psram_info.hDMATx.Init.SrcWidth         = DMA_SRC_WIDTH_BYTE;
    psram_info.hDMATx.Init.DestWidth        = DMA_DST_WIDTH_BYTE;

    /*-----------------------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set DMA_ITC_Callback, DMA_IE_Callback NULL */
    /*-----------------------------------------------------------------------------------*/
    psram_info.hDMARx.XferCpltCallback = NULL;
    psram_info.hDMARx.XferErrorCallback  = NULL;

    HAL_DMA_Init(&psram_info.hDMATx);

    __HAL_LINK_DMA(psram_info.hspi, HDMA_Tx, psram_info.hDMATx);


    psram_info.flag_dma_cfged = true;

//    NVIC_ClearPendingIRQ(DMA_IRQn);
//    NVIC_DisableIRQ(DMA_IRQn);

}
#endif
/******************************************************************************
*@brief : Initilise PSRAM r/w SPI interface
*
*@param : SPIx: SPI instance for PSRAM interace
*@param : dma_rx_ch: DMA channel instance for PSRAM read. set this param NULL when do Not use DMA
*@param : dma_tx_ch: DMA channel instance for PSRAM write. set this param NULL when do Not use DMA
*@return: HAL_SPI_PSRAM_Status
*@note  : IC_APS6404M_Init Initilized with XIP mode enabled.
          When call PSRAM FIFO mode read and write funcions, e.g. IC_APS6404M_Read()
          it will automatically first switched to FIFO mode,
          and then switched to XIP mode when FIFO funtions finished.
*
*         SPIx interrupt should be enalbed for DMA read/write.
*         e.g. set NVIC_EnableIRQ(SPI3_IRQn) in HAL_SPI_MspInit()
******************************************************************************/
#ifdef HAL_DMA_MODULE_ENABLED
    HAL_SPI_PSRAM_Status IC_APS6404M_Init(SPI_TypeDef *SPIx, DMA_Channel_TypeDef *dma_rx_ch, DMA_Channel_TypeDef *dma_tx_ch)
#else
    HAL_SPI_PSRAM_Status IC_APS6404M_Init(SPI_TypeDef *SPIx)
#endif
{
    GPIO_InitTypeDef    gpio_cfg = {0};

    psram_info.hspi.Instance                    = SPIx;
    psram_info.hspi.Init.SPI_Mode               = SPI_MODE_MASTER;
    psram_info.hspi.Init.SPI_Work_Mode          = SPI_WORK_MODE_0;
    psram_info.hspi.Init.X_Mode                 = SPI_4X_MODE;
    psram_info.hspi.Init.First_Bit              = SPI_FIRSTBIT_MSB;
    psram_info.hspi.Init.BaudRate_Prescaler     = SPI_BAUDRATE_PRESCALER_2;
    psram_info.hspi.Init.Master_SShift          = SPI_MASTER_SSHIFT_2_HCLK;//SPI_MASTER_SSHIFT_2_HCLK;//SPI_MASTER_SSHIFT_NONE;

    psram_info.hspi.CSx                          = SPI_CS_CS0;

    HAL_SPI_Init(&psram_info.hspi);

    SPI_PSRAM_CS_Pin_select(&psram_info);

    SPI_Mem_inf_init_4x(SPIx);

    __SPI_PSRAM_SET_1X_MODE();

    SPIx->MEMO_ACC.acc_en = 1;

    //IC_APS6404M_Reset();

    psram_info.mode = PSRAM_SPI_MODE;

#ifdef HAL_DMA_MODULE_ENABLED
    if ((dma_rx_ch == NULL) || (dma_tx_ch == NULL))
    {
        psram_info.flag_dma_cfged = false;
    }
    else
    {
        assert_param(IS_DMA_ALL_INSTANCE(dma_rx_ch));
        assert_param(IS_DMA_ALL_INSTANCE(dma_tx_ch));
        assert_param(dma_rx_ch != dma_tx_ch);
        IC_APS6404M_DMA_RW_Cfg(dma_rx_ch, dma_tx_ch);
        psram_info.flag_dma_cfged = true;
    }
#endif
    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : Read PSRAM id
*
*@param : eid[6]: The PSRAM eid will be stored in eid[]
*@return: PSRAM ID
******************************************************************************/
uint16_t IC_APS6404M_Read_ID(uint8_t eid[6])
{
    uint8_t tmp[8];

    uint32_t spi_baud_bak;

    tmp[0] = APS6404M_CMD_RD_ID;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;

    spi_baud_bak = psram_info.hspi.Instance->BAUD;
    psram_info.hspi.Instance->BAUD = 8; //read ID cmd is limit to 33MHz. for 200MHz sysclk, div by 8 is 25MHz

    __SWITCH_TO_FIFO_MODE();

    /* CS Select */
    SPI_PSRAM_CS_L();
    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 4);
    /* Recieve Manufacture ID and Device ID */
    __SPI_PSRAM_Read_Data(tmp, 8);
    /* CS Realse */
    SPI_PSRAM_CS_H();

    psram_info.hspi.Instance->BAUD = spi_baud_bak;

    __SWITCH_TO_XIP_MODE();

    if (eid != NULL)
    {
        uint8_t i;
        for (i = 0; i < 6; i++)
            eid[i] = tmp[2 + i];
    }

    return ((uint16_t)tmp[0] << 8 | (uint16_t)tmp[1]);
}

#if 0
/* 6404 have no mode register */
uint8_t IC_APS6404M_Read_ModeRegister(void)
{
    uint8_t tmp[8];

    tmp[0] = APS6404M_CMD_MOD_REG_RD;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    tmp[4] = 0;

    __SWITCH_TO_FIFO_MODE();

    /* CS Select */
    SPI_PSRAM_CS_L();
    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 5);
    /* Recieve Manufacture ID and Device ID */
    __SPI_PSRAM_Read_Data(tmp, 1);
    /* CS Realse */
    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return tmp[0];
}


uint8_t IC_APS6404M_Write_ModeRegister(uint8_t mr_val)
{
    uint8_t tmp[8];

    tmp[0] = APS6404M_CMD_MOD_REG_WR;
    tmp[1] = 0;
    tmp[2] = 0;
    tmp[3] = 0;
    tmp[4] = mr_val;

    __SWITCH_TO_FIFO_MODE();

    /* CS Select */
    SPI_PSRAM_CS_L();
    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 5);
    /* CS Realse */
    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return tmp[0];
}
#endif
/******************************************************************************
*@brief : PSRAM read 1x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
*@note  : The APS6404M_CMD_RD cmd may only support low clock spped, e.g. 33MHz, using IC_APS6404M_FastRead instead.
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_Read(uint32_t addr, void *buf,  uint32_t len)
{

    uint32_t spi_baud_bak;
    uint8_t tmp[4];

    tmp[0] = APS6404M_CMD_RD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    spi_baud_bak = psram_info.hspi.Instance->BAUD;
    psram_info.hspi.Instance->BAUD = 8; //read cmd is limit to 33MHz. for 200MHz sysclk, div by 8 is 25MHz

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 4);
    /* Recieve Data */
    __SPI_PSRAM_Read_Data(buf, len);

    SPI_PSRAM_CS_H();

    psram_info.hspi.Instance->BAUD = spi_baud_bak;

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM read 1x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_FastRead(uint32_t addr, void *buf,  uint32_t len)
{

    uint8_t tmp[5];

    tmp[0] = APS6404M_CMD_FAST_RD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);
    tmp[4] = 0; //wait cycles

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 5);
    /* Recieve Data */
    __SPI_PSRAM_Read_Data(buf, len);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM write 1x mode
*
*@param : addr: The PSRAM address for writing
*@param : buf:  Data buffer address pointer of writing data
*@param : len:  Data length to write
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_Write(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[4];

    tmp[0] = APS6404M_CMD_WR;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    /* Send command */
    __SPI_PSRAM_Write_Data(tmp, 4);
    /* Send Data */
    __SPI_PSRAM_Write_Data(buf, len);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM read 4x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_Read(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[8];

    tmp[0] = APS6404M_CMD_FAST_QRD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);
    tmp[4] = 0xFF;
    tmp[5] = 0xFF;
    tmp[6] = 0xFF;

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    tmp[0] = APS6404M_CMD_FAST_QRD;
    __SPI_PSRAM_Write_Data(tmp, 1);

    __SPI_PSRAM_SET_4X_MODE();

    __SPI_PSRAM_Write_Data(&tmp[1], 6);

    __SPI_PSRAM_Read_Data(buf, len);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();


    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM write 4x mode
*
*@param : addr: The PSRAM address for writing
*@param : buf:  Data buffer address pointer of writing data
*@param : len:  Data length to write
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_Write(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[8];

    tmp[0] = APS6404M_CMD_QWR;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 1);

    __SPI_PSRAM_SET_4X_MODE();

    __SPI_PSRAM_Write_Data(&tmp[1], 3);

    /* Recieve Data */
    __SPI_PSRAM_Write_Data(buf, len);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

#ifdef HAL_DMA_MODULE_ENABLED
/******************************************************************************
*@brief : PSRAM DMA read 1x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_DMA_Read(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[4];

    if (!psram_info.flag_dma_cfged)
        return ERR_PSRAM_DMANOTCFGED;

    tmp[0] = APS6404M_CMD_RD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 4);

    HAL_SPI_Receive_DMA(&psram_info.hspi, buf, len);
    while (HAL_SPI_GetRxState(&psram_info.hspi) == SPI_RX_STATE_RECEIVING);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM DMA read 1x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_DMA_FastRead(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[5];

    if (!psram_info.flag_dma_cfged)
        return ERR_PSRAM_DMANOTCFGED;

    tmp[0] = APS6404M_CMD_FAST_RD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);
    tmp[4] = 0; //wait cycles

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 5);

    HAL_SPI_Receive_DMA(&psram_info.hspi, buf, len);
    while (HAL_SPI_GetRxState(&psram_info.hspi) == SPI_RX_STATE_RECEIVING);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM DMA write 1x mode
*
*@param : addr: The PSRAM address for writing
*@param : buf:  Data buffer address pointer of writing data
*@param : len:  Data length to write
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_DMA_Write(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[4];

    if (!psram_info.flag_dma_cfged)
        return ERR_PSRAM_DMANOTCFGED;

    tmp[0] = APS6404M_CMD_WR;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 4);

    HAL_SPI_Transmit_DMA(&psram_info.hspi, buf, len);
    while (HAL_SPI_GetTxState(&psram_info.hspi) == SPI_TX_STATE_SENDING);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM DMA read 4x mode
*
*@param : addr: The PSRAM address for reading
*@param : buf:  Data buffer address pointer for storing PSRAM data
*@param : len:  Data length to read
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_DMA_Read(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[8];


    if (!psram_info.flag_dma_cfged)
        return ERR_PSRAM_DMANOTCFGED;

    tmp[0] = APS6404M_CMD_FAST_QRD;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);
    tmp[4] = 0xFF;
    tmp[5] = 0xFF;
    tmp[6] = 0xFF;

    __SWITCH_TO_FIFO_MODE();
    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 1);

    __SPI_PSRAM_SET_4X_MODE();

    __SPI_PSRAM_Write_Data(&tmp[1], 6);

    HAL_SPI_Receive_DMA(&psram_info.hspi, buf, len);
    while (HAL_SPI_GetRxState(&psram_info.hspi) == SPI_RX_STATE_RECEIVING);

    SPI_PSRAM_CS_H();
    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM DMA write 4x mode
*
*@param : addr: The PSRAM address for writing
*@param : buf:  Data buffer address pointer of writing data
*@param : len:  Data length to write
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_QPI_DMA_Write(uint32_t addr, void *buf,  uint32_t len)
{
    uint8_t tmp[8];

    if (!psram_info.flag_dma_cfged)
        return ERR_PSRAM_DMANOTCFGED;

    tmp[0] = APS6404M_CMD_QWR;
    tmp[1] = (uint8_t)(addr >> 16 & 0xFF);
    tmp[2] = (uint8_t)(addr >> 8  & 0xFF);
    tmp[3] = (uint8_t)(addr >> 0  & 0xFF);

    __SWITCH_TO_FIFO_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(tmp, 1);

    __SPI_PSRAM_SET_4X_MODE();

    __SPI_PSRAM_Write_Data(&tmp[1], 3);

    HAL_SPI_Transmit_DMA(&psram_info.hspi, buf, len);
    while (HAL_SPI_GetTxState(&psram_info.hspi) == SPI_TX_STATE_SENDING);

    SPI_PSRAM_CS_H();
    __SWITCH_TO_XIP_MODE();

    return ERR_PSRAM_NONE;
}
#endif
/******************************************************************************
*@brief : Set PSRAM in QPI mode
*
*@param : None
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_Enter_QPI_Mode(void)
{
    //0x35
    uint8_t tmp;

    tmp = APS6404M_CMD_ENTER_QPI_MODE;

    __SWITCH_TO_FIFO_MODE();

    __SPI_PSRAM_SET_1X_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(&tmp, 1);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    psram_info.mode = PSRAM_QPI_MODE;

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : PSRAM Exit QPI mode
*
*@param : None
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_Exit_QPI_Mode(void)
{
    //0xF5
    uint8_t tmp;

    tmp = APS6404M_CMD_EXIT_QPI_MODE;

    __SWITCH_TO_FIFO_MODE();

    __SPI_PSRAM_SET_4X_MODE();  //0902

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(&tmp, 1);

    SPI_PSRAM_CS_H();

    __SWITCH_TO_XIP_MODE();

    psram_info.mode = PSRAM_SPI_MODE;

    return ERR_PSRAM_NONE;
}

/******************************************************************************
*@brief : Reset PSRAM
*
*@param : None
*@return: HAL_SPI_PSRAM_Status
******************************************************************************/
HAL_SPI_PSRAM_Status IC_APS6404M_Reset(void)
{

    //0x35
    uint8_t tmp;
    uint32_t spi_baud_bak;

    spi_baud_bak = psram_info.hspi.Instance->BAUD;
    psram_info.hspi.Instance->BAUD = 8;

    tmp = APS6404M_CMD_RST_EN;

    __SWITCH_TO_FIFO_MODE();

    __SPI_PSRAM_SET_1X_MODE();

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(&tmp, 1);

    SPI_PSRAM_CS_H();

    tmp = APS6404M_CMD_RST;

    SPI_PSRAM_CS_L();

    __SPI_PSRAM_Write_Data(&tmp, 1);

    SPI_PSRAM_CS_H();

    psram_info.hspi.Instance->BAUD = spi_baud_bak;

    __SWITCH_TO_XIP_MODE();

    psram_info.mode = PSRAM_SPI_MODE;

    return ERR_PSRAM_NONE;
}