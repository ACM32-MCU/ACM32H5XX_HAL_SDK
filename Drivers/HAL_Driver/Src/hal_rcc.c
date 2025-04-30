/******************************************************************************
*@file  : hal_rcc.c
*@brief : RCC HAL module driver.
******************************************************************************/
#include "hal.h"

#ifdef HAL_RCC_MODULE_ENABLED

/******************************************************************************
*@brief : RCC clock ready interrupt request.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_ReadyIRQHandler(void)
{
    // RCH clock ready
    if ((RCC->CIR & (RCC_CIR_RCHRDYIE | RCC_CIR_RCHRDYIF)) == (RCC_CIR_RCHRDYIE | RCC_CIR_RCHRDYIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_RCHRDYIC;
        HAL_RCC_RCHReadyCallback();
    }
    
    // XTH clock ready
    if ((RCC->CIR & (RCC_CIR_XTHRDYIE | RCC_CIR_XTHRDYIF)) == (RCC_CIR_XTHRDYIE | RCC_CIR_XTHRDYIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_XTHRDYIC;
        HAL_RCC_XTHReadyCallback();
    }

    // PLL1 clock ready
    if ((RCC->CIR & (RCC_CIR_PLL1LOCKIE | RCC_CIR_PLL1LOCKIF)) == (RCC_CIR_PLL1LOCKIE | RCC_CIR_PLL1LOCKIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_PLL1LOCKIC;
        HAL_RCC_PLL1ReadyCallback();
    }
    
    // PLL2 clock ready
    if ((RCC->CIR & (RCC_CIR_PLL2LOCKIE | RCC_CIR_PLL2LOCKIF)) == (RCC_CIR_PLL2LOCKIE | RCC_CIR_PLL2LOCKIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_PLL2LOCKIC;
        HAL_RCC_PLL2ReadyCallback();
    }
    
    // PLL3 clock ready
    if ((RCC->CIR & (RCC_CIR_PLL3LOCKIE | RCC_CIR_PLL3LOCKIF)) == (RCC_CIR_PLL3LOCKIE | RCC_CIR_PLL3LOCKIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_PLL3LOCKIC;
        HAL_RCC_PLL3ReadyCallback();
    }
    
    // RCL clock ready
    if ((RCC->CIR & (RCC_CIR_RCLRDYIE | RCC_CIR_RCLRDYIF)) == (RCC_CIR_RCLRDYIE | RCC_CIR_RCLRDYIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_RCLRDYIC;
        HAL_RCC_RCLReadyCallback();
    }
    
    // XTL clock ready
    if ((RCC->CIR & (RCC_CIR_XTLRDYIE | RCC_CIR_XTLRDYIF)) == (RCC_CIR_XTLRDYIE | RCC_CIR_XTLRDYIF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_XTLRDYIC;
        HAL_RCC_XTLReadyCallback();
    }
}

/******************************************************************************
*@brief : RCC XTH clock stop interrupt request.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTHStopIRQHandler(void)
{
    // XTH stop
    if ((RCC->CIR & (RCC_CIR_XTHSDIE | RCC_CIR_XTHSDF)) == (RCC_CIR_XTHSDIE | RCC_CIR_XTHSDF))
    {
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_XTHSDIC;
        HAL_RCC_XTHStopCallback();
    }
}

/******************************************************************************
*@brief : RCC XTL clock stop interrupt request.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTLStopIRQHandler(void)
{
    // XTL stop
    if ((RCC->CIR & (RCC_CIR_XTLSDIE | RCC_CIR_XTLSDF)) == (RCC_CIR_XTLSDIE | RCC_CIR_XTLSDF))
    {
        RCC->STDBYCTRL &= ~RCC_STDBYCTRL_XTLSDEN;
        RCC->STDBYCTRL &= ~RCC_STDBYCTRL_XTLEN;
        RCC->CIR = (RCC->CIR & ~RCC_IT_CLEAR_FLAG_MASK) | RCC_CIR_XTLSDIC;
        HAL_RCC_XTLStopCallback();
    }
}

/******************************************************************************
*@brief : RCH clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_RCHReadyCallback(void)
{
}

/******************************************************************************
*@brief : RCL clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_RCLReadyCallback(void)
{
}

/******************************************************************************
*@brief : XTH clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTHReadyCallback(void)
{
}

/******************************************************************************
*@brief : XTL clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTLReadyCallback(void)
{
}

/******************************************************************************
*@brief : PLL1 clock ready interrupt callback.
*@param : None
*@return: None
******************************************************************************/
__attribute__((weak)) void HAL_RCC_PLL1ReadyCallback(void)
{
}

/******************************************************************************
*@brief : PLL2 clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_PLL2ReadyCallback(void)
{
}

/******************************************************************************
*@brief : PLL3 clock ready interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_PLL3ReadyCallback(void)
{
}

/******************************************************************************
*@brief : XTH clock stop interrupt callback.
*@param : None
*@return: None
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTHStopCallback(void)
{
}

/******************************************************************************
*@brief : XTL clock stop interrupt callback.
*@param : None.
*@return: None.
******************************************************************************/
__attribute__((weak)) void HAL_RCC_XTLStopCallback(void)
{
}

/******************************************************************************
*@brief : Initializes the RCC Oscillators according to the specified parameters
*         in the RCC_OscInitTypeDef.
*@param : RCC_OscInit: pointer to an RCC_OscInitTypeDef structure that contains 
*         the configuration information for the RCC Oscillators.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInit)
{
    /* Check the parameters */
    assert_param(IS_RCC_OSC_TYPE(RCC_OscInit->OscType));
    
    /*------------------------------- RCH Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_RCH)
    {
        if (HAL_RCC_RCHConfig(RCC_OscInit->RCH, RCC_OscInit->RCHDiv16) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- RCL Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_RCL)
    {
        if (HAL_RCC_RCLConfig(RCC_OscInit->RCL) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- XTH Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_XTH)
    {
        if (HAL_RCC_XTHConfig(RCC_OscInit->XTH, RCC_OscInit->XTHBypass) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- XTL Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_XTL)
    {
        if (HAL_RCC_XTLConfig(RCC_OscInit->XTL, RCC_OscInit->XTLBypass) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- PLL1 Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_PLL1)
    {
        if (RCC_OscInit->PLL1.SSC == DISABLE)
        {
            if (HAL_RCC_PLL1Config(RCC_OscInit->PLL1.PLL, RCC_OscInit->PLL1.Source, RCC_OscInit->PLL1.PLLN, \
                                   RCC_OscInit->PLL1.PLLF, RCC_OscInit->PLL1.PLLP, RCC_OscInit->PLL1.PLLQ) != HAL_OK)
                return HAL_ERROR;
        }
        else
        {
            if (HAL_RCC_PLL1SSCConfig(RCC_OscInit->PLL1.PLL, RCC_OscInit->PLL1.Source, RCC_OscInit->PLL1.PLLN, \
                                      RCC_OscInit->PLL1.PLLF, RCC_OscInit->PLL1.PLLP, RCC_OscInit->PLL1.PLLQ, \
                                      RCC_OscInit->PLL1.SSC, RCC_OscInit->PLL1.Mode, RCC_OscInit->PLL1.Period, \
                                      RCC_OscInit->PLL1.Step) != HAL_OK)
                return HAL_ERROR;
        }
        
        if (HAL_RCC_PLL1PCLKConfig(RCC_OscInit->PLL1.PLLPCLK) != HAL_OK)
            return HAL_ERROR;
        
        if (HAL_RCC_PLL1QCLKConfig(RCC_OscInit->PLL1.PLLQCLK) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- PLL2 Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_PLL2)
    {
        if (RCC_OscInit->PLL1.SSC == DISABLE)
        {
            if (HAL_RCC_PLL2Config(RCC_OscInit->PLL2.PLL, RCC_OscInit->PLL2.Source, RCC_OscInit->PLL2.PLLN, \
                                   RCC_OscInit->PLL2.PLLF, RCC_OscInit->PLL2.PLLP, RCC_OscInit->PLL2.PLLQ) != HAL_OK)
                return HAL_ERROR;
        }
        else
        {
            if (HAL_RCC_PLL2SSCConfig(RCC_OscInit->PLL2.PLL, RCC_OscInit->PLL2.Source, RCC_OscInit->PLL2.PLLN, \
                                      RCC_OscInit->PLL2.PLLF, RCC_OscInit->PLL2.PLLP, RCC_OscInit->PLL2.PLLQ, \
                                      RCC_OscInit->PLL1.SSC, RCC_OscInit->PLL1.Mode, RCC_OscInit->PLL1.Period, \
                                      RCC_OscInit->PLL1.Step) != HAL_OK)
                return HAL_ERROR;
        }
        
        if (HAL_RCC_PLL2PCLKConfig(RCC_OscInit->PLL2.PLLPCLK) != HAL_OK)
            return HAL_ERROR;
        
        if (HAL_RCC_PLL2QCLKConfig(RCC_OscInit->PLL2.PLLQCLK) != HAL_OK)
            return HAL_ERROR;
    }
    
    /*------------------------------- PLL3 Configuration ------------------------*/
    if (RCC_OscInit->OscType & RCC_OSC_TYPE_PLL3)
    {
        if (HAL_RCC_PLL3Config(RCC_OscInit->PLL3.PLL, RCC_OscInit->PLL3.Source, RCC_OscInit->PLL3.PLLN, \
                               RCC_OscInit->PLL3.PLLF, RCC_OscInit->PLL3.PLLP, RCC_OscInit->PLL3.PLLQ) != HAL_OK)
            return HAL_ERROR;
        
        if (HAL_RCC_PLL3PCLKConfig(RCC_OscInit->PLL3.PLLPCLK) != HAL_OK)
            return HAL_ERROR;
        
        if (HAL_RCC_PLL3QCLKConfig(RCC_OscInit->PLL3.PLLQCLK) != HAL_OK)
            return HAL_ERROR;
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the internal high speed oscillator (RCH).
*@param : RCH: new state of the RCH. 
*             This parameter can be: ENABLE or DISABLE.
*@param : Div16: The new state of the 16th frequency division of RCH. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_RCHConfig(uint32_t RCH, uint32_t Div16)
{
    uint32_t temp;
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(RCH));
    
    if (RCH == DISABLE)
    {
        if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL_1) == 0)
            return (HAL_ERROR);
        
        if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL)) == RCC_PLL1CR_PLL1EN)
            return (HAL_ERROR);
        
        if ((RCC->PLL2CR & (RCC_PLL2CR_PLL2SLEEP | RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2SRCSEL)) == RCC_PLL2CR_PLL2EN)
            return (HAL_ERROR);
        
        if ((RCC->PLL3CR & (RCC_PLL3CR_PLL3SLEEP | RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3SRCSEL)) == RCC_PLL3CR_PLL3EN)
            return (HAL_ERROR);
        
        /* disable RCH */
        RCC->RCHCR &= ~(RCC_RCHCR_RCHEN | RCC_RCHCR_RCHDIV);
        HAL_SimpleDelay(2);
        
        timeout = RCC_RCHP_UNREADY_TIMEOUT;
        while (RCC->RCHCR & RCC_RCHCR_RCHRDY)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_FUNCTIONAL_STATE(Div16));
        
        if (Div16 == DISABLE)
        {
            if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL)) == RCC_PLL1CR_PLL1EN)
                return (HAL_ERROR);
            
            if ((RCC->PLL2CR & (RCC_PLL2CR_PLL2SLEEP | RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2SRCSEL)) == RCC_PLL2CR_PLL2EN)
                return (HAL_ERROR);
            
            if ((RCC->PLL3CR & (RCC_PLL3CR_PLL3SLEEP | RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3SRCSEL)) == RCC_PLL3CR_PLL3EN)
                return (HAL_ERROR);
            
            RCC->RCHCR &= ~RCC_RCHCR_RCHDIV;
        }

        RCC->RCHCR |= RCC_RCHCR_RCHEN;
        
        HAL_SimpleDelay(5);
        
        timeout = RCC_RCH_READY_TIMEOUT;
        while ((RCC->RCHCR & RCC_RCHCR_RCHRDY) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        if (Div16)
            RCC->RCHCR |= RCC_RCHCR_RCHDIV;
        
        if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == RCC_SYSCLK_SOURCE_RCH)
        {
            temp = SystemCoreClock;
            SystemCoreClock = HAL_RCC_GetSysCoreClockFreq();
            if (temp != SystemCoreClock)
            {
                HAL_InitTick(g_systickHandle.intPrio,g_systickHandle.msPeriod);
            }
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the internal low speed oscillator (RCL).
*@param : RCL: new state of the RCL. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_RCLConfig(uint32_t RCL)
{
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(RCL));
    
    if (RCL == DISABLE)
    {
        if ((RCC->STDBYCTRL & (RCC_STDBYCTRL_RTCEN | RCC_STDBYCTRL_RTCSEL)) == RCC_STDBYCTRL_RTCEN)
            return (HAL_ERROR);
        
        /* disable RCL */
        RCC->STDBYCTRL = (RCC->STDBYCTRL & ~(RCC_STDBYCTRL_RCLDIS | RCC_STDBYCTRL_RCLEN)) | \
                          (0x0AUL << RCC_STDBYCTRL_RCLDIS_Pos);
        HAL_SimpleDelay(2);
        
        timeout = RCC_RCL_UNREADY_TIMEOUT;
        while (RCC->STDBYCTRL & RCC_STDBYCTRL_RCLRDY)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    else
    {
        /* enable RCL */
        RCC->STDBYCTRL |= RCC_STDBYCTRL_RCLEN;
        HAL_SimpleDelay(5);
        
        /* Wait till RCL is ready */
        timeout = RCC_RCL_READY_TIMEOUT;
        while (!(RCC->STDBYCTRL & RCC_STDBYCTRL_RCLRDY))
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the external high speed oscillator (XTH).
*@param : XTH: new state of the XTH. 
*             This parameter can be: ENABLE or DISABLE.
*@param : Bypass: new state of the XTH oscillator bypass. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_XTHConfig(uint32_t XTH, uint32_t Bypass)
{
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(XTH));
    
    if (DISABLE == XTH)
    {
        if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == RCC_CCR1_SYSCLKSEL_1)
            return (HAL_ERROR);
        
        if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL)) == (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL))
            return (HAL_ERROR);
        
        if ((RCC->PLL2CR & (RCC_PLL2CR_PLL2SLEEP | RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2SRCSEL)) == (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL))
            return (HAL_ERROR);
        
        if ((RCC->PLL3CR & (RCC_PLL3CR_PLL3SLEEP | RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3SRCSEL)) == (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1SRCSEL))
            return (HAL_ERROR);
        
        /* disable XTH */
        RCC->XTHCR &= ~RCC_XTHCR_XTHEN;
        HAL_SimpleDelay(2);
        
        timeout = RCC_XTH_UNREADY_TIMEOUT;
        while (RCC->XTHCR & RCC_XTHCR_XTHRDY)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_FUNCTIONAL_STATE(Bypass));
        
        if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == RCC_CCR1_SYSCLKSEL_1)
        {
            if (Bypass)
            {
                if ((RCC->XTHCR & RCC_XTHCR_XTHBYP) == 0)
                    return (HAL_ERROR);
            }
            else
            {
                if (RCC->XTHCR & RCC_XTHCR_XTHBYP)
                    return (HAL_ERROR);
            }
        }
        
        if (Bypass)
            RCC->XTHCR |= RCC_XTHCR_XTHRDYTIME | RCC_XTHCR_XTHBYP | RCC_XTHCR_XTHEN;
        else
            RCC->XTHCR = (RCC->XTHCR & ~RCC_XTHCR_XTHBYP) | RCC_XTHCR_XTHRDYTIME | RCC_XTHCR_XTHEN;

        HAL_SimpleDelay(5);
        timeout = RCC_XTH_READY_TIMEOUT;
        while (!(RCC->XTHCR & RCC_XTHCR_XTHRDY))
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the external low speed oscillator (XTL).
*@param : XTL: new state of the XTL. 
*             This parameter can be: ENABLE or DISABLE.
*@param : Bypass: new state of the XTL oscillator bypass. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_XTLConfig(uint32_t XTL, uint32_t Bypass)
{
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(XTL));
    
    if (DISABLE == XTL)
    {
        if ((RCC->STDBYCTRL & (RCC_STDBYCTRL_RTCEN | RCC_STDBYCTRL_RTCSEL)) == (RCC_STDBYCTRL_RTCEN | RCC_STDBYCTRL_RTCSEL))
            return (HAL_ERROR);
        
        /* disable XTL */
        RCC->STDBYCTRL &= ~RCC_STDBYCTRL_XTLEN;
        HAL_SimpleDelay(5);
        
        timeout = RCC_XTL_UNREADY_TIMEOUT;
        while (RCC->STDBYCTRL & RCC_STDBYCTRL_XTLRDY)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_FUNCTIONAL_STATE(Bypass));
        
        /* XTL oscillator bypass configuration */
        if (Bypass)
        {
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~RCC_STDBYCTRL_XTLDRV) | RCC_STDBYCTRL_XTLBYP | \
                              RCC_STDBYCTRL_XTLDRV_1 | RCC_STDBYCTRL_XTLDRV_0 | RCC_STDBYCTRL_XTLEN;
        }
        else
        {
            RCC->STDBYCTRL = (RCC->STDBYCTRL & ~(RCC_STDBYCTRL_XTLBYP | RCC_STDBYCTRL_XTLDRV)) | \
                              RCC_STDBYCTRL_XTLDRV_1 | RCC_STDBYCTRL_XTLDRV_0 | RCC_STDBYCTRL_XTLEN;
        }
        HAL_SimpleDelay(5);
        
        timeout = RCC_XTL_READY_TIMEOUT;
        while ((RCC->STDBYCTRL & RCC_STDBYCTRL_XTLRDY) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        RCC->STDBYCTRL |= RCC_STDBYCTRL_XTLDRV_2;
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL1 clock.
*@note    Automatically disable SSC function.
*@param : PLL1: specifies the new state of the PLL1. 
*             This parameter can be: ENABLE or DISABLE.
*@param : ClockSource: specifies the PLL1 entry clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SOURCE_RCH_DIV16: RCH oscillator clock divided by 16 selected as PLL1 clock entry.
*             @arg RCC_PLL_SOURCE_XTH: XTH oscillator clock selected as PLL1 clock entry.
*@param : PLLN: specifies the PLL1 down factor divider field.
*             This parameter must be a number between 1 and 63.
*@param : PLLF: specifies the PLL1 multiplication factor.
*             This parameter must be a number between 50 and 511.
*@param : PLLP: PLL1PCLK output frequency division control field.
*             This parameter must be one of 2, 4, 6 and 8.
*@param : PLLQ: PLL1QCLK output frequency division control field.
*             This parameter must be a number between 1 and 15.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL1Config(uint32_t PLL1, uint32_t ClockSource, \
                                     uint32_t PLLN, uint32_t PLLF, uint32_t PLLP, uint32_t PLLQ)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t lock;
    uint32_t timeout;
    #ifdef USE_FULL_ASSERT
    double freq;
    #endif
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(PLL1));
    
    /* check whether the current configured clock is the system clock */
    if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == (RCC_CCR1_SYSCLKSEL_0 | RCC_CCR1_SYSCLKSEL_1))
        return (HAL_ERROR);
    
    if (PLL1 == DISABLE)
    {
        /* disable PLL1 */
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1EN;
        /* enter sleep mode */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1SLEEP;
        HAL_SimpleDelay(15);
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_RCC_PLL_CLOCK_SOURCE(ClockSource));
        assert_param(IS_RCC_PLL1_PLLN(PLLN));
        assert_param(IS_RCC_PLL1_PLLF(PLLF));
        assert_param(IS_RCC_PLL1_PLLP(PLLP));
        assert_param(IS_RCC_PLL1_PLLQ(PLLQ));
        
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
        {
            if ((RCC->RCHCR & (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != \
                              (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
                return (HAL_ERROR);
            lock = RCC_PLL1CR_PLL1FREERUN;
            #ifdef USE_FULL_ASSERT
            freq = (double)4000000;
            #endif
        }
        else
        {
            if ((RCC->XTHCR & (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN)) != (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN))
                return (HAL_ERROR);
            lock = RCC_PLL1CR_PLL1LOCK;
            #ifdef USE_FULL_ASSERT
            freq = (double)XTH_VALUE;
            #endif
        }

        // FCLK_VCO=FCLKIN*F/N; FCLK_P=FCLK_VCO /P; FCLK_Q=FCLK_VCO /Q

        #ifdef USE_FULL_ASSERT
        assert_param((freq >= 1000000) && (freq <= 132000000));
        freq = freq / PLLN;
        assert_param((freq >= 1000000) && (freq <= 2000000));
        freq = freq * PLLF;
        assert_param((freq >= 100000000) && (freq <= 550000000));
        assert_param(((freq / PLLP) >= 30000000) && ((freq / PLLP) <= 220000000));
        assert_param(((freq / PLLQ) >= 16000000) && ((freq / PLLQ) <= 220000000));
        #endif
        
        RCC->PLL1CR |= RCC_PLL1CR_PLL1LOCKSEL;
        RCC->PLL1CR &= ~(RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1QCLKEN);
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
            RCC->PLL1CR &= ~RCC_PLL1CR_PLL1SRCSEL;
        else
            RCC->PLL1CR |= RCC_PLL1CR_PLL1SRCSEL;
        
        /* The delay is 370us~440us, except 512us at 1/2/3MHz. */
        temp1 = SystemCoreClock >>  20;
        if ((SystemCoreClock - (temp1 << 20)) >= (1UL << 19))
            temp1++;
        temp2 = temp1 >> 2;
        temp1 = temp1 - (temp2 << 2);
        temp2 = temp2 * 3;
        temp1 = temp2 + temp1;
        RCC->PLL1CR = (RCC->PLL1CR & ~RCC_PLL1CR_PLL1LOCKDLY) | (temp1 << RCC_PLL1CR_PLL1LOCKDLY_Pos);
        
        /* enable PLL1 */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1EN;
        RCC->PLL1SCR = 0;
        
        /* exit sleep mode */
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1SLEEP;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL1_READY_TIMEOUT;
        while ((RCC->PLL1CR & RCC_PLL1CR_PLL1FREERUN) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        
        PLLP = (PLLP >> 1) - 1;
        
        RCC->PLL1CFR = ((PLLQ << RCC_PLL1CFR_PLL1Q_Pos) & RCC_PLL1CFR_PLL1Q) | \
                       ((PLLP << RCC_PLL1CFR_PLL1P_Pos) & RCC_PLL1CFR_PLL1P) | \
                       ((PLLN << RCC_PLL1CFR_PLL1N_Pos) & RCC_PLL1CFR_PLL1N) | \
                       ((PLLF << RCC_PLL1CFR_PLL1F_Pos) & RCC_PLL1CFR_PLL1F);
        
        
        if (ClockSource == RCC_PLL_SOURCE_XTH)
        {
            RCC->PLL1CR &= ~RCC_PLL1CR_PLL1LOCKSEL;
        }
        
        /* update pll */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1UPDATEEN;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL1_READY_TIMEOUT;
        while ((RCC->PLL1CR & lock) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL1 clock and SSC function.
*@param : PLL1: specifies the new state of the PLL1. 
*             This parameter can be: ENABLE or DISABLE.
*@param : ClockSource: specifies the PLL1 entry clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SOURCE_RCH_DIV16: RCH oscillator clock divided by 16 selected as PLL1 clock entry.
*             @arg RCC_PLL_SOURCE_XTH: XTH oscillator clock selected as PLL1 clock entry.
*@param : PLLN: specifies the PLL1 down factor divider field.
*             This parameter must be a number between 1 and 63.
*@param : PLLF: specifies the PLL1 multiplication factor.
*             This parameter must be a number between 50 and 511.
*@param : PLLP: specifies the PLL1PCLK output frequency division control field.
*             This parameter must be one of 2, 4, 6 and 8.
*@param : PLLQ: specifies the PLL1QCLK output frequency division control field.
*             This parameter must be a number between 1 and 15.
*@param : SSC:  specifies the new state of the SSC.
*             This parameter can be: ENABLE or DISABLE.
*@param : Mode: specifies the SSC mode.
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SSC_MODE_CENTER: center spreading.
*             @arg RCC_PLL_SSC_MODE_DOWN: down spreading.
*@param : Period: specifies the SSC modulation period.
*             This parameter must be a number between 0 and 8191.
*@param : Step: specifies the SSC modulation step.
*             This parameter must be a number between 0 and 32767.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL1SSCConfig(uint32_t PLL1, uint32_t ClockSource, \
                            uint32_t PLLN, uint32_t PLLF, uint32_t PLLP, uint32_t PLLQ, \
                            uint32_t SSC, uint32_t Mode, uint32_t Period, uint32_t Step)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t lock;
    uint32_t timeout;
    #ifdef USE_FULL_ASSERT
    double freq;
    #endif
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(PLL1));
    
    /* check whether the current configured clock is the system clock */
    if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == (RCC_CCR1_SYSCLKSEL_0 | RCC_CCR1_SYSCLKSEL_1))
        return (HAL_ERROR);
    
    if (PLL1 == DISABLE)
    {
        /* disable PLL1 */
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1EN;
        RCC->PLL1SCR = 0;
        /* enter sleep mode */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1SLEEP;
        HAL_SimpleDelay(15);
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_RCC_PLL_CLOCK_SOURCE(ClockSource));
        assert_param(IS_RCC_PLL1_PLLN(PLLN));
        assert_param(IS_RCC_PLL1_PLLF(PLLF));
        assert_param(IS_RCC_PLL1_PLLP(PLLP));
        assert_param(IS_RCC_PLL1_PLLQ(PLLQ));
        assert_param(IS_FUNCTIONAL_STATE(SSC));
        
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
        {
            if ((RCC->RCHCR & (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != \
                              (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
                return (HAL_ERROR);
            lock = RCC_PLL1CR_PLL1FREERUN;
            #ifdef USE_FULL_ASSERT
            freq = (double)4000000;
            #endif
        }
        else
        {
            if ((RCC->XTHCR & (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN)) != (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN))
                return (HAL_ERROR);
            lock = RCC_PLL1CR_PLL1LOCK;
            #ifdef USE_FULL_ASSERT
            freq = (double)XTH_VALUE;
            #endif
        }

        // FCLK_VCO=FCLKIN*F/N; FCLK_P=FCLK_VCO /P; FCLK_Q=FCLK_VCO /Q

        #ifdef USE_FULL_ASSERT
        assert_param((freq >= 1000000) && (freq <= 132000000));
        freq = freq / PLLN;
        assert_param((freq >= 1000000) && (freq <= 2000000));
        freq = freq * PLLF;
        assert_param((freq >= 100000000) && (freq <= 550000000));
        assert_param(((freq / PLLP) >= 30000000) && ((freq / PLLP) <= 222000000));
        assert_param(((freq / PLLQ) >= 16000000) && ((freq / PLLQ) <= 222000000));
        #endif
        
        RCC->PLL1CR |= RCC_PLL1CR_PLL1LOCKSEL;
        RCC->PLL1CR &= ~(RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1QCLKEN);
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
            RCC->PLL1CR &= ~RCC_PLL1CR_PLL1SRCSEL;
        else
            RCC->PLL1CR |= RCC_PLL1CR_PLL1SRCSEL;
        
        /* The delay is 370us~440us, except 512us at 1/2/3MHz. */
        temp1 = SystemCoreClock >>  20;
        if ((SystemCoreClock - (temp1 << 20)) >= (1UL << 19))
            temp1++;
        temp2 = temp1 >> 2;
        temp1 = temp1 - (temp2 << 2);
        temp2 = temp2 * 3;
        temp1 = temp2 + temp1;
        RCC->PLL1CR = (RCC->PLL1CR & ~RCC_PLL1CR_PLL1LOCKDLY) | (temp1 << RCC_PLL1CR_PLL1LOCKDLY_Pos);
        
        /* enable PLL1 */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1EN;
    
        if (SSC == DISABLE)
        {
            RCC->PLL1SCR = 0;
        }
        else
        {
            assert_param(IS_RCC_SSC_MODE(Mode));
            assert_param(IS_RCC_SSC_PERIOD(Period));
            assert_param(IS_RCC_SSC_STEP(Step));
            
            RCC->PLL1SCR = ((Step << RCC_PLL1SCR_PLL1SSCSTP_Pos) & RCC_PLL1SCR_PLL1SSCSTP) | \
                            ((Period << RCC_PLL1SCR_PLL1SSCPER_Pos) & RCC_PLL1SCR_PLL1SSCPER) | \
                            Mode | RCC_PLL1SCR_PLL1SSCEN;
        }                       
        
        /* exit sleep mode */
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1SLEEP;
        
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL1_READY_TIMEOUT;
        while ((RCC->PLL1CR & RCC_PLL1CR_PLL1FREERUN) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        
        PLLP = (PLLP >> 1) - 1;
        
        RCC->PLL1CFR = ((PLLQ << RCC_PLL1CFR_PLL1Q_Pos) & RCC_PLL1CFR_PLL1Q) | \
                       ((PLLP << RCC_PLL1CFR_PLL1P_Pos) & RCC_PLL1CFR_PLL1P) | \
                       ((PLLN << RCC_PLL1CFR_PLL1N_Pos) & RCC_PLL1CFR_PLL1N) | \
                       ((PLLF << RCC_PLL1CFR_PLL1F_Pos) & RCC_PLL1CFR_PLL1F);
        
        if (ClockSource == RCC_PLL_SOURCE_XTH)
        {
            RCC->PLL1CR &= ~RCC_PLL1CR_PLL1LOCKSEL;
        }
        
        /* update pll */
        RCC->PLL1CR |= RCC_PLL1CR_PLL1UPDATEEN;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL1_READY_TIMEOUT;
        while ((RCC->PLL1CR & lock) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL1PCLK.
*@param : PLL1PCLK: new state of the PLL1PCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL1PCLKConfig(uint32_t PLL1PCLK)
{
    /* check whether the current configured clock is the system clock */
    if ((RCC->CCR1 & RCC_CCR1_SYSCLKSEL) == (RCC_CCR1_SYSCLKSEL_1 | RCC_CCR1_SYSCLKSEL_0))
        return (HAL_ERROR);
    
    if (PLL1PCLK == DISABLE)
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1PCLKEN;
    else
        RCC->PLL1CR |= RCC_PLL1CR_PLL1PCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configures the PLL1QCLK.
*@param : PLL1QCLK: new state of the PLL1QCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL1QCLKConfig(uint32_t PLL1QCLK)
{
    if (PLL1QCLK == DISABLE)
        RCC->PLL1CR &= ~RCC_PLL1CR_PLL1QCLKEN;
    else
        RCC->PLL1CR |= RCC_PLL1CR_PLL1QCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configures the PLL2 clock.
*@note        Automatically disable SSC function.
*@param : PLL2: specifies the new state of the PLL2. 
*             This parameter can be: ENABLE or DISABLE.
*@param : ClockSource: specifies the PLL2 entry clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SOURCE_RCH_DIV16: RCH oscillator clock divided by 16 selected as PLL2 clock entry.
*             @arg RCC_PLL_SOURCE_XTH: XTH oscillator clock selected as PLL2 clock entry.
*@param : PLLN: specifies the PLL2 down factor divider field.
*             This parameter must be a number between 1 and 63.
*@param : PLLF: specifies the PLL2 multiplication factor.
*             This parameter must be a number between 50 and 511.
*@param : PLLP: PLL2PCLK output frequency division control field.
*             This parameter must be one of 2, 4, 6 and 8.
*@param : PLLQ: PLL2QCLK output frequency division control field.
*             This parameter must be a number between 1 and 15.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL2Config(uint32_t PLL2, uint32_t ClockSource, \
                            uint32_t PLLN, uint32_t PLLF, uint32_t PLLP, uint32_t PLLQ)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t temp3;
    uint32_t lock;
    uint32_t timeout;
    #ifdef USE_FULL_ASSERT
    double freq;
    #endif
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(PLL2));
    
    if (PLL2 == DISABLE)
    {
        /* disable PLL2 */
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2EN;
        /* enter sleep mode */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2SLEEP;
        HAL_SimpleDelay(15);
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_RCC_PLL_CLOCK_SOURCE(ClockSource));
        assert_param(IS_RCC_PLL2_PLLN(PLLN));
        assert_param(IS_RCC_PLL2_PLLF(PLLF));
        assert_param(IS_RCC_PLL2_PLLP(PLLP));
        assert_param(IS_RCC_PLL2_PLLQ(PLLQ));
        
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
        {
            if ((RCC->RCHCR & (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != \
                              (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
                return (HAL_ERROR);
            lock = RCC_PLL2CR_PLL2FREERUN;
            #ifdef USE_FULL_ASSERT
            freq = (double)4000000;
            #endif
        }
        else
        {
            if ((RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN) != (RCC->XTHCR & (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN)))
                return (HAL_ERROR);
            lock = RCC_PLL2CR_PLL2LOCK;
            #ifdef USE_FULL_ASSERT
            freq = (double)XTH_VALUE;
            #endif
        }
        
        // FCLK_VCO=FCLKIN*F/N; FCLK_P=FCLK_VCO /P; FCLK_Q=FCLK_VCO /Q
        #ifdef USE_FULL_ASSERT
        assert_param((freq >= 1000000) && (freq <= 132000000));
        freq = freq / PLLN;
        assert_param((freq >= 1000000) && (freq <= 2000000));
        freq = freq * PLLF;
        assert_param((freq >= 100000000) && (freq <= 550000000));
        assert_param(((freq / PLLP) >= 30000000) && ((freq / PLLP) <= 220000000));
        assert_param(((freq / PLLQ) >= 16000000) && ((freq / PLLQ) <= 220000000));
        #endif
        
        RCC->PLL2CR |= RCC_PLL2CR_PLL2LOCKSEL;
        RCC->PLL2CR &= ~(RCC_PLL2CR_PLL2PCLKEN | RCC_PLL2CR_PLL2QCLKEN);
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
            RCC->PLL2CR &= ~RCC_PLL2CR_PLL2SRCSEL;
        else
            RCC->PLL2CR |= RCC_PLL2CR_PLL2SRCSEL;
        
        /* The delay is 370us~440us, except 512us at 1/2/3MHz. */
        temp1 = SystemCoreClock >>  20;
        if ((SystemCoreClock - (temp1 << 20)) >= (1UL << 19))
            temp1++;
        temp2 = temp1 >> 2;
        temp1 = temp1 - (temp2 << 2);
        temp2 = temp2 * 3;
        temp1 = temp2 + temp1;
        temp3 = 0;
        if (temp1 > 63)
        {
            temp3 = temp1 - 63;
            temp1 = 63;
            temp3 = temp3 * 5 + temp2 * 2;
        }
        RCC->PLL2CR = (RCC->PLL2CR & ~RCC_PLL2CR_PLL2LOCKDLY) | (temp1 << RCC_PLL2CR_PLL2LOCKDLY_Pos);
        
        /* enable PLL2 */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2EN;
        RCC->PLL2SCR = 0;
        
        /* exit sleep mode */
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2SLEEP;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL2_READY_TIMEOUT;
        while ((RCC->PLL2CR & RCC_PLL2CR_PLL2FREERUN) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        
        while (temp3--)
        {
            RCC_PLL_DELAY();
        }
        
        PLLP = (PLLP >> 1) - 1;
        
        RCC->PLL2CFR = ((PLLQ << RCC_PLL2CFR_PLL2Q_Pos) & RCC_PLL2CFR_PLL2Q) | \
                       ((PLLP << RCC_PLL2CFR_PLL2P_Pos) & RCC_PLL2CFR_PLL2P) | \
                       ((PLLN << RCC_PLL2CFR_PLL2N_Pos) & RCC_PLL2CFR_PLL2N) | \
                       ((PLLF << RCC_PLL2CFR_PLL2F_Pos) & RCC_PLL2CFR_PLL2F);
        
        if (ClockSource == RCC_PLL_SOURCE_XTH)
        {
            RCC->PLL2CR &= ~RCC_PLL2CR_PLL2LOCKSEL;
        }
        
        /* update pll */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2UPDATEEN;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL2_READY_TIMEOUT;
        while ((RCC->PLL2CR & lock) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL2 clock and SSC function.
*@param : PLL2: specifies the new state of the PLL1. 
*             This parameter can be: ENABLE or DISABLE.
*@param : ClockSource: specifies the PLL2 entry clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SOURCE_RCH_DIV16: RCH oscillator clock divided by 16 selected as PLL2 clock entry.
*             @arg RCC_PLL_SOURCE_XTH: XTH oscillator clock selected as PLL2 clock entry.
*@param : PLLN: specifies the PLL2 down factor divider field.
*             This parameter must be a number between 1 and 63.
*@param : PLLF: specifies the PLL2 multiplication factor.
*             This parameter must be a number between 50 and 511.
*@param : PLLP: specifies the PLL2PCLK output frequency division control field.
*             This parameter must be one of 2, 4, 6 and 8.
*@param : PLLQ: specifies the PLL2QCLK output frequency division control field.
*             This parameter must be a number between 1 and 15.
*@param : SSC:  specifies the new state of the SSC.
*             This parameter can be: ENABLE or DISABLE.
*@param : Mode: specifies the SSC mode.
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SSC_MODE_CENTER: center spreading.
*             @arg RCC_PLL_SSC_MODE_DOWN: down spreading.
*@param : Period: specifies the SSC modulation period.
*             This parameter must be a number between 0 and 8191.
*@param : Step: specifies the SSC modulation step.
*             This parameter must be a number between 0 and 32767.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL2SSCConfig(uint32_t PLL2, uint32_t ClockSource, \
                            uint32_t PLLN, uint32_t PLLF, uint32_t PLLP, uint32_t PLLQ, \
                            uint32_t SSC, uint32_t Mode, uint32_t Period, uint32_t Step)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t temp3;
    uint32_t lock;
    uint32_t timeout;
    #ifdef USE_FULL_ASSERT
    double freq;
    #endif
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(PLL2));
    
    if (PLL2 == DISABLE)
    {
        /* disable PLL2 */
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2EN;
        
        /* enter sleep mode */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2SLEEP;
        HAL_SimpleDelay(15);
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_RCC_PLL_CLOCK_SOURCE(ClockSource));
        assert_param(IS_RCC_PLL2_PLLN(PLLN));
        assert_param(IS_RCC_PLL2_PLLF(PLLF));
        assert_param(IS_RCC_PLL2_PLLP(PLLP));
        assert_param(IS_RCC_PLL2_PLLQ(PLLQ));
        
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
        {
            if ((RCC->RCHCR & (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != \
                              (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
                return (HAL_ERROR);
            lock = RCC_PLL2CR_PLL2FREERUN;
            #ifdef USE_FULL_ASSERT
            freq = (double)4000000;
            #endif
        }
        else
        {
            if ((RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN) != (RCC->XTHCR & (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN)))
                return (HAL_ERROR);
            lock = RCC_PLL2CR_PLL2LOCK;
            #ifdef USE_FULL_ASSERT
            freq = (double)XTH_VALUE;
            #endif
        }
        
        // FCLK_VCO=FCLKIN*F/N; FCLK_P=FCLK_VCO /P; FCLK_Q=FCLK_VCO /Q
        #ifdef USE_FULL_ASSERT
        assert_param((freq >= 1000000) && (freq <= 132000000));
        freq = freq / PLLN;
        assert_param((freq >= 1000000) && (freq <= 2000000));
        freq = freq * PLLF;
        assert_param((freq >= 100000000) && (freq <= 550000000));
        assert_param(((freq / PLLP) >= 30000000) && ((freq / PLLP) <= 220000000));
        assert_param(((freq / PLLQ) >= 16000000) && ((freq / PLLQ) <= 220000000));
        #endif
        
        RCC->PLL2CR |= RCC_PLL2CR_PLL2LOCKSEL;
        RCC->PLL2CR &= ~(RCC_PLL2CR_PLL2PCLKEN | RCC_PLL2CR_PLL2QCLKEN);
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
            RCC->PLL2CR &= ~RCC_PLL2CR_PLL2SRCSEL;
        else
            RCC->PLL2CR |= RCC_PLL2CR_PLL2SRCSEL;
        
        /* The delay is 370us~440us, except 512us at 1/2/3MHz. */
        temp1 = SystemCoreClock >>  20;
        if ((SystemCoreClock - (temp1 << 20)) >= (1UL << 19))
            temp1++;
        temp2 = temp1 >> 2;
        temp1 = temp1 - (temp2 << 2);
        temp2 = temp2 * 3;
        temp1 = temp2 + temp1;
        temp3 = 0;
        if (temp1 > 63)
        {
            temp3 = temp1 - 63;
            temp1 = 63;
            temp3 = temp3 * 5 + temp2 * 2;
        }
        RCC->PLL2CR = (RCC->PLL2CR & ~RCC_PLL2CR_PLL2LOCKDLY) | (temp1 << RCC_PLL2CR_PLL2LOCKDLY_Pos);
        
        /* enable PLL2 */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2EN;
                              
        if (SSC == DISABLE)
        {
            RCC->PLL2SCR = 0;
        }
        else
        {
            RCC->PLL2SCR = ((Step << RCC_PLL2SCR_PLL2SSCSTP_Pos) & RCC_PLL2SCR_PLL2SSCSTP) | \
                            ((Period << RCC_PLL2SCR_PLL2SSCPER_Pos) & RCC_PLL2SCR_PLL2SSCPER) | \
                            Mode | RCC_PLL2SCR_PLL2SSCEN;
        }
        
        /* exit sleep mode */
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2SLEEP;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL2_READY_TIMEOUT;
        while ((RCC->PLL2CR & RCC_PLL2CR_PLL2FREERUN) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        
        while (temp3--)
        {
            RCC_PLL_DELAY();
        }
        
        PLLP = (PLLP >> 1) - 1;
        
        RCC->PLL2CFR = ((PLLQ << RCC_PLL2CFR_PLL2Q_Pos) & RCC_PLL2CFR_PLL2Q) | \
                       ((PLLP << RCC_PLL2CFR_PLL2P_Pos) & RCC_PLL2CFR_PLL2P) | \
                       ((PLLN << RCC_PLL2CFR_PLL2N_Pos) & RCC_PLL2CFR_PLL2N) | \
                       ((PLLF << RCC_PLL2CFR_PLL2F_Pos) & RCC_PLL2CFR_PLL2F);
        
        if (ClockSource == RCC_PLL_SOURCE_XTH)
        {
            RCC->PLL2CR &= ~RCC_PLL2CR_PLL2LOCKSEL;
        }
        
        /* update pll */
        RCC->PLL2CR |= RCC_PLL2CR_PLL2UPDATEEN;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL2_READY_TIMEOUT;
        while ((RCC->PLL2CR & lock) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL2PCLK.
*@param : PLL2PCLK: new state of the PLL2PCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL2PCLKConfig(uint32_t PLL2PCLK)
{
    if (PLL2PCLK == DISABLE)
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2PCLKEN;
    else
        RCC->PLL2CR |= RCC_PLL2CR_PLL2PCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configures the PLL2QCLK.
*@param : PLL2QCLK: new state of the PLL2QCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL2QCLKConfig(uint32_t PLL2QCLK)
{
    if (DISABLE == PLL2QCLK)
        RCC->PLL2CR &= ~RCC_PLL2CR_PLL2QCLKEN;
    else
        RCC->PLL2CR |= RCC_PLL2CR_PLL2QCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configures the PLL3 clock.
*@param : PLL3: specifies the new state of the PLL3. 
*             This parameter can be: ENABLE or DISABLE.
*@param : ClockSource: specifies the PLL3 entry clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_PLL_SOURCE_RCH_DIV16: RCH oscillator clock divided by 16 selected as PLL3 clock entry.
*             @arg RCC_PLL_SOURCE_XTH: XTH oscillator clock selected as PLL3 clock entry.
*@param : PLLN: specifies the PLL3 down factor divider field.
*             This parameter must be a number between 1 and 64.
*@param : PLLF: specifies the PLL3 multiplication factor.
*             This parameter must be a number between 1 and 128.
*@param : PLLP: PLL3PCLK output frequency division control field.
*             This parameter must be one of 1, 2, 4 and 8.
*@param : PLLQ: PLL3QCLK output frequency division control field.
*             This parameter must be one of 1, 2, 4 and 8.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL3Config(uint32_t PLL3, uint32_t ClockSource, \
                                     uint32_t PLLN, uint32_t PLLF, uint32_t PLLP, uint32_t PLLQ)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t lock;
    uint32_t timeout;
    #ifdef USE_FULL_ASSERT
    double freq;
    #endif
    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(PLL3));
    
    if (DISABLE == PLL3)
    {
        /* disable PLL3 */
        RCC->PLL3CR &= ~RCC_PLL3CR_PLL3EN;
        /* enter sleep mode */
        RCC->PLL3CR |= RCC_PLL3CR_PLL3SLEEP;
    }
    else
    {
        /* Check the parameters */
        assert_param(IS_RCC_PLL_CLOCK_SOURCE(ClockSource));
        assert_param(IS_RCC_PLL3_PLLN(PLLN));
        assert_param(IS_RCC_PLL3_PLLF(PLLF));
        assert_param(IS_RCC_PLL3_PLLP(PLLP));
        assert_param(IS_RCC_PLL3_PLLQ(PLLQ));
        
        if (RCC_PLL_SOURCE_RCH_DIV16 == ClockSource)
        {
            if ((RCC->RCHCR & (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != \
                              (RCC_RCHCR_RCHDIV | RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
                return (HAL_ERROR);
            lock = RCC_PLL3CR_PLL3FREERUN;
            #ifdef USE_FULL_ASSERT
            freq = (double)4000000;
            #endif
        }
        else
        {
            if ((RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN) != (RCC->XTHCR & (RCC_XTHCR_XTHRDY | RCC_XTHCR_XTHEN)))
                return (HAL_ERROR);
            lock = RCC_PLL3CR_PLL3LOCK;
            #ifdef USE_FULL_ASSERT
            freq = (double)XTH_VALUE;
            #endif
        }

        // FCLK_VCO=FCLKIN*F/N; FCLK_P=FCLK_VCO /P; FCLK_Q=FCLK_VCO /Q

        #ifdef USE_FULL_ASSERT
        assert_param((freq >= 1000000) && (freq <= 50000000));
        freq = freq / PLLN;
        freq = freq * PLLF;
        assert_param((freq >= 200000000) && (freq <= 500000000));
        assert_param(((freq / PLLP) >= 25000000) && ((freq / PLLP) <= 220000000));
        assert_param(((freq / PLLQ) >= 25000000) && ((freq / PLLQ) <= 220000000));
        #endif
        
        RCC->PLL3CR |= RCC_PLL3CR_PLL3LOCKSEL;
        RCC->PLL3CR &= ~(RCC_PLL3CR_PLL3PCLKEN | RCC_PLL3CR_PLL3QCLKEN);
        if (ClockSource == RCC_PLL_SOURCE_RCH_DIV16)
            RCC->PLL3CR &= ~RCC_PLL3CR_PLL3SRCSEL;
        else
            RCC->PLL3CR |= RCC_PLL3CR_PLL3SRCSEL;
        
        /* The delay is 100us~170us, except 512us at 1MHz, and 256us at 2MHz. */
        temp1 = SystemCoreClock >>  20;
        if ((SystemCoreClock - (temp1 << 20)) >= (1UL << 19))
            temp1++;
        temp2 = temp1 >> 2;
        temp1 = temp1 - (temp2 << 2);
        temp2 = temp2 * 3;
        temp1 = temp2 + temp1;
        if (temp1 == 0)
            temp1 = 1;
        RCC->PLL3CR = (RCC->PLL3CR & ~RCC_PLL3CR_PLL3LOCKDLY) | (temp1 << RCC_PLL3CR_PLL3LOCKDLY_Pos);
        
        /* enable PLL3 */
        RCC->PLL3CR |= RCC_PLL3CR_PLL3EN;
        
        /* exit sleep mode */
        RCC->PLL3CR &= ~RCC_PLL3CR_PLL3SLEEP;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL3_READY_TIMEOUT;
        while ((RCC->PLL3CR & RCC_PLL3CR_PLL3FREERUN) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
        
        PLLQ = PLLQ >> 1;
        if (PLLQ == 4)
            PLLQ = 3;
        
        PLLP = PLLP >> 1;
        if (PLLP == 4)
            PLLP = 3;
        
        PLLN--;
        PLLF--;
        
        RCC->PLL3CFR = ((PLLQ << RCC_PLL3CFR_PLL3Q_Pos) & RCC_PLL3CFR_PLL3Q) | \
                       ((PLLP << RCC_PLL3CFR_PLL3P_Pos) & RCC_PLL3CFR_PLL3P) | \
                       ((PLLN << RCC_PLL3CFR_PLL3N_Pos) & RCC_PLL3CFR_PLL3N) | \
                       ((PLLF << RCC_PLL3CFR_PLL3F_Pos) & RCC_PLL3CFR_PLL3F);
        
        if (ClockSource == RCC_PLL_SOURCE_XTH)
        {
            RCC->PLL3CR &= ~RCC_PLL3CR_PLL3LOCKSEL;
        }
        
        /* update pll */
        RCC->PLL3CR |= RCC_PLL3CR_PLL3UPDATEEN;
        HAL_SimpleDelay(15);
        
        /* Wait till PLL is ready */
        timeout = RCC_PLL3_READY_TIMEOUT;
        while ((RCC->PLL3CR & lock) == 0)
        {
            if (timeout-- == 0)
                return (HAL_TIMEOUT);
        }
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the PLL3PCLK.
*@param : PLL3PCLK: new state of the PLL3PCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL3PCLKConfig(uint32_t PLL3PCLK)
{
    if (PLL3PCLK == DISABLE)
        RCC->PLL3CR &= ~RCC_PLL3CR_PLL3PCLKEN;
    else
        RCC->PLL3CR |= RCC_PLL3CR_PLL3PCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Configures the PLL3QCLK.
*@param : PLL3QCLK: new state of the PLL3QCLK. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PLL3QCLKConfig(uint32_t PLL3QCLK)
{
    if (PLL3QCLK == DISABLE)
        RCC->PLL3CR &= ~RCC_PLL3CR_PLL3QCLKEN;
    else
        RCC->PLL3CR |= RCC_PLL3CR_PLL3QCLKEN;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Enables or disables the specified RCC interrupts.
*@param : IT: specifies the RCC interrupt sources to be enabled or disabled. 
*             This parameter can be any combination of the following values:
*             @arg RCC_IT_RCL_READY: RCL ready interrupt.
*             @arg RCC_IT_XTL_READY: XTL ready interrupt.
*             @arg RCC_IT_RCH_READY: RCH ready interrupt.
*             @arg RCC_IT_XTH_READY: XTH ready interrupt.
*             @arg RCC_IT_PLL1_READY: PLL1 ready interrupt.
*             @arg RCC_IT_PLL2_READY: PLL2 ready interrupt.
*             @arg RCC_IT_PLL3_READY: PLL3 ready interrupt.
*             @arg RCC_IT_XTH_STOP: XTH stop interrupt.
*             @arg RCC_IT_XTL_STOP: XTL stop interrupt.
*@param : NewStatus: new state of the specified RCC interrupts. 
*             This parameter can be: ENABLE or DISABLE.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_ITConfig(uint32_t IT, uint32_t NewStatus)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(IT));
    
    if (NewStatus)
        RCC->CIR |= ((IT & RCC_IT_READY_MASK) << 8) | ((IT & RCC_IT_STOP_MASK) >> 3);
    else
        RCC->CIR &= ~((((IT & RCC_IT_READY_MASK)) << 8) | ((IT & RCC_IT_STOP_MASK) >> 3));
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Checks whether the specified RCC interrupt has occurred or not.
*@param : IT: specifies the RCC interrupt source to check. 
*             This parameter can be one of the following values:
*             @arg RCC_IT_RCL_READY: RCL ready interrupt.
*             @arg RCC_IT_XTL_READY: XTL ready interrupt.
*             @arg RCC_IT_RCH_READY: RCH ready interrupt.
*             @arg RCC_IT_XTH_READY: XTH ready interrupt.
*             @arg RCC_IT_PLL1_READY: PLL1 ready interrupt.
*             @arg RCC_IT_PLL2_READY: PLL2 ready interrupt.
*             @arg RCC_IT_PLL3_READY: PLL3 ready interrupt.
*             @arg RCC_IT_XTH_STOP: XTH stop interrupt.
*             @arg RCC_IT_XTL_STOP: XTL stop interrupt.
*@return: The new state of RCC_IT (SET or RESET).
******************************************************************************/
uint32_t HAL_RCC_GetITFlag(uint32_t IT)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(IT));
    
    if ((RCC->CIR & IT) == IT)
        return SET;
    else
        return RESET;
}

/******************************************************************************
*@brief : Clears the RCC's interrupt pending bits.
*@param : IT: specifies the interrupt pending bit to clear. 
*             This parameter can be any combination of the following values:
*             @arg RCC_IT_RCL_READY: RCL ready interrupt.
*             @arg RCC_IT_XTL_READY: XTL ready interrupt.
*             @arg RCC_IT_RCH_READY: RCH ready interrupt.
*             @arg RCC_IT_XTH_READY: XTH ready interrupt.
*             @arg RCC_IT_PLL1_READY: PLL1 ready interrupt.
*             @arg RCC_IT_PLL2_READY: PLL2 ready interrupt.
*             @arg RCC_IT_PLL3_READY: PLL3 ready interrupt.
*             @arg RCC_IT_XTH_STOP: XTH stop interrupt.
*             @arg RCC_IT_XTL_STOP: XTL stop interrupt.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_ClearITFlag(uint32_t IT)
{
    /* Check the parameters */
    assert_param(IS_RCC_IT(IT));
    
    RCC->CIR |=  ((IT & RCC_IT_READY_MASK) << 16) | ( (IT & RCC_IT_STOP_MASK) >> 2);
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Initializes the CPU, AHB and APB buses clocks according to the 
*         specified parameters in the @ref RCC_ClkInitTypeDef.
*@param : RCC_ClkInit: pointer to an RCC_ClkInitTypeDef structure that
*         contains the configuration information for the RCC peripheral.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInit)
{
    /* Check the parameters */
    assert_param(IS_RCC_CLOCK_TYPE(RCC_ClkInit->ClockType));
    
    /* system clok source configuration */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_SYSCLK)
    {
        if (HAL_RCC_SYSCLKSourceConfig(RCC_ClkInit->SYSCLKSource) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* first level frequency division of system clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_SYSDIV0)
    {
        if (HAL_RCC_SYSCLKDiv0Config(RCC_ClkInit->SYSCLKDiv0) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* second level frequency division of system clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_SYSDIV1)
    {
        if (HAL_RCC_SYSCLKDiv1Config(RCC_ClkInit->SYSCLKDiv1) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* frequency division of PCLK1 clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_PCLK1)
    {
        if (HAL_RCC_PCLK1DivConfig(RCC_ClkInit->PCLK1Div) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* frequency division of PCLK2 clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_PCLK2)
    {
        if (HAL_RCC_PCLK2DivConfig(RCC_ClkInit->PCLK2Div) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* frequency division of PCLK3 clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_PCLK3)
    {
        if (HAL_RCC_PCLK3DivConfig(RCC_ClkInit->PCLK3Div) != HAL_OK)
            return HAL_ERROR;
    }
    
    /* frequency division of PCLK4 clock */
    if (RCC_ClkInit->ClockType & RCC_CLOCK_TYPE_PCLK4)
    {
        if (HAL_RCC_PCLK4DivConfig(RCC_ClkInit->PCLK4Div) != HAL_OK)
            return HAL_ERROR;
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the source of the system clock.
*@param : ClockSource: the source of the system clock.
*         This parameter can be one of the following values:
*             @arg RCC_SYSCLK_SOURCE_RCH: The RCH is used as system clock source.
*             @arg RCC_SYSCLK_SOURCE_XTH: The XTH is used as system clock source.
*             @arg RCC_SYSCLK_SOURCE_PLL1PCLK: The PLL1PCLK is used as system clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_SYSCLKSourceConfig(uint32_t ClockSource)
{
    uint32_t temp;
    
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_SOURCE(ClockSource));
    
    if (ClockSource == RCC_SYSCLK_SOURCE_RCH)
    {
        /* Check whether the RCH clock is turned on and running stably */
        if ((RCC->RCHCR & (RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != (RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
            return HAL_ERROR;
        
        /* system clok source configuration */
        RCC->CCR1 = RCC->CCR1 & ~RCC_CCR1_SYSCLKSEL;
    }
    else if (RCC_SYSCLK_SOURCE_XTH == ClockSource)
    {
        /* Check whether the XTH clock is turned on and running stably */
        if ((RCC->XTHCR & (RCC_XTHCR_XTHEN | RCC_XTHCR_XTHRDY)) != (RCC_XTHCR_XTHEN | RCC_XTHCR_XTHRDY))
            return HAL_ERROR;
        
        /* system clok source configuration */
        RCC->CCR1 = (RCC->CCR1 & ~RCC_CCR1_SYSCLKSEL) | RCC_CCR1_SYSCLKSEL_1;
    }
    else
    {
        /* Check whether the PLL clock is turned on and running stably */
        if (RCC->PLL1CR & RCC_PLL1CR_PLL1SRCSEL)
        {
            if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1LOCK)) != \
                                                      (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1LOCK))
                return HAL_ERROR;
        }
        else
        {
            if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1FREERUN)) != \
                                                      (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1PCLKEN | RCC_PLL1CR_PLL1FREERUN))
                return HAL_ERROR;
        }
        
        /* system clok source configuration */
        RCC->CCR1 = (RCC->CCR1 & ~RCC_CCR1_SYSCLKSEL) | RCC_CCR1_SYSCLKSEL_1 | RCC_CCR1_SYSCLKSEL_0;
    }
    
    temp = SystemCoreClock;
    SystemCoreClock = HAL_RCC_GetSysCoreClockFreq();
    if (temp != SystemCoreClock)
    {
        HAL_InitTick(g_systickHandle.intPrio,g_systickHandle.msPeriod);
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the first level frequency division of system clock.
*@param : Div: the first level frequency division of system clock.
*             This parameter must be a number between 1 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_SYSCLKDiv0Config(uint32_t Div)
{
    uint32_t temp;
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_DIV(Div));
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_SYSDIV0) | (((Div - 1) << RCC_CCR2_SYSDIV0_Pos) & RCC_CCR2_SYSDIV0);
    HAL_SimpleDelay(5);
    
    /* Wait till the update frequency division to complete */
    timeout = 256u;
    while ((RCC->CCR2 & RCC_CCR2_DIVDONE) == 0)
    {
        if (timeout-- == 0)
            return (HAL_TIMEOUT);
    }
    
    temp = SystemCoreClock;
    SystemCoreClock = HAL_RCC_GetSysCoreClockFreq();
    if (temp != SystemCoreClock)
    {
        HAL_InitTick(g_systickHandle.intPrio,g_systickHandle.msPeriod);
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the second level frequency division of system clock.
*@param : Div: the second level frequency division of system clock.
*             This parameter must be a number between 1 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_SYSCLKDiv1Config(uint32_t Div)
{
    uint32_t temp;
    uint32_t timeout;
    
    /* Check the parameters */
    assert_param(IS_RCC_SYSCLK_DIV(Div));
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_SYSDIV1) | (((Div - 1) << RCC_CCR2_SYSDIV1_Pos) & RCC_CCR2_SYSDIV1);
    HAL_SimpleDelay(2);
    
    /* Wait till the update frequency division to complete */
    timeout = 256u;
    while ((RCC->CCR2 & RCC_CCR2_DIVDONE) == 0)
    {
        if (timeout-- == 0)
            return (HAL_TIMEOUT);
    }
    
    temp = SystemCoreClock;
    SystemCoreClock = HAL_RCC_GetSysCoreClockFreq();
    if (temp != SystemCoreClock)
    {
        HAL_InitTick(g_systickHandle.intPrio,g_systickHandle.msPeriod);
    }
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the frequency division of PCLK1 clock.
*@param : Div: the frequency division of PCLK1 clock.
*             This parameter must be one of 1, 2, 4, 8 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PCLK1DivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK_DIV(Div));
    
    if (Div == 1)
        Div = 0;
    else
    {
        Div = Div >> 2;
        if (Div == 4)
            Div = 3;
        Div += 4;
    }
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_PCLK1DIV) | (Div << RCC_CCR2_PCLK1DIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the frequency division of PCLK2 clock.
*@param : Div: the frequency division of PCLK1 clock.
*             This parameter must be one of 1, 2, 4, 8 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PCLK2DivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK_DIV(Div));
    
    if (Div == 1)
        Div = 0;
    else
    {
        Div = Div >> 2;
        if (Div == 4)
            Div = 3;
        Div += 4;
    }
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_PCLK2DIV) | (Div << RCC_CCR2_PCLK2DIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the frequency division of PCLK3 clock.
*@param : Div: the frequency division of PCLK1 clock.
*             This parameter must be one of 1, 2, 4, 8 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PCLK3DivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK_DIV(Div));
    
    if (Div == 1)
        Div = 0;
    else
    {
        Div = Div >> 2;
        if (Div == 4)
            Div = 3;
        Div += 4;
    }
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_PCLK3DIV) | (Div << RCC_CCR2_PCLK3DIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the frequency division of PCLK4 clock.
*@param : Div: the frequency division of PCLK1 clock.
*             This parameter must be one of 1, 2, 4, 8 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_PCLK4DivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_PCLK_DIV(Div));
    
    if (Div == 1)
        Div = 0;
    else
    {
        Div = Div >> 2;
        if (Div == 4)
            Div = 3;
        Div += 4;
    }
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_PCLK4DIV) | (Div << RCC_CCR2_PCLK4DIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the frequency division of the HRNG slow clock.
*@param : Div: the frequency division of the HRNG slow clock.
*             This parameter must be a number between 1 and 128.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_HRNGSClockDivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_HRNGS_CLK_DIV(Div));
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_HRNGSDIV) | (((Div - 1) << RCC_CCR2_HRNGSDIV_Pos) & RCC_CCR2_HRNGSDIV);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure the FLTCLK (LVD and COMP) clock source.
*@param : ClockSource: the FLTCLK (LVD and COMP) clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_FLT_CLK_SOURCE_PCLK1_DIV32: Divide by 32 of PCLK is used as FLTCLK source.
*             @arg RCC_FLT_CLK_SOURCE_RCL: The RCL is used as FLTCLK source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_FLTClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_FLT_CLK_SOURCE(ClockSource));
    
    RCC->CCR2 = (RCC->CCR2 & ~RCC_CCR2_FLTCLKSEL) | ((ClockSource << RCC_CCR2_FLTCLKSEL_Pos) & RCC_CCR2_FLTCLKSEL);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPUART1 clock source.
*@param : ClockSource: LPUART1 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPUART1_CLK_SOURCE_RCL : the RCL is used as LPUART1 clock source.
*             @arg RCC_LPUART1_CLK_SOURCE_XTL : the XTL is used as LPUART1 clock source.
*             @arg RCC_LPUART1_CLK_SOURCE_PCLK1_DIV4 : Divide by 4 of PCLK is used as LPUART1 clock source.
*             @arg RCC_LPUART1_CLK_SOURCE_PCLK1_DIV8 : Divide by 8 of PCLK is used as LPUART1 clock source.
*             @arg RCC_LPUART1_CLK_SOURCE_PCLK1_DIV16 : Divide by 16 of PCLK is used as LPUART1 clock source.
*             @arg RCC_LPUART1_CLK_SOURCE_PCLK1_DIV32 : Divide by 32 of PCLK is used as LPUART1 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPUART1ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPUART1_CLK_SOURCE(ClockSource));
    
    if (ClockSource <= RCC_LPUART1_CLK_SOURCE_XTL)
        RCC->PERCFGR = (RCC->PERCFGR & ~(RCC_PERCFGR_LPUART1CKS | RCC_PERCFGR_LPUART1DIV)) | \
                       (ClockSource << RCC_PERCFGR_LPUART1CKS_Pos);
    else
        RCC->PERCFGR = (RCC->PERCFGR & ~(RCC_PERCFGR_LPUART1CKS | RCC_PERCFGR_LPUART1DIV)) | \
                       RCC_PERCFGR_LPUART1CKS_1 | ((ClockSource - 2) << RCC_PERCFGR_LPUART1DIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM1 clock source.
*@param : ClockSource: LPTIM1 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM1_CLK_SOURCE_PCLK1 : the PCLK1 is used as LPTIM1 clock source.
*             @arg RCC_LPTIM1_CLK_SOURCE_RCL : the RCL is used as LPTIM1 clock source.
*             @arg RCC_LPTIM1_CLK_SOURCE_RCH : the RCH is used as LPTIM1 clock source.
*             @arg RCC_LPTIM1_CLK_SOURCE_XTL : the XTL is used as LPTIM1 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM1ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM1CKS) | (ClockSource << RCC_PERCFGR_LPTIM1CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM2 clock source.
*@param : ClockSource: LPTIM2 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM2_CLK_SOURCE_PCLK1 : the PCLK1 is used as LPTIM2 clock source.
*             @arg RCC_LPTIM2_CLK_SOURCE_RCL : the RCL is used as LPTIM2 clock source.
*             @arg RCC_LPTIM2_CLK_SOURCE_RCH : the RCH is used as LPTIM2 clock source.
*             @arg RCC_LPTIM2_CLK_SOURCE_XTL : the XTL is used as LPTIM2 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM2ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM2_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM2CKS) | (ClockSource << RCC_PERCFGR_LPTIM2CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM3 clock source.
*@param : ClockSource: LPTIM3 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM3_CLK_SOURCE_PCLK3 : the PCLK3 is used as LPTIM3 clock source.
*             @arg RCC_LPTIM3_CLK_SOURCE_RCL : the RCL is used as LPTIM3 clock source.
*             @arg RCC_LPTIM3_CLK_SOURCE_RCH : the RCH is used as LPTIM3 clock source.
*             @arg RCC_LPTIM3_CLK_SOURCE_XTL : the XTL is used as LPTIM3 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM3ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM3_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM345CKS) | (ClockSource << RCC_PERCFGR_LPTIM345CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM4 clock source.
*@param : ClockSource: LPTIM4 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM4_CLK_SOURCE_PCLK3 : the PCLK3 is used as LPTIM4 clock source.
*             @arg RCC_LPTIM4_CLK_SOURCE_RCL : the RCL is used as LPTIM4 clock source.
*             @arg RCC_LPTIM4_CLK_SOURCE_RCH : the RCH is used as LPTIM4 clock source.
*             @arg RCC_LPTIM4_CLK_SOURCE_XTL : the XTL is used as LPTIM4 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM4ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM4_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM345CKS) | (ClockSource << RCC_PERCFGR_LPTIM345CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM5 clock source.
*@param : ClockSource: LPTIM5 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM5_CLK_SOURCE_PCLK3 : the PCLK3 is used as LPTIM5 clock source.
*             @arg RCC_LPTIM5_CLK_SOURCE_RCL : the RCL is used as LPTIM5 clock source.
*             @arg RCC_LPTIM5_CLK_SOURCE_RCH : the RCH is used as LPTIM5 clock source.
*             @arg RCC_LPTIM5_CLK_SOURCE_XTL : the XTL is used as LPTIM5 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM5ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM5_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM345CKS) | (ClockSource << RCC_PERCFGR_LPTIM345CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure LPTIM6 clock source.
*@param : ClockSource: LPTIM6 clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_LPTIM6_CLK_SOURCE_PCLK3 : the PCLK3 is used as LPTIM6 clock source.
*             @arg RCC_LPTIM6_CLK_SOURCE_RCL : the RCL is used as LPTIM6 clock source.
*             @arg RCC_LPTIM6_CLK_SOURCE_RCH : the RCH is used as LPTIM6 clock source.
*             @arg RCC_LPTIM6_CLK_SOURCE_XTL : the XTL is used as LPTIM6 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LPTIM6ClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM6_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_LPTIM6CKS) | (ClockSource << RCC_PERCFGR_LPTIM6CKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure the SDMMC clock source.
*@param : ClockSource: the SDMMC clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_SDMMC_CLK_SOURCE_SYSCLK: the system clock HCLK is used as SDMMC clock source.
*             @arg RCC_SDMMC_CLK_SOURCE_PLL2PCLK: the PLL2PCLK is used as SDMMC clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_SDMMCClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC_CLK_SOURCE(ClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_SDMMCCKS) | (ClockSource << RCC_PERCFGR_SDMMCCKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure the SDMMC sample clock source.
*@param : ClockSource: the SDMMC sample clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_SDMMC_SCLK_SOURCE_SDMMC_CLK_DELAY: Clock of MMC card passing through delay unit is used as SDMMC sample clock source.
*             @arg RCC_SDMMC_SCLK_SOURCE_SDMMC1_CKIN: Feedback clock of external driver of MMC card 1 is used as SDMMC sample clock source.
*             @arg RCC_SDMMC_SCLK_SOURCE_SDMMC2_CKIN: Feedback clock of external driver of MMC card 2 is used as SDMMC sample clock source.
*             @arg RCC_SDMMC_SCLK_SOURCE_SDMMC_CLK_NO_DELAY: Clock of MMC card without delay unit is used as SDMMC sample clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_SDMMCSampleClockSourceConfig(uint32_t SampleClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC_SCLK_SOURCE(SampleClockSource));
    
    RCC->PERCFGR = (RCC->PERCFGR & ~RCC_PERCFGR_SDMMCSCKS) | (SampleClockSource << RCC_PERCFGR_SDMMCSCKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure the pixel clock frequency division.
*@param : Div: pixel clock frequency division.
*             This parameter must be one of 2, 4, 8 and 16.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_LCDPiexlClockDivConfig(uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_RCC_LCD_CLK_DIV(Div));
    
    Div = Div >> 2;
    if (Div == 4)
        Div = 3;
    
    RCC->DCKCFG = (RCC->DCKCFG & ~RCC_DCKCFG_LCDDIV) | (Div << RCC_DCKCFG_LCDDIV_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configure the RTC clock source.
*@param : ClockSource: the RTC clock source.
*             This parameter can be the one of the following values:
*             @arg RCC_RTC_CLK_SOURCE_RCL: the RCL is used as RTC clock source.
*             @arg RCC_RTC_CLK_SOURCE_XTL: the XTL is used as RTC clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_RTCClockSourceConfig(uint32_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_RCC_RTC_CLK_SOURCE(ClockSource));
    
    RCC->STDBYCTRL = (RCC->STDBYCTRL & ~RCC_STDBYCTRL_RTCSEL) | (ClockSource << RCC_STDBYCTRL_RTCSEL_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Configures the RCC_ClkInitStruct according to the internal
*         RCC configuration registers.
*@param : RCC_ClkInit: pointer to an RCC_ClkInitTypeDef structure that
*         contains the configuration information for the RCC peripheral.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetClock(RCC_ClkInitTypeDef  *RCC_ClkInit)
{
    uint32_t temp;
    
    if (RCC_ClkInit == NULL)
        return HAL_ERROR;
    
    HAL_RCC_GetSYSCLKSource(&RCC_ClkInit->SYSCLKSource);
    
    RCC_ClkInit->SYSCLKDiv0 = ((RCC->CCR2 & RCC_CCR2_SYSDIV0) >> RCC_CCR2_SYSDIV0_Pos) + 1;
    RCC_ClkInit->SYSCLKDiv1 = ((RCC->CCR2 & RCC_CCR2_SYSDIV1) >> RCC_CCR2_SYSDIV1_Pos) + 1;
    
    temp = (RCC->CCR2 & RCC_CCR2_PCLK1DIV) >> RCC_CCR2_PCLK1DIV_Pos;
    if (temp < 4)
        temp = 1;
    else
        temp = 1 << (temp - 3);
    RCC_ClkInit->PCLK1Div = temp;
    
    temp = (RCC->CCR2 & RCC_CCR2_PCLK2DIV) >> RCC_CCR2_PCLK2DIV_Pos;
    if (temp < 4)
        temp = 1;
    else
        temp = 1 << (temp - 3);
    RCC_ClkInit->PCLK2Div = temp;
    
    temp = (RCC->CCR2 & RCC_CCR2_PCLK3DIV) >> RCC_CCR2_PCLK3DIV_Pos;
    if (temp < 4)
        temp = 1;
    else
        temp = 1 << (temp - 3);
    RCC_ClkInit->PCLK3Div = temp;
    
    temp = (RCC->CCR2 & RCC_CCR2_PCLK4DIV) >> RCC_CCR2_PCLK4DIV_Pos;
    if (temp < 4)
        temp = 1;
    else
        temp = 1 << (temp - 3);
    RCC_ClkInit->PCLK4Div = temp;
    
    RCC_ClkInit->ClockType = RCC_CLOCK_TYPE_SYSCLK | RCC_CLOCK_TYPE_SYSDIV0 | RCC_CLOCK_TYPE_SYSDIV1 | \
                             RCC_CLOCK_TYPE_PCLK1 | RCC_CLOCK_TYPE_PCLK2 | RCC_CLOCK_TYPE_PCLK3 | RCC_CLOCK_TYPE_PCLK4;
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Returns the system clock source.
*@param : pClockSource: pointer to the system clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetSYSCLKSource(uint32_t *pClockSource)
{
    uint32_t source;
    
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    source = (RCC->CCR1 & RCC_CCR1_SYSCLKSEL) >> RCC_CCR1_SYSCLKSEL_Pos;
    
    if (source <= 1)
        source = RCC_SYSCLK_SOURCE_RCH;
    else if (source == 2)
        source = RCC_SYSCLK_SOURCE_XTH;
    else
        source = RCC_SYSCLK_SOURCE_PLL1PCLK;
    
    *pClockSource = source;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the HRNG slow clock frequency division.
*@param : pClockDiv: pointer to the HRNG slow clock frequency division.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetHRNGSlowClockDiv(uint32_t *pClockDiv)
{
    if (pClockDiv == NULL)
        return HAL_ERROR;
        
    *pClockDiv = ((RCC->CCR2 & RCC_CCR2_HRNGSDIV) >> RCC_CCR2_HRNGSDIV_Pos) + 1;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the FLT clock source.
*@param : pClockSource: pointer to the FLT clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetFLTClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
    
    *pClockSource = (RCC->CCR2 & RCC_CCR2_FLTCLKSEL) >> RCC_CCR2_FLTCLKSEL_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPUART1 clock source.
*@param : pClockSource: pointer to the LPUART1 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPUART1ClockSource(uint32_t *pClockSource)
{
    uint32_t source;
    
    if (pClockSource == NULL)
        return HAL_ERROR;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPUART1CKS) >> RCC_PERCFGR_LPUART1CKS_Pos;
    
    if (source <= 1)
    {
        *pClockSource = source;
        return HAL_OK;
    }
    
    if (source == 2)
    {
        source = (RCC->PERCFGR & RCC_PERCFGR_LPUART1DIV) >> RCC_PERCFGR_LPUART1DIV_Pos;
        *pClockSource = source + 2;
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

/******************************************************************************
*@brief : Returns the LPTIM1 clock source.
*@param : pClockSource: pointer to the LPTIM1 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM1ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM1CKS) >> RCC_PERCFGR_LPTIM1CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPTIM2 clock source.
*@param : pClockSource: pointer to the LPTIM2 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM2ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM2CKS) >> RCC_PERCFGR_LPTIM2CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPTIM3 clock source.
*@param : pClockSource: pointer to the LPTIM3 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM3ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPTIM4 clock source.
*@param : pClockSource: pointer to the LPTIM4 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM4ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPTIM5 clock source.
*@param : pClockSource: pointer to the LPTIM5 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM5ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the LPTIM6 clock source.
*@param : pClockSource: pointer to the LPTIM6 clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetLPTIM6ClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_LPTIM6CKS) >> RCC_PERCFGR_LPTIM6CKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the SDMMC clock source.
*@param : pClockSource: pointer to the SDMMC clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetSDMMCClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_SDMMCCKS) >> RCC_PERCFGR_SDMMCCKS_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the SDMMC sample clock source.
*@param : pClockSource: pointer to the SDMMC sample clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetSDMMCSampleClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    *pClockSource = (RCC->PERCFGR & RCC_PERCFGR_SDMMCSCKS) >> RCC_PERCFGR_SDMMCSCKS_Pos;

    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the RTC clock source.
*@param : pClockSource: pointer to the RTC clock source.
*@return: HAL status.
******************************************************************************/
HAL_StatusTypeDef HAL_RCC_GetRTCClockSource(uint32_t *pClockSource)
{
    if (pClockSource == NULL)
        return HAL_ERROR;
        
    if (RCC->STDBYCTRL & RCC_STDBYCTRL_RTCSEL_1)
        return HAL_ERROR;
    
    *pClockSource = (RCC->STDBYCTRL & RCC_STDBYCTRL_RTCSEL) >> RCC_STDBYCTRL_RTCSEL_Pos;
    
    return HAL_OK;
}

/******************************************************************************
*@brief : Returns the RCH TRIM frequency.
*@param : None.
*@return: RCH TRIM frequency.
******************************************************************************/
uint32_t HAL_RCC_GetRCHTrimFreq(void)
{
    uint32_t freq;
    
    HAL_EFUSE_ReadBytes(EFUSE1, 0x1C, (uint8_t *)&freq, 4, 100000);
    
    if ((freq > 76800000) || (freq < 51200000))
        return 64000000;
    else
        return freq;
}

/******************************************************************************
*@brief : Returns the RCL TRIM frequency.
*@param : None.
*@return: RCL TRIM frequency.
******************************************************************************/
uint32_t HAL_RCC_GetRCLTrimFreq(void)
{
    uint32_t freq;
    
    HAL_EFUSE_ReadBytes(EFUSE1, 0x20, (uint8_t *)&freq, 4, 100000);
    
    if ((freq > 39321) || (freq < 26214))
        return 32768;
    else
        return freq;
}

/******************************************************************************
*@brief : Returns the RCH frequency.
*@param : None.
*@return: RCH frequency.
******************************************************************************/
uint32_t HAL_RCC_GetRCHFreq(void)
{
    uint32_t freq;
    
    if ((RCC->RCHCR & (RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY)) != (RCC_RCHCR_RCHEN | RCC_RCHCR_RCHRDY))
        return (0);
    
    freq = HAL_RCC_GetRCHTrimFreq();
    
    if (RCC->RCHCR & RCC_RCHCR_RCHDIV)
        freq = freq >> 4;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the RCL frequency.
*@param : None.
*@return: RCL frequency.
******************************************************************************/
uint32_t HAL_RCC_GetRCLFreq(void)
{
    if ((RCC->STDBYCTRL & (RCC_STDBYCTRL_RCLEN | RCC_STDBYCTRL_RCLRDY)) != (RCC_STDBYCTRL_RCLEN | RCC_STDBYCTRL_RCLRDY))
        return (0);
    
    return (HAL_RCC_GetRCLTrimFreq());
}


/******************************************************************************
*@brief : Returns the XTH frequency.
*@param : None.
*@return: XTH frequency.
******************************************************************************/
uint32_t HAL_RCC_GetXTHFreq(void)
{
    if ((RCC->XTHCR & (RCC_XTHCR_XTHEN | RCC_XTHCR_XTHRDY)) == (RCC_XTHCR_XTHEN | RCC_XTHCR_XTHRDY))
        return (XTH_VALUE);
    else
        return 0;
}

/******************************************************************************
*@brief : Returns the XTL frequency.
*@param : None.
*@return: XTL frequency.
******************************************************************************/
uint32_t HAL_RCC_GetXTLFreq(void)
{
    if ((RCC->STDBYCTRL & (RCC_STDBYCTRL_XTLEN | RCC_STDBYCTRL_XTLRDY)) == (RCC_STDBYCTRL_XTLEN | RCC_STDBYCTRL_XTLRDY))
        return (32768U);
    else
        return (0);
}

/******************************************************************************
*@brief : Returns the PLL1 frequency.
*@param : None.
*@return: PLL1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL1Freq(void)
{
    uint32_t freq;
    uint32_t plln;
    uint32_t pllf;
    
    if (RCC->PLL1CR & RCC_PLL1CR_PLL1SRCSEL)
    {
        if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1LOCK | RCC_PLL1CR_PLL1PCLKEN)) != \
                                                  (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1LOCK | RCC_PLL1CR_PLL1PCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetXTHFreq();
    }
    else
    {
        if ((RCC->PLL1CR & (RCC_PLL1CR_PLL1SLEEP | RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1FREERUN | RCC_PLL1CR_PLL1PCLKEN)) != \
                                                  (RCC_PLL1CR_PLL1EN | RCC_PLL1CR_PLL1FREERUN | RCC_PLL1CR_PLL1PCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetRCHFreq();
    }
    
    if (freq == 0)
        return (0);
    
    plln = (RCC->PLL1CFR & RCC_PLL1CFR_PLL1N_Msk) >> RCC_PLL1CFR_PLL1N_Pos;
    if (plln == 0)
        plln = 1;
    
    pllf = (RCC->PLL1CFR & RCC_PLL1CFR_PLL1F_Msk) >> RCC_PLL1CFR_PLL1F_Pos;
    if (pllf < 50)
        pllf = 50;
    
    freq = (uint32_t)((uint64_t)freq * pllf / plln);
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL1PCLK frequency.
*@param : None.
*@return: PLL1PCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL1PCLKFreq(void)
{
    uint32_t freq;
    uint32_t pllp;
    
    freq = HAL_RCC_GetPLL1Freq();
    if (freq == 0)
        return (0);
    
    pllp = (RCC->PLL1CFR & RCC_PLL1CFR_PLL1P_Msk) >> RCC_PLL1CFR_PLL1P_Pos;
    pllp = (pllp + 1) << 1;
    
    freq = freq / pllp;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL2 frequency.
*@param : None.
*@return: PLL2 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL2Freq(void)
{
    uint32_t freq;
    uint32_t plln;
    uint32_t pllf;
    
    if (RCC->PLL2CR & RCC_PLL2CR_PLL2SRCSEL)
    {
        if ((RCC->PLL2CR & (RCC_PLL2CR_PLL2SLEEP | RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2LOCK | RCC_PLL2CR_PLL2PCLKEN)) != \
                                                  (RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2LOCK | RCC_PLL2CR_PLL2PCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetXTHFreq();
    }
    else
    {
        if ((RCC->PLL2CR & (RCC_PLL2CR_PLL2SLEEP | RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2FREERUN | RCC_PLL2CR_PLL2PCLKEN)) != \
                                                  (RCC_PLL2CR_PLL2EN | RCC_PLL2CR_PLL2FREERUN | RCC_PLL2CR_PLL2PCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetRCHFreq();
    }
    
    if (freq == 0)
        return (0);
    
    plln = (RCC->PLL2CFR & RCC_PLL2CFR_PLL2N_Msk) >> RCC_PLL2CFR_PLL2N_Pos;
    if (plln == 0)
        plln = 1;
    
    pllf = (RCC->PLL2CFR & RCC_PLL2CFR_PLL2F_Msk) >> RCC_PLL2CFR_PLL2F_Pos;
    if (pllf < 50)
        pllf = 50;
    
    freq = (uint32_t)((uint64_t)freq * pllf / plln);
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL2QCLK frequency.
*@param : None.
*@return: PLL2QCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL2QCLKFreq(void)
{
    uint32_t freq;
    uint32_t pllq;
    
    freq = HAL_RCC_GetPLL2Freq();
    if (freq == 0)
        return (0);
    
    pllq = (RCC->PLL2CFR & RCC_PLL2CFR_PLL2Q_Msk) >> RCC_PLL2CFR_PLL2Q_Pos;
    if (pllq == 0)
        pllq = 1;
    
    freq = freq / pllq;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL2PCLK frequency.
*@param : None.
*@return: PLL2PCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL2PCLKFreq(void)
{
    uint32_t freq;
    uint32_t pllp;
    
    freq = HAL_RCC_GetPLL2Freq();
    if (freq == 0)
        return (0);
    
    pllp = (RCC->PLL2CFR & RCC_PLL2CFR_PLL2P_Msk) >> RCC_PLL2CFR_PLL2P_Pos;
    pllp = (pllp + 1) << 1;
    
    freq = freq / pllp;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL3 frequency.
*@param : None.
*@return: PLL3 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL3Freq(void)
{
    uint32_t freq;
    uint32_t plln;
    uint32_t pllf;
    
    if (RCC->PLL3CR & RCC_PLL3CR_PLL3SRCSEL)
    {
        if ((RCC->PLL3CR & (RCC_PLL3CR_PLL3SLEEP | RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3LOCK | RCC_PLL3CR_PLL3QCLKEN)) != \
                                                  (RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3LOCK | RCC_PLL3CR_PLL3QCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetXTHFreq();
    }
    else
    {
        if ((RCC->PLL3CR & (RCC_PLL3CR_PLL3SLEEP | RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3FREERUN | RCC_PLL3CR_PLL3QCLKEN)) != \
                                                  (RCC_PLL3CR_PLL3EN | RCC_PLL3CR_PLL3FREERUN | RCC_PLL3CR_PLL3QCLKEN))
        {
            return (0);
        }
        freq = HAL_RCC_GetRCHFreq();
    }
    
    if (freq == 0)
        return (0);
    
    plln = ((RCC->PLL3CFR & RCC_PLL3CFR_PLL3N_Msk) >> RCC_PLL3CFR_PLL3N_Pos) + 1;
    
    pllf = ((RCC->PLL3CFR & RCC_PLL3CFR_PLL3F_Msk) >> RCC_PLL3CFR_PLL3F_Pos) + 1;
    
    freq = (uint32_t)((uint64_t)freq * pllf / plln);
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL3QCLK frequency.
*@param : None.
*@return: PLL3QCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL3QCLKFreq(void)
{
    uint32_t freq;
    uint32_t pllq;
    
    freq = HAL_RCC_GetPLL3Freq();
    if (freq == 0)
        return (0);
    
    pllq = (RCC->PLL3CFR & RCC_PLL3CFR_PLL3Q_Msk) >> RCC_PLL3CFR_PLL3Q_Pos;
    if (pllq == 0)
        pllq = 1;
    else if (pllq == 1)
        pllq = 2;
    else if (pllq == 2)
        pllq = 4;
    else
        pllq = 8;
    
    freq = freq / pllq;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the PLL3PCLK frequency.
*@param : None.
*@return: PLL3PCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPLL3PCLKFreq(void)
{
    uint32_t freq;
    uint32_t pllp;
    
    freq = HAL_RCC_GetPLL3Freq();
    if (freq == 0)
        return (0);
    
    pllp = (RCC->PLL3CFR & RCC_PLL3CFR_PLL3P_Msk) >> RCC_PLL3CFR_PLL3P_Pos;
    if (pllp == 0)
        pllp = 1;
    else if (pllp == 1)
        pllp = 2;
    else if (pllp == 2)
        pllp = 4;
    else
        pllp = 8;
    
    freq = freq / pllp;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the system clock frequency.
*@param : None.
*@return: system clock frequency.
******************************************************************************/
uint32_t HAL_RCC_GetSYSCLKFreq(void)
{
    uint32_t source;
    
    source = (RCC->CCR1 & RCC_CCR1_SYSCLKSEL) >> RCC_CCR1_SYSCLKSEL_Pos;
    
    if (source < 1)
        return HAL_RCC_GetRCHFreq();
    
    if (source == 2)
        return HAL_RCC_GetXTHFreq();
    
    return HAL_RCC_GetPLL1PCLKFreq();
}

/******************************************************************************
*@brief : Returns the system core clock frequency.
*@param : None.
*@return: system core clock frequency.
******************************************************************************/
uint32_t HAL_RCC_GetSysCoreClockFreq(void)
{
    uint32_t div;
    uint32_t freq = 0U;
    
    freq = HAL_RCC_GetSYSCLKFreq();
    
    /* first level frequency division of system clock */
    div = ((RCC->CCR2 & RCC_CCR2_SYSDIV0) >> RCC_CCR2_SYSDIV0_Pos) + 1;
    
    freq = freq / div;
    
    /* second level frequency division of system clock */
    div = ((RCC->CCR2 & RCC_CCR2_SYSDIV1) >> RCC_CCR2_SYSDIV1_Pos) + 1;
    
    freq = freq / div;
    
    SystemCoreClock = freq;
    
    return (freq);
}

/******************************************************************************
*@brief : Returns the FCLK frequency.
*@param : None.
*@return: FCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetFCLKFreq(void)
{
    return SystemCoreClock;
}

/******************************************************************************
*@brief : Returns the HCLK frequency.
*@param : None.
*@return: HCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return SystemCoreClock;
}

/******************************************************************************
*@brief : Returns the PCLK1 frequency.
*@param : None.
*@return: PCLK1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
    if ((RCC->CCR2 & RCC_CCR2_PCLK1DIV_2) == 0)
        return (SystemCoreClock);
    
    return (SystemCoreClock >> (((RCC->CCR2 & (RCC_CCR2_PCLK1DIV_0 | RCC_CCR2_PCLK1DIV_1)) >> RCC_CCR2_PCLK1DIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the PCLK2 frequency.
*@param : None.
*@return: PCLK1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPCLK2Freq(void)
{
    if ((RCC->CCR2 & RCC_CCR2_PCLK2DIV_2) == 0)
        return (SystemCoreClock);
    
    return (SystemCoreClock >> (((RCC->CCR2 & (RCC_CCR2_PCLK2DIV_0 | RCC_CCR2_PCLK2DIV_1)) >> RCC_CCR2_PCLK2DIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the PCLK3 frequency.
*@param : None.
*@return: PCLK1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPCLK3Freq(void)
{
    if ((RCC->CCR2 & RCC_CCR2_PCLK3DIV_2) == 0)
        return (SystemCoreClock);
    
    return (SystemCoreClock >> (((RCC->CCR2 & (RCC_CCR2_PCLK3DIV_0 | RCC_CCR2_PCLK3DIV_1)) >> RCC_CCR2_PCLK3DIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the PCLK4 frequency.
*@param : None.
*@return: PCLK1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetPCLK4Freq(void)
{
    if ((RCC->CCR2 & RCC_CCR2_PCLK4DIV_2) == 0)
        return (SystemCoreClock);
    
    return (SystemCoreClock >> (((RCC->CCR2 & (RCC_CCR2_PCLK4DIV_0 | RCC_CCR2_PCLK4DIV_1)) >> RCC_CCR2_PCLK4DIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the HRNG slow clock frequency.
*@param : None.
*@return: HRNG slow clock frequency.
******************************************************************************/
uint32_t HAL_RCC_GetHRNGSClockFreq(void)
{
    return (SystemCoreClock / (((RCC->CCR2 & RCC_CCR2_HRNGSDIV) >> RCC_CCR2_HRNGSDIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the FLTCLK (LVD and COMP) frequency.
*@param : None.
*@return: FLTCLK frequency.
******************************************************************************/
uint32_t HAL_RCC_GetFLTClockFreq(void)
{
    if ((RCC->CCR2 & RCC_CCR2_FLTCLKSEL) == 0)
        return (HAL_RCC_GetPCLK1Freq() >> 5);
    else
        return (HAL_RCC_GetRCLFreq());
}

/******************************************************************************
*@brief : Returns the LPUART1 clock frequency.
*@param : None.
*@return: LPUART1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPUART1ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPUART1CKS) >> RCC_PERCFGR_LPUART1CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 1)
        return (HAL_RCC_GetXTLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetPCLK1Freq() >> (((RCC->PERCFGR & RCC_PERCFGR_LPUART1DIV) >> RCC_PERCFGR_LPUART1DIV_Pos) + 2));
    
    return (0);
}

/******************************************************************************
*@brief : Returns the LPTIM1 clock frequency.
*@param : None.
*@return: LPTIM1 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM1ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM1CKS) >> RCC_PERCFGR_LPTIM1CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK1Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the LPTIM2 clock frequency.
*@param : None.
*@return: LPTIM2 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM2ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM2CKS) >> RCC_PERCFGR_LPTIM2CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK1Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the LPTIM3 clock frequency.
*@param : None.
*@return: LPTIM3 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM3ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK3Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the LPTIM4 clock frequency.
*@param : None.
*@return: LPTIM4 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM4ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK3Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the LPTIM5 clock frequency.
*@param : None.
*@return: LPTIM5 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM5ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM345CKS) >> RCC_PERCFGR_LPTIM345CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK3Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the LPTIM6 clock frequency.
*@param : None.
*@return: LPTIM6 frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLPTIM6ClockFreq(void)
{
    uint32_t source;
    
    source = (RCC->PERCFGR & RCC_PERCFGR_LPTIM6CKS) >> RCC_PERCFGR_LPTIM6CKS_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetPCLK3Freq());
    
    if (source == 1)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 2)
        return (HAL_RCC_GetRCHFreq());
    
    return (HAL_RCC_GetXTLFreq());
}

/******************************************************************************
*@brief : Returns the SDMMC clock frequency.
*@param : None.
*@return: SDMMC frequency.
******************************************************************************/
uint32_t HAL_RCC_GetSDMMCClockFreq(void)
{
    if ((RCC->PERCFGR & RCC_PERCFGR_SDMMCCKS) == 0)
        return (HAL_RCC_GetHCLKFreq());
    else
        return (HAL_RCC_GetPLL2PCLKFreq());
}

/******************************************************************************
*@brief : Returns the LCD piexl clock frequency.
*@param : None.
*@return: LCD piexl clock frequency.
******************************************************************************/
uint32_t HAL_RCC_GetLCDPiexlClockFreq(void)
{
    return (HAL_RCC_GetPLL2QCLKFreq() >> (((RCC->DCKCFG & RCC_DCKCFG_LCDDIV) >> RCC_DCKCFG_LCDDIV_Pos) + 1));
}

/******************************************************************************
*@brief : Returns the RTC clock frequency.
*@param : None.
*@return: RTC frequency.
******************************************************************************/
uint32_t HAL_RCC_GetRTCClockFreq(void)
{
    uint32_t source;
    
    if ((RCC->STDBYCTRL & RCC_STDBYCTRL_RTCEN) == 0)
        return (0);
    
    source = (RCC->STDBYCTRL & RCC_STDBYCTRL_RTCSEL) >> RCC_STDBYCTRL_RTCSEL_Pos;
    
    if (source == 0)
        return (HAL_RCC_GetRCLFreq());
    
    if (source == 1)
        return (HAL_RCC_GetXTLFreq());
    
    return (0);
}

/******************************************************************************
*@brief : Selects the clock source to output on MCO1 pin(PA8) or on MCO2 pin(PC9).
*@note    PA8/PC9 should be configured in alternate function mode.
*@param   MCO_InitStruct: pointer to an RCC_MCOInitTypeDef structure that contains 
*         the configuration information for the MCO output.
*@return: HAL status
******************************************************************************/
uint32_t HAL_RCC_MCOConfig(RCC_MCOInitTypeDef *MCO_InitStruct)
{
    volatile uint32_t temp;
    
    /* Check the parameters */
    assert_param(IS_RCC_MCO(MCO_InitStruct->MCO));
    assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO1));
    assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO2));
    
    temp = MCO_InitStruct->MCO;
    
    if (MCO_InitStruct->MCO1)
    {
        assert_param(IS_RCC_MCO1_DIV(MCO_InitStruct->MCO1Div));
        assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO1RevPol));
        
        /* MCO1 clock polarity configuration */
        if (MCO_InitStruct->MCO1RevPol)
            temp |= RCC_CLKOCR_MCO1POL;
        
        /* MCO1 division configuration */
        temp |= RCC_CLKOCR_MCO1SEL | RCC_CLKOCR_MCO1EN | ((MCO_InitStruct->MCO1Div - 1) << RCC_CLKOCR_MCO1DIV_Pos);
    }
    else
    {
        assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO1RevPol));
        
        /* MCO1 clock polarity configuration */
        if (MCO_InitStruct->MCO1RevPol)
            temp |= RCC_CLKOCR_MCO1POL;
    }
    
    if (MCO_InitStruct->MCO2)
    {
        assert_param(IS_RCC_MCO2_DIV(MCO_InitStruct->MCO2Div));
        assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO2RevPol));
        
        /* MCO1 clock polarity configuration */
        if (MCO_InitStruct->MCO2RevPol != DISABLE)
            temp |= RCC_CLKOCR_MCO2POL;
        
        /* MCO1 division configuration */
        temp |= RCC_CLKOCR_MCO2EN | (((MCO_InitStruct->MCO2Div - 1) << RCC_CLKOCR_MCO2DIV_Pos) & RCC_CLKOCR_MCO2DIV);
    }
    else
    {
        assert_param(IS_FUNCTIONAL_STATE(MCO_InitStruct->MCO2RevPol));
        
        /* MCO1 clock polarity configuration */
        if (MCO_InitStruct->MCO2RevPol != DISABLE)
            temp |= RCC_CLKOCR_MCO2POL;
    }
    
    RCC->CLKOCR = temp;
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Selects the clock source to output on MCO1 pin(PA8).
*@note    PA8 should be configured in alternate function mode.
*@param   MCO: specifies the output direction for the clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_MCO_HCLK: HCLK selected as MCO1 source.
*             @arg RCC_MCO_RCH: RCH selected as MCO1 source.
*             @arg RCC_MCO_RCL: RCL selected as MCO1 source.
*             @arg RCC_MCO_XTH: XTH selected as MCO1 source.
*             @arg RCC_MCO_XTL: XTL selected as MCO1 source.
*             @arg RCC_MCO_PLL1PCLK: PLL1PCLK selected as MCO1 source.
*             @arg RCC_MCO_PLL2PCLK: PLL2PCLK selected as MCO1 source.
*             @arg RCC_MCO_PLL2QCLK: PLL2QCLK selected as MCO1 source.
*             @arg RCC_MCO_PLL3PCLK: PLL3PCLK selected as MCO1 source.
*             @arg RCC_MCO_PLL3QCLK: PLL3QCLK selected as MCO1 source.
*             @arg RCC_MCO_SYSCLK: SYSCLK selected as MCO1 source.
*             @arg RCC_MCO_LPUART1_CLK: LPUART1 clock selected as MCO1 source.
*             @arg RCC_MCO_FCLK_DIV8: FCLK_DIV8 selected as MCO1 source.
*             @arg RCC_MCO_USB1_48M: USB1_48M selected as MCO1 source.
*             @arg RCC_MCO_USB2_48M: USB2_48M selected as MCO1 source.
*             @arg RCC_MCO_RTC_PCLK: RTC_PCLK selected as MCO1 source.
*             @arg RCC_MCO_SDMMC_SAMPLE_CLK: SDMMC_SAMPLE_CLK selected as MCO1 source.
*             @arg RCC_MCO_SDMMC_DRIVE_CLK: SDMMC_DRIVE_CLK selected as MCO1 source.
*@param   NewStatus: specifies the new state of the MCO1.
*             This parameter can be: ENABLE or DISABLE.
*@param : Div: the frequency division of the MCO1.
*             This parameter must be a number between 1 and 65536.
*@return: HAL status
******************************************************************************/
uint32_t HAL_RCC_MCO1Config(uint32_t MCO, uint32_t NewStatus, uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewStatus));
    
    if (NewStatus == DISABLE)
    {
        RCC->CLKOCR = (RCC->CLKOCR & ~RCC_CLKOCR_MCO1EN) | RCC_CLKOCR_MCO1SEL;
        return HAL_OK;
    }
    
    assert_param(IS_RCC_MCO(MCO));
    assert_param(IS_RCC_MCO1_DIV(Div));
    
    RCC->CLKOCR = (RCC->CLKOCR & ~(RCC_CLKOCR_MCO1DIV | RCC_CLKOCR_MCOCLKS)) | \
                   RCC_CLKOCR_MCO1EN | RCC_CLKOCR_MCO1SEL | \
                  ((Div - 1) << RCC_CLKOCR_MCO1DIV_Pos) | \
                  (MCO << RCC_CLKOCR_MCOCLKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Selects the clock source to output on MCO2 pin(PC9).
*@note    PC9 should be configured in alternate function mode.
*@param   MCO: specifies the output direction for the clock source. 
*             This parameter can be one of the following values: 
*             @arg RCC_MCO_HCLK: HCLK selected as MCO2 source.
*             @arg RCC_MCO_RCH: RCH selected as MCO2 source.
*             @arg RCC_MCO_RCL: RCL selected as MCO2 source.
*             @arg RCC_MCO_XTH: XTH selected as MCO2 source.
*             @arg RCC_MCO_XTL: XTL selected as MCO2 source.
*             @arg RCC_MCO_PLL1PCLK: PLL1PCLK selected as MCO2 source.
*             @arg RCC_MCO_PLL2PCLK: PLL2PCLK selected as MCO2 source.
*             @arg RCC_MCO_PLL2QCLK: PLL2QCLK selected as MCO2 source.
*             @arg RCC_MCO_PLL3PCLK: PLL3PCLK selected as MCO2 source.
*             @arg RCC_MCO_PLL3QCLK: PLL3QCLK selected as MCO2 source.
*             @arg RCC_MCO_SYSCLK: SYSCLK selected as MCO2 source.
*             @arg RCC_MCO_LPUART1_CLK: LPUART1 clock selected as MCO2 source.
*             @arg RCC_MCO_FCLK_DIV8: FCLK_DIV8 selected as MCO2 source.
*             @arg RCC_MCO_USB1_48M: USB1_48M selected as MCO2 source.
*             @arg RCC_MCO_USB2_48M: USB2_48M selected as MCO2 source.
*             @arg RCC_MCO_RTC_PCLK: RTC_PCLK selected as MCO2 source.
*             @arg RCC_MCO_SDMMC_SAMPLE_CLK: SDMMC_SAMPLE_CLK selected as MCO2 source.
*             @arg RCC_MCO_SDMMC_DRIVE_CLK: SDMMC_DRIVE_CLK selected as MCO2 source.
*@param   NewStatus: specifies the new state of the MCO2.
*             This parameter can be: ENABLE or DISABLE.
*@param : Div: the frequency division of the MCO2.
*             This parameter must be a number between 1 and 64.
*@return: HAL status
******************************************************************************/
uint32_t HAL_RCC_MCO2Config(uint32_t MCO, uint32_t NewStatus, uint32_t Div)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewStatus));
    
    if (DISABLE == NewStatus)
    {
        RCC->CLKOCR = RCC->CLKOCR & ~RCC_CLKOCR_MCO2EN;
        return HAL_OK;
    }
    
    assert_param(IS_RCC_MCO(MCO));
    assert_param(IS_RCC_MCO2_DIV(Div));
    
    RCC->CLKOCR = (RCC->CLKOCR & ~(RCC_CLKOCR_MCO2DIV | RCC_CLKOCR_MCOCLKS)) | RCC_CLKOCR_MCO2EN | \
                  ((Div - 1) << RCC_CLKOCR_MCO2DIV_Pos) | (MCO << RCC_CLKOCR_MCOCLKS_Pos);
    
    return (HAL_OK);
}

/******************************************************************************
*@brief : Software reset
*@return: None
******************************************************************************/
void HAL_RCC_SoftwareReset(void)
{
    RCC->RCR &= ~RCC_RCR_SRST_MAP;
}

/******************************************************************************
*@brief : Standby power domain reset
*@return: None
******************************************************************************/
void HAL_RCC_StandbyReset(void)
{
    RCC->STDBYCTRL &= ~RCC_STDBYCTRL_STDBYRST;
}

/******************************************************************************
*@brief : Specify whether LOCKUP will cause the system to reset
*@param   NewStatus: specifies the new state of the system reset.
*             This parameter can be: ENABLE or DISABLE.
*@return: None
******************************************************************************/
void HAL_RCC_LockupResetConfig(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState)
        RCC->RCR |= RCC_RCR_LOCKRST_EN;
    else
        RCC->RCR &= ~RCC_RCR_LOCKRST_EN;
}

/******************************************************************************
*@brief : Specify whether IWDT will cause the system to reset
*@param   NewStatus: specifies the new state of the system reset.
*             This parameter can be: ENABLE or DISABLE.
*@return: None
******************************************************************************/
void HAL_RCC_IWDTResetConfig(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState)
        RCC->RCR |= RCC_RCR_IWDTRST_EN;
    else
        RCC->RCR = (RCC->RCR & ~(RCC_RCR_IWDTRST_DIS | RCC_RCR_IWDTRST_EN)) | (0xB5UL << RCC_RCR_IWDTRST_DIS_Pos);
}

/******************************************************************************
*@brief : Specify whether WDT will cause the system to reset
*@param   NewStatus: specifies the new state of the system reset.
*             This parameter can be: ENABLE or DISABLE.
*@return: None
******************************************************************************/
void HAL_RCC_WDTResetConfig(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState)
        RCC->RCR |= RCC_RCR_WDTRST_EN;
    else
        RCC->RCR &= ~RCC_RCR_WDTRST_EN;
}

/******************************************************************************
*@brief : Specify whether LVD will cause the system to reset
*@param   NewStatus: specifies the new state of the system reset.
*             This parameter can be: ENABLE or DISABLE.
*@return: None
******************************************************************************/
void HAL_RCC_LVDResetConfig(uint32_t NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    
    if (NewState)
        RCC->RCR |= RCC_RCR_LVDRST_EN;
    else
        RCC->RCR &= ~RCC_RCR_LVDRST_EN;
}

/******************************************************************************
*@brief : Returns the system reset source.
*@return: reset source
******************************************************************************/
uint32_t HAL_RCC_GetResetSource(void)
{
    return (RCC->RSR & RCC_RESET_SOURCE_MASK);
}

/******************************************************************************
*@brief : Clear all reset source flag.
*@return: None
******************************************************************************/
void HAL_RCC_ClearAllResetSource(void)
{
    RCC->RSR = RCC_RSR_RSTFLAGCLR;
}

#endif /* HAL_RCC_MODULE_ENABLED */
