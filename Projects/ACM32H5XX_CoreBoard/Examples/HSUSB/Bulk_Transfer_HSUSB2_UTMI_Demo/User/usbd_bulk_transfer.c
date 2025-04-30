/**
  ******************************************************************************
  * @file    usbd_hid.c
  * @author  MCD Application Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                UMS Class  Description
  *          ===================================================================
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick
  *             - Collection : Application
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usbd_bulk_transfer.h"
#include "usbd_ctlreq.h"


  
 
#define USB_DISK_BLOCK_SIZE   512    
 
uint32_t bulk_transfer_test_times;  
 
MSCCMD  ScsiCBW;   
 
static SENSE_CODE MscStatusCode;
static uint8_t scsi_state;  // 0, command; 1, data;  2, status;    

static uint8_t USBD_UMS_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_UMS_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_UMS_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_UMS_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);  
static uint8_t USBD_UMS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);  

static uint8_t *USBD_UMS_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_UMS_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_UMS_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t *USBD_UMS_GetDeviceQualifierDesc(uint16_t *length);

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Variables
  * @{
  */

USBD_ClassTypeDef USBD_UMS = {
  USBD_UMS_Init,
  USBD_UMS_DeInit,
  USBD_UMS_Setup,
  NULL,              /* EP0_TxSent */
  NULL,              /* EP0_RxReady */
  USBD_UMS_DataIn,   /* DataIn */
  USBD_UMS_DataOut,              /* DataOut */
  NULL,              /* SOF */
  NULL,
  NULL,
  USBD_UMS_GetHSCfgDesc,
  NULL,
  USBD_UMS_GetOtherSpeedCfgDesc,
  USBD_UMS_GetDeviceQualifierDesc,
};





/* USB HID device HS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_UMS_CfgHSDesc[32] __ALIGN_END = 
{
   //ÅäÖÃÃèÊö·û
  
	0x09,	             //bLength
	0x02,	             //bDescriptor 02
	CONFIG_TOTAL_LENGTH & 0xFF,   //wTotalLength 
    0x00,
	0x01,	          	   //bNumInterface       	   
	0x01,		           //bConfiguration Value
	0x00,			       //iConfiguration
	0x80,			       //bmAttributes,BUS-POWER
	0xfa,                   //0x64	 //bMaxPower,200mA
  

  //½Ó¿Ú1ÃèÊö·û(UMS1)
  
	0x09,		         //bLength: 0x09 bytes
	0x04,		         //bDescriptorType: INTERFACE
	0x00,                //bInterfaceNumber: interface 0
	0x00,                //bAlternateSetting: alternate setting 0
	0x02,                //bNumEndpoints: 2 endpoint + zero ep

	USB_CLASS_CODE_TEST_CLASS_DEVICE,   //bInterfaceClass: vendor specific interface
	USB_SUBCLASS_CODE_TEST_CLASS,      //bInterfaceSubClass: 06 SCSI Command Set
	USB_PROTOCOL_CODE_TEST_CLASS,//bInterfaceProtocol:

	0x00,                 //iInterface: index of string
  
  //¶ËµãÃèÊö·û£¬EP1 BULK IN
  
	0x07,			     //bLength: 0x07 bytes
	0x05,                //bDescriptorType: ENDPOINT
	(UMS_EPIN_ADDR),                //bEndpointAddress: IN endpoint 1
	0x02,                //bmAttributes: BULK-ONLY(00:Control 01:isoch 10:bulk 11:intr
	EPX_MAX_PACKET_SIZE & 0xFF,    //wMaxPacketSize: 64 bytes 
    (EPX_MAX_PACKET_SIZE >> 8) & 0xFF, 
	0x00,                 //bInterval: polling interval is 1 ms
  
    //¶ËµãÃèÊö·û£¬EP2 BULK OUT
  
	 0x07,               //bLength: 0x07 bytes
	 0x05,               //bDescriptorType: ENDPOINT
	 (UMS_EPOUT_ADDR),               //bEndpointAddress: out endpoint 2
	 0x02,               //bmAttributes: BULK-ONLY
	 EPX_MAX_PACKET_SIZE & 0xFF,   //wMaxPacketSize: 64 bytes
     (EPX_MAX_PACKET_SIZE >> 8) & 0xFF,   
	 0x00, 		    	 //bInterval: polling interval is 1 ms
  

};


/* USB HID device HS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_UMS_CfgOtherSpeedDesc[32] __ALIGN_END = 
{
   //ÅäÖÃÃèÊö·û
  
	0x09,	             //bLength
	0x02,	             //bDescriptor 02
	CONFIG_TOTAL_LENGTH & 0xFF,   //wTotalLength 
    0x00,
	0x01,	          	   //bNumInterface       	   
	0x01,		           //bConfiguration Value
	0x00,			       //iConfiguration
	0x80,			       //bmAttributes,BUS-POWER
	0xfa,                   //0x64	 //bMaxPower,200mA
  

  //½Ó¿Ú1ÃèÊö·û(UMS1)
  
	0x09,		         //bLength: 0x09 bytes
	0x04,		         //bDescriptorType: INTERFACE
	0x00,                //bInterfaceNumber: interface 0
	0x00,                //bAlternateSetting: alternate setting 0
	0x02,                //bNumEndpoints: 2 endpoint + zero ep

	USB_CLASS_CODE_TEST_CLASS_DEVICE,   //bInterfaceClass: vendor specific interface
	USB_SUBCLASS_CODE_TEST_CLASS,      //bInterfaceSubClass: 06 SCSI Command Set
	USB_PROTOCOL_CODE_TEST_CLASS,//bInterfaceProtocol:

	0x00,                 //iInterface: index of string
  
  //¶ËµãÃèÊö·û£¬EP1 BULK IN
  
	0x07,			     //bLength: 0x07 bytes
	0x05,                //bDescriptorType: ENDPOINT
	(UMS_EPIN_ADDR),                //bEndpointAddress: IN endpoint 1
	0x02,                //bmAttributes: BULK-ONLY(00:Control 01:isoch 10:bulk 11:intr
	64 & 0xFF,    //wMaxPacketSize: 64 bytes 
    (64 >> 8) & 0xFF, 
	0x00,                 //bInterval: polling interval is 1 ms
  
    //¶ËµãÃèÊö·û£¬EP2 BULK OUT
  
	 0x07,               //bLength: 0x07 bytes
	 0x05,               //bDescriptorType: ENDPOINT
	 (UMS_EPOUT_ADDR),               //bEndpointAddress: out endpoint 2
	 0x02,               //bmAttributes: BULK-ONLY
	 64 & 0xFF,          //wMaxPacketSize: 64 bytes
     (64 >> 8) & 0xFF,   
	 0x00, 		    	 //bInterval: polling interval is 1 ms  
  

};


/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_UMS_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};


/**
  * @}
  */

/** @defgroup USBD_HID_Private_Functions
  * @{
  */


uint8_t  ums_rx_data[512];  

/**
  * @brief  USBD_UMS_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_UMS_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);  

  scsi_state = 0;  
    
  bulk_transfer_test_times = 0;
    
  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    pdev->ep_in[UMS_EPIN_ADDR & 0xFU].bInterval = UMS_HS_BINTERVAL;
  }
  else   /* LOW and FULL-speed endpoints */
  {
    pdev->ep_in[UMS_EPIN_ADDR & 0xFU].bInterval = UMS_FS_BINTERVAL;
  }
  
  (void)USBD_LL_OpenEP(pdev, UMS_EPOUT_ADDR, USBD_EP_TYPE_BULK, UMS_EPOUT_SIZE);
  pdev->ep_out[UMS_EPOUT_ADDR & 0xFU].is_used = 1U; 

    /* Open EP IN */
  (void)USBD_LL_OpenEP(pdev, UMS_EPIN_ADDR, USBD_EP_TYPE_BULK, UMS_EPIN_SIZE);
  pdev->ep_in[UMS_EPIN_ADDR & 0xFU].is_used = 1U; 
  
  (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data,   
                               EPX_MAX_PACKET_SIZE);    

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_HID_DeInit
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_UMS_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Close HID EPs */
  (void)USBD_LL_CloseEP(pdev, UMS_EPIN_ADDR);
  pdev->ep_in[UMS_EPIN_ADDR & 0xFU].is_used = 0U;
  pdev->ep_in[UMS_EPIN_ADDR & 0xFU].bInterval = 0U;

  /* FRee allocated memory */
  if (pdev->pClassData != NULL)
  {
    (void)USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return (uint8_t)USBD_OK;
}


/**
  * @brief  USBD_HID_GetOtherSpeedCfgDesc
  *         return other speed configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_UMS_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_UMS_CfgOtherSpeedDesc);

  return USBD_UMS_CfgOtherSpeedDesc;  
}

/**
  * @brief  USBD_UMS_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_UMS_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_StatusTypeDef ret = USBD_OK;     
#if 0  
    
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;
  USBD_StatusTypeDef ret = USBD_OK;
  uint16_t len;
  uint8_t *pbuf;
  uint16_t status_info = 0U;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {
    case HID_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;

    case HID_REQ_GET_PROTOCOL:
      (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->Protocol, 1U);
      break;

    case HID_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;

    case HID_REQ_GET_IDLE:
      (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->IdleState, 1U);
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_STATUS:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_GET_DESCRIPTOR:
//      if ((req->wValue >> 8) == HID_REPORT_DESC)
//      {
//        len = MIN(HID_REPORT_DESC_SIZE, req->wLength);
//        pbuf = (uint8_t *)hid_report_descr;
//      }
//      else if ((req->wValue >> 8) == HID_DESCRIPTOR_TYPE)
//      {
//        pbuf = USBD_HID_Desc;
//        len = MIN(USB_HID_DESC_SIZ, req->wLength);
//      }
//      else
//      {
//        USBD_CtlError(pdev, req);
//        ret = USBD_FAIL;
//        break;
//      }
      (void)USBD_CtlSendData(pdev, pbuf, len);
      break;

    case USB_REQ_GET_INTERFACE :
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->AltSetting, 1U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_SET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        hhid->AltSetting = (uint8_t)(req->wValue);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_CLEAR_FEATURE:
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    break;
  }
#endif 

  return (uint8_t)ret;
}


/**
  * @brief  USBD_HID_GetPollingInterval
  *         return polling interval from endpoint descriptor
  * @param  pdev: device instance
  * @retval polling interval
  */
uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef *pdev)
{
  uint32_t polling_interval;

  /* HIGH-speed endpoints */
  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Sets the data transfer polling interval for high speed transfers.
     Values between 1..16 are allowed. Values correspond to interval
     of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
    polling_interval = (((1U << (UMS_HS_BINTERVAL - 1U))) / 8U);
  }
  else   /* LOW and FULL-speed endpoints */
  {
    /* Sets the data transfer polling interval for low and full
    speed transfers */
    polling_interval =  UMS_FS_BINTERVAL;
  }

  return ((uint32_t)(polling_interval));
}

/**
  * @brief  USBD_HID_GetCfgFSDesc
  *         return FS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */


/**
  * @brief  USBD_HID_GetCfgHSDesc
  *         return HS configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_UMS_GetHSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_UMS_CfgHSDesc);

  return USBD_UMS_CfgHSDesc;
}

uint8_t *USBD_UMS_GetCfgDesc_ex(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_UMS_CfgHSDesc);

  return USBD_UMS_CfgHSDesc;
}




MSCCSW ScsiCSW =
{
	0x53425355, // USBS
	0x00,       // TAG
	0x00,       // Residue
	0x00        // status
};

const uint8_t SCSIInquiryData[36] = 
{
	0x00,	// Peripheral Device Type: direct access devices , 0x05±íÊ¾¹âÅÌ£¬
	0x80,	// Removable: UFD is removable,	0x00±íÊ¾²»ÏÔÊ¾ÅÌ·û
	0x02,	// ANSI version
	0x02, 	// Response Data Format: compliance with UFI  
	0x1F,	// Additional Length (Number of unsigned chars following this one): 31, totally 36 unsigned chars	
	0x00, 0x00, 0x00,	// reserved
	'U',	// -- Vender information start
	'S',	//
	'B',	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	//
	' ',    //-- Vend Information end 
	'F', 	// -- Production Identification start
	'l', 	//
	'a',	//
	's',	//
	'h',	//
	'D',	//
	'i',	//
	's',	//
	'k', 	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	// 
	' ',	//	
	' ', 	//
	' ',	//-- Production Identification end
	0x31,	//"1" -- Production Revision Level start
	0x2e,	//"."	
	0x30,	//"0"
	0x30	//"0" -- Production Revision Level end
};

const uint8_t SCSIInquiryData_Nodev[36] = 
{
	0x00,	// Peripheral Device Type: direct access devices
	0x00,	// Removable: UFD is removable
	0x02,	// ANSI version
	0x02, 	// Response Data Format: compliance with UFI  
	0x1F,	// Additional Length (Number of unsigned chars following this one): 31, totally 36 unsigned chars	
	0x00, 0x00, 0x00,	// reserved
	'U',	// -- Vender information start
	'S',	//
	'B',	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	//
	' ',    //-- Vend Information end 
	'F', 	// -- Production Identification start
	'l', 	//
	'a',	//
	's',	//
	'h',	//
	'D',	//
	'i',	//
	's',	//
	'k', 	//
	' ',	//
	' ',	//
	' ', 	//
	' ',	// 
	' ',	//	
	' ', 	//
	' ',	//-- Production Identification end
	0x31,	//"1" -- Production Revision Level start
	0x2e,	//"."	
	0x30,	//"0"
	0x30	//"0" -- Production Revision Level end
};

const SENSE_CODE MscSenseCode[] = 
{
	{0x00,0x00,0x00},	/*NO_SENSE	0*/ 
	{0x05,0x24,0x00},	/*INVALID_FIELD_IN_COMMAND	1*/ 
	{0x06,0x28,0x00},	/*NEW_MEDIUM_ARRIVEL 2*/ 
	{0x07,0x27,0x00},	/*WRITE_PROTECTED	3*/ 
	{0x09,0x01,0x00},	/*PASSWORD_ERROR	4*/ 
	{0x09,0x00,0x00},	/*NOT_CONFIG	5*/ 
	{0x09,0x02,0x00},	/*USER_LOCKED	6*/ 
	{0x02,0x3A,0x00},	/*MEDIUM_NOT_PRESENT  7*/ 
	{0x02,0x04,0x01},	/*LOGDRIVER_NOT_READY  8*/  /*Repeat send the data*/
	{0x05,0x21,0x00},	/*ADDRESS_OUT_RANGE  9*/	
	{0x03,0x03,0x00},	/*WRITE FAULT 10*/	 /*½«128¸ösector ²ð³É8¸ösector ·¢ËÍ*/
	{0x00,0x00,0x00},	/*CBW_PHASE_ERROR  11*/	 
	{0x03,0x11,0x00},	/*UNRECOVERED_READ_ERROR  12*/	 
	{0x02,0x53,0x00},	/*MEDIA REMOVAL PREVENTED 13*/	 
	{0x05,0x20,0x00},	/*INVALID_COMMAND_OPERATION_CODE 14*/  
	{0x04,0x00,0x00},   /*HARDWARE ERROR 15*/ 
};


static void mscSet_Status(uint8_t status)
{
	if (status != NO_SENSE) 
	{
		MscStatusCode = MscSenseCode[status];
		if (CBW_PHASE_ERROR == status)
		{
			ScsiCSW.Status = MSC_PHASE_ERROR;
		}
		else
		{
			ScsiCSW.Status = MSC_CMD_FAILED;
		}
	}
	else
	{
		ScsiCSW.Status = MSC_GOOD_STATUS;
	}
}

static uint8_t RBC_Inquiry(PCD_HandleTypeDef *p_hpcd)   
{
	uint32_t txlen; 
    
	txlen = MIN(sizeof(SCSIInquiryData),ScsiCBW.CbwTrxLength);
	   
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)SCSIInquiryData;  
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, (uint8_t *)SCSIInquiryData, txlen);    
    
//    scsi_state = 1;
    
    return 0;  
}

static uint8_t Read_Format_Capacity(PCD_HandleTypeDef *p_hpcd)   
{
	uint8_t format_capacity[16];  
    
	format_capacity[0]  = 0;  
    format_capacity[1]  = 0;  
    format_capacity[2]  = 0;  
    format_capacity[3]  = 8;   
    format_capacity[4]  = 0x00;  
    format_capacity[5]  = 0x01; 
    format_capacity[6]  = 0x00; 
    format_capacity[7]  = 0x00; 
    format_capacity[8]  = 0x03; 
    format_capacity[9]  = 0x00; 
    format_capacity[10] = 0x02; 
    format_capacity[11] = 0x00;   
       
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)format_capacity;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, format_capacity, 12);        
    
//    scsi_state = 1;
    
    return 0;  
} 

static uint8_t Read_Capacity(PCD_HandleTypeDef *p_hpcd)   
{
	uint8_t capacity[16];  
    
	capacity[0]  = 0x00;  
    capacity[1]  = 0x01;  
    capacity[2]  = 0x01;  
    capacity[3]  = 0x00;   
    capacity[4]  = 0x00;  
    capacity[5]  = 0x00; 
    capacity[6]  = 0x02; 
    capacity[7]  = 0x00;    
      
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)capacity;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, capacity, 8);          
    
//    scsi_state = 1;
    
    return 0;  
} 

static uint8_t Read_Mode_Sense(PCD_HandleTypeDef *p_hpcd)   
{
	uint8_t mode_sense[16];  
    
	mode_sense[0]  = 0x03;  
    mode_sense[1]  = 0x00;  
    mode_sense[2]  = 0x00;  
    mode_sense[3]  = 0x00;   
          
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)mode_sense;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, mode_sense, 4);          
    
//    scsi_state = 1;
    
    return 0;  
} 


static uint8_t Read_Command10(PCD_HandleTypeDef *p_hpcd)   
{ 
    uint8_t dummy[USB_DISK_BLOCK_SIZE];     
    
    uint32_t i;
    
    for (i = 0; i < USB_DISK_BLOCK_SIZE; i++)
    {
        dummy[i] = i & 0xFF;  
    }
//    memset(dummy,0, sizeof(dummy));  
      
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)dummy;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, dummy, USB_DISK_BLOCK_SIZE);                  
    
//    scsi_state = 1;
    
    return 0;  
}        


static uint8_t RBC_RequestSense(PCD_HandleTypeDef *p_hpcd)     
{
	uint8_t sense[18];

	memset(sense,0,18);
	sense[0] = 0x70;
	sense[2] = MscStatusCode.SenseKey;
	sense[7] = 10;
	sense[12] = MscStatusCode.ASC;
	sense[13] = MscStatusCode.ASCQ;
	      
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)sense;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, sense, 12);        
    
//    scsi_state = 1;    
    
    return 0;  
}

uint8_t test_data[256]; 

static uint8_t RBC_Vendor_Test_Communication(PCD_HandleTypeDef *p_hpcd) 
{
    
    return 0;   
}


static uint8_t PadData(PCD_HandleTypeDef *p_hpcd)  
{
	uint8_t dummy[512];  
    
    memset(dummy,0, sizeof(dummy));  
          
    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)dummy;    
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, dummy, ScsiCBW.CbwTrxLength);  


	return 0;
}

static uint8_t SendCSW(PCD_HandleTypeDef *p_hpcd)  
{
  if(MSC_GOOD_STATUS == ScsiCSW.Status)
	{
	   MscStatusCode = MscSenseCode[NO_SENSE];
	   ScsiCSW.Residue = 0;
	}
	else 
	{
	   ScsiCSW.Residue = ScsiCBW.CbwTrxLength;
	}

    p_hpcd->IN_ep[UMS_EPIN_ADDR & 0x0F].xfer_buff = (uint8_t *)&ScsiCSW;  
    
    USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, (uint8_t *)&ScsiCSW, 13);    
    
    return 0;   	
}


/**
  * @brief  USBD_UMS_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_UMS_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    if ( (epnum & 0x0F) == (UMS_EPIN_ADDR & 0x0F) ) 
    {
//        if (scsi_state == 1)
//        {
//            SendCSW(pdev->pData);   
//            scsi_state = 2;  
//        }
//        else if (scsi_state == 2)  
//        {
//            (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, 31);    
//            scsi_state = 0;   
//        }
//        else
//        {
//            //error   
//        }
    }
    
    
    return (uint8_t)USBD_OK;
}

 

static uint8_t USBD_UMS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    USB_OTG_EPTypeDef *ep;   
    PCD_HandleTypeDef *p_hpcd; 
    uint32_t i, int_mask;    
    HAL_StatusTypeDef  ret;  
    USBD_StatusTypeDef result;  
       
    p_hpcd = (PCD_HandleTypeDef *)pdev->pData;  
    
    int_mask = p_hpcd->Instance->GINTMSK;  
    
    p_hpcd->Instance->GINTMSK = 0;  // Disable usb interrupt 
    
    if (epnum != UMS_EPOUT_ADDR)
    {
        result = USBD_FAIL;
        goto END_PROCESS; 
    } 
        
    ep = &(p_hpcd->OUT_ep[epnum & 0x0F]);  
    
    USB_printfS("RX: %d\n", ep->xfer_count);  

#if 0  // This section code seems useless, but keep it inactive       
    if (ep->xfer_count == 0)
    {
        if  (0 == (USBD_EPx_Has_Receive_Data_Started(p_hpcd, epnum & 0x0F) ) )  
        {
            USB_printfS("Received zero Data, Prepare_000:\n");   
            (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);     
        }
        
        result = USBD_OK;  
        goto END_PROCESS;   
    }
#endif 
    
    ret = 0x11; 
    
    USBD_EPx_Clear_Received_Data(p_hpcd, epnum & 0x0F);    // Clear Out EP information  
    
    if (0xBB == ums_rx_data[0] ) // Flag, means this frame is 512 Bytes frame 
    {
        USB_printfS("Send 512 bytes back to Host:\n"); 
        i = 0;  
        
        while(1)
        {
            ret = USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, ums_rx_data, EPX_MAX_PACKET_SIZE );  
            USB_printfS("Rt512:%d\n", ret);   
            i++; 
            
            if (HAL_OK == ret) //Send Successfully  
            {
                bulk_transfer_test_times++;   
                if (0 == (bulk_transfer_test_times%1000) ) 
                {
                    USB_printfS("Test_Times: %d\n", bulk_transfer_test_times);  
                }
                break;   
            }
            USB_printfS("Retry111: %d\n", i);  
            
            if (HAL_ERROR == ret) // Bus reset or suspend, return 
            {
                USB_printfS("Bus reset or suspend, exit:\n"); 
                result = USBD_FAIL; 
                goto END_PROCESS;   
            }
            
            if (USBD_EPx_Host_Sent_Data_to_OutEP(p_hpcd, epnum & 0x0F) ) // Out EP received OUT Token, to prepare RX EP 
            {
                USB_printfS("Host send data to Out EP, Prepare_111:\n"); 
                (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);    
                result = USBD_FAIL; 
                goto END_PROCESS;                     
            }   
            
            if (i >= SEND_RETRY_TIMES_MAX)
            {
                USB_printfS("Timeout, Prepare_222:\n");  
                (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);     
                result = USBD_FAIL;   
                goto END_PROCESS;     
            }
        }                
    }
    else
    {
        USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, ums_rx_data, ep->xfer_count );   // other frame      
    }  
 
#ifdef SEND_EMPTY_PACKET_ENABLE    
    ret = 0x22;   
    if ( (ep->xfer_count%EPX_MAX_PACKET_SIZE == 0) && (ep->xfer_count > 0) )
    {
        USBD_EPx_Clear_Received_Data(p_hpcd, epnum & 0x0F);    
        i = 0;      
        USB_printfS("Send Empty packet to Host:\n"); 
        while(1)
        {
            ret = USBD_EPx_Send_Data(p_hpcd, UMS_EPIN_ADDR & 0x0F, ums_rx_data, 0);  
            i++;  
            USB_printfS("Rt000:%d\n", ret);     
                       
            if (HAL_OK == ret)
            {
                break;   
            }
            if (HAL_ERROR == ret)
            {
                result = USBD_FAIL;   
                goto END_PROCESS;  
            }   

            if (USBD_EPx_Host_Sent_Data_to_OutEP(p_hpcd, epnum & 0x0F) )
            {
                USB_printfS("Host send data to Out EP, Prepare_333:\n");  
                (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);     
                result = USBD_FAIL;   
                goto END_PROCESS;    
            }   

            if (i >= SEND_RETRY_TIMES_MAX)   
            {
                USB_printfS("Timeout, Prepare_444:\n"); 
                (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);     
                result = USBD_FAIL;   
                goto END_PROCESS;       
            }            
        }               
    }
 #endif    
    
    ep->xfer_count = 0;   
    USB_printfS("Prepare_555:\n");     
    (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, ums_rx_data, EPX_MAX_PACKET_SIZE);    
     
    END_PROCESS:
    
    p_hpcd->Instance->GINTMSK = int_mask;   // unmask 
    
    return (uint8_t)result;  
}


/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t *USBD_UMS_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_UMS_DeviceQualifierDesc);

  return USBD_UMS_DeviceQualifierDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT *** *****END OF FILE****/
