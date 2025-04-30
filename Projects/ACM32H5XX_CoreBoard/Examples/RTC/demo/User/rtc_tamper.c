

#include "app.h"  


/******************************************************************************
*@brief : RTC temper initialization
*@param : adge :trigger edge
*@param : clearBkp :Clear backup register
*@return: none
******************************************************************************/
void RTC_Tamper_Init(uint32_t adge, uint32_t clearBkp)
{
    GPIO_InitTypeDef GPIO_Handle;
    RTC_TemperTypeDef temp_Temper;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    //PI8--RTC_TAMP2, PC13--RTC_TAMP1
    /* Initialization PI8 */
    GPIO_Handle.Pin       = GPIO_PIN_8;   
    GPIO_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_Handle.Pull      = GPIO_PULLUP;
    GPIO_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(GPIOI, &GPIO_Handle);
    /* Initialization PC13 */
    GPIO_Handle.Pin       = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_Handle);
        
    temp_Temper.u32_TemperEdge  = adge;
    temp_Temper.u32_InterruptEN = RTC_TEMP_INT_ENABLE;
    temp_Temper.u32_ClearBackup = clearBkp;
    temp_Temper.u32_FilterClk   = RTC_TEMP_FILTER_512_RTCCLK;
    temp_Temper.u32_Filter      = RTC_TEMP_FILTER_4_RTCCLK;
    temp_Temper.u32_FilterEn    = DISABLE;
    HAL_RTC_Tamper(RTC_TEMPER_1, &temp_Temper);
    HAL_RTC_Tamper(RTC_TEMPER_2, &temp_Temper);
    
    NVIC_ClearPendingIRQ(RTC_XTLSD_IRQn);
    NVIC_EnableIRQ(RTC_XTLSD_IRQn);
}



/******************************************************************************
*@brief : RTC tamper test
*@param : none
*@return: none
******************************************************************************/
void RTC_Tamper_Test(void)
{
    RTC_ConfigTypeDef RTC_Handle;
    uint32_t clktampMsk = (RTC_CLKSTAMP1_HRSTP1_Msk | RTC_CLKSTAMP1_MINSTP1_Msk | RTC_CLKSTAMP1_SECSTP1_Msk); 
    uint32_t caltampMsk = (RTC_CALSTAMP1_YEARSTP1_Msk | RTC_CALSTAMP1_MONSTP1_Msk | RTC_CALSTAMP1_WKSTP1_Msk | RTC_CALSTAMP1_DAYSTP1_Msk); 
    printfS("RTC Tamper test\r\n");    
    gRTCIrqFlag = 0;
    

    RTC_Handle.ClockSource       = RTC_CLOCK_RC32K;
    RTC_Handle.Compensation      = COMPENSATION_INCREASE;    // 开始时钟补偿，如不需补偿设置补偿值为0
    RTC_Handle.CompensationValue = 0x00;                     // 开始时钟补偿，如不需补偿设置补偿值为0
    HAL_RTC_Config(&RTC_Handle);    
    
    RTC_SetDateTime();
    printfS("Tamper1 & 2 Falling edge trigger\r\n");
    RTC_Tamper_Init(RTC_TEMP_EDGE_FALLING, RTC_TEMP_CLEAR_ENABLE);
    HAL_RTC_WriteBackupRegister(RTC_BKP_DR0, 0xAAAA5555);
    HAL_RTC_WriteBackupRegister(RTC_BKP_DR5, 0x5555AAAA);
    HAL_RTC_WriteBackupRegister(RTC_BKP_DR10, 0x3333CCCC);
    HAL_RTC_WriteBackupRegister(RTC_BKP_DR15, 0xCCCC3333);
    while (1) 
    {
        RTC_GetDateTime();
        if((gRTCIrqFlag & RTC_SR_STP1FIF) == RTC_SR_STP1FIF) 
        {
            gRTCIrqFlag = 0; 
            printf("Tamper1 interrupt has been triggered!!! \r\n");                    
            printf("RTC CLKSTEMP1 Date: %08x, Time: %08x\r\n", (RTC->CALSTAMP1&caltampMsk), (RTC->CLKSTAMP1&clktampMsk));
            printf("backup0 = %08x, backup10 = %08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR0),HAL_RTC_ReadBackupRegister(RTC_BKP_DR10));
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR0, 0xAAAA5555);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR5, 0x5555AAAA);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR10, 0x3333CCCC);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR15, 0xCCCC3333);
            
        }
        if((gRTCIrqFlag & RTC_SR_STP2FIF) == RTC_SR_STP2FIF)  
        {
            gRTCIrqFlag = 0;
            printf("Tamper2 interrupt has been triggered!!! \r\n"); 
            printf("RTC CLKSTEMP2 Date: %08x, Time: %08x\r\n", (RTC->CALSTAMP2&caltampMsk), (RTC->CLKSTAMP2&clktampMsk));
            printf("backup5 = %08x, backup15 = %08x\r\n", HAL_RTC_ReadBackupRegister(RTC_BKP_DR5),HAL_RTC_ReadBackupRegister(RTC_BKP_DR15));
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR0, 0xAAAA5555);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR5, 0x5555AAAA);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR10, 0x3333CCCC);
            HAL_RTC_WriteBackupRegister(RTC_BKP_DR15, 0xCCCC3333);
        } 
    }
        

}


