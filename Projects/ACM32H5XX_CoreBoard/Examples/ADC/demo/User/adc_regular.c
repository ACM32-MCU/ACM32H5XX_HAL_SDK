
#include "hal.h"


extern ADC_HandleTypeDef ADC_RegularHandle;



/******************************************************************************
* @brief : ADC Regular Polling Mode test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Regular_Polling_Nchannels_Test(void)
{
    uint8_t i;
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0};
    uint32_t adcxValBuffer[21];             //ADC Value

    printfS("The ADC_Regular_Polling_Nchannels_Test start \r\n");  
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
   
    while(1)
    {
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            adcxValBuffer[i] = 0;
        }

        HAL_ADC_Polling(&ADC_RegularHandle, adcxValBuffer, ADC_RegularHandle.ChannelNum, 0);
       
        printfS("\r\n************************************************************\r\n");
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            printfS("[%d]Channel %d = 0x%03x(%d)\r\n", i, (adcxValBuffer[i] >> 16 & 0xFF), (adcxValBuffer[i] & 0xFFF), (adcxValBuffer[i] & 0xFFF));
        }
        
        for(i = 0; i < 9; i++)
        {
            printfS("CHDR[%d] = 0x%08x\r\n", i, ADC_RegularHandle.Instance->CHDR[i]);                
        }
        
        printfS("ADC->CALFACT = 0x%x\r\n", ADC_RegularHandle.Instance->CALFACT);
        printfS("\r\n************************************************************\r\n");
        HAL_DelayMs(1500);     
    }
}




/******************************************************************************
* @brief : ADC Regular Polling Continuous Mode test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Regular_Polling_Nchannels_ContinuousMode_Test(void)
{
    uint8_t i;
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0};
    uint32_t adcxValBuffer[21];             //ADC Value

    printfS("The ADC_Regular_Polling_Nchannels_Test start \r\n");  
    /* ADC时钟来源 */
    ADC_RegularHandle.Init.ClockSource                  = ADC_CLOCKSOURCE_HCLK;
    /* ADC_CLK分频 */
    ADC_RegularHandle.Init.ClockPrescaler               = 16;
    /* 分辨率 */       
    ADC_RegularHandle.Init.Resolution                   = ADC_RESOLUTION_12B;
    /* 数据对齐 */    
    ADC_RegularHandle.Init.DataAlign                    = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */   
    ADC_RegularHandle.Init.ConConvMode                  = ENABLE;               
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
    
    HAL_ADC_Start(&ADC_RegularHandle);
    
    while(1)
    {
        for(i = 0; i < 9; i++)
        {
            printfS("CHDR[%d] = 0x%08x\r\n", i, ADC_RegularHandle.Instance->CHDR[i]);                
        }
        printfS("\r\n************************************************************\r\n");
        HAL_DelayMs(100);     
    }
}



/******************************************************************************
* @brief : ADC Differential Mode test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Differential_Nchannel_Test(void)
{
    uint8_t i;
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0};
    uint32_t adcxValBuffer[21];             //ADC Value

    printfS("The ADC_Regular_Polling_Nchannels_Test start \r\n");  

    ADC_RegularHandle.Init.ClockSource                  = ADC_CLOCKSOURCE_HCLK;//ADC时钟来源
    ADC_RegularHandle.Init.ClockPrescaler               = 16;       //ADC_CLK分频选择
    ADC_RegularHandle.Init.Resolution                   = ADC_RESOLUTION_12B;    //分辨率
    ADC_RegularHandle.Init.DataAlign                    = ADC_DATAALIGN_RIGHT;   //数据对齐
    ADC_RegularHandle.Init.ConConvMode                  = DISABLE;               //连续转换模式

    ADC_RegularHandle.Init.DiscontinuousConvMode        = DISABLE;        //间断模式
    ADC_RegularHandle.Init.NbrOfDiscConversion          = 0;               //间断模式通道计数
    ADC_RegularHandle.Init.ExternalTrigConv             = ADC_SOFTWARE_START;  //触发模式：外部触发或软件触发
    ADC_RegularHandle.Init.ExternalTrigConvEdge         = ADC_EXTERNALTRIGCONVEDGE_NONE;//外部触发边沿选择
    ADC_RegularHandle.Init.DMAMode                      = DISABLE;          //DMA选择
    ADC_RegularHandle.Init.OverMode                     = DISABLE;        //溢出时是否保留上次采样数据

    ADC_RegularHandle.Init.OverSampMode                 = DISABLE;//过采样使能
    ADC_RegularHandle.Init.Oversampling.Ratio           = ADC_OVERSAMPLING_RATIO_8;//过采样率
    ADC_RegularHandle.Init.Oversampling.RightBitShift   = ADC_RIGHTBITSHIFT_3;//过采样移位系数
    ADC_RegularHandle.Init.Oversampling.TriggeredMode   = 0;          //过采样触发模式    
    ADC_RegularHandle.Init.AnalogWDGEn                  = DISABLE;  //模拟看门狗
    ADC_RegularHandle.Init.ChannelEn                    = ADC_CHANNEL_1_EN | ADC_CHANNEL_2_EN | ADC_CHANNEL_3_EN | \
                                                            ADC_CHANNEL_4_EN | ADC_CHANNEL_5_EN | ADC_CHANNEL_6_EN | ADC_CHANNEL_7_EN | \
                                                            ADC_CHANNEL_8_EN | ADC_CHANNEL_9_EN | ADC_CHANNEL_10_EN | ADC_CHANNEL_11_EN | \
                                                            ADC_CHANNEL_14_EN | ADC_CHANNEL_15_EN | \
                                                            ADC_CHANNEL_16_EN | ADC_CHANNEL_17_EN | ADC_CHANNEL_18_EN; 

    ADC_RegularHandle.Instance                          = ADC1;  

    HAL_ADC_Init(&ADC_RegularHandle);


    
    /* The total adc regular channels number */
    ADC_RegularHandle.ChannelNum                        = 1;

    //offset config
    ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE; //偏移通道选择
    ADC_ChannelConf.Offset                              = 0; //偏移量
    ADC_ChannelConf.OffsetCalculate                     = 0; //偏移计算方式
    ADC_ChannelConf.Offsetsign                          = 0;//偏移结果格式选择
    
    ADC_ChannelConf.Diff                                = ENABLE; //差分模式使能
    
     /* Add adc channels */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_5;
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;  //转换顺序
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;//采样时间
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);

    //转换后的值 = 2048 × [1 + (VINP-VINN) / (VREF+)]
   
    while(1)
    {
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            adcxValBuffer[i] = 0;
        }

        HAL_ADC_Polling(&ADC_RegularHandle, adcxValBuffer, ADC_RegularHandle.ChannelNum, 0);
       
        printfS("\r\n************************************************************\r\n");
        for (i = 0; i < ADC_RegularHandle.ChannelNum; i++)
        {
            printfS("[%d]Channel %d = 0x%x(%d)\r\n", i, (adcxValBuffer[i] >> 16 & 0xFF), (adcxValBuffer[i] & 0xFFF), (adcxValBuffer[i] & 0xFFF));
        }
        printfS("ADC->CALFACT = 0x%x\r\n", ADC_RegularHandle.Instance->CALFACT);
        printfS("\r\n************************************************************\r\n");
        HAL_DelayMs(1500);     
    }
}
