/**
  ******************************************************************************
  * @file    usbd_cdc_acm.c
  * @author  firmware Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                CDC Class  Description
  *          ===================================================================
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



STR_USBD_CDC_T gCdcInfo; 
 
uint32_t cdc_usb_rx_count;  
 
UART_HandleTypeDef CDC_UART_Handle;  

uint8_t  cdc_rx_usb_data[1024];  
 
uint8_t  cdc_tx_data[512];  

void CDC_Acm_Init_Info(void);  

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

	0x02,               //bInterfaceClass: CDC
	0x02,               //bInterfaceSubClass: CDC 
	0x01,               //bInterfaceProtocol: ACM 

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

	0x02,               //bInterfaceClass: CDC
	0x02,               //bInterfaceSubClass: CDC 
	0x01,               //bInterfaceProtocol: ACM  

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
  * @brief  USBD_UMS_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_UMS_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);  
    
  cdc_usb_rx_count = 0;  
    
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
  
  (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, cdc_rx_usb_data,   
                               EPX_MAX_PACKET_SIZE);    

  CDC_Acm_Init_Info();
  
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

    }
        
    return (uint8_t)USBD_OK;
}

void CDC_Acm_Send_USB_Data(uint8_t *pdata, uint32_t length)
{
    HAL_UART_Transmit(&CDC_UART_Handle, pdata, length, 0);      
} 

static uint8_t USBD_UMS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    USB_OTG_EPTypeDef *ep;   
    PCD_HandleTypeDef *p_hpcd; 
    uint32_t int_mask;    

    USBD_StatusTypeDef result;  
       
    p_hpcd = (PCD_HandleTypeDef *)pdev->pData;  
    
    int_mask = p_hpcd->Instance->GINTMSK;  
    
    p_hpcd->Instance->GINTMSK = 0;  // Disable usb interrupt 
    
    if (epnum != UMS_EPOUT_ADDR)
    {
        result = USBD_FAIL;
        p_hpcd->Instance->GINTMSK = int_mask;   // unmask   
        return result;  
    } 
        
    ep = &(p_hpcd->OUT_ep[epnum & 0x0F]);  
    
    USB_PrintfS("RX: %d\n", ep->xfer_count);  
 
    
    if(cdc_usb_rx_count > 0) 
    {
        USB_PrintfS("Overrun\n");    
    }  
    
    cdc_usb_rx_count = ep->xfer_count;   
    ep->xfer_count = 0; 
    
    (void)USBD_LL_PrepareReceive(pdev, UMS_EPOUT_ADDR, cdc_rx_usb_data, EPX_MAX_PACKET_SIZE);   
    
    p_hpcd->Instance->GINTMSK = int_mask; 
    
    return (uint8_t)result;  
} 


void CDC_Itf_Init(void)  
{  
    CDC_UART_Handle.Instance = CDC_UART_INSTANCE;   
    
    CDC_UART_Handle.RxCount = 0;
    CDC_UART_Handle.RxSize = 0; 
    CDC_UART_Handle.RxBusy = false;  
    CDC_UART_Handle.TxBusy = false;     
    CDC_UART_Handle.TxCount = 0;
    CDC_UART_Handle.TxSize = 0;  
    CDC_UART_Handle.ErrorCode = HAL_UART_ERROR_NONE;  
       
    CDC_UART_Handle.Init.BaudRate   = LINE_DEFAULT_BAUD_RATE;  
    CDC_UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    CDC_UART_Handle.Init.StopBits   = UART_STOPBITS_1;
    CDC_UART_Handle.Init.Parity     = UART_PARITY_NONE;
    CDC_UART_Handle.Init.Mode       = UART_MODE_TX_RX;
    CDC_UART_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;   
    
    HAL_UART_Init(&CDC_UART_Handle);    
    
    HAL_UART_Receive_IT(&CDC_UART_Handle, cdc_tx_data, sizeof(cdc_tx_data));     
}
   
void CDC_Acm_Init_Info(void)
{    
    gCdcInfo.u32DTERate = LINE_DEFAULT_BAUD_RATE;  
    gCdcInfo.u8CharFormat = LINE_STOP_1; 
    gCdcInfo.u8DataBits = LINE_DATA_LEN_8;  
    gCdcInfo.u8ParityType = LINE_PARITY_NONE;   
    
    CDC_Itf_Init();       
}


void ComPort_Config(void)
{
    HAL_UART_DeInit(&CDC_UART_Handle);   
    
    switch(gCdcInfo.u8CharFormat)
    {
        case LINE_STOP_1: 
        CDC_UART_Handle.Init.StopBits = UART_STOPBITS_1; 
        break;
        
        case LINE_STOP_2: 
        CDC_UART_Handle.Init.StopBits = UART_STOPBITS_2;  
        break;
        
        default:   
        CDC_UART_Handle.Init.StopBits = UART_STOPBITS_1;     
        break;   
    }
    
    switch(gCdcInfo.u8ParityType) 
    {
        case LINE_PARITY_NONE: 
        CDC_UART_Handle.Init.Parity = UART_PARITY_NONE;    
        break; 
        
        case LINE_PARITY_ODD:  
        CDC_UART_Handle.Init.Parity = UART_PARITY_ODD;   
        break;
        
        case LINE_PARITY_EVEN:  
        CDC_UART_Handle.Init.Parity = UART_PARITY_EVEN;     
        break;   
        
        default:  
        CDC_UART_Handle.Init.Parity = UART_PARITY_NONE;     
        break;  
        
    }   
    
    CDC_UART_Handle.Init.BaudRate = gCdcInfo.u32DTERate;        
    CDC_UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;   
       
    HAL_UART_Init(&CDC_UART_Handle);    

    HAL_UART_Receive_IT(&CDC_UART_Handle, cdc_tx_data, sizeof(cdc_tx_data) );            
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


void CDC_Acm_Set_Line_Coding(USBD_HandleTypeDef *pdev, uint32_t length)
{   
    USBD_CtlPrepareRx(pdev, (UINT8 *)&gCdcInfo, length);  
    
    USBD_EP0_Send_Data(pdev->pData, NULL, 0);   
    
    ComPort_Config();           
}

void CDC_Acm_Get_Line_Coding(USBD_HandleTypeDef *pdev, uint32_t length)  
{
    USBD_EP0_Send_Data(pdev->pData, (UINT8 *)&gCdcInfo, MIN(sizeof(STR_USBD_CDC_T), length) );   
       
}   

void CDC_Acm_Send_Uart_Data(USBD_HandleTypeDef *pdev)
{
    uint32_t rx_len;
        
    if ( (CDC_UART_Handle.RxCount) && (false == CDC_UART_Handle.RxBusy) )     
    {
        rx_len = CDC_UART_Handle.RxCount; 

        if  (USBD_STATE_CONFIGURED == pdev->dev_state)
        {
            USBD_EPx_Send_Data(pdev->pData, UMS_EPIN_ADDR & 0x0F, cdc_tx_data, rx_len);     
            if (rx_len%EPX_MAX_PACKET_SIZE == 0)
            {
                USBD_EPx_Send_Data(pdev->pData, UMS_EPIN_ADDR & 0x0F, cdc_tx_data, 0);     
            }
        }
        CDC_UART_Handle.RxCount = 0;    
        HAL_UART_Receive_IT(&CDC_UART_Handle, cdc_tx_data, sizeof(cdc_tx_data));       
    }
}

// process in data from uart, send them out by USB 
void VCOM_Process_InData(USBD_HandleTypeDef *pdev) 
{
    CDC_Acm_Send_Uart_Data(pdev);   
}  

//Send data which received from usb by UART  
void VCOM_Process_OutData(void)  
{
    if (cdc_usb_rx_count)
    {
        CDC_Acm_Send_USB_Data(cdc_rx_usb_data, cdc_usb_rx_count);     
        cdc_usb_rx_count = 0;   
    }
}  


