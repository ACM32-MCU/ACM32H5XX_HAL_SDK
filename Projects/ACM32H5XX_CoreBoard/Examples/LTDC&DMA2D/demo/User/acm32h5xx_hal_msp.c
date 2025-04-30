/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/

#include "app.h"

static void delay(uint32_t n)
{
	while(n--);
}


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



#ifdef HAL_I2C_MODULE_ENABLED
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{    
    /* For Example */
    GPIO_InitTypeDef GPIO_Handle; 
    
    /* I2C1 */
    if (hi2c->Instance == I2C1)
    {
        /* Enable Clock */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();


        GPIO_Handle.Pin            = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
        
//        GPIO_Handle.Pin            = GPIO_PIN_8;
//        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
//        GPIO_Handle.Pull           = GPIO_PULLUP;
//		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
//        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
//        HAL_GPIO_Init(GPIOG, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C1_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C1_IRQn);   
    }
    /* I2C2 */
    else if (hi2c->Instance == I2C2) 
    {
        /* Enable Clock */
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();

        GPIO_Handle.Pin            = GPIO_PIN_10|GPIO_PIN_11;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOI, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C2_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C2_IRQn);     
    }
    else if (hi2c->Instance == I2C3) 
    {
        assert_param(0);        
    }
    else if (hi2c->Instance == I2C4) 
    {
        /* Enable Clock */
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /* I2C1 SDA  PB7 */
        /* I2C1 SCL  PB6 */
        GPIO_Handle.Pin            = GPIO_PIN_8;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOB, &GPIO_Handle);
        
        GPIO_Handle.Pin            = GPIO_PIN_13;
        GPIO_Handle.Mode           = GPIO_MODE_AF_OD;
        GPIO_Handle.Pull           = GPIO_PULLUP;
		GPIO_Handle.Drive          = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate      = GPIO_FUNCTION_5;
        HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        
        /* Clear Pending Interrupt */
        NVIC_ClearPendingIRQ(I2C4_IRQn);
        
        /* Enable External Interrupt */
        NVIC_EnableIRQ(I2C4_IRQn);           
    }
}
#endif	//#ifdef HAL_I2C_MODULE_ENABLED


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
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* A2:Tx  A3:Rx */
        GPIO_Uart.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        
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

    }
}


#ifdef HAL_LTDC_MODULE_ENABLED
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	int i;
	const GPIO_CfgDef gpio_cfg_tbl[]=
	{
#ifdef ACM32H5XX_176_EVB	//176 EVB config

		{GPIOB, GPIO_PIN_6,		GPIO_FUNCTION_12},		//LCD_HSYNC
		{GPIOB, GPIO_PIN_7,		GPIO_FUNCTION_12},		//LCD_VSYNC
		{GPIOD, GPIO_PIN_8,		GPIO_FUNCTION_14},		//LCD_CLK

		{GPIOH,	GPIO_PIN_2,		GPIO_FUNCTION_14},		//LCD_R0
		{GPIOH, GPIO_PIN_3,		GPIO_FUNCTION_14},		//LCD_R1
		{GPIOE, GPIO_PIN_9,		GPIO_FUNCTION_15},		//LCD_R2
		{GPIOH, GPIO_PIN_9,		GPIO_FUNCTION_14},		//LCD_R3
		{GPIOH, GPIO_PIN_10,	GPIO_FUNCTION_14},		//LCD_R4
		{GPIOH, GPIO_PIN_11,	GPIO_FUNCTION_14},		//LCD_R5
		{GPIOH, GPIO_PIN_12,	GPIO_FUNCTION_14},		//LCD_R6
		{GPIOE, GPIO_PIN_15,	GPIO_FUNCTION_14},		//LCD_R7

		{GPIOE, GPIO_PIN_5,		GPIO_FUNCTION_14},		//LCD_G0
		{GPIOE, GPIO_PIN_6, 	GPIO_FUNCTION_14},		//LCD_G1
		{GPIOH, GPIO_PIN_13, 	GPIO_FUNCTION_14},		//LCD_G2
		{GPIOE, GPIO_PIN_11, 	GPIO_FUNCTION_14},		//LCD_G3
		{GPIOB, GPIO_PIN_10, 	GPIO_FUNCTION_14},		//LCD_G4
		{GPIOC, GPIO_PIN_3, 	GPIO_FUNCTION_14},		//LCD_G5
		{GPIOI, GPIO_PIN_1, 	GPIO_FUNCTION_14},		//LCD_G6
		{GPIOD, GPIO_PIN_9, 	GPIO_FUNCTION_14},		//LCD_G7

		{GPIOE, GPIO_PIN_4,		GPIO_FUNCTION_14},		//LCD_B0
		{GPIOB, GPIO_PIN_2, 	GPIO_FUNCTION_14},		//LCD_B1
		{GPIOF, GPIO_PIN_12, 	GPIO_FUNCTION_14},		//LCD_B2
		{GPIOD, GPIO_PIN_10, 	GPIO_FUNCTION_14},		//LCD_B3
		{GPIOE, GPIO_PIN_12, 	GPIO_FUNCTION_14},		//LCD_B4
		{GPIOA, GPIO_PIN_3, 	GPIO_FUNCTION_14},		//LCD_B5
		{GPIOI, GPIO_PIN_6, 	GPIO_FUNCTION_14},		//LCD_B6
		{GPIOI, GPIO_PIN_7, 	GPIO_FUNCTION_14},		//LCD_B7

		{GPIOE, GPIO_PIN_13, 	GPIO_FUNCTION_14}		//LCD_DE

#endif

	};

	for(i=0; i<(sizeof(gpio_cfg_tbl)/sizeof(gpio_cfg_tbl[0])); i++)
	{
		gpio_func_set(gpio_cfg_tbl[i].pGPIO_Port, gpio_cfg_tbl[i].GPIO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, gpio_cfg_tbl[i].GPIO_Alt);
	}

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_MspInit could be implemented in the user file
   */ 
}
#endif	//#ifdef HAL_LTDC_MODULE_ENABLED



#ifdef HAL_SDMMC_ENABLED
void BSP_SDIO_Pwr_Ctl_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct ={0};

	//SD card power control pin
	GPIO_InitStruct.Pin 	  = BSP_LDO_CTL_PIN;
	GPIO_InitStruct.Mode	  = BSP_LDO_CTL_MODE;
	GPIO_InitStruct.Pull	  = BSP_LDO_CTL_PULL;
	GPIO_InitStruct.Alternate = BSP_LDO_CTL_ALTERNATE;
	HAL_GPIO_Init(BSP_LDO_CTL_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_LDO_CTL_GPIO, BSP_LDO_CTL_PIN, SD_CARD_PWR_OFF);
	HAL_Delay(200);
	HAL_GPIO_WritePin(BSP_LDO_CTL_GPIO, BSP_LDO_CTL_PIN, SD_CARD_PWR_ON);

	//IO LDO_SEL
	GPIO_InitStruct.Pin 	  = BSP_LDO_SEL_PIN;
	GPIO_InitStruct.Mode	  = BSP_LDO_SEL_MODE;
	GPIO_InitStruct.Pull	  = BSP_LDO_SEL_PULL;
	GPIO_InitStruct.Alternate = BSP_LDO_SEL_ALTERNATE;
	HAL_GPIO_Init(BSP_LDO_SEL_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_LDO_SEL_GPIO, BSP_LDO_SEL_PIN, SD_CARD_IO_VCC_3V3);
}


void HAL_SDMMC_MspInit(SDMMC_HandleTypeDef *hsdmmc)
{
	GPIO_InitTypeDef    GPIO_InitStruct ={0};
    /* 
      NOTE: This function should be modified, when the callback is needed,
      the HAL_SDMMC_MspInit can be implemented in the user file.
    */

	int i;
	const GPIO_CfgDef gpio_cfg_ch1_tbl[]=
	{
		{GPIOC, GPIO_PIN_1,		GPIO_FUNCTION_12},		//SDMMC_CLK
//		{GPIOG, GPIO_PIN_8,		GPIO_FUNCTION_9},		//SDMMC_CLKIN
		{GPIOD, GPIO_PIN_11,	GPIO_FUNCTION_6},		//SDMMC_CMD

		{GPIOE, GPIO_PIN_14, 	GPIO_FUNCTION_6},		//SDMMC_D0
		{GPIOG, GPIO_PIN_10, 	GPIO_FUNCTION_10},		//SDMMC_D1
		{GPIOG, GPIO_PIN_11, 	GPIO_FUNCTION_12},		//SDMMC_D2
		{GPIOG, GPIO_PIN_12, 	GPIO_FUNCTION_13},		//SDMMC_D3
#if 0
		{GPIOX, GPIO_PIN_X, 	GPIO_FUNCTION_XX},		//SDMMC_D4
		{GPIOX, GPIO_PIN_X, 	GPIO_FUNCTION_XX},		//SDMMC_D5
		{GPIOX, GPIO_PIN_X, 	GPIO_FUNCTION_XX},		//SDMMC_D6
		{GPIOX, GPIO_PIN_X, 	GPIO_FUNCTION_XX},		//SDMMC_D7
#endif
	};

	/* Enable Clock */
//	RCC_AHB3_CLK_ENABLE(RCC_AHB3_IPCKENR_SDIOCKEN);
	__HAL_RCC_SDMMC_CLK_ENABLE();

    if(hsdmmc->Init.Ch == SDMMC_CH0)
    {
		;
    }
	else if(hsdmmc->Init.Ch == SDMMC_CH1)
	{
		for(i=0; i<(sizeof(gpio_cfg_ch1_tbl)/sizeof(gpio_cfg_ch1_tbl[0])); i++)
		{
			gpio_func_set(gpio_cfg_ch1_tbl[i].pGPIO_Port, gpio_cfg_ch1_tbl[i].GPIO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, gpio_cfg_ch1_tbl[i].GPIO_Alt);
		}
	
        /* NVIC Config */
        NVIC_ClearPendingIRQ(SDMMC_IRQn);
        NVIC_SetPriority(SDMMC_IRQn, 5);
//        NVIC_EnableIRQ(SDMMC_IRQn);
		NVIC_DisableIRQ(SDMMC_IRQn);
	}
}


#endif


/**
* @brief LTDC MSP Initialization
* @param huart: UART handle pointer
* @retval None
*/
void HAL_FMC_SDRAM_MspInit(void)
{
	typedef struct
	{
		GPIO_TypeDef *pGPIO_Port;
		uint16_t	GPIO_Pin;
		uint16_t	GPIO_Alt;
	}GPIO_CfgDef;
	
	int i;
	const GPIO_CfgDef gpio_cfg_tbl[]=
	{
		//Address signal
		{GPIOF, GPIO_PIN_0,		GPIO_FUNCTION_11},		//A0
		{GPIOF, GPIO_PIN_1,		GPIO_FUNCTION_11},		//A1
		{GPIOF, GPIO_PIN_2,		GPIO_FUNCTION_11},		//A2
		{GPIOF,	GPIO_PIN_3,		GPIO_FUNCTION_11},		//A3
		{GPIOF, GPIO_PIN_4,		GPIO_FUNCTION_11},		//A4
		{GPIOF, GPIO_PIN_5,		GPIO_FUNCTION_11},		//A5
		{GPIOF, GPIO_PIN_12,	GPIO_FUNCTION_11},		//A6
		{GPIOF, GPIO_PIN_13,	GPIO_FUNCTION_11},		//A7
		{GPIOF, GPIO_PIN_14,	GPIO_FUNCTION_11},		//A8
		{GPIOF, GPIO_PIN_15,	GPIO_FUNCTION_11},		//A9
		{GPIOG, GPIO_PIN_0,		GPIO_FUNCTION_11},		//A10
		{GPIOG, GPIO_PIN_1,		GPIO_FUNCTION_11},		//A11
		{GPIOG, GPIO_PIN_2,		GPIO_FUNCTION_11},		//A12

		//data signal
		{GPIOD, GPIO_PIN_14,	GPIO_FUNCTION_10},		//D0
		{GPIOD, GPIO_PIN_15, 	GPIO_FUNCTION_10},		//D1
		{GPIOD, GPIO_PIN_0, 	GPIO_FUNCTION_10},		//D2
		{GPIOD, GPIO_PIN_1, 	GPIO_FUNCTION_10},		//D3
		{GPIOE, GPIO_PIN_7, 	GPIO_FUNCTION_10},		//D4
		{GPIOE, GPIO_PIN_8, 	GPIO_FUNCTION_10},		//D5
		{GPIOE, GPIO_PIN_9, 	GPIO_FUNCTION_10},		//D6
		{GPIOE, GPIO_PIN_10, 	GPIO_FUNCTION_10},		//D7
		{GPIOE, GPIO_PIN_11,	GPIO_FUNCTION_10},		//D8
		{GPIOE, GPIO_PIN_12, 	GPIO_FUNCTION_10},		//D9
		{GPIOE, GPIO_PIN_13, 	GPIO_FUNCTION_10},		//D10
		{GPIOE, GPIO_PIN_14, 	GPIO_FUNCTION_10},		//D11
		{GPIOE, GPIO_PIN_15, 	GPIO_FUNCTION_10},		//D12
		{GPIOD, GPIO_PIN_8, 	GPIO_FUNCTION_10},		//D13
		{GPIOD, GPIO_PIN_9, 	GPIO_FUNCTION_10},		//D14
		{GPIOD, GPIO_PIN_10, 	GPIO_FUNCTION_10},		//D15
#if 0
		{GPIOQ, GPIO_PIN_0, 	GPIO_FUNCTION_15},		//D16
		{GPIOQ, GPIO_PIN_1, 	GPIO_FUNCTION_15},		//D17
		{GPIOQ, GPIO_PIN_2, 	GPIO_FUNCTION_15},		//D18
		{GPIOQ, GPIO_PIN_3, 	GPIO_FUNCTION_15},		//D19
		{GPIOQ, GPIO_PIN_4, 	GPIO_FUNCTION_15},		//D20
		{GPIOQ, GPIO_PIN_5, 	GPIO_FUNCTION_15},		//D21
		{GPIOQ, GPIO_PIN_6, 	GPIO_FUNCTION_15},		//D22
		{GPIOQ, GPIO_PIN_7, 	GPIO_FUNCTION_15},		//D23
		{GPION, GPIO_PIN_0, 	GPIO_FUNCTION_15},		//D24
		{GPION, GPIO_PIN_1, 	GPIO_FUNCTION_15},		//D25
		{GPION, GPIO_PIN_2, 	GPIO_FUNCTION_15},		//D26
		{GPION, GPIO_PIN_3, 	GPIO_FUNCTION_15},		//D27
		{GPION, GPIO_PIN_4, 	GPIO_FUNCTION_15},		//D28
		{GPION, GPIO_PIN_5, 	GPIO_FUNCTION_15},		//D29
		{GPION, GPIO_PIN_6, 	GPIO_FUNCTION_15},		//D30
		{GPION, GPIO_PIN_7, 	GPIO_FUNCTION_15},		//D31
#endif

		//control signal
		{GPIOG, GPIO_PIN_8, 	GPIO_FUNCTION_11},		//CLK
		{GPIOC, GPIO_PIN_0, 	GPIO_FUNCTION_11},		//WE
		{GPIOC, GPIO_PIN_3, 	GPIO_FUNCTION_11},		//CKE
		{GPIOC, GPIO_PIN_2, 	GPIO_FUNCTION_11},		//SDNE
		{GPIOG, GPIO_PIN_15, 	GPIO_FUNCTION_11},		//CAS
		{GPIOF, GPIO_PIN_11, 	GPIO_FUNCTION_11},		//RAS
		{GPIOG, GPIO_PIN_4, 	GPIO_FUNCTION_11},		//BA0
		{GPIOG, GPIO_PIN_5, 	GPIO_FUNCTION_11},		//BA1

		{GPIOE, GPIO_PIN_0, 	GPIO_FUNCTION_10},		//NBL0
		{GPIOE, GPIO_PIN_1, 	GPIO_FUNCTION_10},		//NBL1
#if 0
		{GPIOQ, GPIO_PIN_14, 	GPIO_FUNCTION_15},		//NBL2
		{GPIOQ, GPIO_PIN_15, 	GPIO_FUNCTION_15},		//NBL3
#endif
	};

	for(i=0; i<(sizeof(gpio_cfg_tbl)/sizeof(gpio_cfg_tbl[0])); i++)
	{
		gpio_func_set(gpio_cfg_tbl[i].pGPIO_Port, gpio_cfg_tbl[i].GPIO_Pin, GPIO_MODE_AF_PP, GPIO_PULLUP, gpio_cfg_tbl[i].GPIO_Alt);
	}

//	SYSCFG->SYSCR |= BIT7;	//SDRAM IO remap

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_LTDC_MspInit could be implemented in the user file
   */
}



