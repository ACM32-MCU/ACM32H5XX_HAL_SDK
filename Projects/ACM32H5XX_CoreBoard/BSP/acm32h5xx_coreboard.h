/******************************************************************************
*@file  : acm32h5xx_coreboard.h
*@brief : This file provides set of firmware functions to manage Leds and 
*         push-button available on ACM32H5XX Core Board 
******************************************************************************/

#ifndef __ACM32H5XX_COREBOARD_H
#define __ACM32H5XX_COREBOARD_H
                                            
#include "hal.h"
#include "./SDIO/bsp_sdio.h"

#define BSP_LED_GPIO            GPIOA
#define BSP_LED_PIN             GPIO_PIN_0
#define BSP_LED_MODE            GPIO_MODE_OUTPUT_PP
#define BSP_LED_PULL            GPIO_PULLUP
#define BSP_LED_DRIVE           GPIO_DRIVE_LEVEL3
#define BSP_LED_ALTERNATE       GPIO_FUNCTION_0
#define BSP_LED_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_PB_GPIO             GPIOA
#define BSP_PB_PIN              GPIO_PIN_1
#define BSP_PB_MODE             GPIO_MODE_IT_FALLING
#define BSP_PB_PULL             GPIO_PULLUP
#define BSP_PB_DRIVE            GPIO_DRIVE_LEVEL3
#define BSP_PB_ALTERNATE        GPIO_FUNCTION_0
#define BSP_PB_EXTI_LINE        EXTI_LINE_1
#define BSP_PB_IRQ              EXTI1_IRQn
#define BSP_PB_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_MCO1_GPIO            GPIOA
#define BSP_MCO1_PIN             GPIO_PIN_8
#define BSP_MCO1_MODE            GPIO_MODE_AF_PP
#define BSP_MCO1_PULL            GPIO_NOPULL
#define BSP_MCO1_DRIVE           GPIO_DRIVE_LEVEL0
#define BSP_MCO1_ALTERNATE       GPIO_FUNCTION_0
#define BSP_MCO1_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_MCO2_GPIO            GPIOC
#define BSP_MCO2_PIN             GPIO_PIN_9
#define BSP_MCO2_MODE            GPIO_MODE_AF_PP
#define BSP_MCO2_PULL            GPIO_NOPULL
#define BSP_MCO2_DRIVE           GPIO_DRIVE_LEVEL7
#define BSP_MCO2_ALTERNATE       GPIO_FUNCTION_3
#define BSP_MCO2_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_LDO_SEL_GPIO     		GPIOD
#define BSP_LDO_SEL_PIN             GPIO_PIN_3
#define BSP_LDO_SEL_MODE            GPIO_MODE_OUTPUT_PP
#define BSP_LDO_SEL_PULL            GPIO_PULLUP
#define BSP_LDO_SEL_DRIVE           GPIO_DRIVE_LEVEL3
#define BSP_LDO_SEL_ALTERNATE       GPIO_FUNCTION_0
#define BSP_LDO_SEL_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

//GPIOB2 control SDIO power on/off
#define BSP_LDO_CTL_GPIO     		GPIOB
#define BSP_LDO_CTL_PIN             GPIO_PIN_2
#define BSP_LDO_CTL_MODE            GPIO_MODE_OUTPUT_PP
#define BSP_LDO_CTL_PULL            GPIO_PULLUP
#define BSP_LDO_CTL_DRIVE           GPIO_DRIVE_LEVEL3
#define BSP_LDO_CTL_ALTERNATE       GPIO_FUNCTION_0
#define BSP_LDO_CTL_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

//SD card power on/off
#define SD_CARD_PWR_OFF				1
#define SD_CARD_PWR_ON				0

//SD card IO voltage select
#define SD_CARD_IO_VCC_1V8			SD_IO_VCC_1V8
#define SD_CARD_IO_VCC_3V3			SD_IO_VCC_3V3

void BSP_LED_Init(void);
void BSP_LED_On(void);
void BSP_LED_Off(void);
void BSP_LED_Toggle(void);
void BSP_PB_Init(void);
uint32_t BSP_PB_GetState(void);
void BSP_UART_Init(UART_TypeDef *uartx, uint32_t baudrate);
void BSP_UART_DeInit(void);
void BSP_MCO_Init(uint32_t mco, uint32_t state, uint32_t mco_div);
void get_reset_source(void);

#endif

