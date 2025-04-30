/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/

#include "app.h"


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


void HAL_FMC_SDRAM_MspInit(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;
    extern uint16_t gid_code; 
  
    /* 使能SDRAM相关的GPIO时钟 */
    __HAL_RCC_GPIOL_CLK_ENABLE();//开启GPIOL时钟
    __HAL_RCC_GPIOM_CLK_ENABLE();//开启GPIOM时钟
    __HAL_RCC_GPION_CLK_ENABLE();//开启GPION时钟
    __HAL_RCC_GPIOO_CLK_ENABLE();//开启GPIOO时钟
    __HAL_RCC_GPIOP_CLK_ENABLE();//开启GPIOP时钟
    __HAL_RCC_GPIOQ_CLK_ENABLE();//开启GPIOQ时钟

    /*-- GPIO 配置 -----------------------------------------------------*/

    /* 通用 GPIO 配置 */       
    GPIO_InitStructure.Mode         = GPIO_MODE_AF_PP;//配置为复用功能
    GPIO_InitStructure.Pull         = GPIO_PULLUP;
    GPIO_InitStructure.Alternate    = GPIO_FUNCTION_15;//AF15
    GPIO_InitStructure.Drive        = GPIO_DRIVE_LEVEL3;
  
  
    /*A[0:12]地址信号线 针对引脚配置*/
    /* A0-PN12, A1-PN13, A2-PN14, A3-PN15 */
    GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
    HAL_GPIO_Init(GPION, &GPIO_InitStructure);

    /* A4-PO6, A5-PO7, A6-PO8, A7-PO9, A8-PO10, A9-PO11 */ 
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11; 
    HAL_GPIO_Init(GPIOO, &GPIO_InitStructure);

    /* A10-PL9, A11-PL10, A12-PL11 */
    GPIO_InitStructure.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11; 
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);


    /*D[0:31]数据信号线 针对引脚配置*/
    /* D0-PQ8, D1-PQ9 */
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9; 
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);

    /* D2-PM14, D3-PM15 */
    GPIO_InitStructure.Pin = GPIO_PIN_14 | GPIO_PIN_15; 
    HAL_GPIO_Init(GPIOM, &GPIO_InitStructure);

    /* D4-PQ10, D5-PQ11, D6-PQ12, D7-PQ13 */
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; 
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);

    /* D8-PM4, D9-PM5, D10-PM6, D11-PM7 */
    GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    HAL_GPIO_Init(GPIOM, &GPIO_InitStructure);

    /*D12-PP6, D13-PP7, D14-PP8, D15-PP9 */
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9; 
    HAL_GPIO_Init(GPIOP, &GPIO_InitStructure);
    
    
    if (gid_code == IC_D569XQH6_PE)
    {  
        /* D16-PQ0, D17-PQ1, D18-PQ2, D19-PQ3, D20-PQ4, D21-PQ5, D22-PQ6, D23-PQ7 */ 
        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
        HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
        
        
        /* D24-PN0, D25-PN1, D26-PN2, D27-PN3, D280-PN4, D29-PN5, D30-PN6, D31-PN7 */ 
        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
        HAL_GPIO_Init(GPION, &GPIO_InitStructure);
        
        /* NBL2-PQ14 */
        GPIO_InitStructure.Pin = GPIO_PIN_14;         
        HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
        
        /* NBL3-PQ15 */
        GPIO_InitStructure.Pin = GPIO_PIN_15;         
        HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
    }

    /*控制信号线*/
    /* SDCLK-PL13 */
    GPIO_InitStructure.Pin = GPIO_PIN_13;          
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);

    /* SDNWE-PO5 */
    GPIO_InitStructure.Pin = GPIO_PIN_5;           
    HAL_GPIO_Init(GPIOO, &GPIO_InitStructure);

    /* SDCKE0-PL12 */
    GPIO_InitStructure.Pin = GPIO_PIN_12;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure); 

    /* SDNE0(CS)-PL6 */
    GPIO_InitStructure.Pin = GPIO_PIN_6;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);

    /* BA0-PL7 */
    GPIO_InitStructure.Pin = GPIO_PIN_7;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);
    
    /* BA1-PL8 */
    GPIO_InitStructure.Pin = GPIO_PIN_8;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);

    /* NRAS-PL5 */
    GPIO_InitStructure.Pin = GPIO_PIN_5;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);
    
    /* NCAS-PL4 */
    GPIO_InitStructure.Pin = GPIO_PIN_4;           
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);

    /* NBL0-PO4 */
    GPIO_InitStructure.Pin = GPIO_PIN_4;         
    HAL_GPIO_Init(GPIOO, &GPIO_InitStructure);
    
    /* NBL1-PL14 */
    GPIO_InitStructure.Pin = GPIO_PIN_14;         
    HAL_GPIO_Init(GPIOL, &GPIO_InitStructure);

    /* SDRAM specific IO mapping */ 
    SYSCFG->SYSCR |= SYSCFG_SYSCR_SDRAM_IO_SWP;
}



