
#include "app.h"
#include "rtc.h"
#include "lowpower.h"

extern uint32_t Get_Wakeup_Source(void);

/******************************************************************************
*@brief : BOR test
*@param : none
*@return: none
******************************************************************************/
void PMU_BOR_test(void)
{
    uint8_t cnt = 0;
    uint32_t bor;
    printfS("PMU BOR test\r\n");
    
    Get_Wakeup_Source();

    HAL_PMU_BorResetEnable(PMU_BOR_2V77_2V90);
    HAL_SimpleDelay(100); 
    bor = (PMU->CTRL2 >> PMU_CTRL2_BORCFG_Pos) & 0x3;
    switch(bor)
    {
        case 0:
            printfS("BOR threshold voltage selection 2.0V/2.1V\r\n");    
        break;
        case 1:
            printfS("BOR threshold voltage selection 2.2V/2.3V\r\n");
        break;  
        case 2:
            printfS("BOR threshold voltage selection 2.49V/2.61V\r\n");
        break;  
        case 3:
            printfS("BOR threshold voltage selection 2.77V/2.90V\r\n");
        break;   
        default:
            printfS("Reserve\r\n");
        break;              
    }                
    while(1)
    {      

        if(cnt++ > 10)
        {
            cnt = 0;
 
            printfS("MCU enter Standby mode \r\n");
            
            __set_PRIMASK(1);    // disable interrupt
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;     // disable systick 
            SCB->ICSR = BIT25;   // clear systick pending bit  
            
            Enter_Standby_RunInSram(WAIT_FOR_INT);            
            
            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // enable systick            
            __set_PRIMASK(0);    // enable interrupt    
            
            printfS("MCU never run here \r\n");
        }
        else
        {
            printfS("[%d]: Systick = %d\r\n", cnt, HAL_GetTick());   
        }        
        HAL_DelayMs(500);        
    }    
}

