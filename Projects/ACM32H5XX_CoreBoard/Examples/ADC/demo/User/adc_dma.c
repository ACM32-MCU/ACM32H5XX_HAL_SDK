
#include "hal.h" 




extern ADC_HandleTypeDef ADC_RegularHandle;

static DMA_HandleTypeDef ADC_DMAHandle = {0};


static uint32_t gadcxValBuffer[21];             // ADC Value
static volatile uint32_t gadcxITCConunt = 0;    // transfer complete interrupt count


void DMA2_CH1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&ADC_DMAHandle);
}


/******************************************************************************
* @brief : DMA adc to memory transfer complete interrupt Callback.
* @param : hdma: pointer to a DMA_HandleTypeDef structure that contains
*                 the configuration information for DMA module.
* @return: none
******************************************************************************/ 
static void DMA_ADC_ITC_Callback(DMA_HandleTypeDef *hdma)
{    
    gadcxITCConunt = 1;
}

/******************************************************************************
* @brief : ADC DMA Mode Test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_DMA_Test(void)
{
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0};
    
    printfS("The ADC_DMA_Test start\r\n");

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
    ADC_RegularHandle.Init.DMAMode                      = ENABLE; 
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
    ADC_RegularHandle.Instance                          = ADC2;  

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



    /*## DMA配置 ##*/
    __HAL_RCC_DMA2_CLK_ENABLE();
    ADC_DMAHandle.Instance          = DMA2_Channel1; 
    ADC_DMAHandle.Init.Mode         = DMA_MODE_CIRCULAR;    
    ADC_DMAHandle.Init.ReqID        = DMA2_REQ_ADC2;         
    ADC_DMAHandle.Init.DataFlow     = DMA_DATAFLOW_P2M;
    ADC_DMAHandle.Init.SrcIncDec    = DMA_SRCINCDEC_DISABLE;
    ADC_DMAHandle.Init.DestIncDec   = DMA_DESTINCDEC_INC;
    ADC_DMAHandle.Init.SrcWidth     = DMA_SRCWIDTH_WORD;   
    ADC_DMAHandle.Init.DestWidth    = DMA_DESTWIDTH_WORD;
    ADC_DMAHandle.Init.SrcBurst     = DMA_SRCBURST_1;
    ADC_DMAHandle.Init.DestBurst    = DMA_DESTBURST_1;    
    ADC_DMAHandle.XferCpltCallback  = DMA_ADC_ITC_Callback;

    HAL_DMA_Init(&ADC_DMAHandle);
    
    ADC_RegularHandle.DMA_Handle    = &ADC_DMAHandle;
    
    NVIC_ClearPendingIRQ(DMA2_CH1_IRQn);
    NVIC_EnableIRQ(DMA2_CH1_IRQn);

    
    HAL_ADC_Start_DMA(&ADC_RegularHandle, gadcxValBuffer, ADC_RegularHandle.ChannelNum);
    
    while(1)
    {   
        HAL_ADC_Start(&ADC_RegularHandle);
        while(!gadcxITCConunt);
        gadcxITCConunt = 0;
        printfS("Channel %d = 0x%x(%d)\r\n", (gadcxValBuffer[0] >> 16 & 0xFF), (gadcxValBuffer[0] & 0xFFF), (gadcxValBuffer[0] & 0xFFF));       
        HAL_DelayMs(1000);  
    }
}


