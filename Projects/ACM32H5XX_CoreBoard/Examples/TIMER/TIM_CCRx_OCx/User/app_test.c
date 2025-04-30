
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"  
#include "app_test.h"   



/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test1(void)
{
    uint32_t i;
    uint32_t j;
    
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_TypeDef *gpio[] = 
    
    {   GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF   };

//    uint32_t pin[] = 
//    {
//        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
//        GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9, 
//    };
    
    uint32_t pin[] = 
    {
//        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
        GPIO_PIN_0, GPIO_PIN_1, 
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,
        GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
        GPIO_PIN_12, GPIO_PIN_15,
    };
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    for (i=0; i<(sizeof(gpio) / sizeof(GPIO_TypeDef *)); i++)
    {
        for (j=0; j<(sizeof(pin) / sizeof(uint32_t)); j++)
        {
            GPIO_InitStruct.Pin       = pin[j];
            GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
//            GPIO_InitStruct.Pull      = GPIO_PULLUP;
//            GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
            GPIO_InitStruct.Pull      = GPIO_NOPULL;
            GPIO_InitStruct.Drive     = GPIO_DRIVE_LEVEL3;
            GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;

            HAL_GPIO_Init(gpio[i], &GPIO_InitStruct);

            HAL_GPIO_WritePin(gpio[i], pin[j], GPIO_PIN_SET);
            HAL_Delay(10);
            HAL_GPIO_WritePin(gpio[i], pin[j], GPIO_PIN_RESET);
            HAL_Delay(10);
        }
    }
}


