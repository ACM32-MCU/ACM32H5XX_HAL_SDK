

#include "hal.h"  

static IWDT_HandleTypeDef IWDT_Handle;

/******************************************************************************
*@brief : IWDT reset initialization 
*@param : none
*@return: none
******************************************************************************/
void IWDT_ResetInit(void)
{
    /* 使能：RC32K */
    HAL_RCC_RCLConfig(ENABLE);

    /* 使能：IWDT复位导致系统复位 */
    HAL_RCC_IWDTResetConfig(ENABLE);

    /* 配置IWDT */
    /* IWDT Reset Time = (Prescaler/32K * Reload) */
    IWDT_Handle.Instance = IWDT;
    IWDT_Handle.Init.Prescaler  = IWDT_CLOCK_PRESCALER_32;
    IWDT_Handle.Init.Reload     = 4000;
    IWDT_Handle.Init.Window     = 4000;//Window < Reload时，窗口功能才会开启
    IWDT_Handle.Init.Wakeup     = 4000;//Wakeup < Reload时，唤醒功能才会工作

    HAL_IWDT_Init(&IWDT_Handle);
    HAL_IWDT_Refresh(&IWDT_Handle);
}


/******************************************************************************
*@brief : IWDT reset test 
*@param : none
*@return: none
******************************************************************************/
void IWDT_Reset_Test(void)
{
    uint8_t cnt;
    printfS("IWDT Reset test\r\n");

    IWDT_ResetInit();
    
    cnt = 0;
    while(1)
    {
        
        if(cnt++ < 20)
        {
            printfS("[%d]feed iwdt time: %u\r\n", cnt, HAL_GetTick()); 
            HAL_IWDT_Refresh(&IWDT_Handle);
        }
        else
        {
            printfS("[%d]: %u\r\n", cnt, HAL_GetTick());            
        }
        HAL_DelayMs(1000);
    }    
}


/******************************************************************************
*@brief : IWDT wakeup standby test 
*@param : none
*@return: none
******************************************************************************/
void IWDT_Reset_Wakeup_Standby(void)
{
    printfS("IWDT Reset wakeup standby test\r\n");

    HAL_PMU_Init();
    if((PMU->SR & PMU_FLAG_IWDT)&&(PMU->SR & PMU_FLAG_STANDBY))
    {
        printfS("MCU wakeup by IWDT\r\n");            
    }
    
    HAL_DelayMs(2000);
    
    IWDT_ResetInit();
    
    printfS("MCU enter Standby mode \r\n");        
    /* disable systick interrupt */ 
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; 
    /* disable interrupt */
    __set_PRIMASK(1);    
    /* clear systick pending bit */ 
    SCB->ICSR = BIT25;
    
    HAL_PMU_EnterStandbyMode(WAIT_FOR_INT); 

    /* enable systick interrupt*/
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; 
    /* enable interrupt */        
    __set_PRIMASK(0);              
    printfS("MCU never run here \r\n");
   
}

