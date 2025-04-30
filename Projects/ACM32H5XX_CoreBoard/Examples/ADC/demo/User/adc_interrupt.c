


#include "hal.h" 




extern ADC_HandleTypeDef ADC_RegularHandle;

static volatile uint32_t gadcxConvertedValue;
static volatile uint32_t gadcxEocFlag = 0;
static volatile uint32_t gadcxEogFlag = 0;




/******************************************************************************
* @brief : ADC interrupt callback function.
* @param : none
* @return: none
******************************************************************************/ 
void HAL_ADC_IRQCallback(ADC_HandleTypeDef* hadc)
{
    /************ Check End of Conversion flag for injected ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_JEOC))
    {   
        /* Conversion complete callback */ 
        if (NULL != hadc->InjectedConvCpltCallback)
        {
            hadc->InjectedConvCpltCallback(hadc);
        }

        /* Clear injected group conversion flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_JEOC);
        
    }

    /************ Check End of Conversion flag for injected group ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_JEOG))
    {	
        if((((hadc->Instance->JSQR & ADC_JSQR_JEXTEN) == 0)    || \
        ((hadc->Instance->CR1 & ADC_CR1_EXTEN_Msk) == 0) && (hadc->Init.ConConvMode == DISABLE)) \
        && ((hadc->Instance->CR1 & ADC_CR1_JAUTO) == 0))
        {
            /* Disable ADC end of conversion interrupt on injected injected */
            HAL_ADC_ITConfig(hadc->Instance, ADC_IT_FLAG_JEOC, DISABLE);

        }

        /* Conversion complete callback */ 
        if (NULL != hadc->InjectedGroupConvCpltCallback)
            hadc->InjectedGroupConvCpltCallback(hadc);

        /* Clear injected group conversion flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_JEOG);
    }

    /************ Check Conversion flag for regular Channel ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_EOC))
    {    
        /* Conversion complete callback */
        if (NULL != hadc->ConvCpltCallback)
            hadc->ConvCpltCallback(hadc);

        /* Clear conversion flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_EOC);
    }

    /************ Check End of Conversion flag for regular group ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_EOG))
    {    
        if((((hadc->Instance->CR1 & ADC_CR1_EXTSEL_Msk) >> ADC_CR1_EXTSEL_Pos) == ADC_SOFTWARE_START) && \
        (hadc->Init.ConConvMode == DISABLE))
        {
            /* Disable ADC end of conversion interrupt on group regular */
            HAL_ADC_ITConfig(hadc->Instance, ADC_IE_EOGIE, DISABLE);
        }

        /* Conversion complete callback */
        if (NULL != hadc->GroupCpltCallback)
        hadc->GroupCpltCallback(hadc);

        /* Clear regular group conversion flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_EOG);
    }

    /************ Check Analog watchdog flags ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_AWD))
    {
        /* Level out of window callback */ 
        if (NULL != hadc->LevelOutOfWindowCallback)
        hadc->LevelOutOfWindowCallback(hadc);

        /* Clear the ADC analog watchdog flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_AWD);
    } 

    /************ Check Sampling completed flags ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_EOSMP))
    {
        /* Clear the ADC Sampling completed flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_EOSMP);
    } 

        /************ Check data overflow flags ************/
    if(SET == HAL_ADC_GetITStatus(hadc->Instance, ADC_IT_FLAG_OVERF))
    {
        /* Clear the ADC data overflow flag */
        HAL_ADC_ClearITPendingBit(hadc->Instance, ADC_IT_FLAG_OVERF);
    } 
}


/******************************************************************************
* @brief : adc channel conversion end interrupt callback function.
* @param : none
* @return: none
******************************************************************************/ 
static void ADCx_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Get the converted value of regular channel */
    gadcxConvertedValue = HAL_ADC_GetValue(hadc);
    
    gadcxEocFlag = 3;	
}

/******************************************************************************
* @brief : adc channel group conversion end interrupt callback function.
* @param : none
* @return: none
******************************************************************************/ 
static void ADCx_GroupCpltCallback(ADC_HandleTypeDef *hadc)
{
    gadcxEogFlag = 3;
}


/******************************************************************************
* @brief : ADC Interrupt Mode test.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_Interrupt_Test(void)
{
    ADC_ChannelConfTypeDef ADC_ChannelConf = {0}; 
    
    printfS("The ADC_Interrupt_Test start\r\n");

    /* ADC时钟来源 */
    ADC_RegularHandle.Init.ClockSource                  = ADC_CLOCKSOURCE_HCLK;    
    /* ADC_CLK分频选择 */
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
    /* DMA选择 */
    ADC_RegularHandle.Init.DMAMode                      = DISABLE;
    /* 溢出时是否保留上次采样数据 */          
    ADC_RegularHandle.Init.OverMode                     = DISABLE;        
    /* 过采样使能 */
    ADC_RegularHandle.Init.OverSampMode                 = DISABLE;
    /* 过采样率 */
    ADC_RegularHandle.Init.Oversampling.Ratio           = ADC_OVERSAMPLING_RATIO_2;
    /* 过采样移位系数 */
    ADC_RegularHandle.Init.Oversampling.RightBitShift   = ADC_RIGHTBITSHIFT_2;
    /* 过采样触发模式 */ 
    ADC_RegularHandle.Init.Oversampling.TriggeredMode   = 0; 
    /* 模拟看门狗 */            
    ADC_RegularHandle.Init.AnalogWDGEn                  = DISABLE;
    /* 通道使能 */    
    ADC_RegularHandle.Init.ChannelEn                    =  ADC_CHANNEL_1_EN | ADC_CHANNEL_2_EN | ADC_CHANNEL_3_EN | \
                                                        ADC_CHANNEL_4_EN | ADC_CHANNEL_5_EN | ADC_CHANNEL_6_EN | ADC_CHANNEL_7_EN | \
                                                        ADC_CHANNEL_8_EN | ADC_CHANNEL_9_EN | ADC_CHANNEL_10_EN | ADC_CHANNEL_11_EN | \
                                                        ADC_CHANNEL_14_EN | ADC_CHANNEL_15_EN | \
                                                        ADC_CHANNEL_16_EN | ADC_CHANNEL_17_EN | ADC_CHANNEL_18_EN; 
    /* ADC选择 */
    ADC_RegularHandle.Instance                          = ADC1;  
	
	ADC_RegularHandle.ConvCpltCallback                  = ADCx_ConvCpltCallback;
    ADC_RegularHandle.GroupCpltCallback                 = ADCx_GroupCpltCallback;
    HAL_ADC_Init(&ADC_RegularHandle);
	
	/* 规则组序列长度 */
    ADC_RegularHandle.ChannelNum                        = 1;
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_1;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;
    /* 采样时间 */
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    /* 差分模式使能 */
    ADC_ChannelConf.Diff                                = DISABLE; 
    /* 偏移组选择 */
	ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE;
    /* 偏移量 */
    ADC_ChannelConf.Offset                              = 0;
    /* 偏移计算方式 */	
    ADC_ChannelConf.OffsetCalculate                     = 0;
    /* 偏移结果格式选择 */ 
    ADC_ChannelConf.Offsetsign                          = 0;
    HAL_ADC_ConfigChannel(&ADC_RegularHandle,&ADC_ChannelConf);
       

    HAL_ADC_Start_IT(&ADC_RegularHandle);
	
    while(1)
    {
        if((gadcxEocFlag == 3) && (gadcxEogFlag == 3))
        {
            gadcxEocFlag =0;
            gadcxEogFlag =0;      
            printfS("Channel %d = 0x%x(%d)\r\n", (gadcxConvertedValue >> 16 & 0xFF), (gadcxConvertedValue & 0xFFF), (gadcxConvertedValue & 0xFFF));
            HAL_DelayMs(1000);
            HAL_ADC_Start_IT(&ADC_RegularHandle);            
        }
        
    }
}

