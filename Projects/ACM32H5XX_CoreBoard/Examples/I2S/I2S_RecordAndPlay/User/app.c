
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "ww8978.h"   


#define RECORD_BUFF_NUM             ( 10 )
#define RECORD_BUFF_SIZE            ( 256 )

I2S_HandleTypeDef I2S1_Handle;
I2C_HandleTypeDef I2C4_Handle;
DMA_HandleTypeDef DMA1CH0_Handle;
DMA_HandleTypeDef DMA2CH0_Handle;

__attribute__ ((aligned (4))) uint32_t g_record_data[RECORD_BUFF_NUM][RECORD_BUFF_SIZE];

volatile uint32_t *g_send_buf = NULL;
volatile uint32_t g_send_len = 0;
volatile uint32_t g_send_over = 0;
volatile uint32_t g_send_complete = 0;
volatile uint32_t g_send_count = 0;

volatile uint32_t g_send_count;
volatile uint32_t g_recv_count;

uint32_t DMATxCompleteCallback(I2S_HandleTypeDef *hi2s);
uint32_t DMARxCompleteCallback(I2S_HandleTypeDef *hi2s);

void Audio_RecordAndPalyer(void);

uint32_t I2C4_Init(void);
uint32_t I2S1_Init(void);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    printfS("Audio Demo: Record and Play\r\n\r\n");
    
	BSP_LED_Init();
    BSP_PB_Init();
    
	BSP_LED_On();
    HAL_Delay(500);
    
    printfS("WW8978 Init \r\n");
    
    if (I2C4_Init()!= HAL_OK)
    {
        printfS("i2c1 init failed.\r\n");
        while (1);
    }
    
    if (I2S1_Init()!= HAL_OK)
    {
        printfS("i2s2 init failed.\r\n");
        while (1);
    }
    
    if (BSP_WM8978_Init(1))
    {
        printfS("WW8978 init failed\r\n");
        while (1);
    }
    
    printfS("\r\n");
    
    printfS("Press the key to start recording from MCI and playing from PHONE.\r\n");
    while (1)
    {
        if (BSP_PB_GetState())
        {
            HAL_Delay(50);
            continue;
        }
        HAL_Delay(20);
        if (BSP_PB_GetState())
        {
            HAL_Delay(50);
            continue;
        }
        break;
    }
    
    Audio_RecordAndPalyer();
    
    while(1)
    {
		HAL_Delay(1000);
    }
}


/******************************************************************************
*@brief : LED off
*@param : none
*@return: none
******************************************************************************/
void Audio_RecordAndPalyer(void)
{
    HAL_I2S_RegisterCallback(&I2S1_Handle, I2S_CALLBACKID_DMA_TX_CPLT, DMATxCompleteCallback);
    HAL_I2S_RegisterCallback(&I2S1_Handle, I2S_CALLBACKID_DMA_RX_CPLT, DMARxCompleteCallback);
    
    memset((void *)&g_record_data[0][0], 0, 2 * RECORD_BUFF_SIZE * 4);
    
    g_send_count = 0;
    g_recv_count = 2;
    
    HAL_I2S_Transmit_Receive_DMA(&I2S1_Handle, (uint32_t *)&g_record_data[2][0], (uint32_t *)&g_record_data[0][0], RECORD_BUFF_SIZE);
}


/******************************************************************************
*@brief : PB init
*@param : none
*@return: none
******************************************************************************/

uint32_t I2C4_Init(void)
{
    I2C4_Handle.Instance           = I2C4;
    I2C4_Handle.Init.I2C_Mode      = I2C_MODE_MASTER;
    I2C4_Handle.Init.Tx_Auto_En    = TX_AUTO_ENABLE;
    I2C4_Handle.Init.Clock_Speed   = CLOCK_SPEED_STANDARD; 
    I2C4_Handle.Init.Stretch_Mode  = STRETCH_MODE_DISABLE;   
    I2C4_Handle.Init.filter_enable = FILTER_ALGO_ENABLE; 
    
    if (HAL_I2C_Init(&I2C4_Handle) != HAL_OK)
        return HAL_ERROR;
    
    return HAL_OK;
}

uint32_t I2S1_Init(void)
{
    I2S1_Handle.Instance            = I2S1;
    I2S1_Handle.Init.Mode           = I2S_MODE_MASTER;
    I2S1_Handle.Init.Standard       = I2S_STANDARD_PHILIPS;
    I2S1_Handle.Init.DataFormat     = I2S_DATA_FORMAT_16B_EXTENDED_TO_16B;
    I2S1_Handle.Init.MCLKOutput     = I2S_MCLKOUT_ENABLE;
    I2S1_Handle.Init.ClockPolarity  = I2S_CLOCK_POLARITY_LOW;
    I2S1_Handle.Init.IOSwitch       = I2S_IO_SWITCH_ENABLE;
    I2S1_Handle.Init.AudioFreq      = 8000U;
    
    if (HAL_I2S_Init(&I2S1_Handle) != HAL_OK)
        return HAL_ERROR;
    
    return HAL_OK;
}

uint32_t DMATxCompleteCallback(I2S_HandleTypeDef *hi2s)
{
    g_send_count++;
    if (g_send_count >= RECORD_BUFF_NUM)
        g_send_count = 0;
    
    HAL_DMA_Start_IT(hi2s->hdmactx, (uint32_t)&g_record_data[g_send_count][0], (uint32_t)&hi2s->Instance->TXDR, RECORD_BUFF_NUM);

    return (0);
}

uint32_t DMARxCompleteCallback(I2S_HandleTypeDef *hi2s)
{
    g_recv_count++;
    if (g_recv_count >= RECORD_BUFF_NUM)
        g_recv_count = 0;
    
    HAL_DMA_Start_IT(hi2s->hdmacrx, (uint32_t)&hi2s->Instance->RXDR, (uint32_t)&g_record_data[g_recv_count][0], RECORD_BUFF_NUM);
    
    return (0);
}
