
#include "app.h"  

/******************************************************************************
*@brief : LVD interrupt callback function
*@param : none
*@return: none
******************************************************************************/
void LVD_IRQCallback(void)
{
    printfS("LVD IRQ, EXTI->PDR = 0x%08x\r\n",EXTI->PDR1);
    g_extiLine = 16;
    EXTI->PDR1 |= EXTI_LINE_LVD;
}

/******************************************************************************
*@brief : LVD test
*@param : none
*@return: none
******************************************************************************/
void PMU_LVD_test(void)
{
    printfS("PMU LVD test\r\n");
    uint8_t temp = 0, cnt = 0;
    uint32_t lvd;
    HAL_PMU_LvdEnable(PMU_LVD_1V71, PMU_LVD_FILTER_16, PMU_CTRL1_LVDFLTEN);
    HAL_SimpleDelay(100);
    RCC->RCR &= ~RCC_RCR_LVDRST_EN;
    __HAL_RCC_EXTI_CLK_ENABLE();
    HAL_EXTI_SetConfigLine(NULL, EXTI_LINE_LVD, EXTI_MODE_IT_RISING);
    NVIC_ClearPendingIRQ(LVD_IRQn);
    NVIC_SetPriority(LVD_IRQn, 5);
    NVIC_EnableIRQ(LVD_IRQn);
    /* ¿ªÆôRC32K */
    HAL_RCC_RCLConfig(ENABLE);
    //LVDÂË²¨Ê±ÖÓÑ¡ÔñRC32K
    RCC->CCR2 |= RCC_CCR2_FLTCLKSEL;
    
    while(1)
    {
        if(cnt++ > 10)
        {
            cnt = 0;
            lvd = (PMU->CTRL1 >> PMU_CTRL1_LVDSEL_Pos) & 0xf;
            switch(lvd)
            {
                case 0:
                    printfS("LVD threshold voltage selection 1.71V\r\n");    
                break;
                case 1:
                    printfS("LVD threshold voltage selection 2.01V\r\n");
                break;  
                case 2:
                    printfS("LVD threshold voltage selection 2.23V\r\n");
                break;  
                case 3:
                    printfS("LVD threshold voltage selection 2.43V\r\n");
                break;  
                case 4:
                    printfS("LVD threshold voltage selection 2.51V\r\n");
                break;  
                case 5:
                    printfS("LVD threshold voltage selection 2.73V\r\n");
                break;  
                case 6:
                    printfS("LVD threshold voltage selection 2.80V\r\n");
                break;  
                case 7:
                    printfS("LVD threshold voltage selection 2.90V\r\n");
                break;  
                default:
                    printfS("Reserve\r\n");
                break;              
            }
            if(temp == 8)
            {
                printfS("Enable LVD Reset, 2.73V\r\n");
                temp = 0;
                PMU->CTRL1 = (PMU->CTRL1 & (~PMU_CTRL1_LVDSEL_Msk)) | (5 << PMU_CTRL1_LVDSEL_Pos);
                HAL_SimpleDelay(100);
                RCC->RCR |= RCC_RCR_LVDRST_EN;
            }
            printfS("MCU enter Stop mode \r\n"); 

            __set_PRIMASK(1);    // disable interrupt 
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick  
            SCB->ICSR = BIT25;   // clear systick pending bit 
            
            HAL_PMU_EnterStop(WAIT_FOR_INT); 

            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick            
            __set_PRIMASK(0);    // enable interrupt  
            
            printfS("MCU exti Stop! Wakeup by EXTI_LINE_%d\r\n", g_extiLine);
            g_extiLine = 0;
            temp++;
            if(temp < 8)
            {    
                PMU->CTRL1 = (PMU->CTRL1 & (~PMU_CTRL1_LVDSEL_Msk)) | (temp << PMU_CTRL1_LVDSEL_Pos);
            }

        }
        else
        {
            printfS("[%d]: Systick = %d\r\n", cnt, HAL_GetTick());   
        }
        HAL_Delay(500);
    }  
}

