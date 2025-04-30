
#include "hal.h"
#include "app.h"

/******************************************************************************
*@brief : ANA_OUT gpio initialization 
*@param : none
*@return: none
******************************************************************************/
void ANAOUT_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_PINx_Handle;   
    __HAL_RCC_GPIOE_CLK_ENABLE(); 
    /* PE2 ------> ANA_OUT */
    GPIO_PINx_Handle.Pin       = GPIO_PIN_2;
    GPIO_PINx_Handle.Mode      = GPIO_MODE_ANALOG_SWITCH_ON;
    GPIO_PINx_Handle.Pull      = GPIO_NOPULL;
    GPIO_PINx_Handle.Drive     = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(GPIOE, &GPIO_PINx_Handle);

}

void PMU_ANAOUT_Test(void)
{
    uint8_t temp = 0;
    GPIO_InitTypeDef GPIO_PINx_Handle;
    printfS("PMU ANAOUT test\r\n");
    
    BSP_PB_Init();

    ANAOUT_GPIO_Init();
    
    __HAL_RCC_PMU_CLK_ENABLE();

    /* 模拟TEST_ANATEST_SR信号选择浮空 */
    PMU->TEST_ANATEST_SR = temp;
    printfS("Please press the button \r\n"); 
    while(1)
    {
        if(g_extiLine)
        {
            HAL_EXTI_ClearConfigLine(EXTI_LINE_0);
            HAL_EXTI_ClearAllPending(); 
            g_extiLine = 0;
            PMU->TEST_ANATEST_SR = temp; 
            switch(PMU->TEST_ANATEST_SR)
            {
                case 0:
                    printfS("ANA_OUT: 0-->Floating\r\n");    
                break;
                case 1:
                    __HAL_RCC_ADC12_CLK_ENABLE();
                    /* VREFINT输出1.2V电压 */
                    ADC12_COMMON->CCR |= ADC_CCR_VREFINTEN;
                    printfS("ANA_OUT: 1-->VREFINT\r\n");
                break;                
                case 2:
                    printfS("ANA_OUT: 2-->VDD12_RET(LPLDO12_TRIM)\r\n");
                break;                
                case 3:
                    printfS("ANA_OUT: 3-->VDD12(LDO12_TRIM)\r\n");
                break;                
                case 4:
                    printfS("ANA_OUT: 4-->VREF1V\r\n");
                break;                
                case 5:
                    printfS("ANA_OUT: 5-->VDD25\r\n");
                break;
                case 6:
                    PMU->TEST_LDOCR |= PMU_TEST_LDOCR_LDO_TESTEN;
                    //PMU->TEST_LDOCAL &= (~(PMU_TEST_LDOCAL_LPLDO12TRIM_Msk)); 
                    printfS("ANA_OUT: 6-->VRF_LP\r\n");
                break;
                case 7:
                    PMU->TEST_LDOCR |= PMU_TEST_LDOCR_LDO_TESTEN;
                    //PMU->TEST_LDOCAL &= (~(PMU_TEST_LDOCAL_LPBGRTRIM_Msk));
                    printfS("ANA_OUT: 7-->VBG_LP(LPBGR_TRIM)\r\n");
                break;                 
                default:
                    printfS("Reserve\r\n");
                break;              
            }
            printfS("PMU->TEST_ANATEST_SR = 0x%08x, PMU->TEST_LDOCAL = 0x%08x\r\n", PMU->TEST_ANATEST_SR, PMU->TEST_LDOCAL);            
            HAL_DWT_DelayMs(2000); 
            HAL_EXTI_SetConfigLine((GPIO_TypeDef *)GPIOA, EXTI_LINE_0, GPIO_MODE_IT_FALLING);
            temp++;
            if(temp > 7)
            {
                temp = 0;
            }
            
            printfS("Please press the button \r\n");           
        }
        
    }
}
