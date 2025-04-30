/******************************************************************************
*@file  : acm32h5xx_hal_msp.c
*@brief : HAL MSP module . not need header file 
******************************************************************************/

#include "app.h"


#define BSP_I2S1_MCK_GPIO           GPIOC
#define BSP_I2S1_MCK_PIN            GPIO_PIN_8
#define BSP_I2S1_MCK_MODE           GPIO_MODE_AF_PP
#define BSP_I2S1_MCK_PULL           GPIO_PULLUP
#define BSP_I2S1_MCK_DRIVE          GPIO_DRIVE_LEVEL7
#define BSP_I2S1_MCK_ALTERNATE      GPIO_FUNCTION_7
#define BSP_I2S1_MCK_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_I2S1_CK_GPIO            GPIOA
#define BSP_I2S1_CK_PIN             GPIO_PIN_5
#define BSP_I2S1_CK_MODE            GPIO_MODE_AF_PP
#define BSP_I2S1_CK_PULL            GPIO_PULLUP
#define BSP_I2S1_CK_DRIVE           GPIO_DRIVE_LEVEL7
#define BSP_I2S1_CK_ALTERNATE       GPIO_FUNCTION_7
#define BSP_I2S1_CK_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_I2S1_WS_GPIO            GPIOA
#define BSP_I2S1_WS_PIN             GPIO_PIN_4
#define BSP_I2S1_WS_MODE            GPIO_MODE_AF_PP
#define BSP_I2S1_WS_PULL            GPIO_PULLUP
#define BSP_I2S1_WS_DRIVE           GPIO_DRIVE_LEVEL7
#define BSP_I2S1_WS_ALTERNATE       GPIO_FUNCTION_7
#define BSP_I2S1_WS_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_I2S1_SDI_GPIO           GPIOA
#define BSP_I2S1_SDI_PIN            GPIO_PIN_6
#define BSP_I2S1_SDI_MODE           GPIO_MODE_AF_PP
#define BSP_I2S1_SDI_PULL           GPIO_PULLUP
#define BSP_I2S1_SDI_DRIVE          GPIO_DRIVE_LEVEL7
#define BSP_I2S1_SDI_ALTERNATE      GPIO_FUNCTION_7
#define BSP_I2S1_SDI_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_I2S1_SDO_GPIO           GPIOB
#define BSP_I2S1_SDO_PIN            GPIO_PIN_5
#define BSP_I2S1_SDO_MODE           GPIO_MODE_AF_PP
#define BSP_I2S1_SDO_PULL           GPIO_PULLUP
#define BSP_I2S1_SDO_DRIVE          GPIO_DRIVE_LEVEL7
#define BSP_I2S1_SDO_ALTERNATE      GPIO_FUNCTION_7
#define BSP_I2S1_SDO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_I2C4_SCL_GPIO           GPIOD
#define BSP_I2C4_SCL_PIN            GPIO_PIN_12
#define BSP_I2C4_SCL_MODE           GPIO_MODE_AF_OD
#define BSP_I2C4_SCL_PULL           GPIO_PULLUP
#define BSP_I2C4_SCL_DRIVE          GPIO_DRIVE_LEVEL7
#define BSP_I2C4_SCL_ALTERNATE      GPIO_FUNCTION_4
#define BSP_I2C4_SCL_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define BSP_I2C4_SDA_GPIO           GPIOD
#define BSP_I2C4_SDA_PIN            GPIO_PIN_13
#define BSP_I2C4_SDA_MODE           GPIO_MODE_AF_OD
#define BSP_I2C4_SDA_PULL           GPIO_PULLUP
#define BSP_I2C4_SDA_DRIVE          GPIO_DRIVE_LEVEL7
#define BSP_I2C4_SDA_ALTERNATE      GPIO_FUNCTION_4
#define BSP_I2C4_SDA_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()



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



/******************************************************************************
*@brief : Initialize the I2S MSP.
*@param : hi2s: pointer to a I2S_HandleTypeDef structure that contains
*               the configuration information for I2S module.
*@return: None
******************************************************************************/
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    
    assert_param(hi2s != NULL);
    assert_param(IS_I2S_INSTANCE(hi2s->Instance));
    
    if (hi2s->Instance == I2S1)
    {
        __HAL_RCC_I2S1_CLK_ENABLE();
        BSP_I2S1_MCK_CLK_ENABLE();
        BSP_I2S1_CK_CLK_ENABLE();
        BSP_I2S1_WS_CLK_ENABLE();
        BSP_I2S1_SDI_CLK_ENABLE();
        BSP_I2S1_SDO_CLK_ENABLE();
        
        GPIO_InitStruct.Pin       = BSP_I2S1_MCK_PIN;
        GPIO_InitStruct.Mode      = BSP_I2S1_MCK_MODE;
        GPIO_InitStruct.Pull      = BSP_I2S1_MCK_PULL;
        GPIO_InitStruct.Drive     = BSP_I2S1_MCK_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2S1_MCK_ALTERNATE;
        HAL_GPIO_Init(BSP_I2S1_MCK_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = BSP_I2S1_CK_PIN;
        GPIO_InitStruct.Mode      = BSP_I2S1_CK_MODE;
        GPIO_InitStruct.Pull      = BSP_I2S1_CK_PULL;
        GPIO_InitStruct.Drive     = BSP_I2S1_CK_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2S1_CK_ALTERNATE;
        HAL_GPIO_Init(BSP_I2S1_CK_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = BSP_I2S1_WS_PIN;
        GPIO_InitStruct.Mode      = BSP_I2S1_WS_MODE;
        GPIO_InitStruct.Pull      = BSP_I2S1_WS_PULL;
        GPIO_InitStruct.Drive     = BSP_I2S1_WS_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2S1_WS_ALTERNATE;
        HAL_GPIO_Init(BSP_I2S1_WS_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = BSP_I2S1_SDI_PIN;
        GPIO_InitStruct.Mode      = BSP_I2S1_SDI_MODE;
        GPIO_InitStruct.Pull      = BSP_I2S1_SDI_PULL;
        GPIO_InitStruct.Drive     = BSP_I2S1_SDI_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2S1_SDI_ALTERNATE;
        HAL_GPIO_Init(BSP_I2S1_SDI_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = BSP_I2S1_SDO_PIN;
        GPIO_InitStruct.Mode      = BSP_I2S1_SDO_MODE;
        GPIO_InitStruct.Pull      = BSP_I2S1_SDO_PULL;
        GPIO_InitStruct.Drive     = BSP_I2S1_SDO_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2S1_SDO_ALTERNATE;
        HAL_GPIO_Init(BSP_I2S1_SDO_GPIO, &GPIO_InitStruct);
        
        __HAL_RCC_I2S1_RESET();
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();
        
        DMA1CH0_Handle.Instance         = DMA1_Channel0;
        DMA1CH0_Handle.Init.Mode        = DMA_MODE_NORMAL;
        DMA1CH0_Handle.Init.ReqID       = DMA1_REQ_I2S1_TX;
        DMA1CH0_Handle.Init.DataFlow    = DMA_DATAFLOW_M2P;
        DMA1CH0_Handle.Init.SrcIncDec   = DMA_SRCINCDEC_INC;
        DMA1CH0_Handle.Init.DestIncDec  = DMA_DESTINCDEC_DISABLE;
        DMA1CH0_Handle.Init.SrcWidth    = DMA_SRCWIDTH_WORD;
        DMA1CH0_Handle.Init.DestWidth   = DMA_DESTWIDTH_WORD;
        DMA1CH0_Handle.Init.SrcBurst    = DMA_SRCBURST_1;
        DMA1CH0_Handle.Init.DestBurst   = DMA_DESTBURST_1;
        DMA1CH0_Handle.Init.SrcMaster   = DMA_SRCMASTER_1;
        DMA1CH0_Handle.Init.DestMaster  = DMA_DESTMASTER_1;
        DMA1CH0_Handle.Init.Lock        = ENABLE;
        HAL_DMA_Init(&DMA1CH0_Handle);
        __HAL_LINKDMA(hi2s, hdmactx, DMA1CH0_Handle);
        
        DMA2CH0_Handle.Instance         = DMA2_Channel0;
        DMA2CH0_Handle.Init.Mode        = DMA_MODE_NORMAL;
        DMA2CH0_Handle.Init.ReqID       = DMA2_REQ_I2S1_RX;
        DMA2CH0_Handle.Init.DataFlow    = DMA_DATAFLOW_P2M;
        DMA2CH0_Handle.Init.SrcIncDec   = DMA_SRCINCDEC_DISABLE;
        DMA2CH0_Handle.Init.DestIncDec  = DMA_DESTINCDEC_INC;
        DMA2CH0_Handle.Init.SrcWidth    = DMA_SRCWIDTH_WORD;
        DMA2CH0_Handle.Init.DestWidth   = DMA_DESTWIDTH_WORD;
        DMA2CH0_Handle.Init.SrcBurst    = DMA_SRCBURST_1;
        DMA2CH0_Handle.Init.DestBurst   = DMA_DESTBURST_1;
        DMA2CH0_Handle.Init.SrcMaster   = DMA_SRCMASTER_2;
        DMA2CH0_Handle.Init.DestMaster  = DMA_DESTMASTER_2;
        DMA2CH0_Handle.Init.Lock        = ENABLE;
        HAL_DMA_Init(&DMA2CH0_Handle);
        __HAL_LINKDMA(hi2s, hdmacrx, DMA2CH0_Handle);
        
        NVIC_ClearPendingIRQ(I2S1_IRQn);
        NVIC_SetPriority(I2S1_IRQn, 0x00);
        NVIC_EnableIRQ(I2S1_IRQn);
        
        NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
        NVIC_SetPriority(DMA1_CH0_IRQn, 0x00);
        NVIC_EnableIRQ(DMA1_CH0_IRQn);
        
        NVIC_ClearPendingIRQ(DMA2_CH0_IRQn);
        NVIC_SetPriority(DMA2_CH0_IRQn, 0x00);
        NVIC_EnableIRQ(DMA2_CH0_IRQn);
    }
}

/******************************************************************************
*@brief : DeInitialize the I2S MSP.
*@param : hi2s: pointer to a I2S_HandleTypeDef structure that contains
*               the configuration information for I2S module.
*@return: None
******************************************************************************/
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
    assert_param(hi2s != NULL);
    assert_param(IS_I2S_INSTANCE(hi2s->Instance));
    
    if (hi2s->Instance == I2S1)
    {
        __HAL_RCC_I2S2_CLK_DISABLE();
        
        NVIC_DisableIRQ(I2S1_IRQn);
        NVIC_ClearPendingIRQ(I2S1_IRQn);
        
        NVIC_DisableIRQ(DMA1_CH0_IRQn);
        NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
        
        NVIC_DisableIRQ(DMA2_CH0_IRQn);
        NVIC_ClearPendingIRQ(DMA2_CH0_IRQn);
        
        HAL_GPIO_DeInit(BSP_I2S1_MCK_GPIO, BSP_I2S1_MCK_PIN);
        HAL_GPIO_DeInit(BSP_I2S1_CK_GPIO, BSP_I2S1_CK_PIN);
        HAL_GPIO_DeInit(BSP_I2S1_WS_GPIO, BSP_I2S1_WS_PIN);
        HAL_GPIO_DeInit(BSP_I2S1_SDI_GPIO, BSP_I2S1_SDI_PIN);
        HAL_GPIO_DeInit(BSP_I2S1_SDO_GPIO, BSP_I2S1_SDO_PIN);
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{    
    GPIO_InitTypeDef  GPIO_InitStruct = {0};
    
    if (hi2c->Instance == I2C4)
    {
        __HAL_RCC_I2C4_CLK_ENABLE();
        BSP_I2C4_SCL_CLK_ENABLE();
        BSP_I2C4_SDA_CLK_ENABLE();
        
        GPIO_InitStruct.Pin       = BSP_I2C4_SCL_PIN;
        GPIO_InitStruct.Mode      = BSP_I2C4_SCL_MODE;
        GPIO_InitStruct.Pull      = BSP_I2C4_SCL_PULL;
        GPIO_InitStruct.Drive     = BSP_I2C4_SCL_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2C4_SCL_ALTERNATE;
        HAL_GPIO_Init(BSP_I2C4_SCL_GPIO, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = BSP_I2C4_SDA_PIN;
        GPIO_InitStruct.Mode      = BSP_I2C4_SDA_MODE;
        GPIO_InitStruct.Pull      = BSP_I2C4_SDA_PULL;
        GPIO_InitStruct.Drive     = BSP_I2C4_SDA_DRIVE;
        GPIO_InitStruct.Alternate = BSP_I2C4_SDA_ALTERNATE;
        HAL_GPIO_Init(BSP_I2C4_SDA_GPIO, &GPIO_InitStruct);

        NVIC_ClearPendingIRQ(I2C4_IRQn);
        NVIC_SetPriority(I2C4_IRQn, 0x00);
        NVIC_EnableIRQ(I2C4_IRQn);
    }
}

/******************************************************************************
*@brief : DeInitialize the I2S MSP.
*@param : hi2s: pointer to a I2S_HandleTypeDef structure that contains
*               the configuration information for I2S module.
*@return: None
******************************************************************************/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C4)
    {
        __HAL_RCC_I2S2_CLK_DISABLE();
        
        NVIC_DisableIRQ(I2C1_IRQn);
        NVIC_ClearPendingIRQ(I2C1_IRQn);
        
        HAL_GPIO_DeInit(BSP_I2C4_SCL_GPIO, BSP_I2C4_SCL_PIN);
        HAL_GPIO_DeInit(BSP_I2C4_SDA_GPIO, BSP_I2C4_SDA_PIN);
    }
}
