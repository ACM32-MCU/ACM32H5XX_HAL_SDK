
#include "hal.h" 



static ADC_HandleTypeDef ADC_InjectHandle = {0};

volatile uint32_t gadcxConvertedValue;
volatile uint32_t gadcxExtTrigFlag = 0;

extern ADC_HandleTypeDef DualADC1_Handle;
extern ADC_HandleTypeDef DualADC2_Handle;


/******************************************************************************
* @brief : adc channel External Trigger interrupt callback function.
* @param : none
* @return: none
******************************************************************************/ 
static void ADC_Injectr_ExternalTrigger_Callback(ADC_HandleTypeDef* hadc)
{    
    if(SET == HAL_ADC_GetFlagStatus(hadc->Instance, ADC_IT_FLAG_JEOG))
    {
        //clear
        HAL_ADC_ClearFlag(hadc->Instance, ADC_IT_FLAG_JEOG);

        gadcxConvertedValue = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);

        gadcxExtTrigFlag = 1;
    }   
}


/******************************************************************************
* @brief : EXTI interrupt callback function.
* @param : Line: EXTI line
* @return: none
******************************************************************************/ 
void HAL_EXTI_LineCallback(uint32_t Line)
{
    if((Line == 11) || (Line == 15))
    {
        if(ADC_InjectHandle.Instance != NULL)
        {
            ADC_Injectr_ExternalTrigger_Callback(&ADC_InjectHandle);
        }
        if(DualADC1_Handle.Instance != NULL)
        {

        }
        if(DualADC2_Handle.Instance != NULL)
        {

        }
    }
}



/******************************************************************************
* brief : ADC External trigger test.
* param : none
* return: none
******************************************************************************/ 
void ADC_External_Trigger_Test(void)
{
    ADC_InjectionConfTypeDef sConfigInjected = {0};
    GPIO_InitTypeDef GPIO_PINx_Handle = {0};
    
    printfS("The ADC_External_Trigger_Test start\r\n");

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
    ADC_InjectHandle.Instance                           = ADC1;

    HAL_ADC_Init(&ADC_InjectHandle);

    /* 规则组序列长度 */
    ADC_InjectHandle.ChannelNum                         = 1;

    /* 注入序列长度 */
    sConfigInjected.InjectedNbrOfConversion             = 1;
    /* 通道采样时间 */ 
    sConfigInjected.InjectedSamplingTime                = ADC_SMP_CLOCK_640; 
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
    HAL_ADCEx_InjectedConfigChannel(&ADC_InjectHandle, &sConfigInjected);
    
    
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
   
    //Start ADC 
    HAL_ADCEx_InjectedStart(&ADC_InjectHandle);
    
    printfS("Wait for EXTI15 Irq, Please press the key!\r\n");
    
    while(1)
    {  	    
        if(gadcxExtTrigFlag)
        {
            gadcxExtTrigFlag = 0; 
            printfS("Channel %d = 0x%x(%d)\r\n", (gadcxConvertedValue >> 16 & 0xFF), (gadcxConvertedValue & 0xFFF), (gadcxConvertedValue & 0xFFF));   
        }
    }
}


