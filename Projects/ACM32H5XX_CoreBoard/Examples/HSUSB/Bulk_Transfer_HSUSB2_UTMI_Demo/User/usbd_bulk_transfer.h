/**
  ******************************************************************************
  * @file    usbd_ums.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_hid_core.c file.
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
#define     GET_MAX_LUN                    0xFE
#define     Bulk_Only_Mass_Storage_Reset   0xFF   


// Scsi Command Operation Code List.
#define     OPCODE_TEST_UNIT_READY       0x00
#define     OPCODE_REQUEST_SENSE         0x03
#define     OPCODE_MODE_SELECT6          0x15   
#define     OPCODE_MODE_SENSE6           0x1A   
#define     OPCODE_INQUIRY               0x12
#define     OPCODE_START_STOP_UNIT       0x1B
#define     OPCODE_PREVENT_REMOVAL       0x1E
#define     OPCODE_Read_Format_Capacity  0x23
#define     OPCODE_READ_CAPACITY         0x25
#define     OPCODE_READ10                0x28
#define     OPCODE_WRITE10               0x2A
#define     OPCODE_VERIFY10              0x2F
#define     OPCODE_MODE_SENSE10          0x5A  

#define     OPCODE_VENDOR_TEST_COMM      0xF7    

#define NO_SENSE                       0
#define INVALID_FIELD_IN_COMMAND       1
#define NEW_MEDIUM_ARRIVEL             2
#define WRITE_PROTECTED                3 
#define PASSWORD_ERROR                 4 
#define NOT_CONFIG                     5 
#define USER_LOCKED                    6 
#define MEDIUM_NOT_PRESENT             7 
#define LOGDRIVER_NOT_READY            8 
#define ADDRESS_OUT_RANGE              9 
#define WRITEFAULT                     10
#define CBW_PHASE_ERROR                11
#define UNRECOVERED_READ_ERROR         12
#define MEDIAREMOVALPREVENTED          13
#define INVALID_COMMAND_OPERATION_CODE 14
#define HARDWARE_ERROR                 15
#define SenseCodeCnt                   16

#define	CBW_DIR_IN				0x80

#define	MSC_GOOD_STATUS			0
#define	MSC_CMD_FAILED			1
#define	MSC_PHASE_ERROR			2

typedef struct _MSCCMD {
	uint32_t		Signature;
	uint32_t		CbwTag;
	uint32_t		CbwTrxLength;
	uint8_t		    CbwFlag;
	uint8_t		    CbwLun;
	uint8_t		    CbwCbLen;
	
	uint8_t		    OpCode;     
	uint8_t		    Cmd_Para[15];   

} MSCCMD;

typedef	struct	_MSCCSW {
	uint32_t		Signature;
	uint32_t		CswTag;
	uint32_t		Residue;
	uint8_t		    Status;
} MSCCSW;

typedef  struct _SENSE_CODE {
	uint8_t	SenseKey;
	uint8_t	ASC;
	uint8_t	ASCQ;   
} SENSE_CODE;  

extern USBD_ClassTypeDef USBD_UMS;
#define USBD_UMS_CLASS &USBD_UMS


uint8_t USBD_UMS_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report,uint16_t len);
uint32_t USBD_UMS_GetPollingInterval(USBD_HandleTypeDef *pdev);







//typedef unsigned           char UINT8;
//typedef unsigned short     int  UINT16;
//typedef unsigned           int  UINT32; 


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


#define USB_SUBCLASS_CODE_TEST_CLASS         0xA0
#define USB_PROTOCOL_CODE_TEST_CLASS         0xB0



typedef  struct  _USB_CONFIGURATION_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 wTotalLength_L;
    UINT8 wTotalLength_H;
    
    UINT8 bNumInterfaces;
    UINT8 bConfigurationValue;
    UINT8 iConfiguration;
    UINT8 bmAttributes;
    UINT8 MaxPower;
} USB_CONFIGURATION_DESCRIPTOR;	 


typedef  struct  _USB_INTERFACE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bInterfaceNumber;
    UINT8 bAlternateSetting;
    UINT8 bNumEndpoints;
    UINT8 bInterfaceClass;
    UINT8 bInterfaceSubClass;
    UINT8 bInterfaceProtocol;
    UINT8 iInterface;
} USB_INTERFACE_DESCRIPTOR;

typedef  struct  _USB_ENDPOINT_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bEndpointAddress;
    UINT8 bmAttributes; 
    UINT8 wMaxPacketSize_L; 
    UINT8 wMaxPacketSize_H; 
    UINT8 bInterval;
} USB_ENDPOINT_DESCRIPTOR; 

typedef  struct _USB_DESCRIPTOR_NCONFIG
{
	USB_CONFIGURATION_DESCRIPTOR config_descr;

    USB_INTERFACE_DESCRIPTOR 	 inter1_descr; 
	USB_ENDPOINT_DESCRIPTOR		 ep1_descr_in;
	USB_ENDPOINT_DESCRIPTOR		 ep1_descr_out;

} USB_DESCRIPTOR_NCONFIG;  

#define CONFIG_TOTAL_LENGTH	  sizeof(USB_DESCRIPTOR_NCONFIG)


extern USB_DESCRIPTOR_NCONFIG  usb_descriptor_nconfig;
//extern UINT8 usb_descriptor_config[32];

/**********************************************************
*	extern variables
**********************************************************/

extern const UINT8 StrDesc_LanguageID[4];
extern const UINT8 StrDesc_Manufacturer[22];
extern const UINT8 StrDesc_Product[30];
extern const UINT8 StrDesc_SerialNumber[34];
//extern USB_STRING_DESCRIPTOR string_descr;
//extern USB_STRING_DESCRIPTOR product_string;
//extern const USB_QUALIFIER_DESCRIPTOR qualifier_descr;
extern UINT8 UsbStrDescSerialNumber[34];
extern const UINT8 usb_report_desc[42];  


#define SEND_RETRY_TIMES_MAX  25    
//#define SEND_EMPTY_PACKET_ENABLE     

//#define USB_LOG_ENABLE   

#ifdef USB_LOG_ENABLE
    #define USB_printfS(fmt, ...)       printf(fmt, ##__VA_ARGS__)
#else
    #define USB_printfS(fmt, ...)         
#endif   


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT *** *****END OF FILE****/
