/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

uint8_t TxBuffer[512];
uint8_t RxBuffer[512];

FDCAN_HandleTypeDef fdcan_handler;
FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;


uint8_t TxData0[] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t TxData1[] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
uint8_t TxData2[] = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
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
    
    printfS("CANFD demo: loop back mode external.\r\n");
    
    fdcan_handler.Instance = FDCAN1;
    fdcan_handler.Init.AutoRetransmission = DISABLE;  //single shot
    fdcan_handler.Init.FrameISOType = FDCAN_FRAME_ISO;
  
    fdcan_handler.Init.Mode = FDCAN_MODE_LBME_SACK;
    
    fdcan_handler.Init.RxBufOverFlowMode = FDCAN_RX_BUF_BLOCKING;    
    
    fdcan_handler.Init.NominalPrescaler     = 10;      
    fdcan_handler.Init.NominalSyncJumpWidth = 4;
    fdcan_handler.Init.NominalTimeSeg1      = 23;    
    fdcan_handler.Init.NominalTimeSeg2      = 9;

    fdcan_handler.Init.DataPrescaler        = 1; 
    fdcan_handler.Init.DataSyncJumpWidth    = 10; 
    fdcan_handler.Init.DataTimeSeg1         = 29;
    fdcan_handler.Init.DataTimeSeg2         = 11; 
    
    HAL_FDCAN_Init(&fdcan_handler);    
    
    TxHeader.ID.b.ID = 0x111;
    TxHeader.ID.b.TTSEN = 0;
    
    TxHeader.FrameInfo.b.BRS = 1;
    TxHeader.FrameInfo.b.DLC = 12;
    TxHeader.FrameInfo.b.FDF = 1;
    TxHeader.FrameInfo.b.IDE = 0;
    TxHeader.FrameInfo.b.RTR = 0;
    
    printfS("\r\nCANFD Transmit message:\r\n");
    HAL_FDCAN_TransmitMessageBySTB(&fdcan_handler, &TxHeader, TxData0);
    
    while(!flag_fdcan_rx_msg);
    flag_fdcan_rx_msg = false;
    
    printfS("\r\nCANFD Received message:\r\n");
    HAL_FDCAN_GetRxMessage(&fdcan_handler, &RxHeader, RxData);    
    printfS("ID = %X\r\n", RxHeader.ID.w);
    hexdump8(RxData, RxHeader.FrameInfo.b.DLC);
    
    printfS("\r\nCANFD Transmit message:\r\n");
    HAL_FDCAN_TransmitMessageBySTB(&fdcan_handler, &TxHeader, TxData1);
    
    while(!flag_fdcan_rx_msg);
    flag_fdcan_rx_msg = false;
    
    printfS("\r\nCANFD Received message:\r\n");
    HAL_FDCAN_GetRxMessage(&fdcan_handler, &RxHeader, RxData);
    printfS("ID = %X\r\n", RxHeader.ID.w);
    hexdump8(RxData, RxHeader.FrameInfo.b.DLC);
}

