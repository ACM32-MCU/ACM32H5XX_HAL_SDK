#include "app.h"
#include "TKey.h"


#define AVERANGE_NUM 20
#define FULL_NUM  25006
#define DISCHARGE_CURRENT_VALUE 28

#define DISCHARGE_RES_VALUE 9

uint8_t Cp_compensate[16];

TKEY_HandleTypeDef TKEY_Handle_auto;

void TKEY_CSD_Init_Only_Current(void)
{  
    TKEY_CSDConfigTypeDef   TKEY_CSDConfig;

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle_auto.Init.Mode       = TKEY_MODE_CSD; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle_auto.Init.WorkMode   = TKEY_WORK_MODE_NORMAL;
    /* 扫描模式：单次或连续 */     
    TKEY_Handle_auto.Init.ScanMode   = TKEY_SCAN_MODE_SINGLE; 

    /* 屏蔽通道使能 */
    TKEY_Handle_auto.Init.ShieldEn   = TKEY_SHIELD_ENABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle_auto.Init.FilterNum  = TKEY_FILTER_NONE;   
    
    /* 充电电压VLDO */    
    TKEY_Handle_auto.Init.Vldo       = TKEY_LDO_2V4;
    /* 比较器电压VREF */      
    TKEY_Handle_auto.Init.Vref       = TKEY_VREF_THREE_QUARTER_VLDO;//TKEY_VREF_HALF_VLDO TKEY_VREF_QUARTER_VLDO;
    /* 扫描时钟扩频使能 */    
    TKEY_Handle_auto.Init.SpreadEn   = TKEY_SPREAD_DISABLE;
    /* TKEY通道使能 */    
    TKEY_Handle_auto.Init.ChannelEn  = TKEY_CHANNEL_0_EN | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;                                                                                           

    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle_auto.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle_auto.Timing.ScanInterval         = 20;
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle_auto.Timing.ScanClkPrescale      = 10;
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle_auto.Timing.ScanClkSW1HSW2L      = 4;
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle_auto.Timing.ScanClkSW1LSW2H      = 4;
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle_auto.Timing.TimeoutOrPwmCycle    = 10; 
    
    /* 采样次数 */    
    TKEY_Handle_auto.Init.SampleNum  = TKEY_SAMPLE_3;
    /* 通道电容补偿选择 */    
    TKEY_Handle_auto.Init.CCPSel  = TKEY_CAPCOMPENSTATE_0PF;
    /* 通道电阻补偿选择 */    
    TKEY_Handle_auto.Init.CCPSel  = TKEY_RESCOMPENSTATE_0_1K;    
    
    HAL_TKEY_Init(&TKEY_Handle_auto);
    
    
    /* CSD模式配置 */
    /* 放电模式选择：电阻和电流源同时放电 */ 
    TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_CURRENT;//TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;
    /* 预充使能 */    
    TKEY_CSDConfig.PrechargeEn              = TKEY_PRECHARGE_DISABLE;//TKEY_PRECHARGE_ENABLE;
    /* 放电电流选择：0.4uA * DischargeCurrent */
    TKEY_CSDConfig.DischargeCurrent         = DISCHARGE_CURRENT_VALUE;
    /* 放电电阻选择：Vref / 512K * DischargeRes */
    TKEY_CSDConfig.DischargeRes             = 2;
    /* 采样比特流的时钟分频：TKCLK / (SampleBitClkDiv + 1) */     
    TKEY_CSDConfig.SampleBitClkDiv          = 0;
    HAL_TKEY_CSDConfig(&TKEY_CSDConfig);
    
    /* 清除扫描结束标志 */
    HAL_TKEY_ClearFlag(TKEY_FLAG_EOC);
    
    NVIC_ClearPendingIRQ(TKEY_IRQn);
    NVIC_SetPriority(TKEY_IRQn, 0x00);
    NVIC_EnableIRQ(TKEY_IRQn);
    
    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
}

void TKEY_CSD_Init_Only_Res(void)
{  
    TKEY_CSDConfigTypeDef   TKEY_CSDConfig;

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle_auto.Init.Mode       = TKEY_MODE_CSD; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle_auto.Init.WorkMode   = TKEY_WORK_MODE_NORMAL;
    /* 扫描模式：单次或连续 */     
    TKEY_Handle_auto.Init.ScanMode   = TKEY_SCAN_MODE_SINGLE; 

    /* 屏蔽通道使能 */
    TKEY_Handle_auto.Init.ShieldEn   = TKEY_SHIELD_ENABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle_auto.Init.FilterNum  = TKEY_FILTER_NONE;   
    
    /* 充电电压VLDO */    
    TKEY_Handle_auto.Init.Vldo       = TKEY_LDO_2V4;
    /* 比较器电压VREF */      
    TKEY_Handle_auto.Init.Vref       = TKEY_VREF_VBG;//TKEY_VREF_HALF_VLDO;//TKEY_VREF_HALF_VLDO TKEY_VREF_QUARTER_VLDO;
    /* 扫描时钟扩频使能 */    
    TKEY_Handle_auto.Init.SpreadEn   = TKEY_SPREAD_DISABLE;
    /* TKEY通道使能 */    
    TKEY_Handle_auto.Init.ChannelEn  = TKEY_CHANNEL_0_EN | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;                                                                                           

    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle_auto.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle_auto.Timing.ScanInterval         = 20;
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle_auto.Timing.ScanClkPrescale      = 10;
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle_auto.Timing.ScanClkSW1HSW2L      = 4;
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle_auto.Timing.ScanClkSW1LSW2H      = 4;
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle_auto.Timing.TimeoutOrPwmCycle    = 10; 
    
    /* 采样次数 */    
    TKEY_Handle_auto.Init.SampleNum  = TKEY_SAMPLE_3;
    /* 通道电容补偿选择 */    
    TKEY_Handle_auto.Init.CCPSel  = TKEY_CAPCOMPENSTATE_0PF;
    /* 通道电阻补偿选择 */    
    TKEY_Handle_auto.Init.CCPSel  = TKEY_RESCOMPENSTATE_0_1K;    
    
    HAL_TKEY_Init(&TKEY_Handle_auto);
    
    
    /* CSD模式配置 */
    /* 放电模式选择：电阻和电流源同时放电 */ 
    TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_RES;//TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;
    /* 预充使能 */    
    TKEY_CSDConfig.PrechargeEn              = TKEY_PRECHARGE_DISABLE;//TKEY_PRECHARGE_ENABLE;
    /* 放电电流选择：0.4uA * DischargeCurrent */
    TKEY_CSDConfig.DischargeCurrent         = DISCHARGE_CURRENT_VALUE;
    /* 放电电阻选择：Vref / 512K * DischargeRes */
    TKEY_CSDConfig.DischargeRes             = DISCHARGE_RES_VALUE;
    /* 采样比特流的时钟分频：TKCLK / (SampleBitClkDiv + 1) */     
    TKEY_CSDConfig.SampleBitClkDiv          = 0;
    HAL_TKEY_CSDConfig(&TKEY_CSDConfig);
    
    /* 清除扫描结束标志 */
    HAL_TKEY_ClearFlag(TKEY_FLAG_EOC);
    
    NVIC_ClearPendingIRQ(TKEY_IRQn);
    NVIC_SetPriority(TKEY_IRQn, 0x00);
    NVIC_EnableIRQ(TKEY_IRQn);
    
    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);
}


void Cp_Autotest(void)
{
    int i,j;
    float duty_circle[16];
    uint32_t sum[16];
    uint16_t channel_data[16];
    float Cp[16];
    TKEY_Handle_auto.ChannelData = TKEY_Config;
	
    TKEY_CSD_Init_Only_Res();   
    memset(sum,0,sizeof(sum));
    //丢弃一开始数据
    for(i = 0;i < 10; i++) 
    {
        HAL_TKEY_ReadChannelData_SingleAll_GND(&TKEY_Handle_auto);
    }
    for(i = 0;i < 10; i++) //取通道初始均值作为baseline
    {
        HAL_TKEY_ReadChannelData_SingleAll(&TKEY_Handle_auto);
    }
    
    //采集20次
    for(i=0;i<AVERANGE_NUM;i++)
    {
        HAL_TKEY_ReadChannelData_SingleAll(&TKEY_Handle_auto);
        for(j=0;j<16;j++)
        {
            sum[j] += TKEY_Handle_auto.ChannelData[j].Tkey_Data->RawData;
        }
    }
  
    for(j=0;j<16;j++)    //Cp
    {
        channel_data[j] = sum[j]/AVERANGE_NUM;
        duty_circle[j]  = (float)channel_data[j] / FULL_NUM;
        
        //Cp = 占空比*8uA*1.72us/0.6V  //20*0.4uA = 8uA
        Cp[j] = ((float)channel_data[j] / FULL_NUM)*(DISCHARGE_RES_VALUE* 1.95 )*1.72/1.4;
    }
    
    /*计算每个通道补偿电容
    补偿目标：	18pf  -基准读数  16000左右
    
    Cp[j]>21       100, 用于调整放电参数
    

    Cp>18p         不补偿
    16p<Cp<18p     2p
    14p<Cp<16p     4p
    12p<Cp<14p     6p
    10p<Cp<12p     8p
    8p<Cp<10p      10p  
    6p<Cp<8p       12p  
    Cp<6p          14p    
    */
	for(j=0;j<16;j++)    //Cp
    {
				 if(Cp[j]>24)
        {
            Cp_compensate[j] = 100;
        }
        else if(Cp[j]>19)
        {
            Cp_compensate[j] = 0;
        }
        else if(Cp[j]>17)
        {
            Cp_compensate[j] = 2;
        }
        else if(Cp[j]>=15)
        {
            Cp_compensate[j] = 4;
        }
        else if(Cp[j]>=13)
        {
            Cp_compensate[j] = 6;
        }
        else if(Cp[j]>=11)
        {
            Cp_compensate[j] = 8;
        }
        else if(Cp[j]>=9)
        {
            Cp_compensate[j] = 10;
        }
        else if(Cp[j]>=7)
        {
            Cp_compensate[j] = 12;
        }
        else
        {   
            Cp_compensate[j] = 14;
        }          
    } 

}