
#include "hal.h"  


ADC_HandleTypeDef DualADC1_Handle = {0};
ADC_HandleTypeDef DualADC2_Handle = {0};
static DMA_HandleTypeDef ADC_DMAHandle = {0};

static uint32_t gadcxValBuffer[21];             //ADC Value
static volatile uint32_t gadcxConvertedValue;
static volatile uint32_t gadcxITCConunt = 0;    // transfer complete interrupt count
extern volatile uint32_t gadcxConvertedValue; 

void DMA1_CH3_IRQHandler(void)
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
* @brief : Dual ADC Test.
* @param : none
* @return: none
******************************************************************************/ 
void Dual_ADC_Test(void) 
{
	uint32_t i;
    GPIO_InitTypeDef GPIO_PINx_Handle = {0};
	ADC_ChannelConfTypeDef ADC_RegChannelConf = {0};
	ADC_InjectionConfTypeDef sConfigInjected = {0};
	ADC_MultiModeTypeDef Multimode = {0};
    
	/*## 配置主ADC1 ##*/
    /* ADC时钟来源 */
    DualADC1_Handle.Init.ClockSource                    = ADC_CLOCKSOURCE_HCLK;
    /* ADC_CLK分频选择 */
    DualADC1_Handle.Init.ClockPrescaler                 = 16; 
    /* 分辨率 */       
    DualADC1_Handle.Init.Resolution                     = ADC_RESOLUTION_12B;
    /* 数据对齐 */     
    DualADC1_Handle.Init.DataAlign                      = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */    
    DualADC1_Handle.Init.ConConvMode                    = DISABLE;                
    /* 间断模式 */ 
    DualADC1_Handle.Init.DiscontinuousConvMode          = DISABLE;
    /* 间断模式通道计数 */         
    DualADC1_Handle.Init.NbrOfDiscConversion            = 0;
    /* 触发模式：外部触发或软件触发 */               
    DualADC1_Handle.Init.ExternalTrigConv               = ADC_SOFTWARE_START;
    /* 外部触发边沿选择 */    
    DualADC1_Handle.Init.ExternalTrigConvEdge           = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA选择 */ 
    DualADC1_Handle.Init.DMAMode                        = ENABLE;
    /* 溢出时是否保留上次采样数据 */          
    DualADC1_Handle.Init.OverMode                       = DISABLE;         
    /* 过采样使能 */
    DualADC1_Handle.Init.OverSampMode                   = DISABLE;
    /* 过采样率 */ 
    DualADC1_Handle.Init.Oversampling.Ratio             = ADC_OVERSAMPLING_RATIO_2;
    /* 过采样移位系数 */ 
    DualADC1_Handle.Init.Oversampling.RightBitShift     = ADC_RIGHTBITSHIFT_2;
    /* 过采样触发模式 */    
    DualADC1_Handle.Init.Oversampling.TriggeredMode     = 0;
    /* 模拟看门狗 */            
    DualADC1_Handle.Init.AnalogWDGEn                    = DISABLE;
    /* 通道使能 */    
    DualADC1_Handle.Init.ChannelEn                      =  ADC_CHANNEL_1_EN | ADC_CHANNEL_2_EN | ADC_CHANNEL_3_EN | \
                                                        ADC_CHANNEL_4_EN | ADC_CHANNEL_5_EN | ADC_CHANNEL_6_EN | ADC_CHANNEL_7_EN | \
                                                        ADC_CHANNEL_8_EN | ADC_CHANNEL_9_EN | ADC_CHANNEL_10_EN | ADC_CHANNEL_11_EN | \
                                                        ADC_CHANNEL_14_EN | ADC_CHANNEL_15_EN | \
                                                        ADC_CHANNEL_16_EN | ADC_CHANNEL_17_EN | ADC_CHANNEL_18_EN; 

    /* ADC选择 */
    DualADC1_Handle.Instance                            = ADC1;  
        
    HAL_ADC_Init(&DualADC1_Handle);

    /* 规则组序列长度 */
    DualADC1_Handle.ChannelNum                          = 1;
    
    /* 偏移通道选择 */
    ADC_RegChannelConf.OffsetNumber                     = ADC_OFR_NONE;
    /* 偏移量 */   
    ADC_RegChannelConf.Offset                           = 0;
    /* 偏移计算方式 */ 
    ADC_RegChannelConf.OffsetCalculate                  = 0;
    /* 偏移结果格式选择 */  
    ADC_RegChannelConf.Offsetsign                       = 0;
    /* 单端/差分模式 */ 
    ADC_RegChannelConf.Diff                             = DISABLE;
    /* 采样时间 */
    ADC_RegChannelConf.Smp                              = ADC_SMP_CLOCK_320;
    
    /* 转换通道 */
    ADC_RegChannelConf.Channel                          = ADC_CHANNEL_4;
    /* 转换顺序 */
    ADC_RegChannelConf.Sq                               = ADC_SEQUENCE_SQ1;
    HAL_ADC_ConfigChannel(&DualADC1_Handle,&ADC_RegChannelConf);

    /* 注入序列长度 */
    sConfigInjected.InjectedNbrOfConversion             = 1; 
    /* 通道采样时间 */  
    sConfigInjected.InjectedSamplingTime                = ADC_SMP_CLOCK_320;
    /* 注入通道单端/差分模式 */   
    sConfigInjected.InjectedDiff                        = DISABLE;      
    /* 注入通道触发模式 */     
    sConfigInjected.ExternalTrigInjecConv               = ADC_EXTERNAL_TIG6;
    /* 注入通道外部触发边沿选择 */ 
    sConfigInjected.ExternalTrigInjecConvEdge           = ADC_EXTERNALTRIGCONVEDGE_RISING;
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
    if(HAL_ADCEx_InjectedConfigChannel(&DualADC1_Handle, &sConfigInjected) != HAL_OK)
    {
        /* Channel Configuration Error */
        printfS("Inject Channel3 Configuration Error \r\n");
    }
   
	/*## 配置从ADC2 ##*/
    /* ADC时钟来源 */
    DualADC2_Handle.Init.ClockSource                    = ADC_CLOCKSOURCE_HCLK;
    /* ADC_CLK分频选择 */
    DualADC2_Handle.Init.ClockPrescaler                 = 16;
    /* 分辨率 */        
    DualADC2_Handle.Init.Resolution                     = ADC_RESOLUTION_12B;
    /* 数据对齐 */     
    DualADC2_Handle.Init.DataAlign                      = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */     
    DualADC2_Handle.Init.ConConvMode                    = DISABLE;               
    /* 间断模式 */
    DualADC2_Handle.Init.DiscontinuousConvMode          = DISABLE;
    /* 间断模式通道计数 */          
    DualADC2_Handle.Init.NbrOfDiscConversion            = 0;
    /* 触发模式：外部触发或软件触发 */                
    DualADC2_Handle.Init.ExternalTrigConv               = ADC_SOFTWARE_START;
    /* 外部触发边沿选择 */   
    DualADC2_Handle.Init.ExternalTrigConvEdge           = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA选择 */
    DualADC2_Handle.Init.DMAMode                        = ENABLE;
    /* 溢出时是否保留上次采样数据 */           
    DualADC2_Handle.Init.OverMode                       = DISABLE;         
    /* 过采样使能 */
    DualADC2_Handle.Init.OverSampMode                   = DISABLE; 
    /* 过采样率 */
    DualADC2_Handle.Init.Oversampling.Ratio             = ADC_OVERSAMPLING_RATIO_2;
    /* 过采样移位系数 */  
    DualADC2_Handle.Init.Oversampling.RightBitShift     = ADC_RIGHTBITSHIFT_2;
    /* 过采样触发模式 */ 
    DualADC2_Handle.Init.Oversampling.TriggeredMode     = 0;
    /* 模拟看门狗 */               
    DualADC2_Handle.Init.AnalogWDGEn                    = DISABLE;
    /* 通道使能 */ 
    DualADC2_Handle.Init.ChannelEn                      = ADC_CHANNEL_1 | ADC_CHANNEL_2 | ADC_CHANNEL_3 | \
                                                        ADC_CHANNEL_4 | ADC_CHANNEL_5 | ADC_CHANNEL_6 | ADC_CHANNEL_7 | \
                                                        ADC_CHANNEL_8 | ADC_CHANNEL_9 | ADC_CHANNEL_10 | ADC_CHANNEL_11 | \
                                                        ADC_CHANNEL_14 | ADC_CHANNEL_15 | ADC_CHANNEL_18; 
    /* ADC选择 */
    DualADC2_Handle.Instance                            = ADC2;  
        
    HAL_ADC_Init(&DualADC2_Handle);

    /* 规则组序列长度 */
    DualADC2_Handle.ChannelNum                          = 1;
    
	/* 偏移组选择 */
    ADC_RegChannelConf.OffsetNumber                     = ADC_OFR_NONE;
    /* 偏移量 */
    ADC_RegChannelConf.Offset                           = 0;
    /* 偏移计算方式 */  
    ADC_RegChannelConf.OffsetCalculate                  = 0;
    /* 偏移结果格式选择 */   
    ADC_RegChannelConf.Offsetsign                       = 0; 
    /* 单端/差分模式 */
    ADC_RegChannelConf.Diff                             = DISABLE;
    /* 采样时间 */
    ADC_RegChannelConf.Smp                              = ADC_SMP_CLOCK_320;   
    /* 转换通道 */
    ADC_RegChannelConf.Channel                          = ADC_CHANNEL_5;
    /* 转换顺序 */
    ADC_RegChannelConf.Sq                               = ADC_SEQUENCE_SQ1;
    HAL_ADC_ConfigChannel(&DualADC2_Handle,&ADC_RegChannelConf);

    /* 注入序列长度 */
    sConfigInjected.InjectedNbrOfConversion             = 1;
    /* 通道采样时间 */   
    sConfigInjected.InjectedSamplingTime                = ADC_SMP_CLOCK_320;
    /* 注入通道单端/差分模式 */   
    sConfigInjected.InjectedDiff                        = DISABLE;      
    /* 注入通道触发模式 */    
    sConfigInjected.ExternalTrigInjecConv               = ADC_EXTERNAL_TIG6;
    /* 注入通道外部触发边沿选择 */  
    sConfigInjected.ExternalTrigInjecConvEdge           = ADC_EXTERNALTRIGCONVEDGE_RISING;
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
    sConfigInjected.InjectedChannel                     = ADC_CHANNEL_11;
    /* 注入序列转换顺序 */
    sConfigInjected.InjectedRank                        = ADC_INJECTED_RANK_1;    
    if(HAL_ADCEx_InjectedConfigChannel(&DualADC2_Handle, &sConfigInjected) != HAL_OK)
    {
        /* Channel Configuration Error */
        printfS("Inject Channel15 Configuration Error \r\n");
    }
	
	/*## 多模式配置 ##*/
    /* 双ADC模式选择 */
	Multimode.Mode                                      = ADC_DUALMODE_REGSIMULT_INJECSIMULT;
    /* 双ADC下DMA功能选择 */ 
	Multimode.DMAAccessMode                             = ADC_DMAACCESSMODE_12_10_BITS;
    /* 2个采样阶段之间的延迟 */
	Multimode.TwoSamplingDelay                          = ADC_TWOSAMPLINGDELAY_5CYCLES;
	if(HAL_ADCEx_MultiModeConfigChannel(&DualADC1_Handle, &Multimode) != HAL_OK)
    {
        /* Channel Configuration Error */
        printfS("Multimode Configuration Error \r\n");
    }
	

	/*## DMA配置 ##*/
    //ADC1   
    __HAL_RCC_DMA1_CLK_ENABLE();
    ADC_DMAHandle.Instance          = DMA1_Channel3; 
    ADC_DMAHandle.Init.Mode         = DMA_MODE_CIRCULAR;    
    ADC_DMAHandle.Init.ReqID        = DMA2_REQ_ADC1;         
    ADC_DMAHandle.Init.DataFlow     = DMA_DATAFLOW_P2M;
    ADC_DMAHandle.Init.SrcIncDec    = DMA_SRCINCDEC_DISABLE;
    ADC_DMAHandle.Init.DestIncDec   = DMA_DESTINCDEC_INC;
    ADC_DMAHandle.Init.SrcWidth     = DMA_SRCWIDTH_WORD;   
    ADC_DMAHandle.Init.DestWidth    = DMA_DESTWIDTH_WORD;
    ADC_DMAHandle.Init.SrcBurst     = DMA_SRCBURST_1;
    ADC_DMAHandle.Init.DestBurst    = DMA_DESTBURST_1;    
    ADC_DMAHandle.XferCpltCallback  = DMA_ADC_ITC_Callback;

    HAL_DMA_Init(&ADC_DMAHandle);
    DualADC1_Handle.DMA_Handle      = &ADC_DMAHandle;

    NVIC_ClearPendingIRQ(DMA1_CH3_IRQn);
    NVIC_EnableIRQ(DMA1_CH3_IRQn);

    /*---------------------------EXTI_Line15-----------------------------*/ 
    __HAL_RCC_EXTI_RESET();
    __HAL_RCC_EXTI_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();   
	/* Initialization GPIO */
    GPIO_PINx_Handle.Pin       = GPIO_PIN_15;
    GPIO_PINx_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_PINx_Handle.Pull      = GPIO_NOPULL;
	GPIO_PINx_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(GPIOB, &GPIO_PINx_Handle);
    
    /* Config EXTI */
    HAL_EXTI_SetConfigLine((GPIO_TypeDef *)GPIOB, EXTI_LINE_15, EXTI_MODE_IT_RISING);
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
	
    /* Enable the ADC */
    SET_BIT(DualADC1_Handle.Instance->CR2, ADC_CR2_EN);
    /* Wait ADC ready */
    while(!(DualADC1_Handle.Instance->SR & ADC_SR_ADRDY));
    /* Enable the ADC */
    SET_BIT(DualADC2_Handle.Instance->CR2, (ADC_CR2_EN));	
    /* Wait ADC ready */
    while(!(DualADC2_Handle.Instance->SR & ADC_SR_ADRDY));    
	while(1)
	{
        //DMA_Common
        HAL_ADCEx_MultiModeStart_DMA(&DualADC1_Handle, gadcxValBuffer, DualADC1_Handle.ChannelNum);
        //规则组软件触发  
        ADC1->CR1 |= ADC_CR1_SWSTART;
        //注入组软件触发  
        //ADC1->CR1 |= ADC_CR1_JSWSTART;

        while(!gadcxITCConunt);
        gadcxITCConunt =0;
        printfS("\r\n*************************************\r\n");
        for (i = 0; i < DualADC1_Handle.ChannelNum; i++)
        {
            printfS("ADC2_CH5 = 0x%x, ADC1_CH4 = 0x%x, ADC_CDR = 0x%08x \r\n", (ADC12_COMMON->CDR >> 16 & 0xffff), ADC12_COMMON->CDR & 0xffff,gadcxValBuffer[i]);            
        }

        while(!READ_BIT(DualADC1_Handle.Instance->SR,ADC_SR_JEOG)){};
        SET_BIT(DualADC1_Handle.Instance->SR,ADC_SR_JEOG);			
        gadcxConvertedValue = HAL_ADCEx_InjectedGetValue(&DualADC1_Handle,ADC_INJECTED_RANK_1);
        printfS("ADC1 Inject Channel %d = 0x%x\r\n", (gadcxConvertedValue >> 16 & 0xFF), (gadcxConvertedValue & 0xfff));

        while(!READ_BIT(DualADC2_Handle.Instance->SR,ADC_SR_JEOG)){};
        SET_BIT(DualADC2_Handle.Instance->SR,ADC_SR_JEOG);			
        gadcxConvertedValue = HAL_ADCEx_InjectedGetValue(&DualADC2_Handle,ADC_INJECTED_RANK_1);
        printfS("ADC2 Inject Channel %d = 0x%x\r\n", (gadcxConvertedValue >> 16 & 0xFF), (gadcxConvertedValue & 0xfff));
        printfS("\r\n*************************************\r\n");


	}
}



