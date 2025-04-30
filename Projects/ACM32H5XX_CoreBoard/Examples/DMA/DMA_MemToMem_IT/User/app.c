
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


#define    DMA_TRANSFER_BUF_SIZE    ( 40960U )

__attribute__ ((aligned (4)))	uint8_t g_srcBuf[DMA_TRANSFER_BUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t g_destBuf[DMA_TRANSFER_BUF_SIZE];

DMA_HandleTypeDef DMA1_Ch0Handle;

volatile uint32_t g_TransferComplete = 0;

void DMATransferComplete(DMA_HandleTypeDef *hdma1);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t i;
    
    printfS("DMA MemToMem IT Demo\r\n\r\n");
    
	BSP_LED_Init();
	
	BSP_LED_On();
    HAL_Delay(500);
    
	DMA1_Ch0Handle.Instance             = DMA1_Channel0;
	DMA1_Ch0Handle.Init.Mode            = DMA_MODE_NORMAL;       // DMA_MODE_CIRCULAR   DMA_MODE_NORMAL
	DMA1_Ch0Handle.Init.DataFlow        = DMA_DATAFLOW_M2M;
	DMA1_Ch0Handle.Init.ReqID           = DMA1_REQ_M2M;
	DMA1_Ch0Handle.Init.SrcIncDec       = DMA_SRCINCDEC_INC;
	DMA1_Ch0Handle.Init.DestIncDec      = DMA_DESTINCDEC_INC;
	DMA1_Ch0Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
	DMA1_Ch0Handle.Init.DestWidth       = DMA_DESTWIDTH_BYTE;
	DMA1_Ch0Handle.Init.SrcBurst        = DMA_SRCBURST_128;
	DMA1_Ch0Handle.Init.DestBurst       = DMA_DESTBURST_128;  
	DMA1_Ch0Handle.Init.Lock            = ENABLE;
	DMA1_Ch0Handle.Init.SrcMaster       = DMA_SRCMASTER_1;
	DMA1_Ch0Handle.Init.DestMaster      = DMA_DESTMASTER_2;
	
	HAL_DMA_Init(&DMA1_Ch0Handle);
	
    for (i = 0; i < DMA_TRANSFER_BUF_SIZE; i++)
        g_srcBuf[i] = i & 0xffu;
    
    for (i = 0; i < DMA_TRANSFER_BUF_SIZE; i++)
        g_destBuf[i] = 0;
    
    g_TransferComplete = 0;
    
    HAL_DMA_RegisterCallback(&DMA1_Ch0Handle, DMA_CALLBACKID_CPLT, DMATransferComplete);
    
    HAL_DMA_Start_IT(&DMA1_Ch0Handle, (uint32_t)g_srcBuf, (uint32_t)g_destBuf, DMA_TRANSFER_BUF_SIZE);
    
    while(1)
    {
        if(g_TransferComplete)
        {
            printfS("transfer completely\r\n"); 
            break;   
        }
        
        printfS("DMA Not Finished\r\n");  
        HAL_Delay(1000); 
    }
}

void DMATransferComplete(DMA_HandleTypeDef *hdma1)
{
    UNUSED(hdma1);
    
    g_TransferComplete++;
}
