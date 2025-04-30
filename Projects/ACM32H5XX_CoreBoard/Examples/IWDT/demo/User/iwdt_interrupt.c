
#include "hal.h"  

static IWDT_HandleTypeDef IWDT_Handle;
static volatile uint32_t g_iwdtIRQFlag = 0;


void IWDT_WKUP_IRQHandler(void)
{
    HAL_IWDT_IRQHandler(&IWDT_Handle);
}


/******************************************************************************
*@brief : IWDT wakeup interrupt callback function 
*@param : none
*@return: none
******************************************************************************/
void HAL_IWDT_Callback(IWDT_HandleTypeDef *hiwdt)
{
    /* 到达唤醒值后进入中断*/
    HAL_IWDT_Refresh(&IWDT_Handle);
    g_iwdtIRQFlag = EXTI->PDR1;
    EXTI->PDR1 = IWDT_EXTI_LINE;
    NVIC_ClearPendingIRQ(IWDT_WKUP_IRQn);  
}


/******************************************************************************
*@brief : IWDT wake-up initialization 
*@param : none
*@return: none
******************************************************************************/
void IWDT_WakeupInit(void)
{  
    /* 使能：RC32K */
    HAL_RCC_RCLConfig(ENABLE);

    /* 使能：IWDT复位导致系统复位 */
    HAL_RCC_IWDTResetConfig(ENABLE); 

    __HAL_RCC_EXTI_CLK_ENABLE();

    /* 配置IWDT */
    /* IWDT wake-up Time = (Prescaler/32K * Wakeup) */
    IWDT_Handle.Instance = IWDT;
    IWDT_Handle.Init.Prescaler  = IWDT_CLOCK_PRESCALER_32;
    IWDT_Handle.Init.Reload     = 4000;
    IWDT_Handle.Init.Window     = 4000;//Window < Reload时，窗口功能才会开启
    IWDT_Handle.Init.Wakeup     = 2000;//Wakeup < Reload时，唤醒功能才会工作
    IWDT_Handle.Init.WakeupMode = IWDT_WAKEUP_MODE_IT;

    HAL_IWDT_Init(&IWDT_Handle);

    HAL_EXTI_SetConfigLine(NULL, IWDT_EXTI_LINE, EXTI_MODE_IT_RISING);

    /* 使能IWDT中断 */
    NVIC_ClearPendingIRQ(IWDT_WKUP_IRQn);
    NVIC_SetPriority(IWDT_WKUP_IRQn, 5);
    NVIC_EnableIRQ(IWDT_WKUP_IRQn);

    HAL_IWDT_Refresh(&IWDT_Handle);
    
}

/******************************************************************************
*@brief : IWDT wake-up test 
*@param : none
*@return: none
******************************************************************************/
void IWDT_Wakeup_Test(void)
{
    uint32_t tick;
    printfS("IWDT Wakeup test\r\n");
    HAL_DelayMs(3000);

    IWDT_WakeupInit();//wakeup中断唤醒
    HAL_PMU_Init();
    
    while(1)
    {
        printfS("mcu enter stop mode!\r\n"); 
        
        HAL_EXTI_ClearAllPending();
        
        __set_PRIMASK(1);    // disable interrupt
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    // disable systick interrupt
        SCB->ICSR = BIT25;   // clear systick pending bit  
        
        HAL_PMU_EnterStop(WAIT_FOR_INT);  
        
        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;//enable systick interrupt            
        __set_PRIMASK(0);    // enable interrupt   
        
        printfS("MCU exti stop, wakeup by EXTI(0x%x) \r\n", g_iwdtIRQFlag);
        
        HAL_DelayMs(1000);
        
        HAL_IWDT_Refresh(&IWDT_Handle);
        
    } 
}




