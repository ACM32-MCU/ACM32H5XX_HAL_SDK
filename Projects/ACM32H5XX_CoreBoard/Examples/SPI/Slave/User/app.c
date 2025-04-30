/******************************************************************************
*@file  : app.c
*@brief : SPI Slave Demos 
******************************************************************************/
#include  "app.h"


#define BUFFER_LENGTH    (256)

uint8_t TxBuffer[BUFFER_LENGTH];
uint8_t RxBuffer[BUFFER_LENGTH];

SPI_HandleTypeDef SPI_Handle;

void printf_buff_byte(uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i = 0; i < length; i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n");
}

void SPI_Init(void)
{
    SPI_Handle.Instance                 = SPI2;
    SPI_Handle.Init.SPI_Mode            = SPI_MODE_SLAVE;
    SPI_Handle.Init.SPI_Work_Mode       = SPI_WORK_MODE_0;
    SPI_Handle.Init.X_Mode              = SPI_1X_MODE;
    SPI_Handle.Init.First_Bit           = SPI_FIRSTBIT_MSB;
    SPI_Handle.Init.Slave_SofteCs_En    = SPI_SLAVE_SOFT_CS_DISABLE;
    
    SPI_Handle.CSx = SPI_CS_CS0;
    
    HAL_SPI_Init(&SPI_Handle);
}

void SPI_Slave_Comm_Test(void)
{
    uint32_t i;
    
    printfS("SPI Slave Comm Demo. (Plesae connect to another board running SPI Master Comm Demo. Slave should run first)\r\n\r\n");

    SPI_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    printfS("SPI slave is running......\r\n");
    
    while (1) 
    {
        HAL_SPI_Receive(&SPI_Handle, RxBuffer,  sizeof(RxBuffer),10000);
        while(HAL_SPI_GetRxState(&SPI_Handle) != SPI_RX_STATE_IDLE);
        if(SPI_Handle.Rx_Count)
        {
            printf_buff_byte(RxBuffer, SPI_Handle.Rx_Count);
            HAL_SPI_Transmit(&SPI_Handle, RxBuffer, SPI_Handle.Rx_Count, 0);
            
            printf("Rx_Count = %d. Tx_Count = %d.\r\n", SPI_Handle.Rx_Count, SPI_Handle.Tx_Count);
        }
        for (i = 0; i < 50; i++)
        {
            RxBuffer[i] = 0;
        }
    }
}

void SPI_Slave_Comm_IT(void)
{
    uint32_t i;
    
    printfS("SPI Slave Comm IT Demo. (Please connect to another board running SPI Master Comm Demo. Slave should run first)\r\n\r\n");

    SPI_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    printfS("SPI slave is running......\r\n");
    
    while (1) 
    {
        HAL_SPI_Receive_IT(&SPI_Handle, RxBuffer, sizeof(RxBuffer));
        CLEAR_BIT(SPI_Handle.Instance->CTL, SPI_CTL_SWCS);   //Soft CS set to LOW.
        while(HAL_SPI_GetRxState(&SPI_Handle) != SPI_RX_STATE_IDLE);
        SET_BIT(SPI_Handle.Instance->CTL, SPI_CTL_SWCS);   //Soft CS set to HIGH.

        HAL_SPI_Transmit_IT(&SPI_Handle, RxBuffer, SPI_Handle.Rx_Count);
        CLEAR_BIT(SPI_Handle.Instance->CTL, SPI_CTL_SWCS);   //Soft CS set to LOW.
        while(HAL_SPI_GetTxState(&SPI_Handle) != SPI_TX_STATE_IDLE);
        SET_BIT(SPI_Handle.Instance->CTL, SPI_CTL_SWCS);   //Soft CS set to HIGH.
        
        printf("Rx_Count = %d. Tx_Count = %d.\r\n", SPI_Handle.Rx_Count, SPI_Handle.Tx_Count);
        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            RxBuffer[i] = 0;
        }
    }
}

/************************For SPI Master DMA Test**************************/ 
#ifdef HAL_DMA_MODULE_ENABLED
DMA_HandleTypeDef DMA_SPIR_Handle;
DMA_HandleTypeDef DMA_SPIT_Handle;

void DMA_SPITransmit_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    DMA_SPIT_Handle.Instance            = DMA1_Channel1;
    DMA_SPIT_Handle.Init.DataFlow       = DMA_DATAFLOW_M2P;
    DMA_SPIT_Handle.Init.ReqID          = DMA1_REQ_SPI2_TX;
    DMA_SPIT_Handle.Init.SrcIncDec      = DMA_SRCINCDEC_INC;    
    DMA_SPIT_Handle.Init.DestIncDec     = DMA_DESTINCDEC_DISABLE;
    DMA_SPIT_Handle.Init.SrcWidth       = DMA_SRCWIDTH_BYTE;
    DMA_SPIT_Handle.Init.DestWidth      = DMA_DESTWIDTH_BYTE;

    /*-----------------------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set DMA_ITC_Callback?¡éDMA_IE_Callback NULL */
    /*-----------------------------------------------------------------------------------*/
    DMA_SPIT_Handle.XferCpltCallback = NULL;
    DMA_SPIT_Handle.XferErrorCallback  = NULL;

    HAL_DMA_Init(&DMA_SPIT_Handle);
    
    SPI_Handle.HDMA_Tx = &DMA_SPIT_Handle;
}

void DMA_SPIReceive_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    DMA_SPIR_Handle.Instance            = DMA2_Channel2;
    DMA_SPIR_Handle.Init.DataFlow       = DMA_DATAFLOW_P2M;
    DMA_SPIR_Handle.Init.ReqID          = DMA2_REQ_SPI2_RX;
    DMA_SPIR_Handle.Init.SrcIncDec      = DMA_SRCINCDEC_DISABLE;
    DMA_SPIR_Handle.Init.DestIncDec     = DMA_DESTINCDEC_INC;
    DMA_SPIR_Handle.Init.SrcWidth       = DMA_SRCWIDTH_BYTE;
    DMA_SPIR_Handle.Init.DestWidth      = DMA_DESTWIDTH_BYTE;

    /*-----------------------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set DMA_ITC_Callback, DMA_IE_Callback NULL */
    /*-----------------------------------------------------------------------------------*/
    DMA_SPIR_Handle.XferCpltCallback = NULL;
    DMA_SPIR_Handle.XferErrorCallback  = NULL;

    HAL_DMA_Init(&DMA_SPIR_Handle);
    
    SPI_Handle.HDMA_Rx = &DMA_SPIR_Handle;
}

void SPI_Slave_DMA_Test(void)
{
    uint32_t i;
    
    uint32_t DataLength = 256;
    
    printfS("SPI Slave Comm DMA Demo. (Please connect to another board running SPI Master Comm DMA Demo. Slave should run first)\r\n\r\n");
    
    SPI_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
//    for (i = 0; i < DataLength; i++)
//    {
//        TxBuffer[i] = 1 + i;
//    }
    printfS("SPI slave is running......\r\n");

    DMA_SPITransmit_Init();
    DMA_SPIReceive_Init();

    while (1) 
    {
        HAL_SPI_Receive_DMA(&SPI_Handle, RxBuffer, DataLength);
        while(HAL_SPI_GetRxState(&SPI_Handle) != SPI_RX_STATE_IDLE);
        printf_buff_byte(RxBuffer, DataLength);
        HAL_SPI_Transmit_DMA(&SPI_Handle, RxBuffer, DataLength);    //HAL_SPI_Transmit_DMA(&SPI_Handle, TxBuffer, DataLength);
        while(HAL_SPI_GetTxState(&SPI_Handle) != SPI_TX_STATE_IDLE);
        
        for (i = 0; i < BUFFER_LENGTH;  i++)
        {
            RxBuffer[i] = 0;
        }
    }  
}
#endif

void spi_slave_none_batch_commucation_test(void)
{
    uint32_t i;
    
    uint32_t COM_OK  = 0;
    uint32_t COM_Err = 0;
    uint32_t DataLength = 128;
    
    printfS("SPI Slave Comm Demo. \r\n\r\n");

    //H5µÄSPI3ºÍ²âÊÔ×Ó°åÉÏµÄG103µÄSPI1ÏàÁ¬
    SPI_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    printfS("SPI slave is running......\r\n");
    
    while(1)
    {
        HAL_SPI_ReceiveNoneBatch(&SPI_Handle, RxBuffer,  DataLength, 0);
        while(HAL_SPI_GetRxState(&SPI_Handle) != SPI_RX_STATE_IDLE);
        if(SPI_Handle.Rx_Count)
        {
            printf_buff_byte(RxBuffer, SPI_Handle.Rx_Count);
            HAL_SPI_TransmitNoneBatch(&SPI_Handle, RxBuffer, SPI_Handle.Rx_Count, 0);
            
            printf("Rx_Count = %d. Tx_Count = %d.\r\n", SPI_Handle.Rx_Count, SPI_Handle.Tx_Count);
        }
        for (i = 0; i < DataLength; i++)
        {
            RxBuffer[i] = 0;
        }
    }
}

void SPI_Slave_Test(enum_TEST_MODE_t fe_Mode)
{        
    switch (fe_Mode)
    {
        case TEST_SLAVE_COMM: 
            SPI_Slave_Comm_Test();
            break; 
        
        case TEST_SLAVE_IT:
            SPI_Slave_Comm_IT();
            break; 
        
#ifdef HAL_DMA_MODULE_ENABLED        
        case TEST_SLAVE_DMA: 
            SPI_Slave_DMA_Test();
            break;
#endif        
       
        default: break;     
    }
}
