

#include "app.h"  




/******************************************************************************
*@brief : sleep test
*@param : none
*@return: none
******************************************************************************/
void PMU_Sleep_test(void)
{
    GPIO_InitTypeDef GPIO_PINx_Handle;
    
    printfS("EXTI_LINE13(PC13) interrupt rising edge wake-up sleep\r\n");
    
    
    __HAL_RCC_EXTI_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();	
    /* Initialization GPIO */
    GPIO_PINx_Handle.Pin       = GPIO_PIN_13;
    GPIO_PINx_Handle.Mode      = GPIO_MODE_INPUT;
    GPIO_PINx_Handle.Pull      = GPIO_NOPULL;
    GPIO_PINx_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(GPIOC, &GPIO_PINx_Handle);

    /* Config EXTI */
    HAL_EXTI_SetConfigLine((GPIO_TypeDef *)GPIOC, EXTI_LINE_13, EXTI_MODE_IT_RISING);
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 5);
    NVIC_EnableIRQ(EXTI15_10_IRQn); 

    while(1)
    {
        HAL_Delay(2500);
        printfS("MCU enter Sleep mode \r\n");   
        
        __set_PRIMASK(1);    // disable interrupt 
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick 
        SCB->ICSR = BIT25;   // clear systick pending bit
        
        HAL_PMU_EnterSleep(WAIT_FOR_INT);
        
        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick              
        __set_PRIMASK(0);    // enable interrupt 
        
        printfS("MCU exti Sleep! Wakeup by EXTI_LINE_%d\r\n", g_extiLine);

        g_extiLine = 0;
        
    }
}



