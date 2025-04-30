

#include "hal.h" 







/******************************************************************************
* @brief : ADC Inject Polling Mode test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Inject_Polling_Nchannels_Test(void)
{
    uint32_t i;
    ADC_HandleTypeDef ADC_InjectHandle = {0};
    ADC_InjectionConfTypeDef sConfigInjected = {0};
    uint32_t adcxConvertedValue;    
    
    printfS("The ADC_Inject_Polling_Nchannels_Test start \r\n");
    
    /* ADC时钟来源 */
    ADC_InjectHandle.Init.ClockSource                   = ADC_CLOCKSOURCE_HCLK;
    /* ADC_CLK分频选择 */
    ADC_InjectHandle.Init.ClockPrescaler                = 16;
    /* 分辨率 */      
    ADC_InjectHandle.Init.Resolution                    = ADC_RESOLUTION_12B;
    /* 数据对齐 */   
    ADC_InjectHandle.Init.DataAlign                     = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */ 
    ADC_InjectHandle.Init.ConConvMode                   = DISABLE;
    /* 间断模式 */             
    ADC_InjectHandle.Init.DiscontinuousConvMode         = DISABLE;
    /* 触发模式：外部触发或软件触发 */      
    ADC_InjectHandle.Init.ExternalTrigConv              = ADC_SOFTWARE_START;
    /* 外部触发边沿选择 */
    ADC_InjectHandle.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA选择 */
    ADC_InjectHandle.Init.DMAMode                       = DISABLE;        
    /* 溢出时是否保留上次采样数据 */
    ADC_InjectHandle.Init.OverMode                      = DISABLE;
    /* 过采样使能 */      
    ADC_InjectHandle.Init.OverSampMode                  = DISABLE;
    /* 模拟看门狗使能位 */
    ADC_InjectHandle.Init.AnalogWDGEn                   = DISABLE;
    /* 通道使能 */    
    ADC_InjectHandle.Init.ChannelEn                     =  ADC_CHANNEL_1_EN | ADC_CHANNEL_2_EN | ADC_CHANNEL_3_EN | \
                                                        ADC_CHANNEL_4_EN | ADC_CHANNEL_5_EN | ADC_CHANNEL_6_EN | ADC_CHANNEL_7_EN | \
                                                        ADC_CHANNEL_8_EN | ADC_CHANNEL_9_EN | ADC_CHANNEL_10_EN | ADC_CHANNEL_11_EN | \
                                                        ADC_CHANNEL_14_EN | ADC_CHANNEL_15_EN |ADC_CHANNEL_18_EN; 
    /* ADC选择 */
    ADC_InjectHandle.Instance                           = ADC2;

    HAL_ADC_Init(&ADC_InjectHandle);

    /* 规则组序列长度 */
    ADC_InjectHandle.ChannelNum                         = 4;

    /* 注入序列长度 */
    sConfigInjected.InjectedNbrOfConversion             = 4;
    /* 通道采样时间 */ 
    sConfigInjected.InjectedSamplingTime                = ADC_SMP_CLOCK_640; 
    /* 注入通道单端/差分模式 */  
    sConfigInjected.InjectedDiff                        = DISABLE;
    /* 注入通道触发模式 */    
    sConfigInjected.ExternalTrigInjecConv               = ADC_SOFTWARE_START;
    /* 注入通道外部触发边沿选择 */
    sConfigInjected.ExternalTrigInjecConvEdge           = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* 注入组自动转换 */
    sConfigInjected.AutoInjectedConv                    = DISABLE;
    /* 注入通道上的间断模式使能控制 */
    sConfigInjected.InjectedDiscontinuousConvMode       = DISABLE;
    /* 偏移组选择 */
    sConfigInjected.InjectedOffsetNumber                = ADC_OFR_NONE;
    /* 注入通道偏移量 */  
    sConfigInjected.InjectedOffset                      = 0;
    /* 注入通道偏移计算方式 */ 
    sConfigInjected.InjectedOffsetCalculate             = 0;
    /* 注入通道偏移结果格式选择 */ 
    sConfigInjected.InjectedOffsetSign                  = 0; 
    
    /* 注入通道过采样使能 */
    sConfigInjected.InjecOversamplingMode               = DISABLE;
    /* 注入通道过采样率 */    
    sConfigInjected.InjecOversampling.Ratio             = ADC_OVERSAMPLING_RATIO_2;
    /* 注入通道过采样移位系数 */
    sConfigInjected.InjecOversampling.RightBitShift     = ADC_RIGHTBITSHIFT_2;
    
    /* 转换通道 */
    sConfigInjected.InjectedChannel                     = ADC_CHANNEL_1;
    /* 注入序列转换顺序 */
    sConfigInjected.InjectedRank                        = ADC_INJECTED_RANK_1;   
    HAL_ADCEx_InjectedConfigChannel(&ADC_InjectHandle, &sConfigInjected);
   
    /* 转换通道 */
    sConfigInjected.InjectedChannel                     = ADC_CHANNEL_14;
    /* 注入序列转换顺序 */
    sConfigInjected.InjectedRank                        = ADC_INJECTED_RANK_2;
    HAL_ADCEx_InjectedConfigChannel(&ADC_InjectHandle, &sConfigInjected);

    //* 转换通道 */
    sConfigInjected.InjectedChannel                     = ADC_CHANNEL_5;
    /* 注入序列转换顺序 */
    sConfigInjected.InjectedRank                        = ADC_INJECTED_RANK_3;
    HAL_ADCEx_InjectedConfigChannel(&ADC_InjectHandle, &sConfigInjected);

    /* 转换通道 */
    sConfigInjected.InjectedChannel                     = ADC_CHANNEL_15;
    /* 注入序列转换顺序 */
    sConfigInjected.InjectedRank                        = ADC_INJECTED_RANK_4;
    HAL_ADCEx_InjectedConfigChannel(&ADC_InjectHandle, &sConfigInjected);
  
    
    while(1)
    {

        HAL_ADCEx_InjectedStart(&ADC_InjectHandle);

        while(!READ_BIT(ADC_InjectHandle.Instance->SR,ADC_SR_JEOG)){};
        SET_BIT(ADC_InjectHandle.Instance->SR,ADC_SR_JEOG);	
		
        adcxConvertedValue = HAL_ADCEx_InjectedGetValue(&ADC_InjectHandle,ADC_INJECTED_RANK_1);
        printfS("[1]Channel %d = 0x%x(%d)\r\n", (adcxConvertedValue >> 16 & 0xFF), (adcxConvertedValue& 0xFFF), (adcxConvertedValue& 0xFFF));
        
        adcxConvertedValue = HAL_ADCEx_InjectedGetValue(&ADC_InjectHandle,ADC_INJECTED_RANK_2);
        printfS("[2]Channel %d = 0x%x(%d)\r\n", (adcxConvertedValue >> 16 & 0xFF), (adcxConvertedValue & 0xFFF), (adcxConvertedValue& 0xFFF));
       
        adcxConvertedValue = HAL_ADCEx_InjectedGetValue(&ADC_InjectHandle,ADC_INJECTED_RANK_3);
        printfS("[3]Channel %d = 0x%x(%d)\r\n", (adcxConvertedValue >> 16 & 0xFF), (adcxConvertedValue & 0xFFF), (adcxConvertedValue& 0xFFF));
       
        adcxConvertedValue = HAL_ADCEx_InjectedGetValue(&ADC_InjectHandle,ADC_INJECTED_RANK_4);
        printfS("[4]Channel %d = 0x%x(%d)\r\n", (adcxConvertedValue >> 16 & 0xFF), (adcxConvertedValue & 0xFFF), (adcxConvertedValue& 0xFFF));

        HAL_DelayMs(1000);   
    }
}


