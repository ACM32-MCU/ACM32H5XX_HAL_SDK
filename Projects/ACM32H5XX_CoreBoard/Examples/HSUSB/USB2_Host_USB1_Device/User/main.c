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
#include "usbd_core.h"
#include "usbd_def.h"  



USBH_HandleTypeDef USB2_Host; 
USBD_HandleTypeDef USBD_Device; 



void USBD_Setup_Process(PCD_HandleTypeDef *p_pcd)  
{
    
}

UINT8 USB_HOST_Init(USBH_HandleTypeDef *phost )
{
	USBH_Init(phost, phost->id);   

	return 0; 
}


uint8_t USB2_USB1_Host_Slave_Test(void)
{
    volatile uint32_t * p_reg;  
    volatile uint32_t reg_data, temp, frame_num1, frame_num2, frame_num3;  
    volatile uint32_t i, dwt1, dwt2, dwt3;   
    
    printfS("\r\nUSB2_USB1_Host_Slave_Test, Start\r\n");  
    
    __HAL_RCC_USB2_CLK_ENABLE();  
    __HAL_RCC_USB1_CLK_ENABLE();    
    
    __HAL_RCC_SYSCFG_CLK_ENABLE();  
    
    HAL_SimpleDelay(2);  
                      
    USBOTG2_PHYC->CR = (BIT0 | BIT5 | BIT4);   // USB2 Use default configure, 12M XTH, PLL, OTG      
    USBOTG1_PHYC->CR = (BIT0 | BIT5 | BIT4);  
    
    HAL_Delay(2);
    
    USB2_Host.id = USB_HS_OTG_ID2;  // USB2_DM PA11, USB2_DP PA12;  USB1_DM PB14, USB1_DP PB15  
    
    USB_HOST_Init(&USB2_Host); 

	USBH_Start(&USB2_Host);  
    
    USBD_Device.id = USB_HS_OTG_ID1;  
    
    USBD_LL_Init(&USBD_Device);   
    
    USBD_Start(&USBD_Device);     
    
    p_reg = (uint32_t *) ( ( (uint32_t)USB_OTG_HS2) + 0x440);  // HPRT 
    
    for(i = 0; i < 3; i++)
    {
        if (*p_reg & BIT1)  // Device detection 
        {
            break; 
        }
        
        HAL_Delay(2);   
    }
    
    if (i >= 3)
    {
        printfS("Device Detection Failed, 0x%x, 0x%x\n", (uint32_t)p_reg, *p_reg);  
        return 1;  
    }
    else
    {
        printfS("Device Detection Successful\n");  
    }
    
    HAL_Delay(5);   
    USBH_LL_ResetPort(&USB2_Host);  
    
    for(i = 0; i < 3; i++)
    {
        if (*p_reg & BIT2)  // Port Enable   
        {
            break; 
        }
        
        HAL_Delay(2);   
    }
    
    if (i >= 3)
    {
        printfS("Bus Reset Failed\n");  
        return 2;  
    }
    else
    {
        printfS("Bus Reset Successful\n");    
    }
    
    reg_data = USBH_LL_GetSpeed(&USB2_Host);  
    
    if (USBH_SPEED_HIGH == reg_data)
    {
        printfS("High Speed, PASS\n");  
    }
    else
    {
        printfS("Not High Speed, FAIL\n");  
        return 3;     
    }
        
    p_reg = (uint32_t *) ( ( (uint32_t)USB_OTG_HS2) + 0x808);  // Device Status  
     
    __set_PRIMASK(1);
    i = 0;
    while (0 == (USB_OTG_HS1->GINTSTS & BIT3) )
    {
        i++;       
        if (i > 80000)
        {
            printfS("Timeout, %d\n", i);  
            return 4;  
        }
    }
    
    USB_OTG_HS1->GINTSTS = BIT3; 
    
    i = 0;
    while (0 == (USB_OTG_HS1->GINTSTS & BIT3) )
    {
        i++;       
        if (i > 80000)
        {
            printfS("Timeout, %d\n", i);  
            return 4;  
        }
    }
    
    dwt1 = DWT->CYCCNT; 
    frame_num1 = (*p_reg >> 8) & 0x3FFF;   
    USB_OTG_HS1->GINTSTS = BIT3; 
    
    i = 0;
    while (0 == (USB_OTG_HS1->GINTSTS & BIT3) )
    {
        i++;       
        if (i > 80000)
        {
            printfS("Timeout, %d\n", i);  
            return 4;  
        }
    }
    
    dwt2 = DWT->CYCCNT; 
    frame_num2 = (*p_reg >> 8) & 0x3FFF;   
    USB_OTG_HS1->GINTSTS = BIT3; 
    
    i = 0;
    while (0 == (USB_OTG_HS1->GINTSTS & BIT3) )
    {
        i++;       
        if (i > 80000)
        {
            printfS("Timeout, %d\n", i);  
            return 4;  
        }
    }
    
    dwt3 = DWT->CYCCNT;  
    frame_num3 = (*p_reg >> 8) & 0x3FFF; 
    
    __set_PRIMASK(0);
    
    printfS("frame_num1:%d, frame_num2:%d, frame_num3:%d\n",  frame_num1, frame_num2, frame_num3);    
    
    if ( (0 == frame_num1) && (0 == frame_num2) && (0 == frame_num3) )
    {
        return 5;  
    }
    
    if ( ( (frame_num2 - frame_num1) > 5) || ( (frame_num3 - frame_num2) > 5 ) ) 
    {
        return 6;   
    }      
    
    printfS("dwt1:%u, dwt2:%u, dwt3:%u\n",  dwt1, dwt2, dwt3);  
    printfS("delta1:%u, delta2:%u\n", dwt2 - dwt1, dwt3 - dwt2);      
    
     p_reg = (uint32_t *) ( ( (uint32_t)USB_OTG_HS2) + 0x440); 
     *p_reg = *p_reg | BIT7;  // Port suspend 
     HAL_Delay(5);  
     *p_reg = *p_reg & (~BIT12);   
     
    __HAL_RCC_USB2_CLK_DISABLE();  
    __HAL_RCC_USB1_CLK_DISABLE();      
    
     HAL_Delay(1); 
     
    __HAL_RCC_USB2_RESET(); 
    
    __HAL_RCC_USB1_RESET();   
    
    printfS("USB2_USB1_Host_Slave_Test, PASS\n");  
    
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
    HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);         
    HAL_DWT_Init(); 
    
    BSP_UART_Init(USART1, 115200); 
    printfS("uart init\n");   
         
	while(1)
	{    
        if (0 != USB2_USB1_Host_Slave_Test() )
        {
            printfS("USB Host Slave Test Error\n");     
            while(1);   
        }
        HAL_Delay(1000); 
	}
}



