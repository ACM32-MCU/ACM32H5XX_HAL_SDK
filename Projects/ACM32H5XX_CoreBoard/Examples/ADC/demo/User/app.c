
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h" 



extern void ADC_Regular_Polling_Nchannels_Test(void);
extern void ADC_Inject_Polling_Nchannels_Test(void);
extern void ADC_Interrupt_Test(void);
extern void ADC_External_Trigger_Test(void);
extern void ADC_DMA_Test(void);
extern void ADC_Analog_Watchdog_Test(void);
extern void Dual_ADC_Test(void);
extern void ADC_Differential_Nchannel_Test(void);
extern void ADC_Regular_Polling_Nchannels_ContinuousMode_Test(void);

ADC_HandleTypeDef ADC_RegularHandle = {0};

#define Hardware_calibration    (0)
#define Software_calibration    (1)


/******************************************************************************
* @brief : ADC Calibration.
* @param : ADCx: ADC1 or ADC2 or ADC3
* @param : diff: Single ended or differential mode
* @param : software: Software or hardware calibration
* @return: none
******************************************************************************/ 
void ADC_Calibration(ADC_TypeDef * ADCx, uint8_t diff, uint8_t software)
{
    uint32_t tempbuf[1];
    uint8_t temp;
    ADC_HandleTypeDef ADC_Handle_Cal = { 0 };
    ADC_ChannelConfTypeDef ADC_ChannelConf;
    
    ADC_Handle_Cal.Init.ClockSource                   = ADC_CLOCKSOURCE_HCLK;           //ADC时钟来源
    ADC_Handle_Cal.Init.ClockPrescaler                = 16;                             //ADC_CLK分频选择
    ADC_Handle_Cal.Init.Resolution                    = ADC_RESOLUTION_12B;             //分辨率
    ADC_Handle_Cal.Init.DataAlign                     = ADC_DATAALIGN_RIGHT;            //数据对齐
    ADC_Handle_Cal.Init.ConConvMode                   = DISABLE;                        //连续转换模式

    ADC_Handle_Cal.Init.DiscontinuousConvMode         = ENABLE;                        //间断模式
    ADC_Handle_Cal.Init.NbrOfDiscConversion           = 0;                              //间断模式通道计数
    ADC_Handle_Cal.Init.ExternalTrigConv              = ADC_SOFTWARE_START;             //触发模式：外部触发或软件触发
    ADC_Handle_Cal.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_NONE;  //外部触发边沿选择
    ADC_Handle_Cal.Init.DMAMode                       = DISABLE;                        //DMA选择
    ADC_Handle_Cal.Init.OverMode                      = DISABLE;                        //溢出时是否保留上次采样数据

    ADC_Handle_Cal.Init.OverSampMode                  = DISABLE;                        //过采样使能
    ADC_Handle_Cal.Init.Oversampling.Ratio            = ADC_OVERSAMPLING_RATIO_2;       //过采样率
    ADC_Handle_Cal.Init.Oversampling.RightBitShift    = ADC_RIGHTBITSHIFT_2;            //过采样移位系数
    ADC_Handle_Cal.Init.Oversampling.TriggeredMode    = 0;                              //过采样触发模式    
    ADC_Handle_Cal.Init.AnalogWDGEn                   = DISABLE;                        //模拟看门狗
    ADC_Handle_Cal.Init.ChannelEn                     = ADC_CHANNEL_0_EN; 

    ADC_Handle_Cal.Instance                           = ADCx;  

    HAL_ADC_Init(&ADC_Handle_Cal);

    /* The total adc regular channels number */
    ADC_Handle_Cal.ChannelNum                           = 1;
    
    //offset config
    ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE;                 //偏移通道选择
    ADC_ChannelConf.Offset                              = 0;                            //偏移量
    ADC_ChannelConf.OffsetCalculate                     = 0;                            //偏移计算方式
    ADC_ChannelConf.Offsetsign                          = 0;                            //偏移结果格式选择
    
    ADC_ChannelConf.Diff                                = DISABLE;
    
     /* Add adc channels */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_0;
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;             //转换顺序
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_640;             //采样时间
    HAL_ADC_ConfigChannel(&ADC_Handle_Cal,&ADC_ChannelConf);

    temp = 0x7f;
    ADC_Handle_Cal.Instance->CALFACT = temp;
    
    if(software == Software_calibration)
    {
        /* 软件校准 */ 
        while(1)
        {
            HAL_ADC_Polling(&ADC_Handle_Cal, tempbuf, ADC_Handle_Cal.ChannelNum, 0); 
            if((tempbuf[0] & 0x7f) == 0)
            {
                printfS("ADC->DR = 0x%x\r\n", ADC_Handle_Cal.Instance->DR & 0x7f);
                printfS("ADC->CR2 = 0x%x\r\n", ADC_Handle_Cal.Instance->CR2);
                printfS("ADC->CALFACT = 0x%x\r\n", ADC_Handle_Cal.Instance->CALFACT); 
                return;            
            }
            else
            {
                temp--;
                if(temp == 0)
                {
                    printfS("ADC->DR = 0x%x\r\n", ADC_Handle_Cal.Instance->DR & 0x7f);
                    break;               
                }
                ADC_Handle_Cal.Instance->CALFACT = temp;
                printfS("[%d]ADC->CALFACT = 0x%x\r\n", temp, ADC_Handle_Cal.Instance->CALFACT);
                printfS("ADC->DR = 0x%x\r\n", ADC_Handle_Cal.Instance->DR & 0x7f); 
            }
        }
        temp = 0;
        ADC_Handle_Cal.Instance->CALFACT = temp;
        HAL_ADC_Polling(&ADC_Handle_Cal, tempbuf, ADC_Handle_Cal.ChannelNum, 0);
        printfS("[%d]ADC->CALFACT = 0x%x\r\n", temp, ADC_Handle_Cal.Instance->CALFACT);
        printfS("ADC->DR = 0x%x\r\n", ADC_Handle_Cal.Instance->DR & 0x7f); 
    }        
    else
    {
        /* 硬件校准 */
        //将在单端输入模式下启动校准
        ADC_Handle_Cal.Instance->CR2 &= ~ADC_CR2_ADCCALDIF;  
        if(ADC_DIFF_DIFFERENTIAL == diff)
        {
            //将在差分输入模式下启动校准
            ADC_Handle_Cal.Instance->CR2 |= ADC_CR2_ADCCALDIF;  
        }
        //校准ADC
        ADC_Handle_Cal.Instance->CR2 |= ADC_CR2_ADCCAL;
        /* Enable the ADC */
        SET_BIT(ADC_Handle_Cal.Instance->CR2, (ADC_CR2_EN));
        /* Wait ADC ready */
        while(!(ADC_Handle_Cal.Instance->SR & ADC_SR_ADRDY));

        //等待校准完成
        while((ADC_Handle_Cal.Instance->CR2 & ADC_CR2_ADCCAL) == ADC_CR2_ADCCAL);
        printfS("[%d]ADC->CALFACT = 0x%x\r\n", temp, ADC_Handle_Cal.Instance->CALFACT);
        printfS("ADC->DR = 0x%x\r\n", ADC_Handle_Cal.Instance->DR & 0x7f); 
    }
}


/******************************************************************************
* @brief : ADC Get The VrefP Value.
* @param : none
* @return: VrefP Value
******************************************************************************/ 
uint32_t ADC_GetVrefP(void)
{
    ADC_HandleTypeDef ADC_Handle_Vrefp = { 0 };
    ADC_ChannelConfTypeDef ADC_ChannelConf;
    uint32_t AdcValue_VrefP[1], temp, VrefP = 0;
    uint16_t TrimValue_3v;
    
    /* 自动校准 */
    ADC_Calibration(ADC2, ADC_DIFF_SINGLE, Hardware_calibration); 
    
    /* ADC时钟来源 */
    ADC_Handle_Vrefp.Init.ClockSource                   = ADC_CLOCKSOURCE_HCLK; 
    /* ADC_CLK分频 */
    ADC_Handle_Vrefp.Init.ClockPrescaler                = 16;
    /* 分辨率 */      
    ADC_Handle_Vrefp.Init.Resolution                    = ADC_RESOLUTION_12B;
    /* 数据对齐 */    
    ADC_Handle_Vrefp.Init.DataAlign                     = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */   
    ADC_Handle_Vrefp.Init.ConConvMode                   = DISABLE;               
    /* 间断模式 */
    ADC_Handle_Vrefp.Init.DiscontinuousConvMode         = DISABLE;
    /* 间断模式通道计数 */        
    ADC_Handle_Vrefp.Init.NbrOfDiscConversion           = 0;
    /* 触发模式：外部触发或软件触发 */               
    ADC_Handle_Vrefp.Init.ExternalTrigConv              = ADC_SOFTWARE_START;
    /* 外部触发边沿 */  
    ADC_Handle_Vrefp.Init.ExternalTrigConvEdge          = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA使能 */
    ADC_Handle_Vrefp.Init.DMAMode                       = DISABLE;
    /* 溢出时是否保留上次采样数据 */          
    ADC_Handle_Vrefp.Init.OverMode                      = DISABLE;        
    /* 过采样使能 */
    ADC_Handle_Vrefp.Init.OverSampMode                  = DISABLE;
    /* 过采样率 */
    ADC_Handle_Vrefp.Init.Oversampling.Ratio            = ADC_OVERSAMPLING_RATIO_2;
    /* 过采样移位系数 */
    ADC_Handle_Vrefp.Init.Oversampling.RightBitShift    = ADC_RIGHTBITSHIFT_2;
    /* 过采样触发模式 */
    ADC_Handle_Vrefp.Init.Oversampling.TriggeredMode    = 0;
    /* 模拟看门狗 */              
    ADC_Handle_Vrefp.Init.AnalogWDGEn                   = DISABLE;
    /* 通道使能 */    
    ADC_Handle_Vrefp.Init.ChannelEn                     = ADC_CHANNEL_VREF_EN; 
    /* ADC选择 */
    ADC_Handle_Vrefp.Instance                           = ADC2;  

    HAL_ADC_Init(&ADC_Handle_Vrefp);

    /* 规则组序列长度 */
    ADC_Handle_Vrefp.ChannelNum                         = 1;
    
    /* 偏移组选择 */
    ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE;    
    /* 偏移量 */
    ADC_ChannelConf.Offset                              = 0;
    /* 偏移计算方式 */ 
    ADC_ChannelConf.OffsetCalculate                     = 0;
    /* 偏移结果格式选择 */ 
    ADC_ChannelConf.Offsetsign                          = 0;
    /* 差分模式 */
    ADC_ChannelConf.Diff                                = DISABLE;
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_VREF;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;
    /* 采样时间 */  
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    
    HAL_ADC_ConfigChannel(&ADC_Handle_Vrefp,&ADC_ChannelConf);

    HAL_ADC_Polling(&ADC_Handle_Vrefp, AdcValue_VrefP, ADC_Handle_Vrefp.ChannelNum, 0);
       
    printfS("BGR: Channel %d = 0x%08x(%d)\r\n", AdcValue_VrefP[0]>>16, AdcValue_VrefP[0] & 0xFFF, AdcValue_VrefP[0] & 0xFFF);

    HAL_EFUSE_ReadBytes(EFUSE1, EFUSE1_ADC_1V2, (uint8_t *)&TrimValue_3v, 2, 100000);//Read the 1.2v trim value in 3.0v vrefp.
    
    printfS("The adc 1.2v trim value is : 0x%04x \r\n", TrimValue_3v);
    if(TrimValue_3v)
    {
        temp = TrimValue_3v & 0xFFF;
        VrefP = (uint32_t)(temp * 3000 / (AdcValue_VrefP[0] & 0xFFF));
    }
    else
    {
        temp = 1638;//When the digital code of the factory ADC 1.2V is 0, it defaults to 1638(1200mV / 3000mV * 4095)
        VrefP = (uint32_t)(temp * 3000 / (AdcValue_VrefP[0] & 0xFFF));   
    }
  
    return VrefP;

}


/******************************************************************************
* @brief : ADC get the temperature sensor value.
* @param : none
* @return: none
******************************************************************************/ 
void ADC_GetTemperatureSensor(void)
{
    ADC_HandleTypeDef ADC_Handle_TS = { 0 };
    ADC_ChannelConfTypeDef ADC_ChannelConf;
    uint32_t AdcValue;
    
    /* 自动校准 */
    ADC_Calibration(ADC2, ADC_DIFF_SINGLE, Hardware_calibration); 
    
    /* ADC时钟来源 */
    ADC_Handle_TS.Init.ClockSource                      = ADC_CLOCKSOURCE_HCLK; 
    /* ADC_CLK分频 */
    ADC_Handle_TS.Init.ClockPrescaler                   = 16;
    /* 分辨率 */      
    ADC_Handle_TS.Init.Resolution                       = ADC_RESOLUTION_12B;
    /* 数据对齐 */    
    ADC_Handle_TS.Init.DataAlign                        = ADC_DATAALIGN_RIGHT;
    /* 连续转换模式 */   
    ADC_Handle_TS.Init.ConConvMode                      = DISABLE;               
    /* 间断模式 */
    ADC_Handle_TS.Init.DiscontinuousConvMode            = DISABLE;
    /* 间断模式通道计数 */        
    ADC_Handle_TS.Init.NbrOfDiscConversion              = 0;
    /* 触发模式：外部触发或软件触发 */               
    ADC_Handle_TS.Init.ExternalTrigConv                 = ADC_SOFTWARE_START;
    /* 外部触发边沿 */  
    ADC_Handle_TS.Init.ExternalTrigConvEdge             = ADC_EXTERNALTRIGCONVEDGE_NONE;
    /* DMA使能 */
    ADC_Handle_TS.Init.DMAMode                          = DISABLE;
    /* 溢出时是否保留上次采样数据 */          
    ADC_Handle_TS.Init.OverMode                         = DISABLE;        
    /* 过采样使能 */
    ADC_Handle_TS.Init.OverSampMode                     = DISABLE;
    /* 过采样率 */
    ADC_Handle_TS.Init.Oversampling.Ratio               = ADC_OVERSAMPLING_RATIO_2;
    /* 过采样移位系数 */
    ADC_Handle_TS.Init.Oversampling.RightBitShift       = ADC_RIGHTBITSHIFT_2;
    /* 过采样触发模式 */
    ADC_Handle_TS.Init.Oversampling.TriggeredMode       = 0;
    /* 模拟看门狗 */              
    ADC_Handle_TS.Init.AnalogWDGEn                      = DISABLE;
    /* 通道使能 */    
    ADC_Handle_TS.Init.ChannelEn                        = ADC_CHANNEL_TEMP_EN; 
    /* ADC选择 */
    ADC_Handle_TS.Instance                              = ADC3;  

    HAL_ADC_Init(&ADC_Handle_TS);

    /* 规则组序列长度 */
    ADC_Handle_TS.ChannelNum                            = 1;
    
    /* 偏移组选择 */
    ADC_ChannelConf.OffsetNumber                        = ADC_OFR_NONE;    
    /* 偏移量 */
    ADC_ChannelConf.Offset                              = 0;
    /* 偏移计算方式 */ 
    ADC_ChannelConf.OffsetCalculate                     = 0;
    /* 偏移结果格式选择 */ 
    ADC_ChannelConf.Offsetsign                          = 0;
    /* 差分模式 */
    ADC_ChannelConf.Diff                                = DISABLE;
    
    /* 转换通道 */
    ADC_ChannelConf.Channel                             = ADC_CHANNEL_TEMP;
    /* 转换顺序 */
    ADC_ChannelConf.Sq                                  = ADC_SEQUENCE_SQ1;
    /* 采样时间 */  
    ADC_ChannelConf.Smp                                 = ADC_SMP_CLOCK_320;
    
    HAL_ADC_ConfigChannel(&ADC_Handle_TS,&ADC_ChannelConf);
    
    while(1)
    {
        AdcValue = 0;
        HAL_ADC_Polling(&ADC_Handle_TS, &AdcValue, ADC_Handle_TS.ChannelNum, 0);
        printfS("TS: Channel %d = 0x%08x(%d)\r\n", (AdcValue >> 16), (AdcValue & 0xFFF), (AdcValue & 0xFFF));
        HAL_Delay(1000);
    }

}


/******************************************************************************
*@brief : ADC test
*@param : test_mode: Test mode selection
*@return: none
******************************************************************************/
void ADC_Test(Test_mode_t test_mode)
{
    uint32_t vrefp, ret;
    
    /* Select Vrefp */
    HAL_ADC_VrefpConfig(VREFP_EXTERN, VREFP_EXTERN_VREF);//External VREF+
    
    /* factory basic parameter settings */
    ret = HAL_ADC_FactoryBasicParameterConfig();
    if(ret == HAL_ERROR)
    {
        /* Automatic calibration */
        ADC_Calibration(ADC1, ADC_DIFF_SINGLE, Hardware_calibration); 
        ADC_Calibration(ADC2, ADC_DIFF_SINGLE, Hardware_calibration); 
        ADC_Calibration(ADC3, ADC_DIFF_SINGLE, Hardware_calibration);
    }
    vrefp = ADC_GetVrefP();
    printfS("vrefp = %d mV\n", vrefp);    
    
    switch(test_mode)
    {
        case Test_ADC_Regular:
        {
            ADC_Regular_Polling_Nchannels_Test();
        }break;
        case Test_ADC_Inject:
        {
            ADC_Inject_Polling_Nchannels_Test();           
        }break;
        case Test_ADC_Interrupt:
        {
            ADC_Interrupt_Test();           
        }break;
        case Test_ADC_External_Trigger:
        {
            ADC_External_Trigger_Test();           
        }break;
        case Test_ADC_DMA:
        {
            ADC_DMA_Test();           
        }break;
        case Test_ADC_AnalogWatchdog:
        {
            ADC_Analog_Watchdog_Test();           
        }break; 
        case Test_ADC_Dual:
        {
            Dual_ADC_Test();           
        }break;
        case Test_ADC_Differential:
        {
            ADC_Differential_Nchannel_Test();
        }break;
        case Test_ADC_TemperatureSensor:
        {
            ADC_GetTemperatureSensor();
        }break;
        case Test_ADC_Regular_ContinuousMode:
        {
            ADC_Regular_Polling_Nchannels_ContinuousMode_Test();
        }break;        
        default:
        {
            printfS("This application does not support\r\n");
        }break;
    }
}

/******************************************************************************
* @brief : app test.
* @param : None
* @return: None
******************************************************************************/
void APP_Test(void)
{

    /*
    Test_ADC_Regular,
	Test_ADC_Inject,
    Test_ADC_Interrupt,
    Test_ADC_External_Trigger,
    Test_ADC_DMA,
    Test_ADC_AnalogWatchdog,
    Test_ADC_Dual,
    Test_ADC_Differential,
    Test_ADC_Regular_ContinuousMode,
    */
    ADC_Test(Test_ADC_Regular_ContinuousMode);
}



