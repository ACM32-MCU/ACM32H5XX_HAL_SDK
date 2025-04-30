
#include "app.h" 
#include "lowpower.h"


/******************************************************************************
*@brief : stop test
*@param : none
*@return: none
******************************************************************************/
void PMU_Stop_test(void)
{
    GPIO_InitTypeDef GPIO_PINx_Handle;
    uint8_t cnt;
    uint8_t temp; 
    
    printfS("EXTI_LINE13(PC13) interrupt rising edge wake-up stop\r\n");
    
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



    __HAL_RCC_USB2_CLK_ENABLE();
    __HAL_RCC_USB1_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    USBOTG1_PHYC->TR0 |= BIT3;
    USBOTG2_PHYC->TR0 |= BIT3;

    cnt = 0;
    while(1)
    {
        if(cnt++ > 6)
        {
            cnt = 0;
            printfS("MCU enter Stop mode \r\n"); 

            Enter_Stop_RunInSram(WAIT_FOR_INT); 
            
            SystemClock_Config(SYSCLK_220M_SRC_RCH, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);

            printfS("MCU exti Stop! Wakeup by EXTI_LINE_%d\r\n", g_extiLine);
            g_extiLine = 0;
        }
        printfS("[%d] \r\n", cnt);
        HAL_Delay(500);
        
    }   
}


