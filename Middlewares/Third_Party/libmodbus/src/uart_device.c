#include <stdio.h>
#include <string.h>
#include "hal.h"
#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"
#include "queue.h"

#include "uart_device.h"

#define UART_RECV_BUF 128

extern UART_HandleTypeDef USART_RS485;

typedef struct UART_Data {
	UART_HandleTypeDef *huart;
	uint8_t recv_char[16];
	SemaphoreHandle_t xSendSemaphore;
	QueueHandle_t xRecvQueue;
}UART_Data, *P_UART_Data;

static UART_Data g_uart4_data = {
	&USART_RS485,
};

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart);
extern void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);


void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags;
    uint32_t ieits;
    uint32_t errorflags;
    uint16_t *pbuf_16;

    assert_param(IS_UART_ALL_INSTANCE(huart->Instance));

    isrflags = READ_REG(huart->Instance->ISR);
    ieits    = READ_REG(huart->Instance->IE);
    
    errorflags =(isrflags & (uint32_t)(UART_ISR_PEI | UART_ISR_OEI | UART_ISR_FEI | UART_ISR_BEI));
    
    /* which isr actually occured */
    isrflags &= ieits; 

    /* TXI */
    if (isrflags & UART_ISR_TXI)
    {
        /* Clear TXI Status */
        CLEAR_STATUS(huart->Instance->ISR, UART_ISR_TXI);
        
        pbuf_16 = (uint16_t *)huart->TxData;

        for(;;)
        {
            if((huart->TxCount == huart->TxSize) && (huart->Instance->FR& USART_FR_TXFE))
            {                               
                huart->TxBusy = false;
         
                /* Disable TX interrupt && error interrupt*/
                CLEAR_BIT(huart->Instance->IE, UART_IE_TXI |
                                               UART_IE_OEI | UART_IE_BEI | UART_IE_PEI | UART_IE_FEI); 

                HAL_UART_TxCpltCallback(huart);
                return;
            }

            if (READ_BIT(huart->Instance->FR, UART_FR_TXFF))
            {
                break;
            }
            
            if (huart->Init.WordLength == UART_WORDLENGTH_9B)
            {
                huart->Instance->DR = (uint16_t)(pbuf_16[huart->TxCount++] & 0x01FFU);
            }
            else
            {
                huart->Instance->DR = huart->TxData[huart->TxCount++];
            }
        }
    }

    /* RXI */
    if (isrflags & UART_ISR_RXI) 
    {
        /* Clear RXI Status */
        CLEAR_STATUS(huart->Instance->ISR, UART_ISR_RXI);
        
        pbuf_16 = (uint16_t *)huart->RxData;
                      
        while(huart->RxCount < huart->RxSize )  
        {
            if(!READ_BIT(huart->Instance->FR, UART_FR_RXFE))
            {
                /* Store Data in buffer */
                if (huart->Init.WordLength == UART_WORDLENGTH_9B)
                {
                    pbuf_16[huart->RxCount++] = (uint16_t)(huart->Instance->DR & 0x1FFU);
                }
                else
                {
                    huart->RxData[huart->RxCount++] = (uint8_t)huart->Instance->DR;	
                }                         
            }  
            else
            {
                break;
            }  
        }
        
        if(huart->RxCount == huart->RxSize )
        {
            huart->RxBusy = false;
        
            /* Disable RX and RTI interrupt && error interrupt*/
            CLEAR_BIT(huart->Instance->IE, (UART_IE_RXI | UART_IE_IDLEI |
                                            UART_IE_OEI | UART_IE_BEI | UART_IE_PEI | UART_IE_FEI));
            
            HAL_UART_RxCpltCallback(huart);
       }
    }
    /* IDLEI */
    else if(isrflags & UART_ISR_IDLEI)  
    {
        /*clear IDLE Status */
        CLEAR_STATUS(huart->Instance->ISR, UART_ISR_IDLEI);
        
        pbuf_16 = (uint16_t *)huart->RxData;

        while(!READ_BIT(huart->Instance->FR, UART_FR_RXFE))
        {
            if (huart->Init.WordLength == UART_WORDLENGTH_9B)
            {
                pbuf_16[huart->RxCount++] = (uint16_t)(huart->Instance->DR & 0x1FFU);
            }
            else
            {                    
                huart->RxData[huart->RxCount++] = (uint8_t)huart->Instance->DR;
            }
        }
        
        huart->RxBusy = false;
        
        /* Disable RX and RTI interrupt && error interrupt*/
        CLEAR_BIT(huart->Instance->IE, (UART_IE_RXI | UART_IE_IDLEI |
                                        UART_IE_OEI | UART_IE_BEI | UART_IE_PEI | UART_IE_FEI));

        

        HAL_UART_IdleCallback(huart);
    }
    
    if(isrflags & UART_ISR_TCI)  
    {
        /*clear IDLE Status */
        CLEAR_STATUS(huart->Instance->ISR, UART_ISR_TCI);
    }
    
	/* if some errors occurred */
	if(errorflags != 0)
	{
		/* UART parity error interrupt occurred */
	    if (((errorflags & UART_ISR_PEI) != 0))
	    {
	    	/* Clear parity error status */
            CLEAR_STATUS(huart->Instance->ISR, UART_ISR_PEI);
	      	huart->ErrorCode |= HAL_UART_ERROR_PE;
	    }

	    /* UART break error interrupt occurred */
	    if (((errorflags & UART_ISR_BEI) != 0))
	    {
            CLEAR_STATUS(huart->Instance->ISR, UART_ISR_BEI);
	      	huart->ErrorCode |= HAL_UART_ERROR_NE;
	    }

	    /* UART frame error interrupt occurred */
	    if (((errorflags & UART_ISR_FEI) != 0))
	    {
            CLEAR_STATUS(huart->Instance->ISR, UART_ISR_FEI);
	      	huart->ErrorCode |= HAL_UART_ERROR_FE;
	    }

	    /* UART Over-Run interrupt occurred */
	    if (((errorflags & UART_ISR_OEI) != 0))
	    {
            CLEAR_STATUS(huart->Instance->ISR, UART_ISR_OEI);
	      	huart->ErrorCode |= HAL_UART_ERROR_ORE;
	    }

        HAL_UART_ErrorCallback(huart);
	}    
}


HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *buf, uint32_t size)
{
    uint16_t *pbuf_16;
    
    assert_param (IS_UART_ALL_INSTANCE(huart->Instance));
    
    if (huart->TxBusy == true) 
    {
        return HAL_BUSY;
    }
    
    if (size == 0 || buf == NULL) 
    {
        return HAL_ERROR;
    }
    
    huart->TxSize  = size;
    huart->TxCount = 0;
    huart->TxData   = buf;
    huart->TxBusy   = true;
    pbuf_16 = (uint16_t *)huart->TxData;
    
    SET_BIT(huart->Instance->CR1, UART_CR1_TXE);
    
    if (huart->Init.Mode == UART_MODE_HALF_DUPLEX) 
    {
        /* disable RX in half-duplex mode */
        CLEAR_BIT(huart->Instance->CR1, UART_CR1_RXE);
    }

    /* Clear TXI Status */
    CLEAR_STATUS(huart->Instance->ISR, UART_ISR_TXI);
    /* FIFO Enable */
//    SET_BIT(huart->Instance->CR3, UART_CR3_FEN);
//	/*FIFO Select*/
//    __HAL_UART_TXI_FIFO_LEVEL_SET(huart->Instance, UART_TX_FIFO_1_2);
    
    for(;;) 
    {	
        /*Data Size less than 16Byte */
	    if(size == huart->TxCount) 
	    {
	        huart->TxBusy = false;
			
			while ((huart->Instance->FR & UART_FR_BUSY)){}
			
            HAL_UART_TxCpltCallback(huart);
			
	        return HAL_OK;  
	    }
	    if(READ_BIT(huart->Instance->FR, UART_FR_TXFF)) 
	    {				
	         break;
	    }

        if (huart->Init.WordLength == UART_WORDLENGTH_9B)
        {
            huart->Instance->DR = (uint16_t)(pbuf_16[huart->TxCount++] & 0x01FFU);
        }
        else
        {
            huart->Instance->DR = huart->TxData[huart->TxCount++];
        }       
    }
	
    /* Enable TX interrupt */
    SET_BIT(huart->Instance->IE, UART_IE_TXI);

    return HAL_OK;
}


HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *buf, uint32_t size)
{
    uint16_t *pbuf_16;
    
    assert_param (IS_UART_ALL_INSTANCE(huart->Instance));
    
    if (huart->RxBusy == true) 
    {
        return HAL_BUSY;
    }

    if (size == 0 || buf == NULL) 
    {
        return HAL_ERROR;
    }
    
    huart->RxSize  = size;
    huart->RxCount = 0;
    huart->RxData  = buf;
    huart->RxBusy  = true;
    pbuf_16 = (uint16_t *)huart->RxData;
    
    SET_BIT(huart->Instance->CR1, UART_CR1_RXE);
    
//    /* FIFO Enable */
//    SET_BIT(huart->Instance->CR3, UART_CR3_FEN);
//	/*FIFO Select*/
//    __HAL_UART_RXI_FIFO_LEVEL_SET(huart->Instance, UART_RX_FIFO_1_4);
    
    /* Clear RXI && IDLEI Status */
    CLEAR_STATUS(huart->Instance->ISR, UART_ISR_RXI | UART_ISR_IDLEI);
    CLEAR_STATUS(huart->Instance->ISR, UART_ISR_PEI | UART_ISR_BEI | UART_ISR_OEI | UART_ISR_FEI);

    /* In case there are datas(which exceeded the FIFO LEVEL of triggering RXI ISR) already in UART FIFO before enabling RXI IE */
//    while (size--) 
//    {
//        if(huart->Instance->FR & UART_FR_RXFE)
//        {
//            goto start_recv_it;
//        }  
//        if (huart->Init.WordLength == UART_WORDLENGTH_9B)
//        {
//            pbuf_16[huart->RxCount++] = (uint16_t)(huart->Instance->DR & 0x1FFU);
//        }
//        else
//        {
//            huart->RxData[huart->RxCount++] = (uint8_t)huart->Instance->DR;	
//        }     
//    }
//    
//    /* have been received all the size of data */
//    HAL_UART_RxCpltCallback(huart);
//    return HAL_OK;   
    
start_recv_it:    
	/* Enable the UART Errors interrupt */
	SET_BIT(huart->Instance->IE, UART_IE_OEI | UART_IE_BEI | UART_IE_PEI | UART_IE_FEI);
    /* Enable RX and RTI interrupt */
    SET_BIT(huart->Instance->IE, UART_IE_RXI | UART_IE_IDLEI); 
    
    return HAL_OK;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 释放信号量 */
	P_UART_Data pData;
	/* 把数据放入队列 */
	if (huart == &USART_RS485)
	{
		pData = &g_uart4_data;
		xSemaphoreGiveFromISR(pData->xSendSemaphore, NULL);
	}	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 	P_UART_Data pData;
	/* 把数据放入队列 */
	if (huart == &USART_RS485)
	{
		pData = &g_uart4_data;
		xQueueSendFromISR(pData->xRecvQueue, &pData->recv_char, NULL);
		
		/* re-enable rxne interrupt */
		UART_Receive_IT(pData->huart, pData->recv_char, 1);

	}	
}

void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
	P_UART_Data pData;
	uint8_t i = 0;
	if (huart == &USART_RS485)
	{
		pData = &g_uart4_data;
		
		while(i<huart->RxCount)
		{
			xQueueSendFromISR(pData->xRecvQueue, &pData->recv_char[i++], NULL);
		}
		/* re-enable rxne interrupt */
		UART_Receive_IT(pData->huart, pData->recv_char, 1);

	}	
}

int uart_init( struct UART_Device *pDev, int baud, char parity, int data_bit, int stop_bit)
{
 	P_UART_Data pData = pDev->priv_data;
	
	if (!pData->xSendSemaphore)
	{
		pData->xSendSemaphore = xSemaphoreCreateBinary( );
		pData->xRecvQueue = xQueueCreate(UART_RECV_BUF, 1);
	}

	/* 使能接收中断 */
	/* enable rxne interrupt */
	UART_Receive_IT(pData->huart, pData->recv_char, 1);
	
    return 0;
}

int uart_send( struct UART_Device *pDev, uint8_t *datas, uint32_t len, int timeout)
{
 	P_UART_Data pData = pDev->priv_data;
    if (HAL_OK == UART_Transmit_IT(pData->huart, datas, len))  /* 触发"发送空中断",并不表示数据已经发送完毕 */
   	{
   		/* 等待信号量 */
		if (pdTRUE == xSemaphoreTake(pData->xSendSemaphore, timeout))			
	        return 0;
		else
			return -1;
	}
    else
        return -1;
}

int uart_recv( struct UART_Device *pDev, uint8_t *data, int timeout)
{
 	P_UART_Data pData = pDev->priv_data;

	/* 读队列 */
	if (pdPASS == xQueueReceive(pData->xRecvQueue, data, timeout))
		return 0;
	else
		return -1;
}

int uart_flush(struct UART_Device *pDev)
{
 	P_UART_Data pData = pDev->priv_data;
	uint8_t data;
	int i = 0;

	/* 读队列 */
	while (pdPASS == xQueueReceive(pData->xRecvQueue, &data, 0))
	{
		i++;
	}
	
    return i;
}


static struct UART_Device g_uart4_dev = {"uart4", uart_init, uart_send, uart_recv, uart_flush, &g_uart4_data};


static struct UART_Device *g_uart_devices[] = {&g_uart4_dev};


struct UART_Device *GetUARTDevice(char *name)
{
	int i = 0;
	for (i = 0; i < sizeof(g_uart_devices)/sizeof(g_uart_devices[0]); i++)
	{
		if (!strcmp(name, g_uart_devices[i]->name))		
			return g_uart_devices[i];
	}
	
	return NULL;
}
