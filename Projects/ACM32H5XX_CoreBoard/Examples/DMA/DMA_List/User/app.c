
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


#define    DMA_TRANSFER_BUF_SIZE    ( 40 )
#define    DMA_NODE_NUM			    ( 8 )

__attribute__ ((aligned (4))) uint8_t g_srcBuf[DMA_NODE_NUM][DMA_TRANSFER_BUF_SIZE];
__attribute__ ((aligned (4))) uint8_t g_destBuf[DMA_NODE_NUM][DMA_TRANSFER_BUF_SIZE];

__attribute__ ((aligned (4))) DMA_LinkTypeDef g_node[DMA_NODE_NUM];

volatile uint32_t g_TransferComplete = 0;

DMA_HandleTypeDef DMA1_Ch0Handle;

void DMATransferComplete(DMA_HandleTypeDef *hmda);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t i;
    uint32_t j;
    uint32_t Error;
    DMA_LinkInitTypeDef DMA_LinkInit;
    
    printfS("DMA List Demo\r\n\r\n");
    
	BSP_LED_Init();
	BSP_LED_On();
    HAL_Delay(500);
	
	DMA1_Ch0Handle.Instance             = DMA1_Channel0;
	DMA1_Ch0Handle.Init.Mode            = DMA_MODE_NORMAL;
	DMA1_Ch0Handle.Init.DataFlow        = DMA_DATAFLOW_M2M;
	DMA1_Ch0Handle.Init.ReqID           = DMA1_REQ_M2M;
	DMA1_Ch0Handle.Init.SrcIncDec       = DMA_SRCINCDEC_INC;
	DMA1_Ch0Handle.Init.DestIncDec      = DMA_DESTINCDEC_INC;
	DMA1_Ch0Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
	DMA1_Ch0Handle.Init.DestWidth       = DMA_DESTWIDTH_BYTE;
	DMA1_Ch0Handle.Init.SrcBurst        = DMA_SRCBURST_4;
	DMA1_Ch0Handle.Init.DestBurst       = DMA_DESTBURST_4;
	DMA1_Ch0Handle.Init.Lock            = ENABLE;
	DMA1_Ch0Handle.Init.SrcMaster       = DMA_SRCMASTER_1;
	DMA1_Ch0Handle.Init.DestMaster      = DMA_DESTMASTER_1;
	
	HAL_DMA_Init(&DMA1_Ch0Handle);
	
	for (i = 0; i < DMA_NODE_NUM; i++)
	{
		for (j = 0; j < DMA_TRANSFER_BUF_SIZE; j++)
		{
			g_srcBuf[i][j] = j & 0xffu;
		}
	}
	
	memset((void *)g_destBuf, 0, sizeof(g_destBuf));
	
	for (i = 0; i < DMA_NODE_NUM; i++)
	{
		if (i == (DMA_NODE_NUM - 1))
            DMA_LinkInit.RawInt = DMA_RAWINT_ENABLE;
        else
            DMA_LinkInit.RawInt    = DMA_RAWINT_DISABLE;
        DMA_LinkInit.SrcIncDec     = DMA_SRCINCDEC_INC;
        DMA_LinkInit.DestIncDec    = DMA_DESTINCDEC_INC;
        DMA_LinkInit.SrcWidth      = DMA_SRCWIDTH_BYTE;
        DMA_LinkInit.DestWidth     = DMA_DESTWIDTH_BYTE;
        DMA_LinkInit.SrcBurst      = DMA_SRCBURST_4;
        DMA_LinkInit.DestBurst     = DMA_DESTBURST_4;
        DMA_LinkInit.NextMaster    = DMA_NEXTMASTER_1;
        DMA_LinkInit.SrcAddr       = (uint32_t)g_srcBuf[i];
        DMA_LinkInit.DestAddr      = (uint32_t)g_destBuf[i];
        DMA_LinkInit.TransferSize  = DMA_TRANSFER_BUF_SIZE;
		DMA_LinkInit.Next = 0u;
		HAL_DMA_InitLink(&g_node[i], &DMA_LinkInit);
	}
	
	for (i = 0; i < (DMA_NODE_NUM - 1); i++)
	{
		HAL_DMA_SetLinkNext(&g_node[i], &g_node[i + 1]);
	}
	
    HAL_DMA_RegisterCallback(&DMA1_Ch0Handle, DMA_CALLBACKID_CPLT, DMATransferComplete);
    
    g_TransferComplete = 0;
    
	HAL_DMA_Start_Link_IT(&DMA1_Ch0Handle, &g_node[0]);
	
	while (g_TransferComplete == 0u)
    {
    };
	
	Error = 0;
	for (i = 0; i < DMA_NODE_NUM; i++)
	{
        if (memcmp((void *)g_srcBuf, (void *)g_destBuf, DMA_TRANSFER_BUF_SIZE) != 0)
        {
            Error = 1;
        }
	}
	
	if (Error == 0)
	{
		printfS("test result: successful.\r\n");
	}
	else
	{
		printfS("test result: failed!\r\n");
	}
	
    while(1)
    {
		if (Error == 0)
		{
			BSP_LED_Toggle();
		}
		else
		{
			BSP_LED_Off();
		}
		HAL_Delay(500);
    };
}

void DMATransferComplete(DMA_HandleTypeDef *hdma)
{
    UNUSED(hdma);
    
    g_TransferComplete = 1;
}
