
/******************************************************************************
*@file  : app.c
*@brief : application
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h" 

uint16_t MDAC_ValBuffer[16];
uint16_t RxBuffer[16];

MDAC_HandleTypeDef MDAC_Handle;

void MDAC_BufValueInit(uint16_t *buf, uint16_t cnt)
{
    uint16_t i;
    for(i = 0; i < cnt; i++)
        buf[i] = 0x123 + i;
}


void MDAC_VoltageDAC_ChannelConfig(uint8_t channel)
{
    VDAC_ChannelConfigTypeDef config;
    
    config.BufferMode = MDAC_VDAC_BUFFER_DISABLE;//MDAC_VDAC_BUFFER_ENABLE;//MDAC_VDAC_BUFFER_DISABLE;
    config.SelfCalibrateEn = DISABLE;//DISABLE;//ENABLE;
    config.SampleEn = ENABLE;
    
    HAL_MDAC_ConfigVoltageChannel(&MDAC_Handle, channel, &config);
}

void MDAC_CurrentDAC_ChannelConfig(uint8_t channel)
{
    IDAC_ChannelConfigTypeDef config;
    
    config.ITurn = 6;//ITURN_FACTORY;
    
    HAL_MDAC_ConfigCurrentChannel(&MDAC_Handle, channel, &config);
}


uint16_t val_step100[100];
void MDAC_OutPutVoltage(float voltage)
{
    int i = 0;
    uint16_t total_cnt;
    uint16_t point_val = 0;
    
    while(point_val < 4096)
    {
        val_step100[i++] = point_val;
        point_val += 200;
    }
    
    //val_step100[i++] = 4095;
    
    total_cnt = i;
    
    if( voltage > 3.3)
    {
        return;
    }
    uint16_t data=((voltage / 3.3) * 4095);
    
    
    i = 0;
    while(1)
    {
        data = val_step100[i++];
        if(i == total_cnt) i = 0;
        HAL_MDAC_SetVoltageValue(&MDAC_Handle, 1, data);
        HAL_Delay(1);        
    }
}

void MDAC_test(void)
{
    bool debug = false;
    uint32_t i;
    uint32_t tmp;
    
    MDAC_BufValueInit(MDAC_ValBuffer, 16);
    
    MDAC_Handle.Instance = MDAC;
    
    HAL_MDAC_Init(&MDAC_Handle);
    
    
    for(i = 0; i < MDAC_MAX_V_CHANNEL_NUM; i++)
    {
        MDAC_VoltageDAC_ChannelConfig(i);
    
        HAL_MDAC_SetVoltageValue(&MDAC_Handle, i, 0x3FF);
    }
    
//    MDAC_VoltageDAC_ChannelConfig(0);
//    
//    HAL_MDAC_SetVoltageValue(&MDAC_Handle, 0, 0x3FF);
//    
//    MDAC_VoltageDAC_ChannelConfig(1);
//    
//    HAL_MDAC_SetVoltageValue(&MDAC_Handle, 1, 0x3FF);
//    
//    MDAC_OutPutVoltage(1);
    
    
    for(i = 0; i < MDAC_MAX_I_CHANNEL_NUM; i++)
    {
        MDAC_CurrentDAC_ChannelConfig(i);
        HAL_MDAC_SetCurrentValue(&MDAC_Handle, i, 0x3FF);
    }
    
    
    while(1);
    
    MDAC_CurrentDAC_ChannelConfig(1);
    
    for(i = 0; i < MDAC_MAX_V_CHANNEL_NUM; i++)
    {
        HAL_MDAC_SetVoltageValue(&MDAC_Handle, i, MDAC_ValBuffer[i]);
    }
    
    for(i = 0; i < MDAC_MAX_V_CHANNEL_NUM; i++)
    {
        RxBuffer[i] = HAL_MDAC_GetVoltageValue(&MDAC_Handle, i);
        if(RxBuffer[i] != MDAC_ValBuffer[i])
            printfS("err@ channel %d\r\n", i);
    }
    
    
    
    
    
    tmp = HAL_MDAC_GetVoltageValue(&MDAC_Handle, 0);
    
    printfS("val = 0x%X\r\n", tmp);
    
    HAL_MDAC_SetCurrentValue(&MDAC_Handle, 0, 0x123);
    
    tmp = HAL_MDAC_GetCurrentValue(&MDAC_Handle, 0);
    
    printfS("val = 0x%X\r\n", tmp);
    
}
