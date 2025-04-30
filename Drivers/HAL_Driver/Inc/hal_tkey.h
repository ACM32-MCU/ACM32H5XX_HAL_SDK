/******************************************************************************
*@file  : hal_tkey.h
*@brief : Header file of TKEY HAL module.
******************************************************************************/
#ifndef __HAL_TKEY_H__
#define __HAL_TKEY_H__

#include "acm32h5xx_hal_conf.h"
//#define TKEY_SCAN_SRC_XTH

extern volatile uint32_t g32TkeyIrqFlag;

#define RES_CONST_DISCHARGE_VALUE 6

/** @defgroup TKEY_Mode
  * @{
  */
#define TKEY_MODE_CSD                               (0U)
#define TKEY_MODE_CSA                               (TKEY_CR_TKMS)
#define IS_TKEY_MODE(__MODE__)                      (((__MODE__) == TKEY_MODE_CSD)  || \
                                                    ((__MODE__) == TKEY_MODE_CSA))
/**
  * @}
  */
  
/** @defgroup TKEY_Work_Mode
  * @{
  */
#define TKEY_WORK_MODE_NORMAL                       (0U)
#define TKEY_WORK_MODE_AUTO                         (TKEY_CR_AUTO)
#define IS_TKEY_WORK_MODE(__WORKMODE__)             (((__WORKMODE__) == TKEY_WORK_MODE_NORMAL)  || \
                                                    ((__WORKMODE__) == TKEY_WORK_MODE_AUTO))
/**
  * @}
  */
  
/** @defgroup TKEY_Scan_Mode
  * @{
  */
#define TKEY_SCAN_MODE_SINGLE                       (0U)
#define TKEY_SCAN_MODE_CONT                         (TKEY_CR_CONT)
#define IS_TKEY_SCAN_MODE(__SCANMODE__)             (((__SCANMODE__) == TKEY_SCAN_MODE_SINGLE)  || \
                                                    ((__SCANMODE__) == TKEY_SCAN_MODE_CONT))
/**
  * @}
  */
  
/** @defgroup TKEY_Shield_State
  * @{
  */
#define TKEY_SHIELD_DISABLE                         (0U)
#define TKEY_SHIELD_ENABLE                          (TKEY_CR_SHIELDEN)
#define IS_TKEY_SHIELD(__SHIELD__)                  (((__SHIELD__) == TKEY_SHIELD_DISABLE)  || \
                                                    ((__SHIELD__) == TKEY_SHIELD_ENABLE))
/**
  * @}
  */


/** @defgroup TKEY_Filter_Num
  * @{
  */
#define TKEY_FILTER_NONE                            (0U)
#define TKEY_FILTER_1                               (TKEY_CFGR1_FLTSEL_0)
#define TKEY_FILTER_2                               (TKEY_CFGR1_FLTSEL_1)
#define TKEY_FILTER_3                               (TKEY_CFGR1_FLTSEL_0 | TKEY_CFGR1_FLTSEL_1)
#define TKEY_FILTER_4                               (TKEY_CFGR1_FLTSEL_2)
#define TKEY_FILTER_5                               (TKEY_CFGR1_FLTSEL_2 | TKEY_CFGR1_FLTSEL_0)
#define TKEY_FILTER_6                               (TKEY_CFGR1_FLTSEL_2 | TKEY_CFGR1_FLTSEL_1)
#define TKEY_FILTER_7                               (TKEY_CFGR1_FLTSEL_2 | TKEY_CFGR1_FLTSEL_1 | TKEY_CFGR1_FLTSEL_0)
#define IS_TKEY_FILTER(__FILTER__)                  (((__FILTER__) == TKEY_FILTER_NONE)  || \
                                                    ((__FILTER__) == TKEY_FILTER_1)      || \
                                                    ((__FILTER__) == TKEY_FILTER_2)      || \
                                                    ((__FILTER__) == TKEY_FILTER_3)      || \
                                                    ((__FILTER__) == TKEY_FILTER_4)      || \
                                                    ((__FILTER__) == TKEY_FILTER_5)      || \
                                                    ((__FILTER__) == TKEY_FILTER_6)      || \
                                                    ((__FILTER__) == TKEY_FILTER_7))
/**
  * @}
  */
  
/** @defgroup TKEY_Factor_Value
  * @{
  */
#define TKEY_FACTOR_RIGHT_SHIFT_0                   (0U)
#define TKEY_FACTOR_RIGHT_SHIFT_1                   (TKEY_CFGR1_COEFSEL_0)
#define TKEY_FACTOR_RIGHT_SHIFT_2                   (TKEY_CFGR1_COEFSEL_1)
#define TKEY_FACTOR_RIGHT_SHIFT_3                   (TKEY_CFGR1_COEFSEL_0 | TKEY_CFGR1_COEFSEL_1)
#define IS_TKEY_FACTOR(__FACTOR__)                  (((__FACTOR__) == TKEY_FACTOR_RIGHT_SHIFT_0)    || \
                                                    ((__FACTOR__) == TKEY_FACTOR_RIGHT_SHIFT_1)     || \
                                                    ((__FACTOR__) == TKEY_FACTOR_RIGHT_SHIFT_2)     || \
                                                    ((__FACTOR__) == TKEY_FACTOR_RIGHT_SHIFT_3))
/**
  * @}
  */  

/** @defgroup TKEY_Sample_Num
  * @{
  */
#define TKEY_SAMPLE_1                               (0U)
#define TKEY_SAMPLE_3                               (TKEY_CFGR1_SMPSEL_0)
#define TKEY_SAMPLE_6                               (TKEY_CFGR1_SMPSEL_1)
#define TKEY_SAMPLE_10                              (TKEY_CFGR1_SMPSEL_0 | TKEY_CFGR1_SMPSEL_1)

#define IS_TKEY_SAMPLE(__SAMPLE__)                  (((__SAMPLE__) == TKEY_SAMPLE_1)    || \
                                                    ((__SAMPLE__) == TKEY_SAMPLE_3)     || \
                                                    ((__SAMPLE__) == TKEY_SAMPLE_6)     || \
                                                    ((__SAMPLE__) == TKEY_SAMPLE_10))
/**
  * @}
  */ 
  
/** @defgroup TKEY_Max_Delete
  * @{
  */
#define TKEY_MAX_DELETE_DISABLE                     (0U)
#define TKEY_MAX_DELETE_ENABLE                      (TKEY_CFGR1_MAXDEL)
#define IS_TKEY_MAX_DELETE(__STATE__)               (((__STATE__) == TKEY_MAX_DELETE_DISABLE)  || \
                                                    ((__STATE__) == TKEY_MAX_DELETE_ENABLE))
/**
  * @}
  */

/** @defgroup TKEY_Min_Delete
  * @{
  */
#define TKEY_MIN_DELETE_DISABLE                     (0U)
#define TKEY_MIN_DELETE_ENABLE                      (TKEY_CFGR1_MINDEL)
#define IS_TKEY_MIN_DELETE(__STATE__)               (((__STATE__) == TKEY_MIN_DELETE_DISABLE)  || \
                                                    ((__STATE__) == TKEY_MIN_DELETE_ENABLE))
/**
  * @}
  */

/** @defgroup TKEY_FastMode
  * @{
  */
#define TKEY_FAST_MODE_DISABLE                      (0U)
#define TKEY_FAST_MODE_ENABLE                       (TKEY_CFGR1_MODESEL)
#define IS_TKEY_FAST_MODE(__STATE__)                (((__STATE__) == TKEY_FAST_MODE_DISABLE)  || \
                                                    ((__STATE__) == TKEY_FAST_MODE_ENABLE))
/**
  * @}
  */ 

/** @defgroup TKEY_VLDO
  * @{
  */
#define TKEY_LDO_1V6                                (0U)
#define TKEY_LDO_2V0                                (TKEY_CFGR1_VCHRSEL_0)
#define TKEY_LDO_2V4                                (TKEY_CFGR1_VCHRSEL_1)
#define TKEY_LDO_VDD33                              (TKEY_CFGR1_VCHRSEL_0 | TKEY_CFGR1_VCHRSEL_1)
#define IS_TKEY_VLDO(__VLDO__)                      (((__VLDO__) == TKEY_LDO_1V6)       || \
                                                    ((__VLDO__) == TKEY_LDO_2V0)        || \
                                                    ((__VLDO__) == TKEY_LDO_2V4)        || \
                                                    ((__VLDO__) == TKEY_LDO_VDD33))
/**
  * @}
  */ 


/** @defgroup TKEY_VREF
  * @{
  */
#define TKEY_VREF_QUARTER_VLDO                      (0U)
#define TKEY_VREF_HALF_VLDO                         (TKEY_CFGR1_VCMPSEL_0)
#define TKEY_VREF_THREE_QUARTER_VLDO                (TKEY_CFGR1_VCMPSEL_1)
#define TKEY_VREF_VBG                               (TKEY_CFGR1_VCMPSEL_0 | TKEY_CFGR1_VCMPSEL_1)
#define IS_TKEY_VREF(__VREF__)                      (((__VREF__) == TKEY_VREF_QUARTER_VLDO)         || \
                                                    ((__VREF__) == TKEY_VREF_HALF_VLDO)             || \
                                                    ((__VREF__) == TKEY_VREF_THREE_QUARTER_VLDO)    || \
                                                    ((__VREF__) == TKEY_VREF_VBG))
/**
  * @}
  */ 

/** @defgroup TKEY_Spread_State
  * @{
  */
#define TKEY_SPREAD_DISABLE                         (0U)
#define TKEY_SPREAD_ENABLE                          (TKEY_CR_SPREAD)
#define IS_TKEY_SPREAD(__SPREAD__)                  (((__SPREAD__) == TKEY_SPREAD_DISABLE)  || \
                                                    ((__SPREAD__) == TKEY_SPREAD_ENABLE))
/**
  * @}
  */

/** @defgroup TKEY_Random_Jitter
  * @{
  */
#define TKEY_RADOM_JITTER_1                         (0U)
#define TKEY_RADOM_JITTER_1_2                       (TKEY_CR_RANDM_0)
#define TKEY_RADOM_JITTER_1_2_3                     (TKEY_CR_RANDM_1)
#define TKEY_RADOM_JITTER_1_2_3_4                   (TKEY_CR_RANDM_0 | TKEY_CR_RANDM_1)
#define IS_TKEY_RADOM_JITTER(__JITTER__)            (((__JITTER__) == TKEY_VREF_QUARTER_VLDO)       || \
                                                    ((__JITTER__) == TKEY_VREF_HALF_VLDO)           || \
                                                    ((__JITTER__) == TKEY_VREF_THREE_QUARTER_VLDO)  || \
                                                    ((__JITTER__) == TKEY_VREF_VBG))
/**
  * @}
  */

/** @defgroup TKEY_Scan_Interval
  * @{
  */
#define IS_TKEY_SCAN_INTERVAL(__INTERVAL__)         ((__INTERVAL__) <= 0xffffU)           
/**
  * @}
  */


/** @defgroup TKEY_Scan_Clk_Prescale
  * @{
  */
#define IS_TKEY_SCAN_CLK_PRESCALE(__PRESCALE__)     ((__PRESCALE__) <= 0xffU)           
/**
  * @}
  */


/** @defgroup TKEY_Scan_Clk_SW1HSW2L
  * @{
  */
#define IS_TKEY_SCAN_CLK_SW1HSW2L(__SW1HSW2L__)     ((__SW1HSW2L__) <= 0xffU)           
/**
  * @}
  */  


/** @defgroup TKEY_Scan_Clk_SW1LSW2H
  * @{
  */
#define IS_TKEY_SCAN_CLK_SW1LSW2H(__SW1LSW2H__)     ((__SW1LSW2H__) <= 0xffU)           
/**
  * @}
  */
  
/** @defgroup TKEY_Sample_Clk_Prescale
  * @{
  */
#define IS_TKEY_SAMPLE_CLK_PRESCALE(__PRESCALE__)     ((__PRESCALE__) <= 0xffU)           
/**
  * @}
  */
  

/** @defgroup TKEY_ScanTimeout_PwmCycle
  * @{
  */
#define IS_TKEY_SCAN_TIMEOUT_PWM_CYCLE(__VALUE__)   ((__VALUE__) <= 0xfffU)           
/**
  * @}
  */  


/** @defgroup TKEY_Flag 
  * @{
  */ 
#define TKEY_FLAG_EOC                               (TKEY_SR_EOC)
#define TKEY_FLAG_YESTOUCH                          (TKEY_SR_YESTOUCH)
#define TKEY_FLAG_TIMEOUT                           (TKEY_SR_TIMEOUT)  
#define TKEY_FLAG_BUSY                              (TKEY_SR_BUSY) 
#define TKEY_FLAG_DONE                              (TKEY_SR_DONE)
#define TKEY_FLAG_CHG                               (TKEY_SR_CHG) 
#define TKEY_FLAG_CHGDONE                           (TKEY_SR_CHGDONE)
#define TKEY_FLAG_MEOC                              (TKEY_SR_MEOC)
#define TKEY_FLAG_ALL                               (0xffff)
#define IS_TKEY_FLAG(__FLAG__)                      (((__FLAG__) == TKEY_FLAG_EOC)      || \
                                                    ((__FLAG__) == TKEY_FLAG_YESTOUCH)  || \
                                                    ((__FLAG__) == TKEY_FLAG_TIMEOUT)   || \
                                                    ((__FLAG__) == TKEY_FLAG_BUSY)      || \
                                                    ((__FLAG__) == TKEY_FLAG_DONE)      || \
                                                    ((__FLAG__) == TKEY_FLAG_CHG)       || \
                                                    ((__FLAG__) == TKEY_FLAG_CHGDONE)   || \
                                                    ((__FLAG__) == TKEY_FLAG_MEOC)      || \
                                                    ((__FLAG__) <= TKEY_FLAG_ALL))   
/**
  * @}
  */

/** @defgroup TKEY_IT 
  * @{
  */  
#define TKEY_IT_EOCIE                               (TKEY_IER_EOCIE)
#define TKEY_IT_YESTOUCHIE                          (TKEY_IER_YESTOUCHIE)
#define TKEY_IT_TIMEOUTIE                           (TKEY_IER_TIMEOUTIE) 
#define TKEY_IT_DONEIE                              (TKEY_IER_DONEIE) 
#define TKEY_IT_CHIE                                (TKEY_IER_CHIE)  
#define TKEY_IT_CHGDONEIE                           (TKEY_IER_CHGDONEIE)
#define TKEY_IT_MEOCIE                              (TKEY_IER_MEOCIE)
#define TKEY_IT_ALL                                 (0xffff)
#define IS_TKEY_IT(__IT__)                          (((__IT__) == TKEY_IT_EOCIE)        || \
                                                    ((__IT__) == TKEY_IT_YESTOUCHIE)    || \
                                                    ((__IT__) == TKEY_IT_TIMEOUTIE)     || \
                                                    ((__IT__) == TKEY_IT_DONEIE)        || \
                                                    ((__IT__) == TKEY_IT_CHIE)          || \
                                                    ((__IT__) == TKEY_IT_CHGDONEIE)     || \
                                                    ((__IT__) == TKEY_IT_MEOCIE)        || \
                                                    ((__IT__) <= TKEY_IT_ALL))   
/**
  * @}
  */   


/** @defgroup TKEY_Channel_Number 
* @{
*/
#define TKEY_CHANNEL_0                              (0U)    
#define TKEY_CHANNEL_1                              (1U)    
#define TKEY_CHANNEL_2                              (2U)    
#define TKEY_CHANNEL_3                              (3U)    
#define TKEY_CHANNEL_4                              (4U)    
#define TKEY_CHANNEL_5                              (5U)    
#define TKEY_CHANNEL_6                              (6U)    
#define TKEY_CHANNEL_7                              (7U)    
#define TKEY_CHANNEL_8                              (8U)    
#define TKEY_CHANNEL_9                              (9U)    
#define TKEY_CHANNEL_10                             (10U)    
#define TKEY_CHANNEL_11                             (11U)    
#define TKEY_CHANNEL_12                             (12U)    
#define TKEY_CHANNEL_13                             (13U)    
#define TKEY_CHANNEL_14                             (14U)    
#define TKEY_CHANNEL_15                             (15U)         
#define IS_TKEY_CHANNEL(CHANNEL)                    (((CHANNEL) == TKEY_CHANNEL_0)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_1)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_2)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_3)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_4)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_5)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_6)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_7)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_8)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_9)   || \
                                                    ((CHANNEL) == TKEY_CHANNEL_10)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_11)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_12)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_13)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_14)  || \
                                                    ((CHANNEL) == TKEY_CHANNEL_15))

/** @defgroup TKEY_Channel_State 
* @{
*/  

 
#define TKEY_CHANNEL_0_EN                           (TKEY_CXENR_CX0EN)    
#define TKEY_CHANNEL_1_EN                           (TKEY_CXENR_CX1EN)    
#define TKEY_CHANNEL_2_EN                           (TKEY_CXENR_CX2EN)    
#define TKEY_CHANNEL_3_EN                           (TKEY_CXENR_CX3EN)    
#define TKEY_CHANNEL_4_EN                           (TKEY_CXENR_CX4EN)    
#define TKEY_CHANNEL_5_EN                           (TKEY_CXENR_CX5EN)    
#define TKEY_CHANNEL_6_EN                           (TKEY_CXENR_CX6EN)    
#define TKEY_CHANNEL_7_EN                           (TKEY_CXENR_CX7EN)    
#define TKEY_CHANNEL_8_EN                           (TKEY_CXENR_CX8EN)    
#define TKEY_CHANNEL_9_EN                           (TKEY_CXENR_CX9EN)    
#define TKEY_CHANNEL_10_EN                          (TKEY_CXENR_CX10EN)    
#define TKEY_CHANNEL_11_EN                          (TKEY_CXENR_CX11EN)    
#define TKEY_CHANNEL_12_EN                          (TKEY_CXENR_CX12EN)    
#define TKEY_CHANNEL_13_EN                          (TKEY_CXENR_CX13EN)    
#define TKEY_CHANNEL_14_EN                          (TKEY_CXENR_CX14EN)    
#define TKEY_CHANNEL_15_EN                          (TKEY_CXENR_CX15EN)
#define TKEY_CHANNEL_ALL_EN                         (0xffffU)
#define TKEY_CHANNEL_ALL_DISABLE                    (0U)         
#define IS_TKEY_CHANNEL_STATE(__STATE__)            ((__STATE__) <= 0xffffU)  
                                                    

/** @defgroup TKEY_ChargeNumDone_State
  * @{
  */
#define TKEY_CHGNUMDONE_DISABLE                     (0U)
#define TKEY_CHGNUMDONE_ENABLE                      (TKEY_CR_CHGDONEEN)
#define IS_TKEY_CHGNUMDONE(__STATE__)               (((__STATE__) == TKEY_CHGNUMDONE_ENABLE)  || \
                                                    ((__STATE__) == TKEY_CHGNUMDONE_DISABLE))
/**
  * @}
  */


/** @defgroup TKEY_ChargeEachDone_State
  * @{
  */
#define TKEY_CHGEACHDONE_DISABLE                    (0U)
#define TKEY_CHGEACHDONE_ENABLE                     (TKEY_CR_CHGEN)
#define IS_TKEY_CHGEACHDONE(__STATE__)              (((__STATE__) == TKEY_CHGEACHDONE_ENABLE)  || \
                                                    ((__STATE__) == TKEY_CHGEACHDONE_DISABLE))
/**
  * @}
  */

/** @defgroup TKEY_Cs_DisChargeTimeOrCMPWaitTime
  * @{
  */
#define IS_TKEY_CSDISCHARGETIME_CMPWAITTIME(__VALUE__)          ((__VALUE__) <= 0xfU)           
/**
  * @}
  */

/** @defgroup TKEY_CompFilter_Num
  * @{
  */
#define IS_TKEY_COMPFILTERNUM(__VALUE__)            ((__VALUE__) <= 0xfU)           
/**
  * @}
  */

/** @defgroup TKEY_CompFilter_State
  * @{
  */
#define TKEY_COMPFILTER_DISABLE                     (0U)
#define TKEY_COMPFILTER_ENABLE                      (TKEY_CFLTR_CFLTEN)
#define IS_TKEY_COMPFILTER(__STATE__)               (((__STATE__) == TKEY_COMPFILTER_ENABLE)  || \
                                                    ((__STATE__) == TKEY_COMPFILTER_DISABLE))
/**
  * @}
  */  
  
/** @defgroup TKEY_ChargeNumber
  * @{
  */
#define IS_TKEY_CHARGENUM(__VALUE__)                ((__VALUE__) <= 0xffffU)           
/**
  * @}
  */ 

/** @defgroup TKEY_ChargeDoneWaitTime
  * @{
  */
#define IS_TKEY_CHARGEDONEWAITTIME(__VALUE__)       ((__VALUE__) <= 0xffffU)           
/**
  * @}
  */
  
/** @defgroup TKEY_DischargeTime
  * @{
  */
#define IS_TKEY_DISCHARGETIME(__VALUE__)       ((__VALUE__) <= 0xfU)           
/**
  * @}
  */
  
/** @defgroup TKEY_MutualDelay
  * @{
  */
#define IS_TKEY_MUTUAL_DELAY_TIME(__TIME__)         ((__TIME__) <= 0xffU)           
/**
  * @}
  */

/** @defgroup TKEY_MutualDelay_State
  * @{
  */
#define TKEY_MUTUAL_DELAY_DISABLE                   (0U)
#define TKEY_MUTUAL_DELAY_ENABLE                    (TKEY_MUTUALR_MTXDLYEN)
#define IS_TKEY_MUTUAL_DELAY_STATE(__STATE__)       (((__STATE__) == TKEY_MUTUAL_DELAY_DISABLE)  || \
                                                    ((__STATE__) == TKEY_MUTUAL_DELAY_ENABLE))
/**
  * @}
  */ 

/** @defgroup TKEY_MutualReset_State
  * @{
  */
#define TKEY_MUTUAL_RESET_DISABLE                   (0U)
#define TKEY_MUTUAL_RESET_ENABLE                    (TKEY_MUTUALR_MTXRST)
#define IS_TKEY_MUTUAL_RESET(__STATE__)             (((__STATE__) == TKEY_MUTUAL_RESET_DISABLE)  || \
                                                    ((__STATE__) == TKEY_MUTUAL_RESET_ENABLE))
/**
  * @}
  */ 
  
/** @defgroup TKEY_Mutual_Channel_State 
* @{
*/  
#define TKEY_MUTUAL_CHANNEL0_EN                     (TKEY_TXENR1_TX0EN)    
#define TKEY_MUTUAL_CHANNEL1_EN                     (TKEY_TXENR1_TX1EN)    
#define TKEY_MUTUAL_CHANNEL2_EN                     (TKEY_TXENR1_TX2EN)    
#define TKEY_MUTUAL_CHANNEL3_EN                     (TKEY_TXENR1_TX3EN)    
#define TKEY_MUTUAL_CHANNEL4_EN                     (TKEY_TXENR1_TX4EN)    
#define TKEY_MUTUAL_CHANNEL5_EN                     (TKEY_TXENR1_TX5EN)    
#define TKEY_MUTUAL_CHANNEL6_EN                     (TKEY_TXENR1_TX6EN)    
#define TKEY_MUTUAL_CHANNEL7_EN                     (TKEY_TXENR1_TX7EN)    
#define TKEY_MUTUAL_CHANNEL8_EN                     (TKEY_TXENR1_TX8EN)    
#define TKEY_MUTUAL_CHANNEL9_EN                     (TKEY_TXENR1_TX9EN)    
#define TKEY_MUTUAL_CHANNEL10_EN                    (TKEY_TXENR1_TX10EN)    
#define TKEY_MUTUAL_CHANNEL11_EN                    (TKEY_TXENR1_TX11EN)    
#define TKEY_MUTUAL_CHANNEL12_EN                    (TKEY_TXENR1_TX12EN)    
#define TKEY_MUTUAL_CHANNEL13_EN                    (TKEY_TXENR1_TX13EN)    
#define TKEY_MUTUAL_CHANNEL14_EN                    (TKEY_TXENR1_TX14EN)    
#define TKEY_MUTUAL_CHANNEL15_EN                    (TKEY_TXENR1_TX15EN)
#define TKEY_MUTUAL_CHANNEL16_EN                    (TKEY_TXENR2_TX16EN)
#define TKEY_MUTUAL_CHANNEL17_EN                    (TKEY_TXENR2_TX17EN)
#define TKEY_MUTUAL_CHANNEL18_EN                    (TKEY_TXENR2_TX18EN)
#define TKEY_MUTUAL_CHANNEL19_EN                    (TKEY_TXENR2_TX19EN)
#define TKEY_MUTUAL_CHANNEL20_EN                    (TKEY_TXENR2_TX20EN)
#define TKEY_MUTUAL_CHANNEL21_EN                    (TKEY_TXENR2_TX21EN)
#define TKEY_MUTUAL_CHANNEL22_EN                    (TKEY_TXENR2_TX22EN)
#define TKEY_MUTUAL_CHANNEL23_EN                    (TKEY_TXENR2_TX23EN)
#define TKEY_MUTUAL_CHANNEL24_EN                    (TKEY_TXENR2_TX24EN)
#define TKEY_MUTUAL_CHANNEL25_EN                    (TKEY_TXENR2_TX25EN)
#define TKEY_MUTUAL_CHANNEL_ALL_EN                  (0x03ffffffU)
#define TKEY_MUTUAL_CHANNEL_ALL_DISABLE             (0U)         
#define IS_TKEY_MUTUAL_CHANNEL_STATE(__STATE__)     ((__STATE__) <= 0x03ffffffU)    

/** @defgroup TKEY_DisChargeMode
  * @{
  */ 
#define TKEY_DISMS_DISCHAG_RES                      (0U)
#define TKEY_DISMS_DISCHAG_CURRENT                  (TKEY_CR_DISMS_0)
#define TKEY_DISMS_DISCHAG_RES_PAY_CURRENT          (TKEY_CR_DISMS_1)
#define TKEY_DISMS_DISCHAG_CURRENT_PAY_RES          (TKEY_CR_DISMS_0 | TKEY_CR_DISMS_1)
#define TKEY_DISMS_DISCHAG_RES_CURRENT              (TKEY_CR_DISMS_2)
#define IS_TKEY_DISMS(__MODE__)                     (((__MODE__) == TKEY_DISMS_DISCHAG_RES)             || \
                                                    ((__MODE__) == TKEY_DISMS_DISCHAG_CURRENT)          || \
                                                    ((__MODE__) == TKEY_DISMS_DISCHAG_RES_PAY_CURRENT)  || \
                                                    ((__MODE__) == TKEY_DISMS_DISCHAG_CURRENT_PAY_RES)  || \
                                                    ((__MODE__) == TKEY_DISMS_DISCHAG_RES_CURRENT))
/**
  * @}
  */  
  

/** @defgroup TKEY_Precharge_State
  * @{
  */
#define TKEY_PRECHARGE_DISABLE                      (0U)
#define TKEY_PRECHARGE_ENABLE                       (TKEY_CR_PCEN)
#define IS_TKEY_PRECHARGE(__STATE__)                (((__STATE__) == TKEY_PRECHARGE_ENABLE)  || \
                                                    ((__STATE__) == TKEY_PRECHARGE_DISABLE))
/**
  * @}
  */  

/** @defgroup TKEY_DischargeCurrent
  * @{
  */
#define IS_TKEY_DISCHARGECURRENT(__VALUE__)         ((__VALUE__) <= 0xffU)           
/**
  * @}
  */

/** @defgroup TKEY_DischargeRes
  * @{
  */
#define IS_TKEY_DISCHARGERES(__VALUE__)             ((__VALUE__) <= 0xffU)           
/**
  * @}
  */

/** @defgroup TKEY_SampleBitClkDiv
  * @{
  */
#define IS_TKEY_SAMPLEBITCLKDIV(__VALUE__)          ((__VALUE__) <= 0xffU)           
/**
  * @}
  */  

/** @defgroup TKEY_ResCompensate
  * @{
  */
#define TKEY_RESCOMPENSTATE_0_1K                    (0U)
#define TKEY_RESCOMPENSTATE_2K                      (1U)
#define TKEY_RESCOMPENSTATE_4K                      (2U)
#define TKEY_RESCOMPENSTATE_8K                      (3U)
#define IS_TKEY_RESCOMPENSTATE(__VALUE__)           (((__VALUE__) == TKEY_RESCOMPENSTATE_0_1K)  || \
                                                    ((__VALUE__) == TKEY_RESCOMPENSTATE_2K)     || \
                                                    ((__VALUE__) == TKEY_RESCOMPENSTATE_4K)     || \
                                                    ((__VALUE__) == TKEY_RESCOMPENSTATE_8K))           
/**
  * @}
  */

/** @defgroup TKEY_CapCompensate
  * @{
  */
#define TKEY_CAPCOMPENSTATE_0PF                     (0U)
#define TKEY_CAPCOMPENSTATE_2PF                     (1U)
#define TKEY_CAPCOMPENSTATE_4PF                     (2U)
#define TKEY_CAPCOMPENSTATE_6PF                     (3U)
#define TKEY_CAPCOMPENSTATE_8PF                     (4U)
#define TKEY_CAPCOMPENSTATE_10PF                    (5U)
#define TKEY_CAPCOMPENSTATE_12PF                    (6U)
#define TKEY_CAPCOMPENSTATE_14PF                    (7U)
#define IS_TKEY_CAPCOMPENSTATE(__VALUE__)           (((__VALUE__) == TKEY_CAPCOMPENSTATE_0PF)   || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_2PF)    || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_4PF)    || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_6PF)    || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_8PF)    || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_10PF)   || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_12PF)   || \
                                                    ((__VALUE__) == TKEY_CAPCOMPENSTATE_14PF))           
/**
  * @}
  */

/** @defgroup TKEY_Auto_Baseline
  * @{
  */
#define IS_TKEY_AUTO_BASELINE(__VALUE__)            ((__VALUE__) <= 0xffffU)           
/**
  * @}
  */
  
/** @defgroup TKEY_Auto_Threshold
  * @{
  */
#define IS_TKEY_AUTO_THRESHOLD(__VALUE__)            ((__VALUE__) <= 0xffU)           
/**
  * @}
  */

/** @defgroup TKEY_Auto_KeyNum
  * @{
  */
#define TKEY_AUTO_KEYNUM_1                          (0U)
#define TKEY_AUTO_KEYNUM_2                          (TKEY_CFGR3_KEYNUM_0)
#define TKEY_AUTO_KEYNUM_3                          (TKEY_CFGR3_KEYNUM_1)
#define TKEY_AUTO_KEYNUM_4                          (TKEY_CFGR3_KEYNUM_0 | TKEY_CFGR3_KEYNUM_1)
#define TKEY_AUTO_KEYNUM_5                          (TKEY_CFGR3_KEYNUM_2)
#define TKEY_AUTO_KEYNUM_6                          (TKEY_CFGR3_KEYNUM_0 | TKEY_CFGR3_KEYNUM_2)
#define TKEY_AUTO_KEYNUM_7                          (TKEY_CFGR3_KEYNUM_1 | TKEY_CFGR3_KEYNUM_2)
#define TKEY_AUTO_KEYNUM_8                          (TKEY_CFGR3_KEYNUM_0 | TKEY_CFGR3_KEYNUM_1 | TKEY_CFGR3_KEYNUM_2)
#define IS_TKEY_AUTO_KEYNUM(__NUM__)                (((__NUM__) == TKEY_AUTO_KEYNUM_1)  || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_2)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_3)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_4)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_5)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_6)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_7)   || \
                                                    ((__NUM__) == TKEY_AUTO_KEYNUM_8))
/**
  * @}
  */
  
/** @defgroup TKEY_Auto_CountMode
  * @{
  */
#define TKEY_AUTO_COUNT_SINGLE                      (0U)
#define TKEY_AUTO_COUNT_ROUND                       (TKEY_CFGR3_ROUND)
#define IS_TKEY_AUTO_COUNT(__MODE__)                (((__MODE__) == TKEY_AUTO_COUNT_SINGLE)  || \
                                                    ((__MODE__) == TKEY_AUTO_COUNT_ROUND))
/**
  * @}
  */
  
  
/**
  * @brief  TKEY Init Structure definition
  */
typedef struct
{
    uint32_t                    Mode;                           /*!< TKEY mode selection. 
                                                                    This parameter can be a value of @ref TKEY_Mode */        
    uint32_t                    WorkMode;                       /*!< Working mode. 
                                                                    This parameter can be a value of @ref TKEY_Work_Mode */  
    uint32_t                    ScanMode;                       /*!< Scan mode. 
                                                                    This parameter can be a value of @ref TKEY_Scan_Mode */ 
    uint32_t                    CCPSel;                         /*!< Specify capacitance compensate value
                                                                    This parameter can be a value of @ref TKEY_CapCompensate */     
    uint32_t                    RCPSel;                         /*!< Specify resistance compensate value
                                                                    This parameter can be a value of @ref TKEY_ResCompensate */     
    uint32_t                    ShieldEn;                       /*!< Shield channel enable control. 
                                                                    This parameter can be a value of @ref TKEY_Shield_State */ 
    uint32_t                    FilterNum;                      /*!< Hardware filtering frequency selection. 
                                                                    This parameter can be a value of @ref TKEY_Filter_Num */         
    uint32_t                    SampleNum;                      /*!< Sample number selection. 
                                                                    This parameter can be a value of @ref TKEY_Sample_Num */         
    uint32_t                    Vldo;                           /*!< Charging voltage VLDO selection. 
                                                                    This parameter can be a value of @ref TKEY_VLDO */     
    uint32_t                    Vref;                           /*!< Comparator voltage VREF selection. 
                                                                    This parameter can be a value of @ref TKEY_VREF */     
    uint32_t                    SpreadEn;                       /*!< Scan clock spread spectrum enable. 
                                                                    This parameter can be a value of @ref TKEY_Spread_State */ 
    uint32_t                    ChannelEn;                      /*!< Specify the enable TKEY channels. 
                                                                    This parameter can be a value of @ref TKEY_Channel_State */                                                                      
}TKEY_InitTypeDef;
/**
  * @}
  */

/**
  * @brief  TKEY Timing parameters structure definition
  */
typedef struct
{
    uint32_t                    RandomJitterNum;                /*!< Scan clock random jitter value. 
                                                                    This parameter can be a value of @ref TKEY_Random_Jitter */   
    uint32_t                    ScanInterval;                   /*!< The interval time between two sets of scans. 
                                                                    This parameter can be a value of @ref TKEY_Scan_Interval */       
    uint32_t                    ScanClkPrescale;                /*!< Scanning clock division. 
                                                                    This parameter can be a value of @ref TKEY_Scan_Clk_Prescale */   
    uint32_t                    ScanClkSW1HSW2L;                /*!< Scan clock SW1 high level (SW2 low level) time.
                                                                    This parameter can be a value of @ref TKEY_Scan_Clk_SW1HSW2L */       
    uint32_t                    ScanClkSW1LSW2H;                /*!< Scan clock SW1 low level (SW2 high level) time. 
                                                                    This parameter can be a value of @ref TKEY_Scan_Clk_SW1LSW2H */   
    uint32_t                    SampleClkPrescale;              /*!< Sampling clock division. 
                                                                    This parameter can be a value of @ref TKEY_Sample_Clk_Prescale */
    uint32_t                    TimeoutOrPwmCycle;              /*!< Set scan timeout or PWM cycle length. 
                                                                    This parameter can be a value of @ref TKEY_ScanTimeout_PwmCycle */       
}TKEY_TimingInitTypeDef;
/**
  * @}
  */

  
#define TKEY_CALIBRATE_BUFF_MAX     20    //采样结果保存次数
  
typedef enum
{
    // Calibration states
    TKEY_STATEID_CALIB              = 0,  /**<  0 - Object is in Calibration */
    // Release states
    TKEY_STATEID_RELEASE            = 1,  /**<  1 - Object is released */
    // Detect states
    TKEY_STATEID_DETECT             = 2, /**<   2 - Object is in Detect */
    //Startuo states
    TKEY_STATEID_STARTUP            = 3, /**<   3 - Object is in Startup */
    // Detecting states
    TKEY_STATEID_DETECTING          = 4, /**<   4 - Object is in Detecting */
  
} TKEY_StateId_enum_T;

typedef enum
{
    TKEY_CRSELECT_DISABLE           = 0,  /**<  0 - Object is in Calibration */
    TKEY_CRSELECT_ENABLE            = 1,  /**<  1 - Object is in Debounce Calibration */
} TKEY_CRSELECT_enum_T;

typedef enum
{
    TKEY_CALIBRAT_IDLE           = 0,   /**<  0 - Not need calibrate */
    TKEY_CALIBRAT_SELF           = 1,   /**<  1 - need self calibrate */
    TKEY_CALIBRAT_QUICK          = 2,   /**<  2 - need quick calibrate */
} TKEY_CALIBRAT_enum_T;

/**
  * @brief  TKEY handle Structure definition
  */
typedef struct
{
    uint16_t                DetectInTH;          /*!< The threshold when the state from release to detect*/ 
    uint16_t                WakeUpTh;            /*!< The threshold when the state from sleep to wakeup*/
}TKEY_BaseParaDef;
/**
  * @}
  */

/**
  * @brief  TKEY handle Structure definition
  */
typedef struct
{
    uint16_t                CalibratTH;          /*!< The threshold when need to calibration*/
    uint16_t                DetectOutTH;         /*!< The threshold when the state from detect to release*/
    uint16_t                BaseLine;            /*!< The reference data  */
    uint16_t                RawData;             /*!< The current data  */
    int16_t                 Delta;               /*!< The delta value  */
    uint8_t                 DebIn;               /*!< The variate value for debounce*/
    uint8_t                 DebOut;              /*!< The variate value for debounce*/
    TKEY_StateId_enum_T     StateId;             /*!< The state machine ID*/
	uint16_t                Noise;
}TKEY_DataDef;
/**
  * @}
  */

/**
  * @brief  TKEY handle Structure definition
  */

typedef struct
{
    uint8_t     pos;
    uint16_t    Caldata[TKEY_CALIBRATE_BUFF_MAX+5];
    uint8_t     ScanTimer;  
    uint32_t    DetectingTimer;       
    uint8_t     CalFlag; 
}TKEY_CalibartData;
/**
  * @}
  */

/**
  * @brief  TKEY handle Structure definition
  */
typedef struct
{
    TKEY_BaseParaDef    *Tkey_RefPara;
    TKEY_DataDef        *Tkey_Data;        
    TKEY_CalibartData   *Tkey_CalData;
    uint16_t             ChannelId;
    uint16_t             Max_Noise;
	uint32_t             Delta_Sum;
	uint8_t              Delta_Sum_Cnt;
	uint16_t             Predict_Touch_Delta;
	
}TKEY_ChannelDataDef;
  
/**
  * @brief  TKEY handle Structure definition
  */
typedef struct
{
    TKEY_InitTypeDef            Init;                           /*!< TKEY init parameters */
    TKEY_TimingInitTypeDef      Timing;                         /*!< TKEY Timing parameters */        
    
    TKEY_ChannelDataDef         *ChannelData;                   /*!< The channel data point*/
    uint16_t                    PressRelaeseFlag;               /*!< TKEY按压与释放标志，bit0-15对应TK0-15，bit为1表示按键按下，为0表示释放*/
    uint16_t                    LongPressFlag;                  /*!< TKEY长按标志，bit0-15对应TK0-15，bit为1表示按键长按，为0表示非长按*/
    uint16_t                    ChannelValue;                   /*!< TKEY Value*/
    uint16_t                    ChannelDetectedNum;             /*!< TKEY detected channel number*/
    uint8_t                     TotalChannelNum;                /*!< TKEY toatal channel number*/
    uint16_t                    DetectingFlag;                  /*!< TKEY按压中标志，bit0-15对应TK0-15，bit为1表示按键按压中，为0表示释放*/  
    uint8_t                     ScanFinishIrq;                  /*!< TKEY Channel scan finish interrupt enable*/
    uint8_t                     ExistDetecting;
    void (*TKEY_ScanFinish_Callback)(void);                     /*!< TKEY Channel scan finish interrupt callback*/          
} TKEY_HandleTypeDef;
/**
  * @}
  */


/**
  * @brief  TKEY CSA parameters structure definition
  */
typedef struct
{
    uint32_t                    ChargeNumDoneEn;                /*!< The charging frequency reaches the set number of times, 
                                                                    and the function is enabled or disabled. 
                                                                    This parameter can be a value of @ref TKEY_ChargeNumDone_State */   
    uint32_t                    ChargeEachDoneEN;               /*!< Enable or disable each charging completion function. 
                                                                    This parameter can be a value of @ref TKEY_ChargeEachDone_State */       
    uint32_t                    ComparerFilterNum;              /*!< Comparator filtering value.
                                                                    This parameter can be a value of @ref TKEY_CompFilter_Num */       
    uint32_t                    ComparerFilterEN;               /*!< Comparator filtering enable or disable. 
                                                                    This parameter can be a value of @ref TKEY_CompFilter_State */   
    uint32_t                    ChargeNum;                      /*!< charging numbers. 
                                                                    This parameter can be a value of @ref TKEY_ChargeNumber */ 
    uint32_t                    ChargeDoneWaitTime;             /*!< Waiting time after reaching the set number of charges. 
                                                                    This parameter can be a value of @ref TKEY_ChargeDoneWaitTime */ 
    uint32_t                    DischargeTime;                   /*!< Discharge time for Cs. 
                                                                    This parameter can be a value of @ref TKEY_DischargeTime */ 
                                                                    
}TKEY_CSAConfigTypeDef;
/**
  * @}
  */

/**
  * @brief  TKEY CSD parameters structure definition
  */
typedef struct
{    
    uint32_t                    DischargeMode;                  /*!< Discharge mode selection. 
                                                                    This parameter can be a value of @ref TKEY_DisChargeMode */   
    uint32_t                    PrechargeEn;                    /*!< Precharge enable or disable.
                                                                    This parameter can be a value of @ref TKEY_Precharge_State */       
    uint32_t                    DischargeCurrent;               /*!< Selection of discharge current for constant current source. 
                                                                    This parameter can be a value of @ref TKEY_DischargeCurrent */   
    uint32_t                    DischargeRes;                   /*!< Resistance discharge resistance selection. 
                                                                    This parameter can be a value of @ref TKEY_DischargeRes */ 
    uint32_t                    SampleBitClkDiv;                /*!<Clock division for sampling bit streams. 
                                                                    This parameter can be a value of @ref TKEY_SampleBitClkDiv */                                                                   
}TKEY_CSDConfigTypeDef;
/**
  * @}
  */

/**
  * @brief  TKEY Mutual parameters structure definition
  */
typedef struct
{      
    uint32_t                    MutualDelayTime;                /*!< Specify the delay time for mutual inductance transmission signals.
                                                                    This parameter can be a value of @ref TKEY_MutualDelay */       
    uint32_t                    MutualDelayEn;                  /*!< Mutual inductance transmission signal delay enable or disable. 
                                                                    This parameter can be a value of @ref TKEY_MutualDelay_State */   
    uint32_t                    MutualResetEn;                  /*!< Mutual inductance transmission channel reset enable or disable. 
                                                                    This parameter can be a value of @ref TKEY_MutualReset_State */ 
    uint32_t                    MutualChannelEn;                /*!< Specify the enable TKEY mutual inductance transmission channel. 
                                                                    This parameter can be a value of @ref TKEY_Mutual_Channel_State */                                                                   
}TKEY_MutualConfigTypeDef;
/**
  * @}
  */

/**
  * @brief  TKEY Mutual parameters structure definition
  */
typedef struct
{  
    uint32_t                    Baseline[16];                   /*!< Specify channel baseline value in automatic mode.
                                                                    This parameter can be a value of @ref TKEY_Auto_Baseline */       
    uint32_t                    Threshold[16];                  /*!< Specify channel threshold value in automatic mode. 
                                                                    This parameter can be a value of @ref TKEY_Auto_Threshold */   
    uint32_t                    FilterNum;                      /*!< Specify the number of hardware filters in automatic mode. 
                                                                    This parameter can be a value of @ref TKEY_Filter_Num */ 
    uint32_t                    KeyNum;                         /*!< Specify the number of buttons in automatic mode. 
                                                                    This parameter can be a value of @ref TKEY_Auto_KeyNum */                                                                   
    uint32_t                    CountMode;                      /*!< Specify the filtering counting method in automatic mode. 
                                                                    This parameter can be a value of @ref TKEY_Auto_CountMode */                     
}TKEY_AutoConfigTypeDef;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/* HAL_TKEY_Init */
HAL_StatusTypeDef HAL_TKEY_Init(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_DeInit */
HAL_StatusTypeDef HAL_TKEY_DeInit(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_StartScan */
void HAL_TKEY_StartScan(void);

/* HAL_TKEY_StopScan */
void HAL_TKEY_StopScan(void);

/* HAL_TKEY_ReadChannelCount */
HAL_StatusTypeDef HAL_TKEY_ReadChannelCount(uint16_t *pScanCount);

/* HAL_TKEY_ReadChannelData */
HAL_StatusTypeDef HAL_TKEY_ReadChannelData(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_ReadLastChannelCount */
uint16_t HAL_TKEY_ReadLastChannelCount(void);

/* HAL_TKEY_GetSacnTimeoutAutoChannelNumber */
uint8_t HAL_TKEY_GetSacnTimeoutAutoChannelNumber(void);

/* HAL_TKEY_AutoConfig */
HAL_StatusTypeDef HAL_TKEY_AutoConfig(TKEY_AutoConfigTypeDef *hauto);

/* HAL_TKEY_CSAConfig */
HAL_StatusTypeDef HAL_TKEY_CSAConfig(TKEY_CSAConfigTypeDef* hcsa);

/* HAL_TKEY_CSDConfig */
HAL_StatusTypeDef HAL_TKEY_CSDConfig(TKEY_CSDConfigTypeDef* hcsd);

/* HAL_TKEY_MutualConfig */
HAL_StatusTypeDef HAL_TKEY_MutualConfig(TKEY_MutualConfigTypeDef* hmutual);

/* HAL_TKEY_MutualEnableDisable */
void HAL_TKEY_MutualEnableDisable(uint32_t NewState);

/* HAL_TKEY_AutoEnableDisable */
void HAL_TKEY_AutoEnableDisable(uint32_t NewState);

/* HAL_TKEY_ResCompensate */
void HAL_TKEY_ResCompensate(uint8_t channel, uint8_t res);

/* HAL_TKEY_CapCompensate */
void HAL_TKEY_CapCompensate(uint8_t channel, uint8_t cap);

/* HAL_TKEY_ReadMultipleSamplingData */
HAL_StatusTypeDef HAL_TKEY_ReadMultipleSamplingData(uint16_t *pData);

/* HAL_TKEY_ScanIntervalConfig */
void HAL_TKEY_ScanIntervalConfig(uint16_t time);

/* HAL_TKEY_ReadScanInterval */
uint16_t HAL_TKEY_ReadScanInterval(void);

/* HAL_TKEY_ITConfig */
void HAL_TKEY_ITConfig(uint32_t irq, FunctionalState NewState);  

/* HAL_TKEY_GetFlagStatus */
FlagStatus HAL_TKEY_GetFlagStatus(uint32_t flag);

/* HAL_TKEY_ClearFlag */
void HAL_TKEY_ClearFlag(uint32_t flag);

/* HAL_TKEY_GetITStatus */
ITStatus HAL_TKEY_GetITStatus(uint32_t irq);

/* HAL_TKEY_ClearITPendingBit */
void HAL_TKEY_ClearITPendingBit(uint32_t irq);

/* HAL_TKEY_GetIT */
uint16_t HAL_TKEY_GetIT(void);

/* HAL_TKEY_EnableDisable */
void HAL_TKEY_EnableDisable(uint32_t NewState);

/* HAL_TKEY_ReadChannelData_SingleAll */
void HAL_TKEY_ReadChannelData_SingleAll(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_ReadChannelData_SingleAll_GND */
void HAL_TKEY_ReadChannelData_SingleAll_GND(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_ReadChannelData_SingleOne */
void HAL_TKEY_ReadChannelData_SingleOne(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_ReadChannelData_SingleOne_Compensate */
void HAL_TKEY_ReadChannelData_SingleOne_Compensate(TKEY_HandleTypeDef* htkey);

/* HAL_TKEY_ReadChannelData_SingleAll_Compensate */
void HAL_TKEY_ReadChannelData_SingleAll_Compensate(TKEY_HandleTypeDef* htkey);

#endif





