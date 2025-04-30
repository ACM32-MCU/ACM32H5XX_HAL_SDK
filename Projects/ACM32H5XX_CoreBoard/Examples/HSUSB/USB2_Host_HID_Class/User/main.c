/***********************************************************************
 * Copyright (c)  2024,Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.

 * Filename    : main.c
 * Description : USB Host demo
 * Author(s)   : xwl     
 * version     : 1.0
 * Modify date : 2024-08-16    
 ***********************************************************************/
#include "app.h" 
#include "usbh_core.h"
#include "usbh_hid_class.h"  


USBH_HandleTypeDef USB2_Host; 

uint8_t * p_uart_receive_buffer = NULL;  
uint8_t * p_uart_send_buffer = NULL; 

volatile uint32_t uart_rx_count = 0; 
volatile uint32_t usb_send_count = 0; 

volatile uint32_t usb_out_process = 0;
volatile uint32_t usb_in_process = 0;


// Receive data with timeout  
void UART_IRQHander(UART_TypeDef * uart_inst) 
{   
    volatile uint32_t timeout = 0;  
    volatile uint8_t temp; 
	
	if ((uart_inst->ISR & UART_RX_INT) || (uart_inst->ISR & UART_IDLE_STATE) ) // received data 
	{	
        if (NULL == p_uart_receive_buffer) // receive buffer is allocated after USB Enumration 
        {
            uart_inst->ISR = UART_RX_INT | UART_IDLE_STATE;  
            if (0 == (uart_inst->FR & UART_RX_FIFO_EMPTY) ) 
            {
                temp = uart_inst->DR&0xff; // readout unwanted data 
                if (0 == temp)
                {
                    __NOP();
                }
                else
                {
                    __NOP();
                    __NOP();  
                }
            }
            NVIC_ClearPendingIRQ(USART1_IRQn);
        }
        p_uart_receive_buffer[0] = 0;        
		while(!(uart_inst->FR & UART_RX_FIFO_EMPTY))  //Rx fifo not empty
		{
			if (FRAME_HEAD == (uart_inst->DR&0xff) )    
			{
				p_uart_receive_buffer[0] = FRAME_HEAD;  
				uart_rx_count = 1;  
				break;   
			}
		}
			
		if( (0 != uart_rx_count) && (FRAME_HEAD == p_uart_receive_buffer[0] )) // find header 
		{
			while(1)
            {
                timeout++; 
                
                if (0 == (uart_inst->FR & UART_RX_FIFO_EMPTY) ) 
                {
                    p_uart_receive_buffer[uart_rx_count] = uart_inst->DR&0xff; 
                    uart_rx_count++;  
                    timeout = 0;   
                }
                
                if (timeout >= 10000)
                {
                    break;   
                }
            }             
            
            uart_inst->IE = 0;  
            NVIC_ClearPendingIRQ(USART1_IRQn);
		}
		
		uart_inst->ISR = UART_RX_INT | UART_IDLE_STATE;  
	}
}


void printf_buff_byte(volatile uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n"); 
}


void USBH_Set_Out_State(USBH_HandleTypeDef *phost, OP_OUT_StateTypeDef state); 
OP_OUT_StateTypeDef USBH_Get_Out_State(USBH_HandleTypeDef *phost);  
void USBH_Set_In_State(USBH_HandleTypeDef *phost, OP_IN_StateTypeDef state);  
OP_IN_StateTypeDef USBH_Get_In_State(USBH_HandleTypeDef *phost);  
USBH_StatusTypeDef USBH_Bulk_Write_Data(USBH_HandleTypeDef *phost, uint32_t length);
USBH_URBStateTypeDef USBH_Bulk_Read_Data(USBH_HandleTypeDef *phost, uint32_t length);   

void uart_monitor(void)
{	
	if(uart_rx_count)
	{	
        usb_out_process = 1;   
        USBH_Set_Out_State(&USB2_Host, OP_DATA_OUT);   
        usb_send_count = uart_rx_count;  
		uart_rx_count = 0;   
	}
    
    // start OUT Transfer  
    if (usb_out_process)
    {
        USBH_Bulk_Write_Data(&USB2_Host, usb_send_count);     
        // Check if OUT Transfer is done 
        if (OP_DATA_OUT_DONE == USBH_Get_Out_State(&USB2_Host) )
        {
            USBH_Set_Out_State(&USB2_Host, OP_OUT_NOTREADY);       
            USBH_Set_In_State(&USB2_Host, OP_DATA_IN);    
            usb_out_process = 0;  
            usb_in_process = 1; 
        }
    }
    
    // start IN Transfer  
    if (usb_in_process)  
    {
        HID_Class_HandleTypeDef *handler;   
        
        handler= (HID_Class_HandleTypeDef *) USB2_Host.pActiveClass->pData; 
        USBH_Bulk_Read_Data(&USB2_Host, USB_READ_BUFFER_LEN);   
        // Check if IN Transfer is done 
        if (OP_DATA_IN_DONE == USBH_Get_In_State(&USB2_Host) )
        {
            USBH_Set_In_State(&USB2_Host, OP_IN_NOTREADY);      
            printf_buff_byte(p_uart_send_buffer, handler->read_cnt);    
            usb_out_process = 0;  
            usb_in_process = 0; 
            SET_BIT(USART1->IE, UART_IE_RXI | UART_IE_IDLEI);     
        }
    }
    
}


typedef void (*pUSERFunc)(USBH_HandleTypeDef *phost, uint8_t id);

UINT8 USB_HOST_Init(USBH_HandleTypeDef *phost,
				pUSERFunc User_Func )
{
	USBH_Init(phost, User_Func, phost->id);   

	/* Add Supported Class */
	USBH_RegisterClass(phost, USBH_HID_CLASS); 

	NVIC_EnableIRQ((USB_HS_OTG_ID1 == phost->id) ? USBOTG1_IRQn : USBOTG2_IRQn);

	/* Start Host Process */
	USBH_Start(phost);

	return 0; 
}

/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : xwl                        Date : 2024  
**********************************************************************************/
int main(void)
{
    volatile uint32_t timeout;  
    USB_OTG_GlobalTypeDef *USB_Global_Regs;    
    
    HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);      
    
    __HAL_RCC_USB2_CLK_ENABLE();       
    __HAL_RCC_SYSCFG_CLK_ENABLE();  
    
    HAL_SimpleDelay(15);  
                      
    USBOTG2_PHYC->CR = (BIT0 | BIT5 | BIT4);   // USB2 Use default configure, 12M XTH, PLL, OTG      
            
    HAL_Delay(2);   // Wait PHY PLL to be stable        
    
    BSP_UART_Init(USART1, 115200); 
    uart_printf("uart init\n"); 
   
    SET_BIT(USART1->IE, UART_IE_RXI | UART_IE_IDLEI);         
    
    USB2_Host.id = USB_HS_OTG_ID2;
    
    USB_HOST_Init(&USB2_Host, NULL); 
    
	while(1)
	{
        USBH_Process(&USB2_Host);        
        uart_monitor();  
	}
}



