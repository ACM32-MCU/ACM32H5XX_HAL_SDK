
#include "hal.h"  


extern ADC_HandleTypeDef ADC_RegularHandle;
static uint32_t gadcxValBuffer[21];             //ADC Value
static volatile uint32_t gadcWatchdogFlag = 0; 


/******************************************************************************
* @brief : adc Analog watchdog interrupt callback function.
* @param : none
* @return: none
******************************************************************************/ 
static void ADCx_AnalogWDGCallback(ADC_HandleTypeDef *hadc)
{
    gadcWatchdogFlag = 3;
}

/******************************************************************************
* @brief : ADC Analog watchdog Test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Analog_Watchdog_Test(void)
{
    uint32_t i;
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0};
    ADC_AnalogWDGConfTypeDef ADC_AnalogWDGConf = {0};

    printfS("The ADC_Analog_Watchdog_Test start \r\n");

    /* ADC时钟来源 */
    ADC_RegularHandle.Init.ClockSource                  = ADC_CLOCKSOURCE_HCLK;
    /* ADC_CLK分频 */
    ADC_RegularHandle.Init.ClockPrescaler               = 16;
    /* 分辨率 */       
    ADC_RegularHandle.Init.Resolution                   = ADC_RESOLUTION_12B;
    /* 数据对齐 */    
    ADC_RegularHandle.Init.DataAlign                    = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */   
    ADC_RegularHandle.Init.ConConvMode                  = DISABLE;               
    /* 间断模式 */
    ADC_RegularHandle.Init.DiscontinuousConvMode        = DISABLE;
    /* 间断模式通道计数 */        
    ADC_RegularHandle.Init.NbrOfDiscConversion          = 0;
    /* 触发模式：外部触发或软件触发 */               
    ADC_RegularHandle.Init.ExternalTrigConv             = ADC_SOFTWARE_START;
    /* 外部触发边沿选择 */  
    ADC_RegularHandle.Init.ExternalTrigConvEdge         = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA使能 */
    ADC_RegularHandle.Init.DMAMode                      = DISABLE; 
    /* 溢出时是否保留上次采样数据 */         
    ADC_RegularHandle.Init.OverMode                     = DISABLE;        
    /* 过采样使能 */
    ADC_RegularHandle.Init.OverSampMode                 = DISABLE;
    /* 过采样率 */
    ADC_RegularHandle.Init.Oversampling.Ratio           = ADC_OVERSAMPLING_RATIO_8;
    /* 过采样移位系数 */
    ADC_RegularHandle.Init.Oversampling.RightBitShift   = ADC_RIGHTBITSHIFT_3;
    /* 过采样触发模式 */
    ADC_RegularHandle.Init.Oversampling.TriggeredMode   = 0;
    /* 模拟看门狗 */              
    ADC_RegularHandle.Init.AnalogWDGEn                  = DISABLE;
    /* 通道使能 */ 
    ADC_RegularHandle.Init.ChannelEn                    = ADC_CHANNEL_1_EN | ADC_CHANNEL_2_EN | ADC_CHANNEL_3_EN | \
                                                            ADC_CHANNEL_4_EN | ADC_CHANNEL_5_EN | ADC_CHANNEL_6_EN | ADC_CHANNEL_7_EN | \
                                                            ADC_CHANNEL_8_EN | ADC_CHANNEL_9_EN | ADC_CHANNEL_10_EN | ADC_CHANNEL_11_EN | \
                                                            ADC_CHANNEL_14_EN | ADC_CHANNEL_15_EN | \
                                                            ADC_CHANNEL_16_EN | ADC_CHANNEL_17_EN | ADC_CHANNEL_18_EN; 
    /* ADC选择 */
    ADC_RegularHandle.Instance                          = ADC1;  

    HAL_ADC_Init(&ADC_RegularHandle);


    
    /* 规则组序列长度 */
    ADC_RegularHandle.ChannelNum                        = 16;

    /* 偏移组选择 */
    ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE;
    /* 偏移量 */ 
    ADC_ChannelConf.Offset                              = 0;
    /* 偏移计算方式 */ 
    ADC_ChannelConf.OffsetCalculate                     = 0;
    /* 偏移结果格式选择 */ 
    ADC_ChannelConf.Offsetsign                          = 0;
    /* 差分模式使能 */
    ADC_ChannelConf.Diff                                = DISABLE; 
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_1;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;
    /* 采样时间 */  
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);

    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_2;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ2;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_3;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ3;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_4;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ4;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_5;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ5;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_6;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ6;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;       
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_7;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ7;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_8;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ8;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_9;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ9;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_10;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ10;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_11;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ11;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_14;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ12;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_15;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ13;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_16;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ14;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_17;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ15;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_18;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ16;
    /* 采样时间 */ 
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);         
    
    /* 模拟看门狗的规则通道 */ 
    ADC_AnalogWDGConf.RegularChannel                    = ADC_CHANNEL_1;
    /* 模拟看门狗的注入通道 */
    ADC_AnalogWDGConf.InjectChannel                     = ADC_CHANNEL_1;
    /* 单一/全部规则/注入通道上使用模拟看门狗 */
    ADC_AnalogWDGConf.WatchdogMode                      = ADC_ANALOGWATCHDOG_RCH_SINGLE;
    /* 模拟看门狗的差分模式 */
    ADC_AnalogWDGConf.Diff                              = DISABLE; 
    /* 模拟看门狗中断使能 */
    ADC_AnalogWDGConf.ITMode                            = ENABLE;
    /* 模拟看门狗的高阈值 */
    ADC_AnalogWDGConf.HighThreshold                     = 0xA00;
    /* 模拟看门狗的低阈值 */
    ADC_AnalogWDGConf.LowThreshold                      = 0x200;
    
    HAL_ADC_AnalogWDGConfig(&ADC_RegularHandle, &ADC_AnalogWDGConf);
    /* 模拟看门狗的中断回调函数 */
    ADC_RegularHandle.LevelOutOfWindowCallback          = ADCx_AnalogWDGCallback;
    gadcWatchdogFlag = 0;
    while(1)
    {
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            gadcxValBuffer[i] = 0;
        }

        HAL_ADC_Polling(&ADC_RegularHandle, gadcxValBuffer, ADC_RegularHandle.ChannelNum, 0);
        
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            printfS("[%d]Channel %d = 0x%x(%d)\r\n", i, (gadcxValBuffer[i] >> 16 & 0xFF), (gadcxValBuffer[i] & 0xFFF), (gadcxValBuffer[i] & 0xFFF));
        }  
        
        if(gadcWatchdogFlag)
        {
            printfS("ADC Channel Level Out Of Window\r\n");
            gadcWatchdogFlag = 0;       
        }         
        HAL_DelayMs(1000);     
    }
}

