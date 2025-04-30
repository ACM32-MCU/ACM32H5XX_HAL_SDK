/******************************************************************************
*@file  : app.c
*@brief : UART Demos 
******************************************************************************/
#include "app.h"

uint8_t TxBuffer[512];
uint8_t RxBuffer[512];

USART_HandleTypeDef  USART1_Handle;
UART_HandleTypeDef  UART2_Handle;


#define CS_PORT (GPIOA)
#define CS_PIN (GPIO_PIN_12)

#define __CS_HIGH()     {HAL_Delay(5); HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);} 

#define __CS_LOW()      {HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); HAL_Delay(5);}

void USART_SPI_CSpin_Init()
{
    GPIO_InitTypeDef GPIO_Handle = {0}; 
    GPIO_Handle.Pin = CS_PIN;
    GPIO_Handle.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull = GPIO_PULLUP;
    GPIO_Handle.Drive = GPIO_DRIVE_LEVEL3;
    HAL_GPIO_Init(CS_PORT, &GPIO_Handle);
    
    __CS_HIGH();
}

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
void USART1_Init(void) 
{
    USART1_Handle.Instance        = USART1;    
    USART1_Handle.Init.BaudRate   = 115200; 
    USART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    USART1_Handle.Init.StopBits   = UART_STOPBITS_1;
    USART1_Handle.Init.Parity     = UART_PARITY_NONE;
    USART1_Handle.Init.Mode       = UART_MODE_TX_RX;
    USART1_Handle.Init.CLKPhase   = USART_PHASE_2EDGE;//USART_PHASE_1EDGE;
    USART1_Handle.Init.CLKPolarity = USART_POLARITY_HIGH;//USART_POLARITY_LOW;
    USART1_Handle.Init.CLKLastBit = USART_LASTBIT_ENABLE;//USART_LASTBIT_DISABLE;//USART_LASTBIT_ENABLE;
    USART1_Handle.Init.MSBFirst   = USART_MSBFIRST_DISABLE;
    
    HAL_USART_Init(&USART1_Handle);  
}


void APP_Test(void)
{
    #define TEST_CNTS   ( 10000 )
    #define TRANS_LEN   (256)
    
	uint8_t ret;
    uint32_t i = 1;
    uint32_t error_cnts = 0;


    printfS("----------USART Communicate to SPI Demo ----------\r\n");
    USART1_Init();
    USART_SPI_CSpin_Init();

    //HAL_Delay(2000);

    for(int i = 0; i < TRANS_LEN; i++)
        TxBuffer[i] = 1 + i;

    for(; i <= TEST_CNTS; ++i)
    {
        __CS_LOW();
        HAL_USART_Transmit(&USART1_Handle, TxBuffer, TRANS_LEN, HAL_MAX_DELAY);
        __CS_HIGH();
        
        HAL_Delay(100); 
        __CS_LOW();

        HAL_USART_Receive(&USART1_Handle, RxBuffer, TRANS_LEN, HAL_MAX_DELAY);
        __CS_HIGH();
        
        if(memcmp(TxBuffer, RxBuffer, TRANS_LEN) != 0)               
        {
            printfS("error cnts %d \r\n", i);
            printf_buff_byte(RxBuffer, TRANS_LEN);
            
            if((++error_cnts) > 5){
                break;
            }
        }
        else{
            if(USART1_Handle.RxCount != TRANS_LEN)
                printfS("[@%d] ",i);
            else
                printfS("%d ",i);
        }
        memset(RxBuffer, 0, TRANS_LEN);

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
