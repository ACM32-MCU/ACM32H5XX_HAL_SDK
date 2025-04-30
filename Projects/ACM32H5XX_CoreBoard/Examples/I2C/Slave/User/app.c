/******************************************************************************
*@file  : app.c
*@brief : I2C Slave Demos 
******************************************************************************/

#include "app.h"


uint8_t TxBuffer[BUFFER_LENGTH];
uint8_t RxBuffer[BUFFER_LENGTH];

I2C_HandleTypeDef I2C_Handle;


/******************************************************************************
*@brief : Initiation I2C as Slave Mode
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Slave_Init(void)
{
    I2C_Handle.Instance             = I2C1;
    I2C_Handle.Init.I2C_Mode        = I2C_MODE_SLAVE;
    I2C_Handle.Init.Tx_Auto_En      = TX_AUTO_ENABLE;
    I2C_Handle.Init.Stretch_Mode    = STRETCH_MODE_DISABLE;
    I2C_Handle.Init.Own_Address     = SLAVE_ADDRESS;
    I2C_Handle.Init.Clock_Speed     = CLOCK_SPEED_STANDARD;
	    
    HAL_I2C_Init(&I2C_Handle);
}

/******************************************************************************
*@brief : I2C slave communication Demo in poll mode
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Slave_Polling_Test(void)
{
    uint32_t i;
    uint32_t lu32_RxLength = 0;
    uint32_t lu32_TxLength = 0;
    
    printfS("I2C Slave Comm Loop Demo. (should connect to another board running I2C Master Comm Demo, & Slave should run first)\r\n\r\n");
    
    I2C_Slave_Init();
    
//    printfS("Please press the USR_PB button to continue......\r\n\r\n");
//    
//    while(GPIO_PIN_SET == BSP_PB_GetState());
	
    printfS("I2C Slave is running......\r\n");
    
    while (1) 
    {
        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            TxBuffer[i] = 0;
            RxBuffer[i] = 0;
        }
        
        while(HAL_OK != HAL_I2C_Slave_Receive(&I2C_Handle, RxBuffer, BUFFER_LENGTH, 1000));

        for (i = 0; i < I2C_Handle.Rx_Count; i++)
        {
            TxBuffer[i] = RxBuffer[i];
        }

        HAL_I2C_Slave_Transmit(&I2C_Handle, TxBuffer, I2C_Handle.Rx_Count, 0);
        
        printfS("Rx Count: %d, Tx Count: %d \r\n", I2C_Handle.Rx_Count, I2C_Handle.Tx_Count);
        
        printfS("I2C Slave Test End (Polling Mode)!!! \r\n");
    }
}

/******************************************************************************
*@brief : I2C slave communication Demo in interrupt mode
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Slave_Interrupt_Test(void)
{
    uint32_t i;
    
    printfS("I2C Slave Comm IT Demo. (should connect to another board running I2C Master Comm Demo, & Slave should run first)\r\n\r\n");
    
    I2C_Slave_Init();
    
//    printfS("Please press the USR_PB button to continue......\r\n\r\n");
//    
//    while(GPIO_PIN_SET == BSP_PB_GetState());
    
    printfS("I2C Slave is running......\r\n");
    
    while (1) 
    {
        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            TxBuffer[i] = 0;
            RxBuffer[i] = 0;
        }

        HAL_I2C_Slave_Receive_IT(&I2C_Handle, RxBuffer, BUFFER_LENGTH);
        while (I2C_Handle.state != I2C_STATE_IDLE);
        
        for (i = 0; i < I2C_Handle.Rx_Count; i++)
        {
            TxBuffer[i] = RxBuffer[i];
        }

        HAL_I2C_Slave_Transmit_IT(&I2C_Handle, TxBuffer, I2C_Handle.Rx_Count);
        while (I2C_Handle.state != I2C_STATE_IDLE);

        printfS("Rx Count: %d, Tx Count: %d \r\n", I2C_Handle.Rx_Count, I2C_Handle.Tx_Count);

        printfS("I2C Slave Test End (Interrupt Mode)!!! \r\n");
    }
}

#ifdef HAL_DMA_MODULE_ENABLED
/**************************For I2C DMA Test******************************/
DMA_HandleTypeDef DMA_I2CR_Handle;

static volatile uint8_t RX_Complete;

void I2C_STOPF_Callback(I2C_HandleTypeDef* hi2c)
{
    RX_Complete = 1;
    
    I2C_Handle.Rx_Count = I2C_Handle.Rx_Size - (DMA_I2CR_Handle.Instance->CXCTRL & 0xFFF);
    
    HAL_DMA_Abort(&DMA_I2CR_Handle);
}

void I2C_DMA_Init(void)
{
    I2C_Handle.Instance             = I2C1;
    I2C_Handle.Init.I2C_Mode        = I2C_MODE_SLAVE;
    I2C_Handle.Init.Tx_Auto_En      = TX_AUTO_ENABLE;
    I2C_Handle.Init.Stretch_Mode    = STRETCH_MODE_DISABLE;
    I2C_Handle.Init.Own_Address     = SLAVE_ADDRESS;
    I2C_Handle.Init.Clock_Speed     = CLOCK_SPEED_STANDARD;

    /*--------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set I2C_STOPF_Callback NULL */
    /*--------------------------------------------------------------------*/
    I2C_Handle.I2C_STOPF_Callback = I2C_STOPF_Callback;

    HAL_I2C_Init(&I2C_Handle);
}

void DMA_I2CRecieve_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    memset((void*)&DMA_I2CR_Handle, 0, sizeof(DMA_I2CR_Handle));

    DMA_I2CR_Handle.Instance                = DMA1_Channel0;
    DMA_I2CR_Handle.Init.DataFlow           = DMA_DATAFLOW_P2M;
    DMA_I2CR_Handle.Init.ReqID              = DMA1_REQ_I2C1_RX;
    DMA_I2CR_Handle.Init.SrcIncDec          = DMA_SRCINCDEC_DISABLE;
    DMA_I2CR_Handle.Init.DestIncDec         = DMA_DESTINCDEC_INC;
    DMA_I2CR_Handle.Init.SrcWidth           = DMA_SRCWIDTH_BYTE;
    DMA_I2CR_Handle.Init.DestWidth          = DMA_SRCWIDTH_BYTE;

    /*-----------------------------------------------------------------------------------*/
    /* Note:If user dons not apply interrupt, Set DMA_ITC_Callback¡¢DMA_IE_Callback NULL */
    /*-----------------------------------------------------------------------------------*/
    DMA_I2CR_Handle.XferCpltCallback = NULL;
    DMA_I2CR_Handle.XferErrorCallback  = NULL;

    HAL_DMA_Init(&DMA_I2CR_Handle);
    
    I2C_Handle.HDMA_Rx = &DMA_I2CR_Handle;
}

/******************************************************************************
*@brief : I2C slave communication Demo in DMA mode
*         
*@param : None
*@return: None
******************************************************************************/
void I2C_Slave_DMA_Test(void)
{
    uint32_t i;
    
    printfS("I2C Slave Comm DMA Demo. (should connect to another board running I2C Master Comm Demo, & Slave should run first)\r\n\r\n");
    
    I2C_DMA_Init();

    DMA_I2CRecieve_Init();
    
//    printfS("Please press the USR_PB button to continue......\r\n\r\n");
    
//    while(GPIO_PIN_SET == BSP_PB_GetState());
  
    printfS("I2C Slave is running......\r\n");

    while (1) 
    {
        for (i = 0; i < BUFFER_LENGTH; i++)
        {
            TxBuffer[i] = 0;
            RxBuffer[i] = 0;
        }

        I2C_Handle.Rx_Size = BUFFER_LENGTH;
        HAL_I2C_Slave_Receive_DMA(&I2C_Handle, RxBuffer, I2C_Handle.Rx_Size);

        while (RX_Complete != 1);

        RX_Complete = 0;

        for (i = 0; i < I2C_Handle.Rx_Count; i++)
        {
            TxBuffer[i] = RxBuffer[i];
        }

        HAL_I2C_Slave_Transmit(&I2C_Handle, TxBuffer, I2C_Handle.Rx_Count,0);

        printfS("Rx Count: %d, Tx Count: %d \r\n", I2C_Handle.Rx_Count, I2C_Handle.Tx_Count);
        
        printfS("I2C Slave Test End (DMA Mode)!!! \r\n");
    }
}
#endif

/******************************************************************************
*@brief : I2C slave communication Demo main entry
*         
*@param : fe_Mode : see@enum_TEST_MODE_t
*@return: None
******************************************************************************/
void I2C_Slave_Test(enum_TEST_MODE_t fe_Mode)
{
    switch (fe_Mode)
    {
        /* Polling Mode */
        case TEST_SLAVE_POLLING: 
            I2C_Slave_Polling_Test();
            break;
        
        /* Interrupt Mode */
        case TEST_SLAVE_INTERRUPT:
            I2C_Slave_Interrupt_Test();
            break;

#ifdef HAL_DMA_MODULE_ENABLED        
        /* DMA Mode */
        case TEST_SLAVE_DMA: 
            I2C_Slave_DMA_Test();
            break;
#endif        
        default: 
            break; 
    }
}




