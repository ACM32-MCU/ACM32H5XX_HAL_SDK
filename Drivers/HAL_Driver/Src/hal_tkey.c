/******************************************************************************
*@file  : hal_tkey.c
*@brief : TKEY HAL module driver.
******************************************************************************/
#include "hal.h" 
#include "acm32h5xx_coreboard.h" 
#include "TKey.h" 

#ifdef HAL_TKEY_MODULE_ENABLED
extern uint8_t Cp_compensate[16];
uint16_t channel_scan_num= 0x01; 
uint16_t g16ChannelMsk;
uint8_t  g8RoundScanOver = 0;

/******************************************************************************
*@brief : TKEY interrupt handler       
*@param : None.
*@return: None
******************************************************************************/
__weak void HAL_TKEY_IRQHandler(void)
{
    /* 
        NOTE: This function should be modified, when the callback is needed
    */
    
    /* For Example */
    if((TKEY->SR & TKEY_SR_EOC) && (TKEY->IER & TKEY_IER_EOCIE))            //Scanning completed 
    {
        TKEY->SR = TKEY_SR_EOC;
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
    
//    if((TKEY->SR & TKEY_SR_CHGDONE) && (TKEY->IER & TKEY_IER_CHGDONEIE))    //Charging frequency reaches the set number of times
//    {
//        TKEY->SR = TKEY_SR_CHGDONE;
//    }
    
    if((TKEY->SR & TKEY_SR_MEOC) && (TKEY->IER & TKEY_IER_MEOCIE))          //Mutual inductance mode channel scan completed
    {
        TKEY->SR = TKEY_SR_MEOC;
    }
    
}


/******************************************************************************
*@brief : Initialize the TKEY MSP: GPIO, CLK, NVIC        
*@param : htkey: a pointer of TKEY_HandleTypeDef structure which contains 
*                the configuration information for the specified TKEY.
*@return: None
******************************************************************************/
__weak void HAL_TKEY_MspInit(TKEY_HandleTypeDef* htkey)
{   
    /*   
        PG0  - TK_CS
        PF12 - TK13
        PF13 - TK14
        PF14 - TK15    
    */
    GPIO_InitTypeDef GPIO_Handle; 
    
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
    /* 
		PF11 - TK12
		PF12 - TK13 ;  
		PF13 - TK14;  
		PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);

    /* 
		PG0  - TK_CS
		PG1  - TK8    
    */
    GPIO_Handle.Pin        = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOG, &GPIO_Handle);
    
    /* 
		PE7 - TK9 ;  
		PE8 - TK10; 
		PE9 - TK11; 
		PE10 - TK0;     
		PE11 - TK1;
		PE12 - TK2;
		PE13 - TK3;
		PE14 - TK4;
		PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);

    /* 
		PB10 - TK6 ;  
		PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
    
}

/******************************************************************************
*@brief : TKEY De-Initialize the TKEY clock, GPIO, IRQ.       
*@param : htkey: a pointer of TKEY_HandleTypeDef structure which contains 
*                the configuration information for the specified TKEY.
*@return: None
******************************************************************************/
__weak void HAL_TKEY_MspDeInit(TKEY_HandleTypeDef* htkey)
{
    /* 
      NOTE : This function is implemented in user xxx_hal_msp.c
    */
    
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htkey);
}


/******************************************************************************
*@brief : Initialize the TKEY module with parameters       
*@param : hospi: a pointer of TKEY_HandleTypeDef structure which contains 
*                the configuration information for the specified TKEY.
*@return: HAL status
******************************************************************************/
HAL_StatusTypeDef HAL_TKEY_Init(TKEY_HandleTypeDef* htkey)
{
    uint32_t u32RegTemp;
    
    /* Check the TKEY handle allocation */
    if (htkey == NULL)
    {
        return HAL_ERROR;
    } 

    /* Check TKEY Parameter */
    assert_param(IS_TKEY_MODE(htkey->Init.Mode));
    assert_param(IS_TKEY_WORK_MODE(htkey->Init.WorkMode));
    assert_param(IS_TKEY_SCAN_MODE(htkey->Init.ScanMode));
    assert_param(IS_TKEY_SHIELD(htkey->Init.ShieldEn));
    assert_param(IS_TKEY_FILTER(htkey->Init.FilterNum));
    assert_param(IS_TKEY_VLDO(htkey->Init.Vldo));
    assert_param(IS_TKEY_VREF(htkey->Init.Vref));
    assert_param(IS_TKEY_SPREAD(htkey->Init.SpreadEn)); 
    assert_param(IS_TKEY_CHANNEL_STATE(htkey->Init.ChannelEn));    
    assert_param(IS_TKEY_RADOM_JITTER(htkey->Timing.RandomJitterNum));
    assert_param(IS_TKEY_SCAN_INTERVAL(htkey->Timing.ScanInterval));
    assert_param(IS_TKEY_SCAN_CLK_PRESCALE(htkey->Timing.ScanClkPrescale));
    assert_param(IS_TKEY_SCAN_CLK_SW1HSW2L(htkey->Timing.ScanClkSW1HSW2L));
    assert_param(IS_TKEY_SCAN_CLK_SW1LSW2H(htkey->Timing.ScanClkSW1LSW2H));
    assert_param(IS_TKEY_SCAN_TIMEOUT_PWM_CYCLE(htkey->Timing.TimeoutOrPwmCycle));
    assert_param(IS_TKEY_CAPCOMPENSTATE(htkey->Init.CCPSel));
    assert_param(IS_TKEY_RESCOMPENSTATE(htkey->Init.RCPSel));   
    
    /* Init the low level hardware : GPIO, CLOCK, NVIC, DMA */
    HAL_TKEY_MspInit(htkey);

    /* PLL3Q = Fin * F / N / Q = 4*125/1/8 = 62.5M */
	HAL_RCC_RCHConfig(ENABLE,ENABLE);
		
    HAL_RCC_PLL3Config(ENABLE, RCC_PLL_SOURCE_RCH_DIV16, 1, 125, 4, 8);//src, n, f, p, q
	/*  Enable PLL3 QCLK*/
    HAL_RCC_PLL3QCLKConfig(ENABLE);

    /* Enable RC32K ,TKEY module control clock source is RC32K*/        
    HAL_RCC_RCLConfig(ENABLE);
    
    /* Enable pclk and scanning clockfor TKEY module, TKEY module scanning clock source is PLL3*/    
    __HAL_RCC_TKEY_CLK_ENABLE(); 

   
    /* Check if the Tkey scan is busy */
    while(TKEY->SR & TKEY_SR_BUSY);
    
    /* disable the Tkey scan */
    TKEY->CR &= ~(TKEY_CR_START); 
       
    /*---------------------------- TKEY CR Configuration -----------------*/
    u32RegTemp = ( htkey->Init.Mode | htkey->Init.WorkMode | htkey->Init.ScanMode | \
                            htkey->Init.ShieldEn | htkey->Init.SpreadEn); 
    if(htkey->Init.SpreadEn == TKEY_SPREAD_ENABLE)
    {
        u32RegTemp |= htkey->Timing.RandomJitterNum;
    }                                 
    TKEY->CR = u32RegTemp;     
    /*---------------------------- TKEY CFGR1 Configuration -----------------*/

    TKEY->CFGR1 = ( htkey->Init.FilterNum | htkey->Init.Vldo | htkey->Init.Vref | htkey->Init.SampleNum| (htkey->Init.CCPSel<<TKEY_CFGR1_CCPSEL_Pos) | (htkey->Init.RCPSel<<TKEY_CFGR1_RCPSEL_Pos));                                                    
    /*---------------------------- TKEY INTVLR Configuration -----------------*/
    TKEY->INTVLR = (htkey->Timing.ScanInterval); 
    
    /*---------------------------- TKEY DIVR Scan CLK div Configuration -----------------*/
    TKEY->DIVR = (TKEY->DIVR & (~TKEY_DIVR_SCANCLKDIV_Msk)) | (htkey->Timing.ScanClkPrescale << TKEY_DIVR_SCANCLKDIV_Pos)|\
    (TKEY->DIVR & (~TKEY_DIVR_SMPCLKDIV_Pos)) | (htkey->Timing.SampleClkPrescale << TKEY_DIVR_SMPCLKDIV_Pos);     
                          
    /*---------------------------- TKEY SCCR Configuration -----------------*/
    TKEY->SCCR = ((htkey->Timing.ScanClkSW1HSW2L << TKEY_SCCR_SW1H_Pos) | htkey->Timing.ScanClkSW1LSW2H); 
    
    /*---------------------------- TKEY TSETR Configuration -----------------*/
    TKEY->TSETR = ( htkey->Timing.TimeoutOrPwmCycle<< TKEY_TSETR_TSET_Pos);
    
    /*---------------------------- TKEY CXENR Configuration -----------------*/
    TKEY->CXENR = (htkey->Init.ChannelEn);
    
    /* Enable channel overall control */
    TKEY->CR |= TKEY_CR_CHEN;     
    
    /* Enable TKEY module */
    TKEY->CR |= TKEY_CR_TKEN;
    
    /* TKEY's internal simulation module requires 10us to stabilize */
    HAL_SimpleDelay(10000);//延迟时间待定
    
    return  HAL_OK;
}

HAL_StatusTypeDef HAL_TKEY_AutoConfig(TKEY_AutoConfigTypeDef *hauto)
{
    uint8_t temp;
    uint32_t u32RegTemp;
    
    /* Check the parameters */
    assert_param(IS_TKEY_FILTER(hauto->FilterNum));
//    assert_param(IS_TKEY_AUTO_KEYNUM(hauto->KeyNum));
//    assert_param(IS_TKEY_AUTO_COUNT(hauto->CountMode));

    
    u32RegTemp = TKEY->CXENR;
    
    if(u32RegTemp == 0)
    {
        return HAL_ERROR;
    }
	for(temp = 0; temp < 16; temp++)
	{
		if (u32RegTemp & 0x01)
		{ 
            /* Set the baseline value for channel x */
            TKEY->CH[temp] = hauto->Baseline[temp];
            
            /* Set the threshold value for channel x */
            TKEY->TH[temp] = hauto->Threshold[temp];
		}
		u32RegTemp >>= 1;
		if(u32RegTemp == 0)
        {
			break;
        }
	}
    
    /* Set hardware filtering times */
    TKEY->CFGR1 = (TKEY->CFGR1 & (~(TKEY_CFGR1_FLTSEL_Msk))) | hauto->FilterNum;

    /* Enable automatic mode */
    TKEY->CR |= TKEY_CR_AUTO;
    
    return HAL_OK;
}
/******************************************************************************
*@brief : De-Initialize the OSPI peripheral       
*@param : hospi: a pointer of OSPI_HandleTypeDef structure which contains 
*                the configuration information for the specified OSPI.
*@return: HAL status
******************************************************************************/
HAL_StatusTypeDef HAL_TKEY_DeInit(TKEY_HandleTypeDef* htkey)
{
    /* Check the TKEY handle allocation */
    if (htkey == NULL)
    {
        return HAL_ERROR;
    }
    
    htkey->Init.Mode = 0;
    htkey->Init.WorkMode = 0;
    htkey->Init.ScanMode = 0;
    htkey->Init.ShieldEn = 0;;
    htkey->Init.FilterNum = 0;
    htkey->Init.Vldo = 0;
    htkey->Init.Vref = 0;
    htkey->Init.SpreadEn = 0;
    htkey->Init.ChannelEn = 0; 

    htkey->Timing.RandomJitterNum = 0;
    htkey->Timing.ScanInterval = 0;
    htkey->Timing.ScanClkPrescale = 0;
    htkey->Timing.ScanClkSW1HSW2L = 0;
    htkey->Timing.ScanClkSW1LSW2H = 0;
    htkey->Timing.TimeoutOrPwmCycle = 0;  
    
    /*Check if the Tkey scan is busy*/
    while(TKEY->SR & TKEY_SR_BUSY);
    
    /*disable the Tkey scan*/
    TKEY->CR &= ~(TKEY_CR_START); 
    
    /*disable the Tkey moudle*/
    TKEY->CR &= ~TKEY_CR_TKEN;

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
    HAL_TKEY_MspDeInit(htkey);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : TKEY module enable or disable      
*@param : NewState: Specify the new state of the TKEY module.
*@return: None
******************************************************************************/
void HAL_TKEY_EnableDisable(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));   

    if (NewState == DISABLE)
    {
        TKEY->CR &= ~TKEY_CR_TKEN;
        HAL_SimpleDelay(10000);//延迟时间待定
    }
    else
    {
        TKEY->CR |= TKEY_CR_TKEN;
        HAL_SimpleDelay(10000);//延迟时间待定
    }   
}

/******************************************************************************
*@brief : TKEY start to scan        
*@param : None.
*@return: None
******************************************************************************/
void HAL_TKEY_StartScan(void)
{
    /*Start the Tkey scan*/    
    TKEY->CR |= TKEY_CR_START;
    /* Waiting for START flag to turn to 0 */
    while(TKEY->CR & TKEY_CR_START);
//    /* Waiting for WAKEUP flag to turn to 0 */  //H5不支持
//    while(TKEY->SR & TKEY_SR_WAKEUP);    
}


/******************************************************************************
*@brief : TKEY stop to scan        
*@param : None.
*@return: None
******************************************************************************/
void HAL_TKEY_StopScan(void)
{
    /*disable the Tkey scan*/
    TKEY->CR &= ~(TKEY_CR_START);    
}

/************************************************************************
 * function   : HAL_TKEY_ReadChannelData
 * Description: Read the count number of the all channels. 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
HAL_StatusTypeDef HAL_TKEY_ReadChannelData(TKEY_HandleTypeDef* htkey)
{
    uint8_t ucI;
    __IO uint32_t *gu32RegTemp;
    
    uint32_t u32RegTemp,channel_cnt;  
   
    if(htkey == NULL)
    {    
        return HAL_ERROR;
    }
    
    u32RegTemp = TKEY->CXENR;
    
    if(u32RegTemp == 0)
    {
        return HAL_ERROR;
    }
    channel_cnt = 0;
    for(ucI = 0; htkey->ChannelData[ucI].ChannelId != 0xFFFF; ucI++)
    {
		if (u32RegTemp & 0x01)
		{
			htkey->ChannelData[ucI].Tkey_Data->RawData = TKEY->CH[ucI];
		}
        u32RegTemp >>= 1;
    }
    return HAL_OK;
}

/******************************************************************************
*@brief : Read the count number of the last channel        
*@param : None.
*@return: The scan count result of the last channel or button touch event channel
******************************************************************************/ 
uint16_t HAL_TKEY_ReadLastChannelCount(void)
{ 
    return TKEY->DR;
}

/******************************************************************************
*@brief : Get the scanning channel number in normal mode or 
*         the channel number for scanning timeout
*         get the key touch channel number in automatic mode       
*@param : None.
*@return: channel number
******************************************************************************/ 
uint8_t HAL_TKEY_GetSacnTimeoutAutoChannelNumber(void)
{   
    return ((TKEY->SR & TKEY_SR_CHNUM_Msk)>> TKEY_SR_CHNUM_Pos);
}

/******************************************************************************
*@brief : CSA mode parameter configuration        
*@param : hcsa: a pointer of TKEY_CSAConfigTypeDef structure which contains 
*                the CSA mode configuration information for the specified TKEY.
*@return: HAL status
******************************************************************************/
HAL_StatusTypeDef HAL_TKEY_CSAConfig(TKEY_CSAConfigTypeDef* hcsa)
{
    /* Check the parameters */
    assert_param(IS_TKEY_CHGNUMDONE(hcsa->ChargeNumDoneEn)); 
    assert_param(IS_TKEY_CHGEACHDONE(hcsa->ChargeEachDoneEN));
    assert_param(IS_TKEY_COMPFILTERNUM(hcsa->ComparerFilterNum));
    assert_param(IS_TKEY_COMPFILTER(hcsa->ComparerFilterEN));
    assert_param(IS_TKEY_CHARGENUM(hcsa->ChargeNum));
    assert_param(IS_TKEY_DISCHARGETIME(hcsa->DischargeTime));
    assert_param(IS_TKEY_CHARGEDONEWAITTIME(hcsa->ChargeDoneWaitTime)); 
   
    /* Comparator filtering settings */
    if(hcsa->ComparerFilterEN == TKEY_COMPFILTER_ENABLE)
    {
        TKEY->CFLTR = (hcsa->ComparerFilterNum << TKEY_CFLTR_CFLTNUM_Pos | hcsa->ComparerFilterEN);
    }
    else
    {
        TKEY->CFLTR = hcsa->ComparerFilterEN;
    }
    
    /* Charging numbers setting */
    TKEY->NSETR = hcsa->ChargeNum;
    
    /* Set the waiting time after reaching the set number of charges */
    TKEY->TWAITR = hcsa->ChargeDoneWaitTime;
    
    /* Enable charging times to reach the function,  enable each charge to complete the function*/
    TKEY->CR = (TKEY->CR & (~(TKEY_CR_CHGDONEEN_Msk | TKEY_CR_CHGEN_Msk))) | hcsa->ChargeNumDoneEn | hcsa->ChargeEachDoneEN;
    
    return HAL_OK;
}


/******************************************************************************
*@brief : CSD mode parameter configuration        
*@param : hcsd: a pointer of TKEY_CSDConfigTypeDef structure which contains 
*                the CSD mode configuration information for the specified TKEY.
*@return: HAL status
******************************************************************************/
HAL_StatusTypeDef HAL_TKEY_CSDConfig(TKEY_CSDConfigTypeDef* hcsd)
{
    /* Check the parameters */
    assert_param(IS_TKEY_DISMS(hcsd->DischargeMode));
    assert_param(IS_TKEY_PRECHARGE(hcsd->PrechargeEn));
    assert_param(IS_TKEY_DISCHARGECURRENT(hcsd->DischargeCurrent));
    assert_param(IS_TKEY_DISCHARGERES(hcsd->DischargeRes));
    assert_param(IS_TKEY_SAMPLEBITCLKDIV(hcsd->SampleBitClkDiv));
    
    /* Set discharge current and resistance */
    TKEY->CFGR2 = (hcsd->DischargeCurrent << TKEY_CFGR2_IDISSEL_Pos) | (hcsd->DischargeRes);
    
    /* Set clock division for sampling bit streams */
    TKEY->DIVR = (TKEY->DIVR & (~TKEY_DIVR_SMPCLKDIV_Msk)) | hcsd->SampleBitClkDiv;
    
    /* Set discharge mode, enable precharge */
    TKEY->CR = (TKEY->CR & (~(TKEY_CR_DISMS_Msk | TKEY_CR_PCEN_Msk))) | (hcsd->DischargeMode) | (hcsd->PrechargeEn);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Mutual inductance mode parameter configuration        
*@param : hmutual: a pointer of TKEY_MutualConfigTypeDef structure which contains 
*                the CSD Mutual inductance configuration information for the specified TKEY.
*@return: HAL status
******************************************************************************/
HAL_StatusTypeDef HAL_TKEY_MutualConfig(TKEY_MutualConfigTypeDef* hmutual)
{
    /* Check the parameters */
    assert_param(IS_TKEY_MUTUAL_DELAY_TIME(hmutual->MutualDelayTime));
    assert_param(IS_TKEY_MUTUAL_DELAY_STATE(hmutual->MutualDelayEn));
    assert_param(IS_TKEY_MUTUAL_RESET(hmutual->MutualResetEn));
    assert_param(IS_TKEY_MUTUAL_CHANNEL_STATE(hmutual->MutualChannelEn));
    
    /* Disable mutual inductance mode */
    TKEY->MUTUALR &= ~TKEY_MUTUALR_MUTUALEN;
    
    /* clear MTXDLY MTXDLYEN MTXRST bits */   
    TKEY->MUTUALR &= ~(TKEY_MUTUALR_MTXDLY_Msk | TKEY_MUTUALR_MTXDLYEN_Msk | TKEY_MUTUALR_MTXRST_Msk);
    
    /* set MTXDLY MTXDLYEN MTXRST bits */   
    TKEY->MUTUALR |= (hmutual->MutualDelayTime << TKEY_MUTUALR_MTXDLY_Pos) | (hmutual->MutualDelayEn) | (hmutual->MutualResetEn);
    
    /* Enable mutual inductance transmission channel */
    if(hmutual->MutualChannelEn > 0xffff)
    {
        TKEY->TXENR[0] = hmutual->MutualChannelEn & 0xffff;
        TKEY->TXENR[1] = (hmutual->MutualChannelEn >> 16) & 0x3ff;             
    }
    else
    {
        TKEY->TXENR[0] = hmutual->MutualChannelEn;        
    }

    /* Enable mutual inductance mode */
    TKEY->MUTUALR |= TKEY_MUTUALR_MUTUALEN;     
        
    return HAL_OK;
}

/******************************************************************************
*@brief : TKEY mutual inductance mode enable or disable      
*@param : NewState: Specify the new state of the TKEY mutual inductance mode.
*@return: None
******************************************************************************/
void HAL_TKEY_MutualEnableDisable(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));   

    if (NewState == DISABLE)
    {
        TKEY->MUTUALR &= ~TKEY_MUTUALR_MUTUALEN;
    }
    else
    {
        TKEY->MUTUALR |= TKEY_MUTUALR_MUTUALEN; 
    }   
}


/******************************************************************************
*@brief : TKEY automatic mode enable or disable      
*@param : NewState: Specify the new state of the TKEY automatic mode.
*@return: None
******************************************************************************/
void HAL_TKEY_AutoEnableDisable(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));   

    if (NewState == DISABLE)
    {
        TKEY->CR &= ~TKEY_CR_AUTO;
    }
    else
    {
        TKEY->CR |= TKEY_CR_AUTO;
    }   
}

/******************************************************************************
*@brief : Set the interval time between two sets of scans         
*@param : time : the interval time.
*@return: None
******************************************************************************/ 
void HAL_TKEY_ScanIntervalConfig(uint16_t time)
{
    /* Check TKEY Parameter */
    assert_param(IS_TKEY_SCAN_INTERVAL(time));
    /* TKEY INTVLR Configuration */
    TKEY->INTVLR = time; 
}


/******************************************************************************
*@brief : Read the interval time between two sets of scans         
*@param : None.
*@return: the interval time
******************************************************************************/ 
uint16_t HAL_TKEY_ReadScanInterval(void)
{
    return TKEY->INTVLR;
}

/******************************************************************************
*@brief : Enables or disables the specified TKEY interrupts.
*@param : irq: specifies the TKEY interrupt sources to be enabled or disabled.                   
*@param : NewState: new state of the specified TKEY interrupts.
*                   This parameter can be: ENABLE or DISABLE.
*@return: None.
******************************************************************************/
void HAL_TKEY_ITConfig(uint32_t irq, FunctionalState NewState)  
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_TKEY_IT(irq));    

    if (NewState != DISABLE)
    {
        /* Enable the selected TKEY interrupts */
        TKEY->IER |= irq;
    }
    else
    {
        /* Disable the selected TKEY interrupts */
        TKEY->IER &= (~irq);
    }
}

/******************************************************************************
*@brief : Checks whether the specified TKEY flag is set or not.
*@param : flag: specifies the flag to check.                                               
*@return: The new state of flag (SET or RESET).
******************************************************************************/
FlagStatus HAL_TKEY_GetFlagStatus(uint32_t flag)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_TKEY_FLAG(flag));

    /* Check the status of the specified TKEY flag */
    if ((TKEY->SR & flag) != RESET)
    {
        /* flag is set */
        bitstatus = SET;
    }
    else
    {
        /* flag is reset */
        bitstatus = RESET;
    }
    /* Return the TKEY_FLAG status */
    return  bitstatus;
}

/******************************************************************************
*@brief : Clears the TKEY's pending flags.
*@param : flag: specifies the flag to clear.                       
*@return: None
******************************************************************************/
void HAL_TKEY_ClearFlag(uint32_t flag)
{
    /* Check the parameters */
    assert_param(IS_TKEY_FLAG(flag));

    /* Clear the selected TKEY flags */
    TKEY->SR = flag;
}

/******************************************************************************
*@brief : Checks whether the specified TKEY interrupt has occurred or not.
*@param : irq: specifies the TKEY interrupt source to check.                       
*@return: The new state of irq (SET or RESET).
******************************************************************************/
ITStatus HAL_TKEY_GetITStatus(uint32_t irq)
{
    ITStatus bitstatus = RESET;
    uint32_t enablestatus = 0;

    /* Check the parameters */
    assert_param(IS_TKEY_IT(irq));

    /* Get the irq enable bit status */
    enablestatus = (TKEY->IER & irq) ;

    /* Check the status of the specified TKEY interrupt */
    if (((TKEY->SR & irq) != RESET) && enablestatus)
    {
        /* irq is set */
        bitstatus = SET;
    }
    else
    {
        /* irq is reset */
        bitstatus = RESET;
    }
    /* Return the irq status */
    return  bitstatus;
}

/******************************************************************************
*@brief : Clears the TKEY's interrupt pending bits.
*@param : irq: specifies the TKEY interrupt pending bit to clear.
*@return: None
******************************************************************************/
void HAL_TKEY_ClearITPendingBit(uint32_t irq)
{
    /* Check the parameters */
    assert_param(IS_TKEY_IT(irq));
    
    if((TKEY->IER & irq)!= RESET)
    {
        /* Clear the interrupt pending bits in the TKEY_SR register */
        TKEY->SR = irq;
    } 
}


/******************************************************************************
*@brief : Get enabled TKEY interrupt.
*@param : None.                       
*@return: TKEY interrupt enabled.
******************************************************************************/
uint16_t HAL_TKEY_GetIT(void)
{
    /* Return the irq */
    return  TKEY->IER;
}

#endif


void HAL_TKEY_MspClean()
{
	 GPIO_InitTypeDef GPIO_Handle; 
    
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
			    /* Initialization GPIO */

    GPIO_Handle.Mode      = GPIO_MODE_INPUT;    
    GPIO_Handle.Pull      = GPIO_PULLDOWN;
    GPIO_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    
    /* 
		PF11 - TK12
		PF12 - TK13 ;  
		PF13 - TK14;  
		PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 0);

    /* 
		PG0  - TK_CS
		PG1  - TK8    
    */
//    GPIO_Handle.Pin        = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Handle.Pin        =GPIO_PIN_1;

    HAL_GPIO_Init(GPIOG, &GPIO_Handle);		
    //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, 0);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, 0);

    
    /* 
		PE7 - TK9 ;  
		PE8 - TK10; 
		PE9 - TK11; 
		PE10 - TK0;     
		PE11 - TK1;
		PE12 - TK2;
		PE13 - TK3;
		PE14 - TK4;
		PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);
		
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);

    /* 
		PB10 - TK6 ;  
		PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
		
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);

}

void HAL_TKEY_MspClean_GND()
{
	 GPIO_InitTypeDef GPIO_Handle; 
    
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Initialization GPIO */
   
    GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP; 
    GPIO_Handle.Pull      = GPIO_PULLDOWN;	
    GPIO_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, 0);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, 0);

    /* 
    PG0  - TK_CS
    PG1  - TK8    
    */
    GPIO_Handle.Pin        =  GPIO_PIN_1;
    HAL_GPIO_Init(GPIOG, &GPIO_Handle);		
    //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, 0);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, 0);

    
    /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);
		
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);

    /* 
    PB10 - TK6 ;  
    PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
		
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);

}



void gpio_open_0(void)  //channel 0 gpio开启模拟功能，其他gpio输出0
{   
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);     
}
void gpio_open_1(void)  //channel 1 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON;    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_2(void)  //channel 2 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_3(void)  //channel 3 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_4(void)  //channel 4 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_5(void)  //channel 5 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_6(void)  //channel 6 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
    /* 
    PB10 - TK6 ;  
    PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
}
void gpio_open_7(void)  //channel 7 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
    /* 
    PB10 - TK6 ;  
    PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
}
void gpio_open_8(void)  //channel 8 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PG0  - TK_CS
    PG1  - TK8    
    */
    GPIO_Handle.Pin        = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOG, &GPIO_Handle);    
}
void gpio_open_9(void)  //channel 9 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}   
void gpio_open_10(void)  //channel 10 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}
void gpio_open_11(void)  //channel 11 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}
void gpio_open_12(void)  //channel 12 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_13(void)  //channel 13 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_14(void)  //channel 14 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_15(void)  //channel 15 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
  
/************************************************************************
 * function   : HAL_TKEY_ReadChannelData
 * Description: Read the count number of the all channels(single mode). 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
void HAL_TKEY_ReadChannelData_SingleAll(TKEY_HandleTypeDef* htkey)
{        
	#ifdef TKEY_CHANNEL_0_ENABLE
    gpio_open_0();
    TKEY->CXENR = 1;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[0].Tkey_Data->RawData = TKEY->CH[0];  //读取通道0数据
	#endif

	#ifdef TKEY_CHANNEL_1_ENABLE
    gpio_open_1();
    TKEY->CXENR = 1<<1;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[1].Tkey_Data->RawData = TKEY->CH[1];  //读取通道1数据   
	#endif
	
    #ifdef TKEY_CHANNEL_2_ENABLE
    gpio_open_2();
    TKEY->CXENR = 1<<2;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[2].Tkey_Data->RawData = TKEY->CH[2];  //读取通道2数据    
	#endif

    #ifdef TKEY_CHANNEL_3_ENABLE
    gpio_open_3();
    TKEY->CXENR = 1<<3;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[3].Tkey_Data->RawData = TKEY->CH[3];  //读取通道3数据
	#endif
	
    #ifdef TKEY_CHANNEL_4_ENABLE
    gpio_open_4();
    TKEY->CXENR = 1<<4;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[4].Tkey_Data->RawData = TKEY->CH[4];  //读取通道4数据
	#endif
	
    #ifdef TKEY_CHANNEL_5_ENABLE
    gpio_open_5();
    TKEY->CXENR = 1<<5;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[5].Tkey_Data->RawData = TKEY->CH[5];  //读取通道5数据
	#endif
	
    #ifdef TKEY_CHANNEL_6_ENABLE
    gpio_open_6();
    TKEY->CXENR = 1<<6;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[6].Tkey_Data->RawData = TKEY->CH[6];  //读取通道6数据
	#endif
	
    #ifdef TKEY_CHANNEL_7_ENABLE
    gpio_open_7();
    TKEY->CXENR = 1<<7;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[7].Tkey_Data->RawData = TKEY->CH[7];  //读取通道7数据
	#endif
	
    #ifdef TKEY_CHANNEL_8_ENABLE
    gpio_open_8();
    TKEY->CXENR = 1<<8;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[8].Tkey_Data->RawData = TKEY->CH[8];  //读取通道8数据    
	#endif
	
    #ifdef TKEY_CHANNEL_9_ENABLE
    gpio_open_9();
    TKEY->CXENR = 1<<9;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[9].Tkey_Data->RawData = TKEY->CH[9];  //读取通道9数据 
	#endif
	
    #ifdef TKEY_CHANNEL_10_ENABLE
    gpio_open_10();
    TKEY->CXENR = 1<<10;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[10].Tkey_Data->RawData = TKEY->CH[10];  //读取通道10数据
	#endif
	
    #ifdef TKEY_CHANNEL_11_ENABLE
    gpio_open_11();
    TKEY->CXENR = 1<<11;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[11].Tkey_Data->RawData = TKEY->CH[11];  //读取通道11数据
	#endif
	
    #ifdef TKEY_CHANNEL_12_ENABLE
    gpio_open_12();
    TKEY->CXENR = 1<<12;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[12].Tkey_Data->RawData = TKEY->CH[12];  //读取通道12数据
	#endif
	
    #ifdef TKEY_CHANNEL_13_ENABLE
    gpio_open_13();
    TKEY->CXENR = 1<<13;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[13].Tkey_Data->RawData = TKEY->CH[13];  //读取通道13数据
 	#endif
	
    #ifdef TKEY_CHANNEL_14_ENABLE 
    gpio_open_14();
    TKEY->CXENR = 1<<14;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[14].Tkey_Data->RawData = TKEY->CH[14];  //读取通道14数据
	#endif
	
    #ifdef TKEY_CHANNEL_15_ENABLE
    gpio_open_15();
    TKEY->CXENR = 1<<15;    
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);    
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[15].Tkey_Data->RawData = TKEY->CH[15];  //读取通道15数据
	#endif
	    
}

/************************************************************************
 * function   : HAL_TKEY_ReadChannelData_SingleOne
 * Description: Read the count number of the one channel and start scan of next channel(single mode). 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
void HAL_TKEY_ReadChannelData_SingleOne(TKEY_HandleTypeDef* htkey)
{
	uint16_t scan_channel;
	uint16_t channel_num;

	//read data 	
	channel_num = (TKEY->SR>>4)&0xf;	
	htkey->ChannelData[channel_num].Tkey_Data->RawData = TKEY->CH[channel_num];  //读取通道数据

	//start next scan
	scan_channel = channel_scan_num & g16ChannelMsk;
	if(scan_channel == 0) // channel is off
	{
		if(channel_scan_num == TKEY_CXENR_CX15EN)
		{	
			channel_scan_num = 0x01;
			g8RoundScanOver = 1;
		}
		else
		{
			channel_scan_num = channel_scan_num<<1;  
		}
		return;
	}

	switch(scan_channel)
	{
		case TKEY_CXENR_CX0EN : 
			gpio_open_0();
			TKEY->CXENR = 1;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX1EN : 
			gpio_open_1();
			TKEY->CXENR = 1<<1;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX2EN :     
			gpio_open_2();
			TKEY->CXENR = 1<<2;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX3EN :    
			gpio_open_3();
			TKEY->CXENR = 1<<3;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX4EN : 
			gpio_open_4();
			TKEY->CXENR = 1<<4;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX5EN : 			
			gpio_open_5();
			TKEY->CXENR = 1<<5;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX6EN : 			
			gpio_open_6();
			TKEY->CXENR = 1<<6;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX7EN : 	
			gpio_open_7();
			TKEY->CXENR = 1<<7;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX8EN : 
			gpio_open_8();
			TKEY->CXENR = 1<<8;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX9EN :  			
			gpio_open_9();
			TKEY->CXENR = 1<<9;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX10EN : 
			gpio_open_10();
			TKEY->CXENR = 1<<10;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX11EN : 
			gpio_open_11();
			TKEY->CXENR = 1<<11;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX12EN : 
			gpio_open_12();
			TKEY->CXENR = 1<<12;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX13EN : 
			gpio_open_13();
			TKEY->CXENR = 1<<13;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX14EN : 
			gpio_open_14();
			TKEY->CXENR = 1<<14;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX15EN : 
			gpio_open_15();
			TKEY->CXENR = 1<<15;    
//			HAL_TKEY_StartScan(); 
		default:
			break;		
		}
	if(channel_scan_num == TKEY_CXENR_CX15EN)
	{	
		channel_scan_num = 0x01;
		g8RoundScanOver = 1;
	}
	else
	{
		channel_scan_num = channel_scan_num<<1;  
	}		
}

//扫描每个通道时切换不同补偿电容
void check_compensate(uint8_t cp)
{
    if(cp >= 100) 
    {
        /* 0pf补偿 */
        TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_0PF<<TKEY_CFGR1_CCPSEL_Pos) ;
       /* 恒放电参数+1 */        
        TKEY->CFGR2 = (TKEY->CFGR2 & (~TKEY_CFGR2_RDISSEL_Msk)) |((RES_CONST_DISCHARGE_VALUE+1)<<TKEY_CFGR2_RDISSEL_Pos) ; 
    }
    else  
    {
        /* 写回正常电阻恒放电值 */
        TKEY->CFGR2 = (TKEY->CFGR2 & (~TKEY_CFGR2_RDISSEL_Msk)) |((RES_CONST_DISCHARGE_VALUE)<<TKEY_CFGR2_RDISSEL_Pos) ; 
        //printf("TKEY->CFGR2 = %x\n",TKEY->CFGR2);
        /* 正常通道补偿 */
        if(cp == 0)
        TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_0PF<<TKEY_CFGR1_CCPSEL_Pos) ;
        else if(cp == 2)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_2PF<<TKEY_CFGR1_CCPSEL_Pos) ;                                                    
        else if(cp == 4)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_4PF<<TKEY_CFGR1_CCPSEL_Pos) ; 
        else if(cp == 6)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_6PF<<TKEY_CFGR1_CCPSEL_Pos) ; 
        else if(cp == 8)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_8PF<<TKEY_CFGR1_CCPSEL_Pos) ;
        else if(cp == 10)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_10PF<<TKEY_CFGR1_CCPSEL_Pos) ; 
        else if(cp == 12)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_12PF<<TKEY_CFGR1_CCPSEL_Pos) ;
        else if(cp == 14)
            TKEY->CFGR1 = (TKEY->CFGR1 & (~TKEY_CFGR1_CCPSEL_Msk)) |(TKEY_CAPCOMPENSTATE_14PF<<TKEY_CFGR1_CCPSEL_Pos) ;
    }
}
/************************************************************************
 * function   : HAL_TKEY_ReadChannelData_SingleOne
 * Description: Read the count number of the one channel and start scan of next channel(single mode,each channel use compensation capacitor). 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
void HAL_TKEY_ReadChannelData_SingleOne_Compensate(TKEY_HandleTypeDef* htkey)
{   
	uint16_t scan_channel;
	uint16_t channel_num;

	//read data 	
	channel_num = (TKEY->SR>>4)&0xf;	
	htkey->ChannelData[channel_num].Tkey_Data->RawData = TKEY->CH[channel_num];  //读取通道数据

	//start next scan
	scan_channel = channel_scan_num & g16ChannelMsk;
	if(scan_channel == 0) // channel is off
	{
		if(channel_scan_num == TKEY_CXENR_CX15EN)
		{	
			channel_scan_num = 0x01;
			g8RoundScanOver = 1;
		}
		else
		{
			channel_scan_num = channel_scan_num<<1;  
		}
		return;
	}

	switch(scan_channel)
	{
		case TKEY_CXENR_CX0EN : 
			check_compensate(Cp_compensate[0]);
			gpio_open_0();
			TKEY->CXENR = 1;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX1EN : 
			check_compensate(Cp_compensate[1]);
			gpio_open_1();
			TKEY->CXENR = 1<<1;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX2EN :
			check_compensate(Cp_compensate[2]);			
			gpio_open_2();
			TKEY->CXENR = 1<<2;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX3EN :
			check_compensate(Cp_compensate[3]);			
			gpio_open_3();
			TKEY->CXENR = 1<<3;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX4EN :
			check_compensate(Cp_compensate[4]);			
			gpio_open_4();
			TKEY->CXENR = 1<<4;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX5EN :
			check_compensate(Cp_compensate[5]); 			
			gpio_open_5();
			TKEY->CXENR = 1<<5;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX6EN : 
			check_compensate(Cp_compensate[6]);			
			gpio_open_6();
			TKEY->CXENR = 1<<6;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX7EN : 
			check_compensate(Cp_compensate[7]);			
			gpio_open_7();
			TKEY->CXENR = 1<<7;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX8EN :
			check_compensate(Cp_compensate[8]);			
			gpio_open_8();
			TKEY->CXENR = 1<<8;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX9EN :
			check_compensate(Cp_compensate[9]);  			
			gpio_open_9();
			TKEY->CXENR = 1<<9;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX10EN : 
			check_compensate(Cp_compensate[10]);
			gpio_open_10();
			TKEY->CXENR = 1<<10;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX11EN : 
			check_compensate(Cp_compensate[11]);
			gpio_open_11();
			TKEY->CXENR = 1<<11;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX12EN : 
			check_compensate(Cp_compensate[12]);
			gpio_open_12();
			TKEY->CXENR = 1<<12;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX13EN : 
			check_compensate(Cp_compensate[13]);
			gpio_open_13();
			TKEY->CXENR = 1<<13;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX14EN : 
			check_compensate(Cp_compensate[14]);
			gpio_open_14();
			TKEY->CXENR = 1<<14;
			HAL_TKEY_StartScan(); 
			break;
		
		case TKEY_CXENR_CX15EN :
			check_compensate(Cp_compensate[15]);			
			gpio_open_15();
			TKEY->CXENR = 1<<15;    
//			HAL_TKEY_StartScan(); 
		default:
			break;		
		}
	if(channel_scan_num == TKEY_CXENR_CX15EN)
	{	
		channel_scan_num = 0x01;
		g8RoundScanOver = 1;
	}
	else
	{
		channel_scan_num = channel_scan_num<<1;  
	}	   
}

/************************************************************************
 * function   : HAL_TKEY_ReadChannelData
 * Description: Read the count number of the all channels(single mode,each channel use compensation capacitor). 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
void HAL_TKEY_ReadChannelData_SingleAll_Compensate(TKEY_HandleTypeDef* htkey)
{        
	#ifdef TKEY_CHANNEL_0_ENABLE
	check_compensate(Cp_compensate[0]);
    gpio_open_0();
    TKEY->CXENR = 1;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[0].Tkey_Data->RawData = TKEY->CH[0];  //读取通道0数据
	#endif

	#ifdef TKEY_CHANNEL_1_ENABLE
	check_compensate(Cp_compensate[1]);
    gpio_open_1();
    TKEY->CXENR = 1<<1;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[1].Tkey_Data->RawData = TKEY->CH[1];  //读取通道1数据   
	#endif
	
    #ifdef TKEY_CHANNEL_2_ENABLE
	check_compensate(Cp_compensate[2]);
    gpio_open_2();
    TKEY->CXENR = 1<<2;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[2].Tkey_Data->RawData = TKEY->CH[2];  //读取通道2数据    
	#endif

    #ifdef TKEY_CHANNEL_3_ENABLE
	check_compensate(Cp_compensate[3]);
    gpio_open_3();
    TKEY->CXENR = 1<<3;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[3].Tkey_Data->RawData = TKEY->CH[3];  //读取通道3数据
	#endif
	
    #ifdef TKEY_CHANNEL_4_ENABLE
	check_compensate(Cp_compensate[4]);
    gpio_open_4();
    TKEY->CXENR = 1<<4;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[4].Tkey_Data->RawData = TKEY->CH[4];  //读取通道4数据
	#endif
	
    #ifdef TKEY_CHANNEL_5_ENABLE
	check_compensate(Cp_compensate[5]);
    gpio_open_5();
    TKEY->CXENR = 1<<5;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[5].Tkey_Data->RawData = TKEY->CH[5];  //读取通道5数据
	#endif
	
    #ifdef TKEY_CHANNEL_6_ENABLE
	check_compensate(Cp_compensate[6]);
    gpio_open_6();
    TKEY->CXENR = 1<<6;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[6].Tkey_Data->RawData = TKEY->CH[6];  //读取通道6数据
	#endif
	
    #ifdef TKEY_CHANNEL_7_ENABLE
	check_compensate(Cp_compensate[7]);
    gpio_open_7();
    TKEY->CXENR = 1<<7;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[7].Tkey_Data->RawData = TKEY->CH[7];  //读取通道7数据
	#endif
	
    #ifdef TKEY_CHANNEL_8_ENABLE
	check_compensate(Cp_compensate[8]);
    gpio_open_8();
    TKEY->CXENR = 1<<8;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[8].Tkey_Data->RawData = TKEY->CH[8];  //读取通道8数据    
	#endif
	
    #ifdef TKEY_CHANNEL_9_ENABLE
	check_compensate(Cp_compensate[9]);
    gpio_open_9();
    TKEY->CXENR = 1<<9;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[9].Tkey_Data->RawData = TKEY->CH[9];  //读取通道9数据 
	#endif
	
    #ifdef TKEY_CHANNEL_10_ENABLE
	check_compensate(Cp_compensate[10]);
    gpio_open_10();
    TKEY->CXENR = 1<<10;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[10].Tkey_Data->RawData = TKEY->CH[10];  //读取通道10数据
	#endif
	
    #ifdef TKEY_CHANNEL_11_ENABLE
	check_compensate(Cp_compensate[11]);
    gpio_open_11();
    TKEY->CXENR = 1<<11;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[11].Tkey_Data->RawData = TKEY->CH[11];  //读取通道11数据
	#endif
	
    #ifdef TKEY_CHANNEL_12_ENABLE
	check_compensate(Cp_compensate[12]);
    gpio_open_12();
    TKEY->CXENR = 1<<12;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[12].Tkey_Data->RawData = TKEY->CH[12];  //读取通道12数据
	#endif
	
    #ifdef TKEY_CHANNEL_13_ENABLE
	check_compensate(Cp_compensate[13]);
    gpio_open_13();
    TKEY->CXENR = 1<<13;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[13].Tkey_Data->RawData = TKEY->CH[13];  //读取通道13数据
 	#endif
	
    #ifdef TKEY_CHANNEL_14_ENABLE
	check_compensate(Cp_compensate[14]);	
    gpio_open_14();
    TKEY->CXENR = 1<<14;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[14].Tkey_Data->RawData = TKEY->CH[14];  //读取通道14数据
	#endif
	
    #ifdef TKEY_CHANNEL_15_ENABLE
	check_compensate(Cp_compensate[15]);
    gpio_open_15();
    TKEY->CXENR = 1<<15;    
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);    
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[15].Tkey_Data->RawData = TKEY->CH[15];  //读取通道15数据
	#endif
	    
}

void gpio_open_0_GND(void)  //channel 0 gpio开启模拟功能，其他gpio输出0
{   
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);     
}
void gpio_open_1_GND(void)  //channel 1 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON;    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_2_GND(void)  //channel 2 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_3_GND(void)  //channel 3 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_4_GND(void)  //channel 4 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_5_GND(void)  //channel 5 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle); 
}
void gpio_open_6_GND(void)  //channel 6 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
    /* 
    PB10 - TK6 ;  
    PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
}
void gpio_open_7_GND(void)  //channel 7 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
    /* 
    PB10 - TK6 ;  
    PB11 - TK7;  
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_Handle);
}
void gpio_open_8_GND(void)  //channel 8 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PG0  - TK_CS
    PG1  - TK8    
    */
    GPIO_Handle.Pin        = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOG, &GPIO_Handle);    
}
void gpio_open_9_GND(void)  //channel 9 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}   
void gpio_open_10_GND(void)  //channel 10 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}
void gpio_open_11_GND(void)  //channel 11 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    
     /* 
    PE7 - TK9 ;  
    PE8 - TK10; 
    PE9 - TK11; 
    PE10 - TK0;     
    PE11 - TK1;
    PE12 - TK2;
    PE13 - TK3;
    PE14 - TK4;
    PE15 - TK5;
    */
    GPIO_Handle.Pin        = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOE, &GPIO_Handle);   
}
void gpio_open_12_GND(void)  //channel 12 gpio开启模拟功能，其他gpio输出0
{
   GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_13_GND(void)  //channel 13 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_14_GND(void)  //channel 14 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
void gpio_open_15_GND(void)  //channel 15 gpio开启模拟功能，其他gpio输出0
{
    GPIO_InitTypeDef GPIO_Handle;

    HAL_TKEY_MspClean_GND();    
  
    GPIO_Handle.Mode       = GPIO_MODE_ANALOG_SWITCH_ON; 
    /* 
    PF11 - TK12
    PF12 - TK13 ;  
    PF13 - TK14;  
    PF14 - TK15 
    */
    GPIO_Handle.Pin        = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOF, &GPIO_Handle);
}
  
/************************************************************************
 * function   : HAL_TKEY_ReadChannelData
 * Description: Read the count number of the all channels(single mode). 
 * input      : htkey : TKEY handle
 * return     : HAL_OK: success; HAL_ERROR: failed.
 ************************************************************************/
void HAL_TKEY_ReadChannelData_SingleAll_GND(TKEY_HandleTypeDef* htkey)
{        
    gpio_open_0_GND();
    TKEY->CXENR = 1;    
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    
    htkey->ChannelData[0].Tkey_Data->RawData = TKEY->CH[0];  //读取通道0数据

    gpio_open_1_GND();
    TKEY->CXENR = 1<<1;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[1].Tkey_Data->RawData = TKEY->CH[1];  //读取通道1数据   

    
    gpio_open_2_GND();
    TKEY->CXENR = 1<<2;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[2].Tkey_Data->RawData = TKEY->CH[2];  //读取通道2数据    
 
    gpio_open_3_GND();
    TKEY->CXENR = 1<<3;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[3].Tkey_Data->RawData = TKEY->CH[3];  //读取通道3数据

    gpio_open_4_GND();
    TKEY->CXENR = 1<<4;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[4].Tkey_Data->RawData = TKEY->CH[4];  //读取通道4数据
    
    gpio_open_5_GND();
    TKEY->CXENR = 1<<5;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[5].Tkey_Data->RawData = TKEY->CH[5];  //读取通道5数据
    
    gpio_open_6_GND();
    TKEY->CXENR = 1<<6;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[6].Tkey_Data->RawData = TKEY->CH[6];  //读取通道6数据
    
    gpio_open_7_GND();
    TKEY->CXENR = 1<<7;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[7].Tkey_Data->RawData = TKEY->CH[7];  //读取通道7数据
    
    gpio_open_8_GND();
    TKEY->CXENR = 1<<8;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[8].Tkey_Data->RawData = TKEY->CH[8];  //读取通道8数据    
    
    gpio_open_9_GND();
    TKEY->CXENR = 1<<9;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[9].Tkey_Data->RawData = TKEY->CH[9];  //读取通道9数据 

    gpio_open_10_GND();
    TKEY->CXENR = 1<<10;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[10].Tkey_Data->RawData = TKEY->CH[10];  //读取通道10数据
    
    gpio_open_11_GND();
    TKEY->CXENR = 1<<11;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[11].Tkey_Data->RawData = TKEY->CH[11];  //读取通道11数据

    gpio_open_12_GND();
    TKEY->CXENR = 1<<12;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[12].Tkey_Data->RawData = TKEY->CH[12];  //读取通道12数据
    
    gpio_open_13_GND();
    TKEY->CXENR = 1<<13;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[13].Tkey_Data->RawData = TKEY->CH[13];  //读取通道13数据
  
    gpio_open_14_GND();
    TKEY->CXENR = 1<<14;
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[14].Tkey_Data->RawData = TKEY->CH[14];  //读取通道14数据
    
    gpio_open_15_GND();
    TKEY->CXENR = 1<<15;    
    HAL_TKEY_StartScan(); 
    while(g32TkeyIrqFlag == 0);    
    g32TkeyIrqFlag = 0;
    htkey->ChannelData[15].Tkey_Data->RawData = TKEY->CH[15];  //读取通道15数据
    
}



