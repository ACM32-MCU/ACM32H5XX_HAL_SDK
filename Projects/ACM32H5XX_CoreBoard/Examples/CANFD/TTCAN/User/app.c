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

uint8_t TxData[64];
uint8_t RxData[128];

FDCAN_TriggerTypeDef gtrig;

bool flag_fdcan_rx_msg = false;
bool flag_tt_trig = false;

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

uint8_t slot_id = 1;



void HAL_FDCAN_TT_TimerTriggerCallback(FDCAN_HandleTypeDef *hfdcan)
{
    flag_tt_trig = true;
}

void APP_Test(void)
{
    uint8_t i;
    FDCAN_TriggerTypeDef trig;
    
    printfS("FDCAN TTCAN Demo\r\n");
    
    slot_id = 1;
    
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

    
    HAL_FDCAN_TT_Config(&fdcan_handler, FDCAN_TT_TPRESC_1);
    
    __HAL_FDCAN_DISABLE_IT(&fdcan_handler, FDCAN_IE_TX_STB_COMPLETE);
    
    HAL_FDCAN_TT_ConfigReferenceMessage(&fdcan_handler, FDCAN_STANDARD_ID, 0x668, 0);
    
    TxHeader.ID.b.ID = 0x301;
    TxHeader.ID.b.TTSEN = FDCAN_TIMESTAMP_ENABLE;//FDCAN_TIMESTAMP_DISABLE;
    
    TxHeader.FrameInfo.b.BRS = 0;
    TxHeader.FrameInfo.b.DLC = __HAL_FDCAN_LEN2DLC( 8 );
    TxHeader.FrameInfo.b.FDF = 1;
    TxHeader.FrameInfo.b.IDE = 0;
    TxHeader.FrameInfo.b.RTR = 0;    

//    HAL_FDCAN_TT_AddMessageByTBPtr(&fdcan_handler, &TxHeader, TxData0, 1);
    for(i = 1; i < 16; i++)
    {
        TxData0[0] = i;
        HAL_FDCAN_TT_AddMessageByTBPtr(&fdcan_handler, &TxHeader, TxData0, i);
        TxHeader.ID.b.ID++;
    }

    TxHeader.ID.b.ID = 0x301;


    while(1)
    {
        if(flag_fdcan_rx_msg)
        {
            flag_fdcan_rx_msg = false;
            
            HAL_FDCAN_GetRxMessage(&fdcan_handler, &RxHeader, RxData);      

            if(RxHeader.ID.b.ID == 0x668)
            {
                slot_id = 1;
   
                gtrig.TriggerType = FDCAN_TT_TX_TRIGGER_SINGLE;
                gtrig.TimeMark = 0x100;
                gtrig.TxEnWindow = 15;
                gtrig.TTPtr = slot_id;
                
                HAL_FDCAN_TT_ConfigTrigger(&fdcan_handler, &gtrig);
                
                slot_id ++;

            }
        }
        else
        {
            if(flag_tt_trig)
            {                
                flag_tt_trig = false;
                gtrig.TriggerType = FDCAN_TT_TX_TRIGGER_SINGLE;
                gtrig.TimeMark += 0x200;;
                if(gtrig.TimeMark > 0xFFFF)
                    gtrig.TimeMark = 0x100;
                gtrig.TxEnWindow = 15;
                gtrig.TTPtr = slot_id;
   
                if(slot_id < 16)
                {
                    HAL_FDCAN_TT_ConfigTrigger(&fdcan_handler, &gtrig);

                    slot_id++;
                }
                else
                    slot_id = 1;
                
                    
            }
        }
    }
}

