/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file
******************************************************************************/

#include "main.h"


static bool gpio_func_set(GPIO_TypeDef *gpiox, uint32_t pin, uint32_t mode,
                          uint32_t pull, uint32_t alternate)
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};
    GPIO_TypeDef *pGPIO_Port;
    int num;

    IS_GPIO_ALL_INSTANCE(gpiox);
//    RT_ASSERT(IS_GPIO_PIN(pin));
//    RT_ASSERT(IS_GPIO_MODE(mode));
    IS_GPIO_PULL(pull);
    IS_GPIO_FUNCTION(alternate);

    if (gpiox == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (gpiox == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (gpiox == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if (gpiox == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if (gpiox == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    else if (gpiox == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
    else if (gpiox == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }
    else if (gpiox == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }
    else if (gpiox == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_ENABLE();
    }
    else if (gpiox == GPIOJ)
    {
        __HAL_RCC_GPIOJ_CLK_ENABLE();
    }
    else if (gpiox == GPIOK)
    {
        __HAL_RCC_GPIOK_CLK_ENABLE();
    }
    else if (gpiox == GPIOL)
    {
        __HAL_RCC_GPIOL_CLK_ENABLE();
    }
    else if (gpiox == GPIOM)
    {
        __HAL_RCC_GPIOM_CLK_ENABLE();
    }
    else
    {
        return false;
    }

    GPIO_InitStruct.Pin       = pin;
    GPIO_InitStruct.Mode      = mode;
    GPIO_InitStruct.Pull      = pull;
    GPIO_InitStruct.Alternate = alternate;
    GPIO_InitStruct.Drive = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(gpiox, &GPIO_InitStruct);

    return true;
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /*
      NOTE: This function should be modified, when the callback is needed,
      the HAL_UART_MspInit can be implemented in the user file.
    */

    /* For Example */
    GPIO_InitTypeDef    GPIO_Uart = {0};

    if (huart->Instance == USART1)
    {
        /* Enable Clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* A9:Tx  A10:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;

        HAL_GPIO_Init(GPIOA, &GPIO_Uart);

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS)
        {
            /* A11:CTS */
            GPIO_Uart.Pin = GPIO_PIN_11;

            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS)
        {
            /* A12:RTS */
            GPIO_Uart.Pin = GPIO_PIN_12;

            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, 5);
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (huart->Instance == USART2)
    {
        /* Enable Clock */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /* B1:Tx  B0:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;

        HAL_GPIO_Init(GPIOB, &GPIO_Uart);

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS)
        {
            /* A6:CTS */
            GPIO_Uart.Pin = GPIO_PIN_6;
            GPIO_Uart.Alternate = GPIO_FUNCTION_2;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS)
        {
            /* A7:RTS */
            GPIO_Uart.Pin = GPIO_PIN_7;
            GPIO_Uart.Alternate = GPIO_FUNCTION_2;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
}

void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{
	int i;

    GPIO_InitTypeDef GPIO_Handle = {0};
	const GPIO_CfgDef gpio_cfg_ospi1_tbl[]=
    {
#ifdef ACM32H5XX_176_EVB	//176 EVB config
//		{PSRAM_RST_PORT, PSRAM_RST_PIN,	GPIO_FUNCTION_0},		//OSPI1_RST
		{GPIOC, GPIO_PIN_11,	GPIO_FUNCTION_13},		//OSPI1_CS
		{GPIOC, GPIO_PIN_10,	GPIO_FUNCTION_11},		//OSPI1_CLK

		{GPIOD, GPIO_PIN_0, 	GPIO_FUNCTION_13},		//OSPI1_IO0
		{GPIOD, GPIO_PIN_1, 	GPIO_FUNCTION_13},		//OSPI1_IO1
		{GPIOD, GPIO_PIN_2, 	GPIO_FUNCTION_13},		//OSPI1_IO2
		{GPIOD, GPIO_PIN_3, 	GPIO_FUNCTION_13},		//OSPI1_IO3
		{GPIOD, GPIO_PIN_4, 	GPIO_FUNCTION_13},		//OSPI1_IO4
		{GPIOD, GPIO_PIN_5, 	GPIO_FUNCTION_13},		//OSPI1_IO5
//		{GPIOD, GPIO_PIN_6, 	GPIO_FUNCTION_13},		//OSPI1_IO6	//mini_V1
		{GPIOG, GPIO_PIN_9, 	GPIO_FUNCTION_13},		//OSPI1_IO6	//mini_V2
		{GPIOD, GPIO_PIN_7, 	GPIO_FUNCTION_13},		//OSPI1_IO7

//		{GPIOG, GPIO_PIN_9,		GPIO_FUNCTION_9},		//OSPI1_DQS	//mini_V1
		{GPIOA, GPIO_PIN_15,	GPIO_FUNCTION_13},		//OSPI1_DQS	//mini_V2
#endif
	};

    /* OSPI1 */
    if (hospi->Instance == OSPI1)
    {
		for(i=0; i<(sizeof(gpio_cfg_ospi1_tbl)/sizeof(gpio_cfg_ospi1_tbl[0])); i++)
        {
			gpio_func_set(gpio_cfg_ospi1_tbl[i].pGPIO_Port, gpio_cfg_ospi1_tbl[i].GPIO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, gpio_cfg_ospi1_tbl[i].GPIO_Alt);
        }

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);

        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI1_IRQn);
        NVIC_SetPriority(OSPI1_IRQn, 5);
        NVIC_EnableIRQ(OSPI1_IRQn);
    }
    /* OSPI2 */
    else if (hospi->Instance == OSPI2)
    {
        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI2_IRQn);
        NVIC_SetPriority(OSPI2_IRQn, 5);
        NVIC_EnableIRQ(OSPI2_IRQn);
    }
}


#ifdef HAL_LTDC_MODULE_ENABLED
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    int i;
    const GPIO_CfgDef gpio_cfg_tbl[] =
    {
#ifdef ACM32H5XX_176_EVB    //176 EVB config

        {GPIOB, GPIO_PIN_6,     GPIO_FUNCTION_12},      //LCD_HSYNC
        {GPIOB, GPIO_PIN_7,     GPIO_FUNCTION_12},      //LCD_VSYNC
        {GPIOD, GPIO_PIN_8,     GPIO_FUNCTION_14},      //LCD_CLK

        {GPIOH, GPIO_PIN_2,     GPIO_FUNCTION_14},      //LCD_R0
        {GPIOH, GPIO_PIN_3,     GPIO_FUNCTION_14},      //LCD_R1
        {GPIOE, GPIO_PIN_9,     GPIO_FUNCTION_15},      //LCD_R2
        {GPIOH, GPIO_PIN_9,     GPIO_FUNCTION_14},      //LCD_R3
        {GPIOH, GPIO_PIN_10,    GPIO_FUNCTION_14},      //LCD_R4
        {GPIOH, GPIO_PIN_11,    GPIO_FUNCTION_14},      //LCD_R5
        {GPIOH, GPIO_PIN_12,    GPIO_FUNCTION_14},      //LCD_R6
        {GPIOE, GPIO_PIN_15,    GPIO_FUNCTION_14},      //LCD_R7

        {GPIOE, GPIO_PIN_5,     GPIO_FUNCTION_14},      //LCD_G0
        {GPIOE, GPIO_PIN_6,     GPIO_FUNCTION_14},      //LCD_G1
        {GPIOH, GPIO_PIN_13,    GPIO_FUNCTION_14},      //LCD_G2
        {GPIOE, GPIO_PIN_11,    GPIO_FUNCTION_14},      //LCD_G3
        {GPIOB, GPIO_PIN_10,    GPIO_FUNCTION_14},      //LCD_G4
        {GPIOC, GPIO_PIN_3,     GPIO_FUNCTION_14},      //LCD_G5
        {GPIOI, GPIO_PIN_1,     GPIO_FUNCTION_14},      //LCD_G6
        {GPIOD, GPIO_PIN_9,     GPIO_FUNCTION_14},      //LCD_G7

        {GPIOE, GPIO_PIN_4,     GPIO_FUNCTION_14},      //LCD_B0
        {GPIOB, GPIO_PIN_2,     GPIO_FUNCTION_14},      //LCD_B1
        {GPIOF, GPIO_PIN_12,    GPIO_FUNCTION_14},      //LCD_B2
        {GPIOD, GPIO_PIN_10,    GPIO_FUNCTION_14},      //LCD_B3
        {GPIOE, GPIO_PIN_12,    GPIO_FUNCTION_14},      //LCD_B4
        {GPIOA, GPIO_PIN_3,     GPIO_FUNCTION_14},      //LCD_B5
        {GPIOI, GPIO_PIN_6,     GPIO_FUNCTION_14},      //LCD_B6
        {GPIOI, GPIO_PIN_7,     GPIO_FUNCTION_14},      //LCD_B7

        {GPIOE, GPIO_PIN_13,    GPIO_FUNCTION_14}       //LCD_DE

#endif

    };

    for (i = 0; i < (sizeof(gpio_cfg_tbl) / sizeof(gpio_cfg_tbl[0])); i++)
    {
        gpio_func_set(gpio_cfg_tbl[i].pGPIO_Port, gpio_cfg_tbl[i].GPIO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, gpio_cfg_tbl[i].GPIO_Alt);
    }

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_LTDC_MspInit could be implemented in the user file
     */
}
#endif  //#ifdef HAL_LTDC_MODULE_ENABLED
