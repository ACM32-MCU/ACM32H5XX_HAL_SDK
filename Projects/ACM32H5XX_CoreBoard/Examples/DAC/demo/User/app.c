
/******************************************************************************
*@file  : app.c
*@brief : application
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "app.h" 

DAC_HandleTypeDef hdac1;
#ifdef HAL_DMA_MODULE_ENABLED
DMA_HandleTypeDef hdma_dac1_ch1;
DMA_HandleTypeDef hdma_dac1_ch2;
#endif
TIM_HandleTypeDef TIM6_Handler; 
static volatile uint32_t gtimer_Update_Flag; 
static uint16_t gindex = 0; 


//正弦波单个周期的点数
#define POINT_NUM 250

/* DAC单通道sine波数据 */
uint16_t Singel_Sin_data_12bit[POINT_NUM] = {
    2099,2151,2202,2254,2305,2356,2406,2457,2507,2557,2607,2656,2705,2754,2802,
    2850,2897,2943,2989,3035,3079,3124,3167,3210,3252,3293,3334,3373,3412,3450,
    3487,3523,3558,3593,3626,3658,3690,3720,3749,3777,3804,3830,3855,3879,3901,
    3922,3943,3962,3979,3996,4011,4025,4038,4049,4060,4069,4076,4083,4088, 4092,
    4095,4095,4095,4095,4092,4088,4083,4076,4069,4060,4049,4038,4025,4011,3996,
    3979,3962,3943,3922,3901,3879,3855,3830,3804,3777,3749,3720,3690,3658,3626,
    3593,3558,3523,3487,3450,3412,3373,3334,3293,3252,3210,3167,3124,3079,3035,
    2989,2943,2897,2850,2802,2754,2705,2656,2607,2557,2507,2457,2406,2356,2305,
    2254,2202,2151,2099,2048,1997,1945,1894,1842,1791,1740,1690,1639,1589,1539,
    1489,1440,1391,1342,1294,1246,1199,1153,1107,1061,1017,972,929,886,844,803,
    762,723,684,646,609,573,538,503,470,438,406,376,347,319,292,266,241,217,195,
    174,153,134,117,100,85,71,58,47,36,27,20,13,8,4,1,0,0,1,4,8,13,20,27,36,47,
    58,71,85,100,117,134,153,174,195,217,241,266,292,319,347,376,406,438,470,503,
    538,573,609,646,684,723,762,803,844,886,929,972,1017,1061,1107,1153,1199,1246,
    1294,1342,1391,1440,1489,1539,1589,1639,1690,1740,1791,1842,1894,1945,1997,2048
};

/* DAC双通道sine波数据buffer */
static uint32_t Dual_Sin_data_12bit[POINT_NUM];

/*DMA双数据模式 sine波数据buffer*/
static uint32_t DAMDualData_Sine12bit[POINT_NUM / 2];


/******************************************************************************
* @brief : DAC output noise initialization.
* @param : None.
* @return: None
******************************************************************************/
void DAC_Config_OutPut_Noise(void)
{
    DAC_ChannelConfTypeDef sConfig={0};
    
    hdac1.Instance = DAC1;
    HAL_DAC_Init(&hdac1);

    sConfig.u.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer =  DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    
    /* HAL_DAC_ConfigChannel()一次只配置一个DAC通道 */
    /* DAC通道1配置使用软件触发 */
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);   

    /* DAC通道2配置使用TIM6触发 */
//    sConfig.DAC_Trigger.u.Normal.Trigger = DAC_TRIGGER_T6_TRGO;   //1017
    sConfig.u.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);
}


void DAC_Config_Output_square1KHz(void)
{
    DAC_ChannelConfTypeDef sConfig={0};
    
    hdac1.Instance = DAC1;
    
    HAL_DAC_Init(&hdac1);

    sConfig.u.DAC_Trigger  = DAC_TRIGGER_T6_TRGO;
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer =  DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
    
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    
    /* DAC通道1锯齿波 reset:软件触发,step:软件触发 */
    HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1);
    
    HAL_DACEx_SelfCalibrate(&hdac1, &sConfig, DAC_CHANNEL_1);
    MODIFY_REG(hdac1.Instance->CCR, DAC_CCR_OTRIM1, sConfig.DAC_TrimmingValue << DAC_CCR_OTRIM1_Pos);
}

/******************************************************************************
* @brief : DAC output noise.
* @param : None.
* @return: None
******************************************************************************/
void DAC_OutPut_Noise()
{
    /* 设置DAC 输出的 基本电压 */
    HAL_DACEx_DualSetValue(&hdac1,DAC_ALIGN_12B_R, 1000, 1000);
    
    /* 在DAC 通道1上叠加噪声 */
    HAL_DACEx_NoiseWaveGenerate(&hdac1, DAC_CHANNEL_1, DAC_LFSRUNMASK_BITS9_0); 
    
    /* 在DAC 通道2上叠加三角波 */
    HAL_DACEx_TriangleWaveGenerate(&hdac1, DAC_CHANNEL_2, DAC_TRIANGLEAMPLITUDE_2047);

    /* 启动DAC转换 */
    /* DAC通道2启动后， 后面由TIM6触发 */
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2); 
    
    while(1)
    {
        /* DAC通道1 需要软件来每次触发 */
        HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
        //printfS("%d\r\n", HAL_DAC_GetValue(&hdac1, DAC_CHANNEL_2));
    }
}


void DAC_Config_OutPut_Sawtooth(DAC_TypeDef* DACx)
{
    DAC_ChannelConfTypeDef sConfig={0};
    
    hdac1.Instance = DACx;
    HAL_DAC_Init(&hdac1);

    sConfig.u.SawtoothResetTrigger = DAC_TRIGGER_SOFTWARE;//DAC_TRIGGER_EXTI9;//DAC_TRIGGER_SOFTWARE;//DAC_TRIGGER_EXTI9;//DAC_TRIGGER_SOFTWARE;//;
    sConfig.SawtoothStepTrigger = DAC_TRIGGER_SOFTWARE;//DAC_TRIGGER_EXTI10;//DAC_TRIGGER_SOFTWARE;
    
    
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer =  DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    
    /* DAC通道1锯齿波 reset:软件触发,step:软件触发 */
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);  
    EXTI_GPIO_Config(GPIOE, GPIO_PIN_9, EXTI_LINE_9); //EXTICR2 = 0x20000
//    EXTI_GPIO_Config(GPIOE, GPIO_PIN_10, EXTI_LINE_10); //EXTICR2 = 0x400000

    /* DAC通道2锯齿波 reset:软件触发,step:TIM6触发 */
    sConfig.u.SawtoothResetTrigger = DAC_TRIGGER_SOFTWARE;
    sConfig.SawtoothStepTrigger = DAC_TRIGGER_T15_TRGO;    
    
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);
}

void DAC_OutPut_Sawtooth()
{
    volatile uint16_t ch1_val;
    volatile uint16_t ch2_val;
//    volatile uint32_t cnt = 0;
    /* DAC通道1锯齿波为递增模式, 从0开始递增 */
    HAL_DACEx_SawtoothWaveGenerate(&hdac1, DAC_CHANNEL_1, DAC_SAWTOOTH_POLARITY_INCREMENT, 100, 16);//StepData是0.0625的倍数
    /* DAC通道2锯齿波为递减模式, 从FFF开始递减 */
    HAL_DACEx_SawtoothWaveGenerate(&hdac1, DAC_CHANNEL_2, DAC_SAWTOOTH_POLARITY_DECREMENT, 0xfff, 8);//0.0625
    
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);  
    while(1)
    {
        /* DAC通道1锯齿波的递增配置为使用软件触发，因此需要软件来每次触发 */
        HAL_DACEx_SawtoothWaveDataStep(&hdac1, DAC_CHANNEL_1);  
        
        /* DAC通道1锯齿波的递增配置为使用TIM6触发 */
        //HAL_DACEx_SawtoothWaveDataStep(&hdac1, DAC_CHANNEL_2);

        /* DAC通道1和2的锯齿波的复位都配置为使用软件触发 */
        if((ch1_val = HAL_DAC_GetValue(&hdac1, DAC_CHANNEL_1)) == 0xfff)  
        {
            HAL_DACEx_SawtoothWaveDataResetBySoftware(&hdac1, DAC_CHANNEL_1);  
        } 
        if((ch2_val = HAL_DAC_GetValue(&hdac1, DAC_CHANNEL_2)) == 0x0)     
        {      
            HAL_DACEx_SawtoothWaveDataResetBySoftware(&hdac1, DAC_CHANNEL_2);
        }

    }
}

/******************************************************************************
* @brief : DAC output voltage initialization.
* @param : None.
* @return: None
******************************************************************************/
void DAC_Config_OutPut_Voltage(DAC_TypeDef* DACx)
{
    hdac1.Instance = DACx;
    HAL_DAC_Init(&hdac1);

    DAC_ChannelConfTypeDef sConfig ={0}; 
    sConfig.u.DAC_Trigger = DAC_TRIGGER_SOFTWARE;

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;//DAC_OUTPUTBUFFER_ENABLE;//DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;  
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);    
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);

    /* 自动校准，可根据需要开启/关闭 */
//    HAL_DACEx_SelfCalibrate(&hdac1, &sConfig,DAC_CHANNEL_1);
//    HAL_DACEx_SelfCalibrate(&hdac1, &sConfig,DAC_CHANNEL_2);
    HAL_SimpleDelay(500000);
}

/******************************************************************************
* @brief : DAC output voltage.
* @param : voltage ：output voltage value.
* @return: None
******************************************************************************/
void DAC_OutPutVoltage(float voltage)
{
    if( voltage > 3.3)
    {
        return;
    }
    uint16_t data=((voltage / 3.3) * 4095);
    HAL_DACEx_DualSetValue(&hdac1,DAC_ALIGN_12B_R,data,data);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
}

/******************************************************************************
* @brief : TIM6 interrupt callback function.
* @param : htim : pointer to a TIM_HandleTypeDef structure that contains
*                      the configuration information for Timer module.
* @return: None
******************************************************************************/
void HAL_TIM_Update_Event_Callback(TIM_HandleTypeDef *htim)
{
    UNUSED(htim);
    gtimer_Update_Flag = 1;   
}

/******************************************************************************
* @brief : DAC output Sin initialization.
* @param : None.
* @return: None
******************************************************************************/
void DAC_Config_OutPut_Sinx()
{
    DAC_ChannelConfTypeDef sConfig;
    
    hdac1.Instance = DAC1;
    HAL_DAC_Init(&hdac1);

    sConfig.u.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
    sConfig.SawtoothStepTrigger = DAC_TRIGGER_SOFTWARE;//for asset only
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;//DAC_OUTPUTBUFFER_DISABLE;//DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;//DAC_CHIPCONNECT_BOTH;  
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);    
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);
}


#if 1
/******************************************************************************
* @brief : Timer6 initialization.
* @param : None.
* @return: None
******************************************************************************/
void DAC_TIM6_Init(void)  
{
	TIM6_Handler.Instance = TIM6;
	TIM6_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	TIM6_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM6_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM6_Handler.Init.RepetitionCounter = 0;  

    TIM6_Handler.Init.Prescaler = 240 - 1;  
	TIM6_Handler.Init.Period = 1;  
    __HAL_RCC_TIM6_RESET();   
    __HAL_RCC_TIM6_CLK_ENABLE();          
    NVIC_ClearPendingIRQ(TIM6_IRQn);   
    NVIC_EnableIRQ(TIM6_IRQn);       
	HAL_TIMER_Base_Init(&TIM6_Handler);    
	HAL_TIM_ENABLE_IT(&TIM6_Handler, TIMER_INT_EN_UPD);  
	
	HAL_TIMER_Base_Start(TIM6_Handler.Instance);            
}
#endif

#ifdef HAL_DMA_MODULE_ENABLED
/******************************************************************************
* @brief : DMA config.
* @param : None.
* @return: None
******************************************************************************/
static void DAC_DMA_Config(DAC_TypeDef* DACx)
{
    uint32_t Idx=0;
    /* write Sinx data from   Singel_Sin_data_12bit to Dual_Sin_data_12bit */
    for (Idx = 0; Idx < POINT_NUM; Idx++)
    {
        Dual_Sin_data_12bit[Idx] = (Singel_Sin_data_12bit[Idx] << 16) + (Singel_Sin_data_12bit[Idx]);
    }
    __HAL_RCC_DMA1_CLK_ENABLE();
    /* DAC DMA Init */
    /* DAC_CH1 Init */
    hdma_dac1_ch1.Instance  = DMA1_Channel0;
    hdma_dac1_ch1.Init.Mode = DMA_MODE_CIRCULAR;
    hdma_dac1_ch1.Init.DataFlow = DMA_DATAFLOW_M2P;
    if(DACx == DAC1)    
        hdma_dac1_ch1.Init.ReqID = 39;
    else
        hdma_dac1_ch1.Init.ReqID = 117;//DMA1_REQ_DAC2_CH1;
    hdma_dac1_ch1.Init.SrcIncDec = DMA_SRCINCDEC_INC;
    hdma_dac1_ch1.Init.DestIncDec = DMA_DESTINCDEC_DISABLE;
    hdma_dac1_ch1.Init.SrcWidth = DMA_SRCWIDTH_WORD;
    hdma_dac1_ch1.Init.DestWidth = DMA_DESTWIDTH_WORD;
    HAL_DMA_Init(&hdma_dac1_ch1);
    __HAL_LINK_DMA(hdac1, DMA_Handle1, hdma_dac1_ch1);

    /* DAC_CH2 Init */
    hdma_dac1_ch2.Instance = DMA1_Channel1;
    hdma_dac1_ch2.Init.Mode =DMA_MODE_CIRCULAR;
    hdma_dac1_ch2.Init.DataFlow = DMA_DATAFLOW_M2P;
    if(DACx == DAC1)
        hdma_dac1_ch2.Init.ReqID = 40;
    else
        hdma_dac1_ch2.Init.ReqID = 118;//DMA1_REQ_DAC1_CH2;
    hdma_dac1_ch2.Init.SrcIncDec = DMA_SRCINCDEC_INC;
    hdma_dac1_ch2.Init.DestIncDec = DMA_DESTINCDEC_DISABLE;
    hdma_dac1_ch2.Init.SrcWidth = DMA_SRCWIDTH_WORD;
    hdma_dac1_ch2.Init.DestWidth = DMA_DESTWIDTH_WORD;
    HAL_DMA_Init(&hdma_dac1_ch2);
    __HAL_LINK_DMA(hdac1, DMA_Handle2, hdma_dac1_ch2);
    
    NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
    NVIC_EnableIRQ(DMA1_CH0_IRQn);
} 



static void DAC_DMADualData_Config()
{
    uint32_t i = 0;
    uint32_t j = 0;
    /* write Sinx data from   Singel_Sin_data_12bit to Dual_Sin_data_12bit */
    for (i = 0, j = 0; j < POINT_NUM / 2; i += 2, j++)
    {
        DAMDualData_Sine12bit[j] = (Singel_Sin_data_12bit[i + 1] << 16) + (Singel_Sin_data_12bit[i]);
    }
    __HAL_RCC_DMA1_CLK_ENABLE();
    /* DAC DMA Init */
    /* DAC_CH1 Init */
    hdma_dac1_ch1.Instance  = DMA1_Channel0;
    hdma_dac1_ch1.Init.Mode = DMA_MODE_CIRCULAR;
    hdma_dac1_ch1.Init.DataFlow = DMA_DATAFLOW_M2P;
    hdma_dac1_ch1.Init.ReqID = DMA1_REQ_DAC1_CH1;
    hdma_dac1_ch1.Init.SrcIncDec = DMA_SRCINCDEC_INC;
    hdma_dac1_ch1.Init.DestIncDec = DMA_DESTINCDEC_DISABLE;
    hdma_dac1_ch1.Init.SrcWidth = DMA_SRCWIDTH_WORD;
    hdma_dac1_ch1.Init.DestWidth = DMA_DESTWIDTH_WORD;
    HAL_DMA_Init(&hdma_dac1_ch1);
    __HAL_LINK_DMA(hdac1, DMA_Handle1, hdma_dac1_ch1);

    /* DAC_CH2 Init */
    hdma_dac1_ch2.Instance = DMA1_Channel1;
    hdma_dac1_ch2.Init.Mode =DMA_MODE_CIRCULAR;
    hdma_dac1_ch2.Init.DataFlow = DMA_DATAFLOW_M2P;
    hdma_dac1_ch2.Init.ReqID = DMA1_REQ_DAC1_CH2;
    hdma_dac1_ch2.Init.SrcIncDec = DMA_SRCINCDEC_INC;
    hdma_dac1_ch2.Init.DestIncDec = DMA_DESTINCDEC_DISABLE;
    hdma_dac1_ch2.Init.SrcWidth = DMA_SRCWIDTH_WORD;
    hdma_dac1_ch2.Init.DestWidth = DMA_DESTWIDTH_WORD;
    HAL_DMA_Init(&hdma_dac1_ch2);
    __HAL_LINK_DMA(hdac1, DMA_Handle2, hdma_dac1_ch2);
    
    NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
    NVIC_EnableIRQ(DMA1_CH0_IRQn);
}
#endif

void EXTI_GPIO_Config(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t u32_EXTILine)
{   
    GPIO_InitTypeDef GPIO_PINx_Handle={0};
    
    __HAL_RCC_EXTI_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    	
	/* Initialization GPIO */
    GPIO_PINx_Handle.Pin       = Pin;
    GPIO_PINx_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_PINx_Handle.Pull      = GPIO_PULLUP;
	GPIO_PINx_Handle.Drive     = GPIO_DRIVE_LEVEL3;

    HAL_GPIO_Init(GPIOx, &GPIO_PINx_Handle);
    
    /* Config EXTI */
    HAL_EXTI_SetConfigLine(GPIOx, u32_EXTILine, EXTI_MODE_IT_FALLING);
}

void DAC_LPTIMx_TRGOx_Init(uint8_t LPTimer, uint16_t Period)
{
    LPTIM_HandleTypeDef LPTIM1_Handle = {0};
    switch(LPTimer)
    {
        case 1:
            LPTIM1_Handle.Instance                   = LPTIM1;
            __HAL_RCC_LPTIM1_CLK_ENABLE();
            break;
        
        case 2:
            LPTIM1_Handle.Instance                   = LPTIM2;
            __HAL_RCC_LPTIM2_CLK_ENABLE();
            break;
        
    }
    
    LPTIM1_Handle.Init.ClockSource           = LPTIM_CLOCKSOURCE_INTERNAL_RCH; //LPTIM_CLOCKSOURCE_INTERNAL_RC64M;
    LPTIM1_Handle.Init.ClockPrescaler        = LPTIM_CLOCKPRESCALER_DIV32;
    LPTIM1_Handle.Init.TriggerSource         = LPTIM_TRIGSOURCE_SOFTWARE;
    LPTIM1_Handle.Init.CounterSource         = LPTIM_COUNTERSOURCE_INTERNAL;
    LPTIM1_Handle.Init.Input1Source          = LPTIM_INPUT1SOURCE_0;
    LPTIM1_Handle.Init.UpdateMode            = LPTIM_UPDATE_IMMEDIATE;
    
    LPTIM1_Handle.AutoReloadMatchCallback    = NULL;
    LPTIM1_Handle.RepetitionUpdateCallback   = NULL;
    
    if (HAL_LPTIM_Init(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 init failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_ConfigCountValue(&LPTIM1_Handle, 0, Period, Period/2) != HAL_OK)
    {
        printfS("LPTIM1 Counter config failed\r\n");
        while (1);
    }
    
    if (HAL_LPTIM_EnableIT(&LPTIM1_Handle, LPTIM_IT_ARRM) != HAL_OK)
    {
        printfS("LPTIM1 PWM enable interrupt failed\r\n");
        while (1);
    }

    printfS("LPTIM1 Counter startup\r\n");
    
    if (HAL_LPTIM_PWM_Start(&LPTIM1_Handle) != HAL_OK)
    {
        printfS("LPTIM1 Counter startup failed\r\n");
        while (1);
    }
}
/******************************************************************************
* @brief : DAC Timer Trigger Configuration.
* @param : Timer: Timer selection.
* @param : Trgo: Master mode selection.
* @param : Prescaler: Timer prescaler.
* @param : Period: Timer period.
* @return: None
******************************************************************************/
void DAC_TIMx_TRGOx_Init(uint8_t Timer,uint8_t Trgo,uint16_t Prescaler,uint16_t Period)
{
//    uint32_t timer_clock;
    TIM_HandleTypeDef TIMx_Handler={0};
    TIM_MasterConfigTypeDef sMasterConfig={0};	
    switch(Timer)	
    {
        case 1:
            __HAL_RCC_TIM1_CLK_ENABLE();
            TIMx_Handler.Instance = TIM1;
            break;
        
        case 2:
            __HAL_RCC_TIM2_CLK_ENABLE();
            TIMx_Handler.Instance = TIM2;
            break;

        case 4:
            __HAL_RCC_TIM4_CLK_ENABLE();
            TIMx_Handler.Instance = TIM4;
            break;
        
        case 5:
            __HAL_RCC_TIM4_CLK_ENABLE();
            TIMx_Handler.Instance = TIM5;
            break;
        
        case 6:
            __HAL_RCC_TIM6_CLK_ENABLE();
            TIMx_Handler.Instance = TIM6;
            break;
        
        case 7:
            __HAL_RCC_TIM7_CLK_ENABLE();
            TIMx_Handler.Instance = TIM7;
            break;
        
        case 8:
            __HAL_RCC_TIM8_CLK_ENABLE();
            TIMx_Handler.Instance = TIM8;
            break;
        
        case 15:
            __HAL_RCC_TIM15_CLK_ENABLE();
            TIMx_Handler.Instance = TIM15;
            break;
        
        default:
            printfS("Please select the correct timer");
            return;
    }
        
    TIMx_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_DISABLE;    
    TIMx_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
    TIMx_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    TIMx_Handler.Init.RepetitionCounter = 0;  
    TIMx_Handler.Init.Prescaler = Prescaler - 1;
    TIMx_Handler.Init.Period = Period - 1;  


    HAL_TIMER_MSP_Init(&TIMx_Handler);  
    HAL_TIMER_Base_Init(&TIMx_Handler);  
    switch(Trgo)
    {
        case 1:
            sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
        break;
        case 2:
            sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
        break;
    }

    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMER_Master_Mode_Config(TIMx_Handler.Instance, &sMasterConfig);
    HAL_TIMER_Base_Start(TIMx_Handler.Instance);   
} 

/******************************************************************************
* @brief : DAC Output config.
* @param : None.
* @return: None
******************************************************************************/
void DAC_Config_OutPut(DAC_TypeDef* DACx)
{
    DAC_ChannelConfTypeDef sConfig={0}; 
    
    hdac1.Instance = DACx;//DAC1;
    HAL_DAC_Init(&hdac1);

    sConfig.u.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);   
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
}

void DAC_Config_OutPut_DMADualData()
{
    DAC_ChannelConfTypeDef sConfig={0}; 
    
    hdac1.Instance = DAC1;
    HAL_DAC_Init(&hdac1);

    sConfig.DAC_DMADoubleDataMode = ENABLE;
    sConfig.u.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
    
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_1);   
    HAL_DAC_ConfigChannel(&hdac1, &sConfig,DAC_CHANNEL_2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
}
uint32_t min_max_data[POINT_NUM];
void APP_DAC_Demo(DAC_TEST_MODE_t fe_Mode)
{   
    switch (fe_Mode)
    {
        /* 输出噪音 */
        case DAC_Output_Noise: 
        {
            printfS("DAC Output Noise wave\r\n\r\n");
            DAC_Config_OutPut_Noise();
            DAC_TIMx_TRGOx_Init(6, 1, 300, 2);
            DAC_OutPut_Noise();
        }
        break;
        case DAC_Output_SAWTOOTH: 
        {
            printfS("DAC Output Swatooth wave\r\n\r\n");
            DAC_Config_OutPut_Sawtooth(DAC1);
            /* 使用TIM6来触发DAC通道2的step触发 */
            DAC_TIMx_TRGOx_Init(15, 1, 300, 2);  
//            DAC_LPTIMx_TRGOx_Init(2, 1000);
            DAC_OutPut_Sawtooth();
        }
        break;

        /* 输出电压 */
        case DAC_OutPut_Voltage: 
        {
            printfS("DAC Output Voltage\r\n\r\n");
            DAC_Config_OutPut_Voltage(DAC1);
            DAC_OutPutVoltage(2.5);
        }
        break;
 
        /* TIM6 update中断，软件设置DAC值来输出正弦波 */
        case DAC_OutPut_Sinx:
        {
            printfS("DAC Output SINE Wave by software dynamically setting DAC output Value\r\n\r\n");
            DAC_Config_OutPut_Sinx(); 
            DAC_TIM6_Init();
            gtimer_Update_Flag = 0;
            while(1) 
            {
                if(gtimer_Update_Flag) 
                {
                    gtimer_Update_Flag = 0;
                    HAL_DACEx_DualSetValue(&hdac1,DAC_ALIGN_12B_R,Singel_Sin_data_12bit[gindex],Singel_Sin_data_12bit[gindex]);
                    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
                    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);  
                    gindex++;
                    //若Sin表已到达结尾，重新指向表头
                    if( gindex >=  POINT_NUM)			
                    {
                        gindex=0;								
                    }                      
                }
            }
        }
        break;       
        /* DAC双通道 DMA方式输出正弦波 */
        /* 一个32bit数据对应DAC 两个通道，分别放入DAC的两个通道的DOR */
        /* 一个32bit数据（两个12bit数据，DHRXXD），DAC两个通道同时使用 */
#ifdef HAL_DMA_MODULE_ENABLED                
        case DAC_DMA_Output_Sinx:
        {
            printfS("DAC Output SINE Wave by DMA transferring data to both channels(DAC_DHR8RD)\r\n\r\n");
            /* DAC 通道1选择 TIM6 作为触发输入*/
            DAC_Config_OutPut(DAC1); 
            DAC_DMA_Config(DAC1) ;  
            DAC_TIMx_TRGOx_Init(6, 1, 240,10);
            HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_Dual, Dual_Sin_data_12bit, POINT_NUM, DAC_ALIGN_12B_R);//双通道DMA       
        }
        break; 
        
        /* DMA双数据模式输出正弦波 */
        /* 一次DAM请求搬运一个32bit数据（两个12bit的DAC数据），依次通过外部trigger放入某一个DAC通道的DOR */
        /* 一个32bit数据（两个12bit数据，DHRXX1或DHRXX2），依次给DAC一个通道使用 */
        case DAC_DMADUALDATA_Output_Sinx:
        {
//            uint32_t tmp;
            printfS("DAC Output SINE Wave by DMA transferring dual-12bit-data(one word) to one channel each time(DAC_DHR12Rx)\r\n\r\n");
            DAC_Config_OutPut_DMADualData(); 
            DAC_DMADualData_Config();
            DAC_TIMx_TRGOx_Init(6, 1, 300, 10);
            HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, DAMDualData_Sine12bit, POINT_NUM/2, DAC_ALIGN_12B_R);//通道1 DMA
            
            HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, DAMDualData_Sine12bit, POINT_NUM/2, DAC_ALIGN_12B_R);//通道2 DMA
        }
            break;
#endif         
        default: break; 
    }
}
