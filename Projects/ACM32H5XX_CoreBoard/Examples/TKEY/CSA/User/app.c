/******************************************************************************
*@file  : app.c
*@brief : HRNG Demos 
******************************************************************************/
#include "app.h"
#include "dac7311.h"
#include "TKey.h"
#include "ac_fifo.h"

TKEY_HandleTypeDef TKEY_Handle;
volatile uint32_t g32TkeyIrqFlag;
volatile uint32_t g32TkeyIrqFlag_timeout;

volatile uint16_t g16TkeyScanInterval;
uint32_t g32IdleStartTick;

uint8_t  Cp_compensate[16];
extern uint16_t g16ChannelMsk;
extern uint8_t g8RoundScanOver;

void HAL_TKEY_IRQHandler(void)
{
    /* 
        NOTE: This function should be modified, when the callback is needed
    */
    
    /* For Example */
    if((TKEY->SR & TKEY_SR_EOC) && (TKEY->IER & TKEY_IER_EOCIE))            //Scanning completed 
    {
        TKEY->SR = TKEY_SR_EOC;
        g32TkeyIrqFlag = 1;    
    }
    
    if((TKEY->SR & TKEY_SR_YESTOUCH) && (TKEY->IER & TKEY_IER_YESTOUCHIE))  //Key touch event has occurred
    {
        TKEY->SR = TKEY_SR_YESTOUCH;
    }
    
    if((TKEY->SR & TKEY_SR_TIMEOUT) && (TKEY->IER & TKEY_IER_TIMEOUTIE))    //Channel scan timeout
    {
        TKEY->SR = TKEY_SR_TIMEOUT;
    }
    
//    if((TKEY->SR & TKEY_SR_BUSY) == 0)                                      //Channel scan busy
//    {
//        TKEY->CR |= TKEY_CR_START;
//    }
    
    if((TKEY->SR & TKEY_SR_DONE) && (TKEY->IER & TKEY_IER_DONEIE))          //Comparator flipping completed
    {
        TKEY->SR = TKEY_SR_DONE;
    }
    
    if((TKEY->SR & TKEY_SR_CHG) && (TKEY->IER & TKEY_IER_CHIE))             //Each charge is completed
    {
        TKEY->SR = TKEY_SR_CHG;
    }
    
    if((TKEY->SR & TKEY_SR_CHGDONE) && (TKEY->IER & TKEY_IER_CHGDONEIE))    //Charging frequency reaches the set number of times
    {                                               
        TKEY->SR = TKEY_SR_CHGDONE;
    }
    
    if((TKEY->SR & TKEY_SR_MEOC) && (TKEY->IER & TKEY_IER_MEOCIE))          //Mutual inductance mode channel scan completed
    {
        TKEY->SR = TKEY_SR_MEOC;
    }
    
}

void TKEY_IRQHandler(void)
{
    HAL_TKEY_IRQHandler();
}

void TKEY_CSA_Init_Single(void)
{
    TKEY_CSAConfigTypeDef   TKEY_CSAConfig;

		TKEY_CSDConfigTypeDef   TKEY_CSDConfig;	

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle.Init.Mode       = TKEY_MODE_CSA; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle.Init.WorkMode   = TKEY_WORK_MODE_NORMAL;
    /* 扫描模式：单次或连续 */     
    TKEY_Handle.Init.ScanMode   = TKEY_SCAN_MODE_SINGLE;//  TKEY_SCAN_MODE_CONT; 
    /* 屏蔽通道使能 */
    TKEY_Handle.Init.ShieldEn   = TKEY_SHIELD_ENABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle.Init.FilterNum  = TKEY_FILTER_NONE; 
    /* 常规模式下采样次数 */    
    TKEY_Handle.Init.SampleNum  = TKEY_SAMPLE_1;
	  
    /* 充电电压VLDO */    
    TKEY_Handle.Init.Vldo       = TKEY_LDO_2V4;//TKEY_LDO_2V4;
    /* 比较器电压VREF */      
    TKEY_Handle.Init.Vref       = TKEY_VREF_VBG;//TKEY_VREF_VBG;//TKEY_VREF_HALF_VLDO;TKEY_VREF_QUARTER_VLDO
    /* 扫描时钟扩频使能 */    
    TKEY_Handle.Init.SpreadEn   = TKEY_SPREAD_DISABLE; 
		//  TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN |TKEY_CHANNEL_3_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_10_EN;
    /* TKEY通道使能 */    
    TKEY_Handle.Init.ChannelEn  = TKEY_CHANNEL_0_EN | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;                                                                                          

	g16ChannelMsk = TKEY_Handle.Init.ChannelEn;
    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle.Timing.ScanInterval         = 20; //20
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle.Timing.ScanClkPrescale      = 9;//4;  //4;
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle.Timing.ScanClkSW1HSW2L      = 2; //2
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle.Timing.ScanClkSW1LSW2H      = 2;  //2
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle.Timing.TimeoutOrPwmCycle    = 200;//60;//60;  
       
    HAL_TKEY_Init(&TKEY_Handle);
   
    
    /* CSA模式配置 */
	
    /* CSA模式下，充电次数达到设定次数功能使能选择*/
	//TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;
    TKEY_CSAConfig.ChargeNumDoneEn = TKEY_CHGNUMDONE_DISABLE;  
    /* CSA模式每一次充电完成功能使能选择*/    
    TKEY_CSAConfig.ChargeEachDoneEN = TKEY_CHGEACHDONE_DISABLE; 
    /* Cs电容放电时间 ：(DischargeTime + 1) * RC32k*/    
    TKEY_CSAConfig.DischargeTime = 0;    
    /* CSA比较器滤波值，CSA比较器滤波使能后才有效*/      
    TKEY_CSAConfig.ComparerFilterNum = 1; 
    /* CSA比较器滤波使能*/     
    TKEY_CSAConfig.ComparerFilterEN = TKEY_COMPFILTER_ENABLE;
    /* CSA充电次数设置，充电次数达到设定次数功能使能才有效*/    
    TKEY_CSAConfig.ChargeNum = 2;   
    /* CSA充电次数达到设置次数后等待时间，计时单位：8* RC32k，结束后再过4** RC32kCmod 开始放电*/     
    TKEY_CSAConfig.ChargeDoneWaitTime = 2; 

    HAL_TKEY_CSAConfig(&TKEY_CSAConfig);
		
    NVIC_ClearPendingIRQ(TKEY_IRQn);
    NVIC_SetPriority(TKEY_IRQn, 0x00);
    NVIC_EnableIRQ(TKEY_IRQn);
    
    /* 清除扫描结束标志 */
    HAL_TKEY_ClearFlag(TKEY_FLAG_EOC);
    
    /* 使能通道扫描结束中断 */
    HAL_TKEY_ITConfig(TKEY_IT_EOCIE, ENABLE);

}

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint8_t  ucKey = 0xFF;
	
    #ifndef TKEY_WAVEFORM_PRINTF
		printfS("CAT TKEY CSA tset\r\n\r\n");     
		printf("TOTAL_CHANNELS = %d\n", TOTAL_CHANNELS);
    #endif   
   
    TouchKey_Init();    
	
	/* 开启扫描 */
    HAL_TKEY_StartScan();
	
    while(1)
    {             
		if(g32TkeyIrqFlag)
		{
			g32TkeyIrqFlag = 0;
			HAL_TKEY_ReadChannelData_SingleOne(&TKEY_Handle);
		}
	
		if(g8RoundScanOver)  //wait one round scan over then process
		{
			g8RoundScanOver = 0;
			TKEY_ScanProcess(&TKEY_Handle);
					
			TKEY_Press_Relaese_Process();    
			ucKey = TouchKey_TimerSacn_GetKeyVal();
			if(ucKey != 0xFF)
			{
				#ifndef TKEY_WAVEFORM_PRINTF
				  printf("TK_%d\n", ucKey+1);  
				  g32IdleStartTick = HAL_GetTick();
				#endif
			}
					
			TKEY_Calibrate_Process();
			HAL_TKEY_StartScan();

		}                
    }
}






