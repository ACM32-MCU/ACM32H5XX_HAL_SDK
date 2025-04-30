/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

uint8_t TxBuffer[512];
uint8_t RxBuffer[512];

const char *prompt_str = "Please enter any String/Data";

UART_HandleTypeDef  UART1_Handle;
UART_HandleTypeDef  UART2_Handle;

#ifdef HAL_DMA_MODULE_ENABLED
DMA_HandleTypeDef  DMA_TX_Handle;
DMA_HandleTypeDef  DMA_RX_Handle;
#endif

volatile uint32_t TX_DMA_Status = false;
volatile uint32_t RX_DMA_Status = false;

volatile uint32_t TxCpltStatus = false, TxCpltStatus2 = false;

volatile uint32_t RxCpltStatus = false, RxCpltStatus2 = false;

/******************************************************************************
*@brief : Sample code for initialising UART1 
*@param : none
*@return: none
******************************************************************************/
void UART1_Init(void) 
{
    UART1_Handle.Instance        = USART1;    
    UART1_Handle.Init.BaudRate   = 115200; 
    UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART1_Handle.Init.Parity     = UART_PARITY_NONE;
    UART1_Handle.Init.Mode       = UART_MODE_TX_RX;
    UART1_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART1_Handle);  
}

/******************************************************************************
*@brief : Sample code for initialising UART2 
*@param : none
*@return: none
******************************************************************************/
void UART2_Init(void) 
{
    UART2_Handle.Instance        = USART2;    
    UART2_Handle.Init.BaudRate   = 115200; 
    UART2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    UART2_Handle.Init.StopBits   = UART_STOPBITS_1;
    UART2_Handle.Init.Parity     = UART_PARITY_NONE;
    UART2_Handle.Init.Mode       = UART_MODE_TX_RX;
    UART2_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    
    HAL_UART_Init(&UART2_Handle);  
}



/******************************************************************************
*@brief : UART1 IT tx completed callback
*@param : none
*@return: none
******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance ==USART1)  // Transfor Data By UART1
    {
		TxCpltStatus= true;
    }
    else if(huart->Instance ==USART2) //Transfor Data By UART2 
	{
		TxCpltStatus2 =true;
	}
}
#ifdef HAL_DMA_MODULE_ENABLED
void UART1_TX_DMA_Complete_Callback(DMA_HandleTypeDef *h)
{
    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)h)->Parent;
    huart->TxBusy = false;
    TX_DMA_Status = true;
}

void UART1_RX_DMA_Complete_Callback(DMA_HandleTypeDef *h)
{
    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)h)->Parent;
    CLEAR_BIT(huart->Instance->IE, UART_IE_IDLEI);
    CLEAR_STATUS(huart->Instance->ISR, UART_ISR_IDLEI);
    huart->RxBusy = false;
    huart->RxCount = huart->RxSize - (h->Instance->CXCTRL & DMA_CXCTRL_TRANSFERSIZE_Msk);
    RX_DMA_Status = true;
}
#endif
//void UART1_RX_DMA_Abort_Callback(DMAC_HandleTypeDef *h)
//{
//    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMAC_HandleTypeDef *)h)->Parent;

//    huart->RxCount = huart->RxSize - (h->Instance->CHCTRL & DMA_CHCTRL_TRANSFERSIZE_Msk);
//    
//    HAL_DMA_Abort(h);
//    
//    RX_DMA_Status = true;

//}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{	
    if(huart->Instance == USART1)  // receive data by UART1
    {
		RxCpltStatus = true;
    }
    else if(huart->Instance == USART2) //receive data by UART2  
	{
		RxCpltStatus2 = true;
	}
}
#ifdef HAL_DMA_MODULE_ENABLED
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
    /* This code can be modified by user */
    if(huart->HDMA_Rx != NULL)
    {
        /* UART DMA transfer when idle line occured, */
        huart->RxCount = huart->RxSize - (huart->HDMA_Rx->Instance->CXCTRL & DMA_CXCTRL_TRANSFERSIZE_Msk);

        HAL_DMA_Abort(huart->HDMA_Rx);

        RX_DMA_Status = true;
    }
    else
    {  
        #if 1
        /* Finish receiving directly */
        HAL_UART_RxCpltCallback(huart);
        #else
        /* Finish receiving when all of data have been received */
        if(huart->RxCount < huart->RxSize)
        {
            __HAL_UART_Resume_Receive_IT(huart);
        }
        else
        {
            HAL_UART_RxCpltCallback(huart);
        }
        #endif
    }   
}
#endif

void APP_Test(enum_TEST_MODE_t fe_Mode)
{
	uint8_t ret;
    
    switch (fe_Mode)
    {
        /* Loop mode*/
        case TEST_UART1_LOOP: 
        {
            printfS("----------UART1 LOOP Demo ----------\r\n");
            printfS("%s to [UART1]\r\n\r\n", prompt_str);
            for(;;)
            {
                //HAL_UART_Receive(&UART1_Handle, RxBuffer, sizeof(RxBuffer), 10);
                HAL_UART_Receive_To_IDLEorBCNT(&UART1_Handle, RxBuffer, sizeof(RxBuffer), RECEIVE_TOIDLE, 115200);
                if (UART1_Handle.RxCount)
                {
                    HAL_UART_Transmit(&UART1_Handle, RxBuffer, UART1_Handle.RxCount, HAL_MAX_DELAY);  

                    memset(RxBuffer, 0, sizeof(RxBuffer)); 
                }
                if(UART1_Handle.ErrorCode != HAL_UART_ERROR_NONE)
                {
                    printfS("ErrorCode = %d\r\n", UART1_Handle.ErrorCode);
                    while(1);
                }
            }
        }break;

        /* Interrupt mode */
        case TEST_UART1_IT: 
        {
            printfS("----------UART1 IT Demo ----------\r\n");
            printfS("%s to [UART1]\r\n\r\n", prompt_str);
            while(1)
            {
                /* receive variable length data by intrrupt method */
                HAL_UART_Receive_IT(&UART1_Handle, RxBuffer, sizeof(RxBuffer)); 
						
                while (!RxCpltStatus);
                
                RxCpltStatus=false;
                
                memcpy(TxBuffer, RxBuffer, UART1_Handle.RxCount);

                HAL_UART_Transmit_IT(&UART1_Handle, TxBuffer, UART1_Handle.RxCount); 
                
                while (!TxCpltStatus);  
            
                TxCpltStatus =false;
                
                /* wait tx xfer completed */
                while ((UART1_Handle.Instance->FR & UART_FR_BUSY));
            
            }
        }break;

#ifdef HAL_DMA_MODULE_ENABLED
        /* DMA mode */
        case TEST_UART1_DMA: 
        {
            uint32_t Uart_Dma_Len = sizeof(RxBuffer);
            printfS("----------UART1 DMA Demo ----------\r\n");
            printfS("The UART1 DMA transfer size is set to %d bytes\r\n\r\n", Uart_Dma_Len);
            printfS("%s to [UART1]\r\n\r\n", prompt_str);
            
            __HAL_RCC_DMA1_CLK_ENABLE();
            DMA_TX_Handle.Instance             = DMA2_Channel1;
            DMA_TX_Handle.Init.DataFlow        = DMA_DATAFLOW_M2P;
            DMA_TX_Handle.Init.ReqID           = DMA2_REQ_USART1_TX;
            DMA_TX_Handle.Init.SrcIncDec       = DMA_SRCINCDEC_INC     ;   //DMA_SRCINC_ENABLE;
            DMA_TX_Handle.Init.DestIncDec      = DMA_DESTINCDEC_DISABLE;
            DMA_TX_Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
            DMA_TX_Handle.Init.DestWidth       = DMA_DESTWIDTH_BYTE;
            DMA_TX_Handle.Init.SrcBurst        = DMA_SRCBURST_1;
            DMA_TX_Handle.Init.DestBurst       = DMA_DESTBURST_1;
            DMA_TX_Handle.XferCpltCallback     = UART1_TX_DMA_Complete_Callback;
            
            HAL_DMA_Init(&DMA_TX_Handle);
            
            /* UART1_Handle.HDMA_Tx = &DMA_TX_Handle; DMA_TX_Handle.Parent = &UART1_Handle; */
            __HAL_LINKDMA(&UART1_Handle, HDMA_Tx, DMA_TX_Handle);

            DMA_RX_Handle.Instance             = DMA1_Channel0;
            DMA_RX_Handle.Init.DataFlow        = DMA_DATAFLOW_P2M;
            DMA_RX_Handle.Init.ReqID           = DMA1_REQ_USART1_RX;
            DMA_RX_Handle.Init.SrcIncDec       = DMA_SRCINCDEC_DISABLE;
            DMA_RX_Handle.Init.DestIncDec      = DMA_DESTINCDEC_INC;      //DMA_DESTINC_ENABLE;
            DMA_RX_Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
            DMA_RX_Handle.Init.DestWidth       = DMA_DESTWIDTH_BYTE;
            DMA_RX_Handle.Init.SrcBurst        = DMA_SRCBURST_1;
            DMA_RX_Handle.Init.DestBurst       = DMA_DESTBURST_1;
            DMA_RX_Handle.XferCpltCallback     = UART1_RX_DMA_Complete_Callback;
            
            HAL_DMA_Init(&DMA_RX_Handle);
            
            /* UART1_Handle.HDMA_Rx = &DMA_RX_Handle; DMA_RX_Handle.Parent = &UART1_Handle; */
            __HAL_LINKDMA(&UART1_Handle, HDMA_Rx, DMA_RX_Handle);
            
            NVIC_ClearPendingIRQ(DMA2_CH1_IRQn);
            NVIC_EnableIRQ(DMA2_CH1_IRQn);
            
            NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
            NVIC_EnableIRQ(DMA1_CH0_IRQn);
            
            TX_DMA_Status = true;
            
            for(;;)
            {
                if (TX_DMA_Status) 
                {
                    TX_DMA_Status = false;
                    
                    memset(RxBuffer, 0, 512);
                }
                
                ret = HAL_UART_Receive_DMA(&UART1_Handle, RxBuffer, Uart_Dma_Len);
                
                while(RX_DMA_Status == false);
                RX_DMA_Status = false;

                ret = HAL_UART_Transmit_DMA(&UART1_Handle, RxBuffer, UART1_Handle.RxCount);
                while(TX_DMA_Status == false);

            }
        }break;
#endif

		case TEST_UART2_IT:
		{
            printfS("----------UART2 Demo ----------\r\n");
            printfS("%s to [UART2]\r\n\r\n", prompt_str);           
			while(1)
			{
				HAL_UART_Receive_IT(&UART2_Handle, RxBuffer, sizeof(RxBuffer));    // length should <= sizeof(RxBuffer)   
						
				while(1)  
				{
					while (!RxCpltStatus2);
				
					RxCpltStatus2=false;
				
					memcpy(TxBuffer, RxBuffer, UART2_Handle.RxCount);

					HAL_UART_Transmit_IT(&UART2_Handle, TxBuffer, UART2_Handle.RxCount); 
				
					while (!TxCpltStatus2);  
				
					TxCpltStatus2 =false;
                    
                    /* wait tx xfer completed */
                    while ((UART2_Handle.Instance->FR & UART_FR_BUSY));

					break;		   
				}
			}
		}break;
        
        case TEST_UART2_LOOP: 
        {
            printfS("----------UART2 LOOP Demo ----------\r\n");
            printfS("%s to [UART2]\r\n\r\n", prompt_str);

            for(;;)
            {
                HAL_UART_Receive_To_IDLEorBCNT(&UART2_Handle, RxBuffer, sizeof(RxBuffer), RECEIVE_TOIDLE, 115200);
                if (UART2_Handle.RxCount)
                {
                    HAL_UART_Transmit(&UART2_Handle, RxBuffer, UART2_Handle.RxCount, HAL_MAX_DELAY);  

                    memset(RxBuffer, 0, sizeof(RxBuffer)); 
                }
            }
        }break;
        
        default: break; 
    }
}
