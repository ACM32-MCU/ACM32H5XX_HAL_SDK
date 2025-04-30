/******************************************************************************
*@file  : app.c
*@brief :  
******************************************************************************/
#include "app.h"

uint8_t TxBuffer[512];
uint8_t RxBuffer[512];

FDCAN_HandleTypeDef fdcan_handler;
FDCAN_NewFilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

uint8_t TxData0[] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

uint8_t TxData[64];
uint8_t RxData[128];

bool flag_fdcan_rx_msg = false;

void hexdump8(uint8_t* buf, uint32_t length)
{
    uint32_t i;

    for(i = 0; i < length; i++)
    {
        printfS("%.2x ",buf[i]);    
    }
    printfS("\n");
}

void HAL_FDCAN_RxBufferNewMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{
    flag_fdcan_rx_msg = true;
}


void APP_Test(void)
{
    int i;
    
    fdcan_handler.Instance = FDCAN1;
    fdcan_handler.Init.AutoRetransmission = DISABLE;  //single shot
    fdcan_handler.Init.Mode = FDCAN_MODE_NORMAL;
    fdcan_handler.Init.FrameISOType = FDCAN_FRAME_ISO;//FDCAN_FRAME_NONISO;
    fdcan_handler.Init.RxBufOverFlowMode = FDCAN_RX_BUF_BLOCKING;
    fdcan_handler.Init.NominalPrescaler     = 10;      
    fdcan_handler.Init.NominalSyncJumpWidth = 8;
    fdcan_handler.Init.NominalTimeSeg1      = 24;    
    fdcan_handler.Init.NominalTimeSeg2      = 8;        
    fdcan_handler.Init.DataPrescaler        = 1; 
    fdcan_handler.Init.DataSyncJumpWidth    = 10; 
    fdcan_handler.Init.DataTimeSeg1         = 30;
    fdcan_handler.Init.DataTimeSeg2         = 10; 
    
    HAL_FDCAN_Init(&fdcan_handler);    
    
    /* config FDCAN filters */    
    sFilterConfig.FilterIndex                        = 0,
    sFilterConfig.FilterType                         = FDCAN_FILTER_TYPE1,        
    sFilterConfig.Filter.type1.code32.b.id           = 0x123,
    sFilterConfig.Filter.type1.code32.b.IDE          = FDCAN_STANDARD_ID,
    sFilterConfig.Filter.type1.code32.b.RTR          = 0,        
    sFilterConfig.Filter.type1.code32_2.ext.id       = 0x18FE0001,
    sFilterConfig.Filter.type1.code32_2.ext.IDE      = FDCAN_EXTENDED_ID,
    sFilterConfig.Filter.type1.code32_2.ext.RTR      = 1,
    
    HAL_FDCAN_NewConfigFilter(&fdcan_handler, &sFilterConfig);
    
    HAL_FDCAN_PTBTranmistSingleShot(&fdcan_handler, DISABLE);
    HAL_FDCAN_STBTranmistSingleShot(&fdcan_handler, DISABLE);
    
    TxHeader.ID.b.ID = 0x123;
    TxHeader.ID.b.TTSEN = FDCAN_TIMESTAMP_DISABLE;
    
    TxHeader.FrameInfo.b.BRS = 1;
    TxHeader.FrameInfo.b.DLC = __HAL_FDCAN_LEN2DLC( 12 );
    TxHeader.FrameInfo.b.FDF = 1;
    TxHeader.FrameInfo.b.IDE = 0;
    TxHeader.FrameInfo.b.RTR = 0;
    
    HAL_FDCAN_TransmitMessageBySTB(&fdcan_handler, &TxHeader, TxData0);
    
    for(i = 0; i < 64; i++)
        TxData[i] = 1 + i;
    
    TxHeader.ID.b.ID = 0x222;
    TxHeader.FrameInfo.b.DLC = __HAL_FDCAN_LEN2DLC( 64 );
    
    HAL_FDCAN_TransmitMessageByPTB(&fdcan_handler, &TxHeader, TxData);

    while(1)
    {
        if(flag_fdcan_rx_msg)
        {
            flag_fdcan_rx_msg = false;
        
            HAL_FDCAN_GetRxMessage(&fdcan_handler, &RxHeader, RxData);    
            printfS("ID = %X\r\n", RxHeader.ID.w);
            printfS("FrameInfo = %X\r\n", RxHeader.FrameInfo.w);
            hexdump8(RxData, __HAL_FDCAN_DLC2LEN(RxHeader.FrameInfo.b.DLC));
            
            TxHeader.ID.b.ID = RxHeader.ID.b.ID + 1;
            TxHeader.ID.b.TTSEN = FDCAN_TIMESTAMP_ENABLE;
            
            TxHeader.FrameInfo.b.BRS = RxHeader.FrameInfo.b.BRS;
            TxHeader.FrameInfo.b.DLC = RxHeader.FrameInfo.b.DLC;
            TxHeader.FrameInfo.b.FDF = RxHeader.FrameInfo.b.FDF;
            TxHeader.FrameInfo.b.IDE = RxHeader.FrameInfo.b.IDE;
            TxHeader.FrameInfo.b.RTR = RxHeader.FrameInfo.b.RTR;
            
            HAL_FDCAN_TransmitMessageBySTB(&fdcan_handler, &TxHeader, RxData);
        }     
    }            
}
