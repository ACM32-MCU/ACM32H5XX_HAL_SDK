/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/
#include "hal.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* 
      NOTE: This function can be modified by user.
    */

    GPIO_InitTypeDef    GPIO_Uart = {0};
    
    if (huart->Instance == USART1) 
    {
        /* Enable Clock */
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        GPIO_Uart.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_11;
            HAL_GPIO_Init(GPIOA, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
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
        __HAL_RCC_GPIOD_CLK_ENABLE();
        
        GPIO_Uart.Pin       = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_3;
        
        HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_3;
            GPIO_Uart.Alternate = GPIO_FUNCTION_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            GPIO_Uart.Pin = GPIO_PIN_4;
            GPIO_Uart.Alternate = GPIO_FUNCTION_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (huart->Instance == USART3) 
    {
        /* Enable Clock */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        GPIO_Uart.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_Uart.Mode      = GPIO_MODE_AF_PP;
        GPIO_Uart.Pull      = GPIO_PULLUP;
        GPIO_Uart.Drive     = GPIO_DRIVE_LEVEL3;
        GPIO_Uart.Alternate = GPIO_FUNCTION_3;
        
        HAL_GPIO_Init(GPIOD, &GPIO_Uart);
        
        if (huart->Init.HwFlowCtl & UART_HWCONTROL_CTS) 
        {
            assert_param(0);
        }

        if (huart->Init.HwFlowCtl & UART_HWCONTROL_RTS) 
        {
            assert_param(0);
        }
        /* NVIC Config */
        NVIC_ClearPendingIRQ(USART3_IRQn);
        NVIC_SetPriority(USART3_IRQn, 5);
        NVIC_EnableIRQ(USART3_IRQn);
    }
    
}

void HAL_OSPI_MspInit(OSPI_HandleTypeDef *hospi)
{  
    GPIO_InitTypeDef GPIO_Handle = {0}; 
    
    /* OSPI1 */
    if (hospi->Instance == OSPI1)
    {
        /* Enable Clock */
        __HAL_RCC_OSPI1_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
       
        /****************************
        OSPI1_CS   : PC11-AF13
        OSPI1_NCLK : PC12-AF13
        OSPI1_CLK  : PC10-AF11 
        OSPI1_DQS  : PG9 -AF9 
        OSPI1_IO0  : PD0 -AF13
        OSPI1_IO1  : PD1 -AF13
        OSPI1_IO2  : PD2 -AF13
        OSPI1_IO3  : PD3 -AF13
        OSPI1_IO4  : PD4 -AF13
        OSPI1_IO5  : PD5 -AF13
        OSPI1_IO6  : PD6 -AF13
        OSPI1_IO7  : PD7 -AF13
        ****************************/
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;

        /* OSPI1_DQS-PG9-AF9 */ 
        GPIO_Handle.Alternate = GPIO_FUNCTION_9;
        GPIO_Handle.Pin = GPIO_PIN_9;        
        HAL_GPIO_Init(GPIOG, &GPIO_Handle);
        
        /* OSPI1_CLK-PC10-AF11 */
        GPIO_Handle.Alternate = GPIO_FUNCTION_11;
        GPIO_Handle.Pin = GPIO_PIN_10;      
        HAL_GPIO_Init(GPIOC, &GPIO_Handle);
        
        /* OSPI1_CS-PC11-AF13, OSPI1_NCLK-PC12-AF13 */        
        GPIO_Handle.Alternate = GPIO_FUNCTION_13;
        GPIO_Handle.Pin = GPIO_PIN_11;// | GPIO_PIN_12 ;
        HAL_GPIO_Init(GPIOC, &GPIO_Handle);
        
        /* OSPI1_IO0-PD0-AF13, OSPI1_IO1-PD1-AF13 */
        GPIO_Handle.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        
        if((hospi->Init.XMode == OSPI_4X_MODE) || (hospi->Init.XMode == OSPI_8X_MODE)) 
        {
            /* OSPI1_IO2-PD2-AF13, OSPI1_IO3-PD3-AF13 */
            GPIO_Handle.Pin = GPIO_PIN_2 | GPIO_PIN_3;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        }
        if(hospi->Init.XMode == OSPI_8X_MODE)
        {   
            /* OSPI1_IO4-PD4-AF13, OSPI1_IO5-PD5-AF13, OSPI1_IO6-PD6-AF13, OSPI1_IO7-PD7-AF13 */            
            GPIO_Handle.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
            HAL_GPIO_Init(GPIOD, &GPIO_Handle);
        }       

        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI1_IRQn);
        NVIC_SetPriority(OSPI1_IRQn, 5);
        NVIC_EnableIRQ(OSPI1_IRQn);
    }
    /* OSPI2 */
    else if (hospi->Instance == OSPI2) 
    {
       /* Enable Clock */
        __HAL_RCC_OSPI2_CLK_ENABLE();
        __HAL_RCC_GPIOM_CLK_ENABLE();
        __HAL_RCC_GPIOP_CLK_ENABLE();
        __HAL_RCC_GPIOQ_CLK_ENABLE();
        
        /****************************
        OSPI2_CS   : PQ9 -AF12
        OSPI2_NCLK : PM15-AF12
        OSPI2_CLK  : PM14-AF12
        OSPI2_DQS  : PQ15-AF12
        OSPI2_IO0  : PP6 -AF12
        OSPI2_IO1  : PP7 -AF12
        OSPI2_IO2  : PP8 -AF12
        OSPI2_IO3  : PP9 -AF12
        OSPI2_IO4  : PQ10-AF12
        OSPI2_IO5  : PQ11-AF12
        OSPI2_IO6  : PQ12-AF12
        OSPI2_IO7  : PQ13-AF12
        ****************************/
        GPIO_Handle.Mode = GPIO_MODE_AF_PP;
        GPIO_Handle.Pull = GPIO_PULLUP;
        GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
        GPIO_Handle.Alternate = GPIO_FUNCTION_12;
        
        /* OSPI2_NCLK-PM15-AF12, OSPI2_CLK-PM14-AF12 */
        GPIO_Handle.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOM, &GPIO_Handle);
        
        /* OSPI2_CS-PQ9-AF12, OSPI2_DQS-PQ15-AF12 */        
        GPIO_Handle.Pin = GPIO_PIN_9 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOQ, &GPIO_Handle);
        
        /* OSPI2_IO0-PP6-AF12, OSPI2_IO1-PP7-AF12 */        
        GPIO_Handle.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOP, &GPIO_Handle);

        if((hospi->Init.XMode == OSPI_4X_MODE) || (hospi->Init.XMode == OSPI_8X_MODE))
        {
            /* OSPI2_IO2-PP8-AF12, OSPI2_IO3-PP9-AF12 */
            GPIO_Handle.Pin = GPIO_PIN_8 | GPIO_PIN_9;
            HAL_GPIO_Init(GPIOP, &GPIO_Handle);
        }
        if(hospi->Init.XMode == OSPI_8X_MODE)
        { 
            /* OSPI2_IO4-PQ10-AF12, OSPI2_IO5-PQ11-AF12, OSPI2_IO6-PQ12-AF12, OSPI2_IO7-PQ13-AF12 */      
            GPIO_Handle.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
            HAL_GPIO_Init(GPIOQ, &GPIO_Handle);        
        }
        
        /* NVIC Config */
        NVIC_ClearPendingIRQ(OSPI2_IRQn);
        NVIC_SetPriority(OSPI2_IRQn, 5);
        NVIC_EnableIRQ(OSPI2_IRQn);
    }
}

void HAL_FMC_SDRAM_MspInit(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;
  
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

    /* D16-PQ0, D17-PQ1, D18-PQ2, D19-PQ3, D20-PQ4, D21-PQ5, D22-PQ6, D23-PQ7 */ 
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                            GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
    
    
    /* D24-PN0, D25-PN1, D26-PN2, D27-PN3, D280-PN4, D29-PN5, D30-PN6, D31-PN7 */ 
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                            GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    HAL_GPIO_Init(GPION, &GPIO_InitStructure);


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
    
    /* NBL2-PQ14 */
    GPIO_InitStructure.Pin = GPIO_PIN_14;         
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
    
    /* NBL3-PQ15 */
    GPIO_InitStructure.Pin = GPIO_PIN_15;         
    HAL_GPIO_Init(GPIOQ, &GPIO_InitStructure);
    
    SYSCFG->SYSCR |= BIT7;

}
