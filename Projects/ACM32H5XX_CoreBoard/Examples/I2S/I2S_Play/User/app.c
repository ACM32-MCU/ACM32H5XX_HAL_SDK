
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "audio_data.h" 
#include "ww8978.h"   


#define RECORD_BUFF_NUM             ( 2 )
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

uint32_t TxEmptyCallback(I2S_HandleTypeDef *hi2s);
uint32_t TxCompleteCallback(I2S_HandleTypeDef *hi2s);
uint32_t DMATxCompleteCallback(I2S_HandleTypeDef *hi2s);

void Audio_Palyer(void);

uint32_t I2C4_Init(void);
uint32_t I2S1_Init(void);

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    printfS("Audio Demo: Play\r\n\r\n");
    
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
    
    if (BSP_WM8978_Init(0))
    {
        printfS("WW8978 init failed\r\n");
        while (1);
    }
    
    printfS("\r\n");
    
    while(1)
    {
        printfS("Press the key to start playing music.\r\n");
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
        
        Audio_Palyer();
        
        printfS("The play is over.\r\n");
        printfS("\r\n");
    }
}


/******************************************************************************
*@brief : LED off
*@param : none
*@return: none
******************************************************************************/
void Audio_Palyer(void)
{
    uint32_t i;
    uint32_t index;
    uint32_t first;
    uint32_t size;
    uint16_t *ptr16;
    uint32_t *ptr32;
    
    HAL_I2S_RegisterCallback(&I2S1_Handle, I2S_CALLBACKID_MSUSP, TxCompleteCallback);
    HAL_I2S_RegisterCallback(&I2S1_Handle, I2S_CALLBACKID_DMA_TX_CPLT, DMATxCompleteCallback);
    
    first = 1;
    index = 0;
    g_send_buf = NULL;
    g_send_complete = 0;
    g_send_over = 0;
    
    ptr16 = (uint16_t *)g_audio_data;
    size = sizeof(g_audio_data) >> 1;
    
    while (size)
    {
        index = (index == 0) ? 1 : 0;
        g_send_buf = g_record_data[index];
        g_send_len = (size >= RECORD_BUFF_SIZE) ? RECORD_BUFF_SIZE : size;
        size -= g_send_len;
        
        ptr32 = (uint32_t *)g_send_buf;
        for (i=0; i<g_send_len; i++)
        {
            *ptr32++ = *ptr16++;
        }
        if (first)
        {
            first = 0;
            HAL_I2S_Transmit_DMA(&I2S1_Handle, (uint32_t *)g_send_buf, g_send_len);
        }
        else
        {
            while (g_send_complete == 0);
            g_send_complete = 0;
        }
    }
    g_send_over = 1;
    while (g_send_over);
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

uint32_t TxCompleteCallback(I2S_HandleTypeDef *hi2s)
{
    UNUSED(hi2s);
    
    if (g_send_over == 0)
        return (0);
    
    g_send_over = 0;
    return (1);
}

uint32_t DMATxCompleteCallback(I2S_HandleTypeDef *hi2s)
{
    g_send_complete = 1;
    
    if (g_send_over)
        return (1);
    
    g_send_count++;
    
    HAL_DMA_Start_IT(hi2s->hdmactx, (uint32_t)g_send_buf, (uint32_t)&hi2s->Instance->TXDR, g_send_len);
    return (0);
}
