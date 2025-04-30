/***********************************************************************
 * Copyright (c)  2020,Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.

 * Filename    : main.c
 * Description : Modules studying
 * Author(s)   : xwl     
 * version     : 1.0
 * Modify date : 2020-06-15  
 ***********************************************************************/

#include "app.h"  
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_bulk_transfer.h"  

#define UART_BAUD_RATE  115200

extern HAL_StatusTypeDef PCD_EP_OutXfrComplete_int(PCD_HandleTypeDef *hpcd, uint32_t epnum);  


UART_HandleTypeDef UART1_Handle; 

USBD_HandleTypeDef USBD_Device; 
USBD_SetupReqTypedef usb_request;   

uint8_t configration_value, Alt_value; 
uint16_t status_test; 
uint8_t ep_addr_test;  

void USBD_Setup_Process(PCD_HandleTypeDef *p_pcd)   
{  
    uint16_t length;  
    uint8_t  dev_addr_test, ret;   
    
    USBD_ParseSetupRequest(&usb_request, (uint8_t *)p_pcd->Setup);  
    
    switch (usb_request.bmRequest & 0x1FU)
    {   
        case USB_REQ_RECIPIENT_DEVICE:                        
        switch (usb_request.bmRequest & USB_REQ_TYPE_MASK) 
        {                
            case USB_REQ_TYPE_STANDARD:
            switch (usb_request.bRequest)
            {                                              
                case USB_REQ_GET_DESCRIPTOR:  
                            
                switch (usb_request.wValue >> 8)
                {                                                                                              
                    case USB_DESC_TYPE_DEVICE:  
                                                          
                    p_pcd->IN_ep[0].xfer_buff  = (uint8_t *)USBD_UMS_DeviceDescriptor_ex(&length);                      
                                             
                    if (usb_request.wLength <= length)
                    {
                        length = usb_request.wLength;   
                    }
                    USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, length);  
                    USB_PrintfS("GetDevDes\n");      
                    break;  
                            
                    case USB_DESC_TYPE_CONFIGURATION:                                                                                  
                    p_pcd->IN_ep[0].xfer_buff  = (uint8_t *)USBD_UMS_GetCfgDesc_ex(&length);                              
                                                                      
                    if (usb_request.wLength <= length)
                    {
                        length = usb_request.wLength;   
                    }
                            
                    USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, length);    
                    USB_PrintfS("GetConfDes\n");  
                    break;  
                            
                    case USB_DESC_TYPE_STRING:                                        
                    ret = 0;                                      
                    switch ((uint8_t)(usb_request.wValue))
                    {                                                                                                                          
                        case USBD_IDX_LANGID_STR:
                        if (USBD_Device.pDesc->GetLangIDStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetLangIDStrDescriptor(USBD_Device.dev_speed, &length);
                        }
                        break;

                        case USBD_IDX_MFC_STR:
                        if (USBD_Device.pDesc->GetManufacturerStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetManufacturerStrDescriptor(USBD_Device.dev_speed, &length);
                        } 
                        break;

                        case USBD_IDX_PRODUCT_STR:
                        if (USBD_Device.pDesc->GetProductStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetProductStrDescriptor(USBD_Device.dev_speed, &length);
                        }
                        break;

                        case USBD_IDX_SERIAL_STR:
                        if (USBD_Device.pDesc->GetSerialStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetSerialStrDescriptor(USBD_Device.dev_speed, &length);
                        }
                        break;

                        case USBD_IDX_CONFIG_STR:
                        if (USBD_Device.pDesc->GetConfigurationStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetConfigurationStrDescriptor(USBD_Device.dev_speed, &length);
                        }
                        break;

                        case USBD_IDX_INTERFACE_STR:
                        if (USBD_Device.pDesc->GetInterfaceStrDescriptor != NULL)
                        {
                            p_pcd->IN_ep[0].xfer_buff = USBD_Device.pDesc->GetInterfaceStrDescriptor(USBD_Device.dev_speed, &length);
                        }
                        break; 
                                
                        default: 
                        ret = 1; 
                        USBD_CtlError(&USBD_Device, &usb_request);    
                        break;                                 
                    }  
                     USB_PrintfS("GetString\n");                       
                    if (0 == ret) 
                    {
                        if (usb_request.wLength <= length)
                        {
                            length = usb_request.wLength;   
                        }
                        
                        USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, length);    
                    }                                            
                            
                    break;   
                            
                    case USB_DESC_TYPE_DEVICE_QUALIFIER:
                    if (USBD_Device.dev_speed == USBD_SPEED_HIGH)
                    {
                        p_pcd->IN_ep[0].xfer_buff = USBD_Device.pClass->GetDeviceQualifierDescriptor(&length);                                
                          
                        if (usb_request.wLength <= length)
                        {
                            length = usb_request.wLength;   
                        }                        
                        USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, length);  
                    }
                    else
                    {
                        USBD_CtlError(&USBD_Device, &usb_request);     
                    }
                    USB_PrintfS("GetDevQuality\n"); 
                    break;  
                            
                    case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION: 
                    if (USBD_Device.dev_speed == USBD_SPEED_HIGH)
                    {
                        USB_PrintfS("GetOtherSpeedOK:%d\n", length); 
                        p_pcd->IN_ep[0].xfer_buff = USBD_Device.pClass->GetOtherSpeedConfigDescriptor(&length);
                        p_pcd->IN_ep[0].xfer_buff[1] = USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION;
                                
                        if (usb_request.wLength <= length)
                        {
                            length = usb_request.wLength;   
                        }
                        USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, length);    
                        
                    }
                    else
                    {
                        USB_PrintfS("GetOtherSpeedNO\n");  
                        USBD_CtlError(&USBD_Device, &usb_request);                                                                   
                    }
                                        
                    break; 
                                        
                    default:
                    USBD_CtlError(&USBD_Device, &usb_request);    
                    break; 
                }                      
                break;  // USB_REQ_GET_DESCRIPTOR  

                case USB_REQ_SET_ADDRESS:                 
                dev_addr_test = (uint8_t)(usb_request.wValue) & 0x7FU;  
                
                HAL_PCD_SetAddress(p_pcd, dev_addr_test);   
                USB_PrintfS("SetAdr:%d\n", dev_addr_test); 
                USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);   
                 
                USBD_Device.dev_state = USBD_STATE_ADDRESSED;   
                break;  
                        
                case USB_REQ_SET_CONFIGURATION:    
                
                USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);  
                USBD_Device.pClass->Init(&USBD_Device, 0);  // init interrupt EP for HID 
                USB_PrintfS("SetConf:%d\n", usb_request.wValue);  
                configration_value = usb_request.wValue;   
                USBD_Device.dev_state = USBD_STATE_CONFIGURED; 
                break; 
                                    
                case USB_REQ_GET_CONFIGURATION:
                USB_PrintfS("GetConf:%d\n", configration_value);   
                p_pcd->IN_ep[0].xfer_buff = &configration_value; 
                USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, 1);  
                
                break;
                                    
                case USB_REQ_GET_STATUS:
                USB_PrintfS("GetStatus\n");   
                p_pcd->IN_ep[0].xfer_buff = (uint8_t *)&status_test; 
                USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, 2);  
                
                break;  
                                    
                case USB_REQ_CLEAR_FEATURE: 
                USB_PrintfS("ClearFeature\n");  
                USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);  
                break;  
                                    
                case USB_REQ_SET_FEATURE: 
                USB_PrintfS("SetFeature\n");  
                USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);      

                if (0x02 == usb_request.wValue) // Feature select = test mode  
                {
                    USB_SetTestMode(p_pcd->Instance, (usb_request.wIndex >> 8) << 4);                                      
                }                
                break;  
                        
                default:
                USBD_CtlError(&USBD_Device, &usb_request);    
                break; 
            }
            break;
            
            case USB_REQ_TYPE_CLASS:             
            case USB_REQ_TYPE_VENDOR:           
            default: 
            USBD_CtlError(&USBD_Device, &usb_request);    
            break; 
                                
        } // device request 
        break; 
                    
        case USB_REQ_RECIPIENT_INTERFACE: 
        switch (usb_request.bmRequest & USB_REQ_TYPE_MASK)
        {
            case USB_REQ_TYPE_CLASS :
            switch (usb_request.bRequest)
            {
                case GET_MAX_LUN: 
                {
                    uint8_t lun;
                                        
                    lun = 0;  
                    p_pcd->IN_ep[0].xfer_buff = &lun;                              
                                
                    USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, 1);      
                }
                break; 
                                    
                case Bulk_Only_Mass_Storage_Reset: 
                {
                    USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);    
                }
                break;   
                                    
                default:
                USBD_CtlError(&USBD_Device, &usb_request);    
                break; 
             }
             break;  
                         
             case USB_REQ_TYPE_STANDARD:  
             switch (usb_request.bRequest)
             {
                                    
                case USB_REQ_GET_INTERFACE:
                p_pcd->IN_ep[0].xfer_buff = &Alt_value; 
                USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, 1);  
                USB_PrintfS("GetInterface\n");  
                break;
                                    
                case USB_REQ_SET_INTERFACE: 
                Alt_value = (uint8_t)(usb_request.wValue);
                USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);  
                USB_PrintfS("SetInterface\n");  
                break;
                default: 
                USBD_CtlError(&USBD_Device, &usb_request);    
                break; 
            }
            break; 
                                
            default: 
            USBD_CtlError(&USBD_Device, &usb_request);   
            break;                                       
        }
        break;  // USB_REQ_RECIPIENT_INTERFACE  
                    
        case USB_REQ_RECIPIENT_ENDPOINT: 
        switch (usb_request.bRequest)
        {
            USBD_EndpointTypeDef *pep;
                                
            USBD_StatusTypeDef ret = USBD_OK;
            ep_addr_test = usb_request.wIndex;  
                                
            case USB_REQ_SET_FEATURE:                              
            if (usb_request.wValue == USB_FEATURE_EP_HALT)
            {
                if ((ep_addr_test != 0x00U) && (ep_addr_test != 0x80U) )  
                {                                      
                    (void)USBD_LL_StallEP(&USBD_Device, ep_addr_test);  
                    USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0); 
                }
                else
                {
                    (void)USBD_LL_StallEP(&USBD_Device, ep_addr_test);  
                }
                USB_PrintfS("EP-Set Feature, EP:%d\n", ep_addr_test);                                  
            }
            else
            {
                USBD_CtlError(&USBD_Device, &usb_request);   
            }                                
            break;  
                                
            case USB_REQ_CLEAR_FEATURE:   
            ep_addr_test = usb_request.wIndex;  
            if (usb_request.wValue == USB_FEATURE_EP_HALT)
            {
                if ((ep_addr_test & 0x7FU) != 0x00U)
                {  
                    (void)USBD_LL_ClearStallEP(&USBD_Device, ep_addr_test);
                    USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);          
                }
                else
                {
                    USBD_EP0_Send_Data(USBD_Device.pData, NULL, 0);                   
                }
                USB_PrintfS("EP-Clear Feature, EP: %d\n", ep_addr_test); 
                                                   
            }
            else
            {
                USBD_CtlError(&USBD_Device, &usb_request);   
            }
                                                                                               
            break;    
                                
            case USB_REQ_GET_STATUS:  
            ep_addr_test = usb_request.wIndex;  
            pep = ((ep_addr_test & 0x80U) == 0x80U) ? &USBD_Device.ep_in[ep_addr_test & 0x7FU] : &USBD_Device.ep_out[ep_addr_test & 0x7FU];

            if ((ep_addr_test == 0x00U) || (ep_addr_test == 0x80U))
            {
                pep->status = 0x0000U;
            }
            else if (USBD_LL_IsStallEP(&USBD_Device, ep_addr_test) != 0U)
            {
                pep->status = 0x0001U;
            }
            else
            {
                pep->status = 0x0000U;
            }
                                
            if (0 == ep_addr_test)
            {
                pep->status = 0;  
            }

            status_test = pep->status;  
            p_pcd->IN_ep[0].xfer_buff = (uint8_t *)&status_test; 
            USBD_EP0_Send_Data(USBD_Device.pData, p_pcd->IN_ep[0].xfer_buff, 2);  
                                
            USB_PrintfS("EP-GetStatus:%d, 0x%x\n", ep_addr_test, status_test);    
            break;  
        }
        break;   // USB_REQ_RECIPIENT_ENDPOINT                      
        
        default: 
        USBD_CtlError(&USBD_Device, &usb_request);            
    }
                                                 
} 


/*********************************************************************************
* Function    : main
* Description : 
* Input       : 
* Outpu       : 
* Author      : xwl                        Date : 2022  
**********************************************************************************/
int main(void)
{
    uint32_t temp, USBx_BASE, USB_;   
    USB_OTG_EPTypeDef *ep; 
    USB_OTG_GlobalTypeDef *USB_Global_Regs;   
    PCD_HandleTypeDef * p_pcd;  
       
    HAL_Init();  
    SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);      
    
    BSP_UART_Init(USART1, 115200);  
    
    printfS("Startup\n"); 
    
    __HAL_RCC_USB2_CLK_ENABLE();   
    
    __HAL_RCC_USB1_CLK_ENABLE();  
    
    __HAL_RCC_SYSCFG_CLK_ENABLE();   
    
    USBOTG1_PHYC->TR0 |= BIT3;  // Powerdown USB1 which is not used 
   
    USBOTG2_PHYC->CR = (BIT0 | BIT5 | BIT4);   // USB2 Use default configure, 12M XTH, PLL, OTG  
                
    HAL_Delay(2);   // Wait PHY PLL to be stable    
    
    __HAL_RCC_USB1_CLK_DISABLE(); 
    
    /* Init Device Library */
    USBD_Init(&USBD_Device, &UMS_Desc, USB_HS_OTG_ID2); 
    USB_Global_Regs = ((PCD_HandleTypeDef *)(USBD_Device.pData))->Instance;   
  
    /* Add Supported Class */
    USBD_RegisterClass(&USBD_Device, USBD_UMS_CLASS); 
    
    USBx_BASE = (uint32_t)USB_Global_Regs;   
    p_pcd = (PCD_HandleTypeDef *)USBD_Device.pData;    
    
    USBD_Device.dev_state = USBD_STATE_DEFAULT;     
  
    #ifdef USE_USB_INTERRUPT
    printfS("USB Interrupt Demo, Bulk Transfer, Max Packet is 512\n");  
    NVIC_EnableIRQ(USBOTG2_IRQn); 
    #else
    printfS("USB Poll Demo, Bulk Transfer, Max Packet is 512\n");    
    #endif 
    /* Start Device Process */   
    USBD_Start(&USBD_Device);   
   
    while(1) 
    {
      if (0 != USBD_EPx_Get_Out_EP_Error(p_pcd, UMS_EPOUT_ADDR) )
      {
          printfS("Out EP Error\n");      
          USBD_EPx_Clear_Out_EP_Error(p_pcd, UMS_EPOUT_ADDR);   
      }
      
      #ifndef USE_USB_INTERRUPT 
      USBD_LL_Bus_Activity(&USBD_Device);     
      
      if (USB_Global_Regs->GINTSTS & USB_OTG_GINTSTS_RXFLVL)    
      {         
        if (USB_Global_Regs->GRXSTSR)
        {
            temp = USB_Global_Regs->GRXSTSP;
            ep = &(p_pcd->OUT_ep[temp & USB_OTG_GRXSTSP_EPNUM]); 

            if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_DATA_UPDT)
            {
             
                if ((temp & USB_OTG_GRXSTSP_BCNT) != 0U)
                {
                    (void)USB_ReadPacket(USB_Global_Regs, ep->xfer_buff,
                               (uint16_t)((temp & USB_OTG_GRXSTSP_BCNT) >> 4));

                    ep->xfer_buff += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                    ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;
                }
            }
            else if (((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  STS_SETUP_UPDT)
            {                    
                (void)USB_ReadPacket(USB_Global_Regs, (uint8_t *)p_pcd->Setup, 8U);
                ep->xfer_count += (temp & USB_OTG_GRXSTSP_BCNT) >> 4;          
            }
                       
            else
            {
                /* ... */
            }
             
          }  
                  
        }  
      
        if(USB_Global_Regs->GINTSTS & USB_OTG_GINTSTS_OEPINT)
        {
            uint32_t epint, ep_intr, epnum; 
                      
            epnum = 0U;  
            ep_intr = USB_ReadDevAllOutEpInterrupt(p_pcd->Instance);   
            
            while (ep_intr != 0U) 
            {
                if ((ep_intr & 0x1U) != 0U) 
                {   
                    epint = USB_ReadDevOutEPInterrupt(p_pcd->Instance, (uint8_t)epnum); 
            
                    if ((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC)
                    {
                        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_XFRC);  
                        
                        (void)PCD_EP_OutXfrComplete_int(p_pcd, epnum);  // for EP0, it is normally received zero length byte  
                    }

                    if ((epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP) // setup completely, ep0  
                    {                      
                        uint16_t length;  
                        uint8_t  dev_addr, ret;    
                        
                        CLEAR_OUT_EP_INTR(0, USB_OTG_DOEPINT_STUP);  
                        /*setup phase done for previous decoded setup */                 
                        CLEAR_OUT_EP_INTR(0, USB_OTG_DOEPINT_STPKTRX);                          
                          
                        USBD_Setup_Process(p_pcd);         
                    }

                    if ((epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS)
                    {
                        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPDIS);
                    }

                    /* Clear Status Phase Received interrupt */
                    if ((epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR)
                    {
                        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_OTEPSPR);
                    }

                    /* Clear OUT NAK interrupt */
                    if ((epint & USB_OTG_DOEPINT_NAK) == USB_OTG_DOEPINT_NAK)
                    {
                        CLEAR_OUT_EP_INTR(epnum, USB_OTG_DOEPINT_NAK);  
                    }
                } // if 
                
                epnum++;
                ep_intr >>= 1U;
                
            } // while 
        }  // process all OUT EP events      
        
        if(USB_Global_Regs->GINTSTS & USB_OTG_GINTSTS_IEPINT)  
        {
            uint32_t epint, ep_intr, epnum;       
            
            epnum = 0U;
            
            ep_intr = USB_ReadDevAllInEpInterrupt(p_pcd->Instance);    
            
            while (ep_intr != 0U)
            {
                if ((ep_intr & 0x1U) != 0U) /* In ITR */
                {
                    epint = USB_ReadDevInEPInterrupt(p_pcd->Instance, (uint8_t)epnum); 
            
                    if ((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC) 
                    {
                        if (0 != epnum)
                        {
                            USBD_Device.pClass->DataIn(&USBD_Device, epnum);   
                        }                      
                        CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_XFRC); 
                    }
            
                    if ((epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC)
                    {
                        CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_TOC);
                    }
                    if ((epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE)
                    {
                        CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_ITTXFE);
                    }
                    if ((epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE)
                    {
                        CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_INEPNE);
                    }
                    if ((epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD)
                    {
                        CLEAR_IN_EP_INTR(epnum, USB_OTG_DIEPINT_EPDISD);
                    }  
                    
                }
                epnum++;
                ep_intr >>= 1U;
            }
            
        }    // process all IN EP events    
    #endif    
    } // check GINTSTS 
}



