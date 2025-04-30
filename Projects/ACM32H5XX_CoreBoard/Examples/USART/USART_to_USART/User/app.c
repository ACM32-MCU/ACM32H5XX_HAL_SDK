/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

uint8_t TxBuffer[512];
uint8_t RxBuffer[512];

USART_HandleTypeDef  USART2_Handle;
UART_HandleTypeDef  UART2_Handle;

void printf_buff_byte(uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n");
}
/******************************************************************************
*@brief : UART1 config and init
*@param : none
*@return: none
*@note  : CPOL,CPHA = 0,0 => SPI workmode0
          CPOL,CPHA = 0,1 => SPI workmode1
          CPOL,CPHA = 1,0 => SPI workmode2
          CPOL,CPHA = 1,1 => SPI workmode3
          USART is LSB, so config SPI first_bit = SPI_FIRSTBIT_LSB
******************************************************************************/
void USART2_Init(void) 
{
    USART2_Handle.Instance        = USART2;    
    USART2_Handle.Init.BaudRate   = 115200; 
    USART2_Handle.Init.WordLength = USART_WORDLENGTH_8B;
    USART2_Handle.Init.StopBits   = UART_STOPBITS_1;
    USART2_Handle.Init.Parity     = USART_PARITY_NONE;
    USART2_Handle.Init.Mode       = USART_MODE_TX_RX;
    USART2_Handle.Init.CLKPhase   = USART_PHASE_1EDGE;
    USART2_Handle.Init.CLKPolarity= USART_POLARITY_LOW;
    USART2_Handle.Init.CLKLastBit = USART_LASTBIT_ENABLE;
    USART2_Handle.Init.MSBFirst   = USART_MSBFIRST_DISABLE;
    
    HAL_USART_Init(&USART2_Handle);  
}


void APP_Test(void)
{
    #define TEST_CNTS   ( 10000 )
    #define TRANS_LEN   (256)
    
	uint8_t ret;
    uint32_t i = 1;
    uint32_t n = 0;
    uint32_t error_cnts = 0;


    printfS("----------Two USARTs Communication Demo ----------\r\n");
    
    USART2_Init();

    for(int i = 0; i < TRANS_LEN; i++)
        TxBuffer[i] = 10 + i;

    for(; i <= TEST_CNTS; ++i)
    {
        HAL_USART_Transmit(&USART2_Handle, TxBuffer, TRANS_LEN, HAL_MAX_DELAY);

        HAL_Delay(300);

        memset(RxBuffer, 0, sizeof(RxBuffer));
        
        HAL_USART_Receive(&USART2_Handle, RxBuffer, TRANS_LEN, HAL_MAX_DELAY);
        
        for(n = 0; n < TRANS_LEN; n++)
        {
            if(TxBuffer[n] != RxBuffer[n])               
            {
                printfS("error @ %d \r\n", n);
                printf_buff_byte(RxBuffer, TRANS_LEN);
                
                if((++error_cnts) > 5){
                    break;
                }
            }
        }

        printfS("%d ",i);
        
        memset(RxBuffer, 0, TRANS_LEN);
        
        HAL_Delay(300);

    }

    if(error_cnts>0)
    {
        printfS("test over, error counts:%d ",error_cnts);
    }
    else
    {
        printfS("test pass");   
    }                        

}
