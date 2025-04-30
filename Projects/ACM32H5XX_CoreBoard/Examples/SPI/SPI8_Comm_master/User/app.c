/******************************************************************************
*@file  : app.c
*@brief : SPI Master Demos 
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
    SPI_Handle.Instance                 = SPI8;
    SPI_Handle.Init.SPI_Mode            = SPI_MODE_MASTER;
    SPI_Handle.Init.SPI_Work_Mode       = SPI_WORK_MODE_0;
    SPI_Handle.Init.X_Mode              = SPI_4X_MODE;
    SPI_Handle.Init.First_Bit           = SPI_FIRSTBIT_MSB;    
    SPI_Handle.Init.BaudRate_Prescaler  = SPI_BAUDRATE_PRESCALER_64;
    SPI_Handle.Init.Master_SShift       = SPI_MASTER_SSHIFT_NONE; //SPI_MASTER_SSHIFT_NONE;   //SPI_MASTER_SSHIFT_1_HCLK;
        
    SPI_Handle.CSx                      = SPI_CS_CS0;

    HAL_SPI_Init(&SPI_Handle);
}

void SPI_Master_Comm_Test(void)
{
    uint32_t i;
    
    uint32_t COM_OK  = 0;
    uint32_t COM_Err = 0;
    uint32_t DataLength = 256;
    
    printfS("SPI Master Comm Demo. (should connect to another board running SPI Slave Comm Demo, & Slave should run first)\r\n\r\n");

    SPI_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    do
    {        
        for (i = 0; i < DataLength; i++)
        {
            TxBuffer[i] = 1 + i;
        }

        HAL_SPI_Transmit(&SPI_Handle, TxBuffer, DataLength, 0);        
        HAL_Delay(300);
        HAL_SPI_Receive(&SPI_Handle, RxBuffer, DataLength, 0);
        
        for (i = 0; i < DataLength; i++)
        {
            if (TxBuffer[i] != RxBuffer[i]) 
            {                
                COM_Err++;
                printfS("Error@ %x\n",i);
                goto end;
            }
            else 
            {
                COM_OK++;
            }
        }
        
        printfS("SPI Master Test OK count %d times \r\n",  COM_OK);
        printfS("SPI Master Test Err count %d times \r\n", COM_Err);

        COM_OK = 0;
        COM_Err = 0;
        
        HAL_Delay(300);
    }while(1);
end:
    printf_buff_byte(RxBuffer, DataLength);
    printfS("SPI Master Test End!!! \r\n");
}

#ifdef HAL_DMA_MODULE_ENABLED
/************************For SPI Master DMA Test**************************/ 
DMA_HandleTypeDef DMA_SPIR_Handle;
DMA_HandleTypeDef DMA_SPIT_Handle;

void DMA_SPITransmit_Init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    DMA_SPIT_Handle.Instance            = DMA1_Channel1;
    DMA_SPIT_Handle.Init.DataFlow       = DMA_DATAFLOW_M2P;
    DMA_SPIT_Handle.Init.ReqID          = DMA1_REQ_SPI8_TX;
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
    DMA_SPIR_Handle.Init.ReqID          = DMA2_REQ_SPI8_RX;
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

void SPI_Master_Comm_DMA_Test(void)
{  
    uint32_t i;

    uint32_t COM_OK  = 0;
    uint32_t COM_Err = 0;
    uint32_t DataLength = 256;
    
    printfS("SPI Master Comm DMA Demo(Please connect to another board running SPI Slave Comm Demo. Slave should run first)\r\n\r\n");

    SPI_Init();
    
    DMA_SPITransmit_Init();
    DMA_SPIReceive_Init();
    
    //printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
    //while(GPIO_PIN_SET == BSP_PB_GetState());
    
    do
    {        
        for (i = 0; i < DataLength; i++)
        {
            TxBuffer[i] = 1 + i;
        }

        HAL_SPI_Transmit_DMA(&SPI_Handle, TxBuffer, DataLength);
        while (HAL_SPI_GetTxState(&SPI_Handle) == SPI_TX_STATE_SENDING);
        HAL_Delay(300);

        HAL_SPI_Receive_DMA(&SPI_Handle, RxBuffer, DataLength);
        while (HAL_SPI_GetRxState(&SPI_Handle) == SPI_RX_STATE_RECEIVING);
        
        for (i = 0; i < DataLength; i++)
        {
            if (TxBuffer[i] != RxBuffer[i]) 
            {
                COM_Err++;
                printfS("Error@ %x\n",i);
                goto end2;
            }
            else 
            {
                COM_OK++;
            }
        }
        
        printfS("SPI Master DMA Test OK count %d times \r\n",  COM_OK);
        printfS("SPI Master DMA Test Err count %d times \r\n", COM_Err);
        
        COM_OK = 0;
        COM_Err = 0;

        HAL_Delay(100);
    }while(1);
end2:
    printf_buff_byte(RxBuffer, DataLength);
    printfS("SPI Master DMA Test End!!! \r\n");  
}
#endif

