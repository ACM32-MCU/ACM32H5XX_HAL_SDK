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

extern uint16_t g16ChannelMsk;
extern uint8_t g8RoundScanOver;

int num = 0;
uint32_t g32IdleStartTick;
volatile uint32_t gRTCIrqFlag = 0;

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


void TKEY_CSD_Init(void)
{  
    TKEY_CSDConfigTypeDef   TKEY_CSDConfig;

    HAL_PMU_Init(); 

    /* TKEY模式选择：CSA或CSD */
    TKEY_Handle.Init.Mode       = TKEY_MODE_CSD; 
    /* TKEY工作模式：常规或自动 */    
    TKEY_Handle.Init.WorkMode   = TKEY_WORK_MODE_NORMAL; 
    /* 扫描模式：单次或连续 */     
    TKEY_Handle.Init.ScanMode   = TKEY_SCAN_MODE_SINGLE; 

    /* 屏蔽通道使能 */
    TKEY_Handle.Init.ShieldEn   = TKEY_SHIELD_DISABLE;
    /* 自动模式下按键触摸条件硬件滤波次数 */    
    TKEY_Handle.Init.FilterNum  = TKEY_FILTER_NONE;   
    
    /* 充电电压VLDO */    
    TKEY_Handle.Init.Vldo       = TKEY_LDO_2V0;//TKEY_LDO_2V0;
    /* 比较器电压VREF */      
    TKEY_Handle.Init.Vref       = TKEY_VREF_QUARTER_VLDO;//TKEY_VREF_VBG;//TKEY_VREF_QUARTER_VLDO;//TKEY_VREF_HALF_VLDO TKEY_VREF_QUARTER_VLDO;
    /* 扫描时钟扩频使能 */    
    TKEY_Handle.Init.SpreadEn   = TKEY_SPREAD_DISABLE;
    /* TKEY通道使能 */    
    TKEY_Handle.Init.ChannelEn  = TKEY_CHANNEL_0_EN  | TKEY_CHANNEL_1_EN | TKEY_CHANNEL_2_EN | TKEY_CHANNEL_3_EN   | \
                                  TKEY_CHANNEL_4_EN | TKEY_CHANNEL_5_EN | TKEY_CHANNEL_6_EN | TKEY_CHANNEL_7_EN   | \
                                  TKEY_CHANNEL_8_EN | TKEY_CHANNEL_9_EN | TKEY_CHANNEL_10_EN | TKEY_CHANNEL_11_EN | \
                                  TKEY_CHANNEL_12_EN | TKEY_CHANNEL_13_EN | TKEY_CHANNEL_14_EN | TKEY_CHANNEL_15_EN;

	g16ChannelMsk = TKEY_Handle.Init.ChannelEn;
	
    /* 扫描时钟随机抖动，扫描时钟扩频使能后才有效 */ 
    TKEY_Handle.Timing.RandomJitterNum      = TKEY_RADOM_JITTER_1; 
    /* 两组扫描之间的间隔时间：(ScanInterval+1) * RC32k */    
    TKEY_Handle.Timing.ScanInterval         = 5;
    /*  扫描时钟分频：TKCLK / (ScanClkPrescale + 1) */    
    TKEY_Handle.Timing.ScanClkPrescale      = 25;//50;//25;//20 for RCH;//25 for XTH
    /* 扫描时钟SW1高电平(SW2低电平)：(ScanClkSW1HSW2L + 1)*TscanClk */ 
    TKEY_Handle.Timing.ScanClkSW1HSW2L      = 4;
    /* 扫描时钟SW1低电平(SW2高电平)：(ScanClkSW1LSW2H + 1)*TscanClk */    
    TKEY_Handle.Timing.ScanClkSW1LSW2H      = 4;   
    
    /*CSA超时时间或CSD PWM的周期：(TimeoutOrPwmCycle + 1) * RC32k*/    
    TKEY_Handle.Timing.TimeoutOrPwmCycle    = 10; 
    
    /* 采样次数 */    
    TKEY_Handle.Init.SampleNum  = TKEY_SAMPLE_3;//TKEY_SAMPLE_3;//TKEY_SAMPLE_1;
    /* 通道电容补偿选择 */    
    TKEY_Handle.Init.CCPSel  = TKEY_CAPCOMPENSTATE_4PF;//TKEY_CAPCOMPENSTATE_6PF;//TKEY_CAPCOMPENSTATE_14PF;//TKEY_CAPCOMPENSTATE_2PF;
    /* 通道电阻补偿选择 */    
    TKEY_Handle.Init.RCPSel  = TKEY_RESCOMPENSTATE_0_1K;//TKEY_RESCOMPENSTATE_0_1K;    
    
    HAL_TKEY_Init(&TKEY_Handle);       

    /* CSD模式配置 */
    /* 放电模式选择：电阻和电流源同时放电 */ 
    TKEY_CSDConfig.DischargeMode            = TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;//TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;//TKEY_DISMS_DISCHAG_CURRENT;//TKEY_DISMS_DISCHAG_CURRENT_PAY_RES;
    /* 预充使能 */    
    TKEY_CSDConfig.PrechargeEn              = TKEY_PRECHARGE_DISABLE;//TKEY_PRECHARGE_ENABLE;
    /* 放电电流选择：0.4uA * DischargeCurrent */
    TKEY_CSDConfig.DischargeCurrent         = 4;//17;//9;//17 // 2
    /* 放电电阻选择：Vref / 512K * DischargeRes */
    TKEY_CSDConfig.DischargeRes             = RES_CONST_DISCHARGE_VALUE;//4;//1;//3 // 6
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

/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint8_t  ucKey = 0xFF;

    #ifndef TKEY_WAVEFORM_PRINTF
		printfS("CAT TKEY CSD tset\r\n\r\n");     
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
			HAL_TKEY_ReadChannelData_SingleOne_Compensate(&TKEY_Handle);
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









