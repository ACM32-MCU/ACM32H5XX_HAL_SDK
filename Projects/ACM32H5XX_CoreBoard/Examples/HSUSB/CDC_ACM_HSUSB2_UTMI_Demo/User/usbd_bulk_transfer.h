/**
  ******************************************************************************
  * @file    usbd_ums.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_cdc_acm.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_BULK_TRANSFER_H
#define __USB_BULK_TRANSFER_H   

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"


  
   

#define     EP0_MAX_PACKET_SIZE     64 

#ifdef HSUSB_DEVICE
#define     EPX_MAX_PACKET_SIZE     512  
#else
#define     EPX_MAX_PACKET_SIZE     64    
#endif

#define UMS_EPIN_ADDR                              0x81U
#define UMS_EPOUT_ADDR                             0x02U  
#define UMS_EPIN_SIZE                              EPX_MAX_PACKET_SIZE  
#define UMS_EPOUT_SIZE                             EPX_MAX_PACKET_SIZE   

#define USB_UMS_CONFIG_DESC_SIZ                    32U

#ifndef UMS_HS_BINTERVAL
#define UMS_HS_BINTERVAL                           0x02U
#endif /* UMS_HS_BINTERVAL */

#ifndef UMS_FS_BINTERVAL
#define UMS_FS_BINTERVAL                           0x01U
#endif /* UMS_FS_BINTERVAL */



#define UMS_RX_BUFFER_SIZE                         64    


#define USB_INTERFACE_CLASS_UMS                    0x08 

#define USB_INTERFACE_SUBCLASS_SCSI                0x06   // SCSI Command Set   

#define USB_INTERFACE_PROTOCOL_BBB                 0x50   // bulk only transport   


// Class Request Codes
/* CDC Requests                                   */
/**************************************************/
#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF   


#define LINE_PARITY_NONE 0
#define LINE_PARITY_ODD  1   
#define LINE_PARITY_EVEN 2 

#define LINE_STOP_1  0  
#define LINE_STOP_2  2   

#define LINE_DATA_LEN_8   8    

#define LINE_DEFAULT_BAUD_RATE  115200  

#define CDC_UART_INSTANCE  USART2   


typedef struct DRVUSBD_CDC_STRUCT
{
	uint32_t  u32DTERate;     /* Baud rate    */
	uint8_t   u8CharFormat;   /* stop bit     */
	uint8_t   u8ParityType;   /* parity       */
	uint8_t   u8DataBits;     /* data bits    */

}STR_USBD_CDC_T;  

extern STR_USBD_CDC_T gCdcInfo; 




extern USBD_ClassTypeDef USBD_UMS;
#define USBD_UMS_CLASS &USBD_UMS


uint8_t USBD_UMS_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report,uint16_t len);
uint32_t USBD_UMS_GetPollingInterval(USBD_HandleTypeDef *pdev);

void CDC_Acm_Set_Line_Coding(USBD_HandleTypeDef *pdev, uint32_t length); 
void CDC_Acm_Get_Line_Coding(USBD_HandleTypeDef *pdev, uint32_t length);    
void VCOM_Process_InData(USBD_HandleTypeDef *pdev);  
void VCOM_Process_OutData(void);   


#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_CLASS_CODE_TEST_CLASS_DEVICE    0xDC
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF






typedef  struct  _USB_CONFIGURATION_DESCRIPTOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t wTotalLength_L;
    uint8_t wTotalLength_H;
    
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t MaxPower;
} USB_CONFIGURATION_DESCRIPTOR;	 


typedef  struct  _USB_INTERFACE_DESCRIPTOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} USB_INTERFACE_DESCRIPTOR;

typedef  struct  _USB_ENDPOINT_DESCRIPTOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes; 
    uint8_t wMaxPacketSize_L; 
    uint8_t wMaxPacketSize_H; 
    uint8_t bInterval;
} USB_ENDPOINT_DESCRIPTOR; 

typedef  struct _USB_DESCRIPTOR_NCONFIG
{
	USB_CONFIGURATION_DESCRIPTOR config_descr;

    USB_INTERFACE_DESCRIPTOR 	 inter_descr; 
	USB_ENDPOINT_DESCRIPTOR		 ep_descr_in;
	USB_ENDPOINT_DESCRIPTOR		 ep_descr_out;

} USB_DESCRIPTOR_NCONFIG;  

#define CONFIG_TOTAL_LENGTH	  sizeof(USB_DESCRIPTOR_NCONFIG)


extern USB_DESCRIPTOR_NCONFIG  usb_descriptor_nconfig;

/**********************************************************
*	extern variables
**********************************************************/

extern const uint8_t StrDesc_LanguageID[4];
extern const uint8_t StrDesc_Manufacturer[22];
extern const uint8_t StrDesc_Product[30];
extern const uint8_t StrDesc_SerialNumber[34];

extern uint8_t UsbStrDescSerialNumber[34];
extern const uint8_t usb_report_desc[42];  


  
#ifdef __cplusplus
}
#endif

#endif  



