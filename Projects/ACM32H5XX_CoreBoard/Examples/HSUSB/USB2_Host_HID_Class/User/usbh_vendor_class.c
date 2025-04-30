/**
  ******************************************************************************
  * @file    usbh_msc.c
  * @author  MCD Application Team
  * @brief   This file implements the HID class driver functions
  *          ===================================================================
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 AisinoChip.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usbh_vendor_class.h"
#include "usbh_bulk.h"


extern uint8_t * p_uart_receive_buffer;  
extern uint8_t * p_uart_send_buffer;  

static USBH_StatusTypeDef USBH_AISINOCHIP_InterfaceInit(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_AISINOCHIP_InterfaceDeInit(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_Vendor_Class_Process(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_Vendor_Class_SOFProcess(USBH_HandleTypeDef *phost);



USBH_ClassTypeDef  USBH_Aisinochip =
{
  "BulkTransfer",
  USB_AISINOCHIP_CLASS,  
  USBH_AISINOCHIP_InterfaceInit,
  USBH_AISINOCHIP_InterfaceDeInit,
  NULL,
  USBH_Vendor_Class_Process,  
  USBH_Vendor_Class_SOFProcess,
  NULL,
};



static USBH_StatusTypeDef USBH_AISINOCHIP_InterfaceInit(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status;
  uint8_t interface;
  Vender_Class_HandleTypeDef *pClass_Handle;

  interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, USB_AISINOCHIP_SUBCLASS_CODE, USB_AISINOCHIP_CLASS_PROTOCOL_CODE, 0, HOST_INTF_EP0_VALID);

  if ((interface == 0xFFU) || (interface >= USBH_MAX_NUM_INTERFACES)) /* Not Valid Interface */
  {
    USBH_DbgLog("Cannot Find the interface for %s class.", phost->pActiveClass->Name);
    return USBH_FAIL;
  }

  status = USBH_SelectInterface(phost, interface);

  if (status != USBH_OK)
  {
    return USBH_FAIL;
  }

  phost->pActiveClass->pData = (Vender_Class_HandleTypeDef *)USBH_malloc(sizeof(Vender_Class_HandleTypeDef));
  pClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData;

  if (pClass_Handle == NULL)
  {
    uart_printf("Cannot allocate memory for Class Handle");
    return USBH_FAIL;
  }
  


  /* Initialize msc handler */
  USBH_memset(pClass_Handle, 0, sizeof(Vender_Class_HandleTypeDef));
  
  pClass_Handle->p_readbuff = USBH_malloc(USB_READ_BUFFER_LEN);  
  
  if (pClass_Handle->p_readbuff == NULL)
  {
    uart_printf("Cannot allocate memory for read buffer");
    return USBH_FAIL;
  }
  
  pClass_Handle->p_writebuff = USBH_malloc(USB_WRITE_BUFFER_LEN);  
  
  if (pClass_Handle->p_writebuff == NULL)
  {
    uart_printf("Cannot allocate memory for write buffer");
    return USBH_FAIL;
  }

  if (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80U)
  {
    uart_printf("ep in:0x%02x\n", phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress);
    pClass_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress);
    pClass_Handle->InEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
  }
  else
  {
    uart_printf("ep out:0x%02x\n", phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress);
    pClass_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress);
    pClass_Handle->OutEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
  }

  if (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress & 0x80U)
  {
    uart_printf("ep in:0x%02x\n", phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress);
    pClass_Handle->InEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress);
    pClass_Handle->InEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].wMaxPacketSize;
  }
  else
  {
    uart_printf("ep out:0x%02x\n", phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress); 
    pClass_Handle->OutEp = (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].bEndpointAddress);
    pClass_Handle->OutEpSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[1].wMaxPacketSize;
  }

  pClass_Handle->OutPipe = USBH_AllocPipe(phost, pClass_Handle->OutEp);
  pClass_Handle->InPipe = USBH_AllocPipe(phost, pClass_Handle->InEp);


  /* Open the new channels */
  USBH_OpenPipe(phost, pClass_Handle->OutPipe, pClass_Handle->OutEp,
                phost->device.address, phost->device.speed,
                USB_EP_TYPE_BULK, pClass_Handle->OutEpSize);

  USBH_OpenPipe(phost, pClass_Handle->InPipe, pClass_Handle->InEp,
                phost->device.address, phost->device.speed, USB_EP_TYPE_BULK,
                pClass_Handle->InEpSize);

  USBH_LL_SetToggle(phost, pClass_Handle->InPipe, 0U);
  USBH_LL_SetToggle(phost, pClass_Handle->OutPipe, 0U);  
  
  pClass_Handle->out_state = OP_OUT_NOTREADY;   
  pClass_Handle->in_state = OP_IN_NOTREADY;   
  
  p_uart_receive_buffer = pClass_Handle->p_writebuff;
  p_uart_send_buffer = pClass_Handle->p_readbuff;  

  return USBH_OK;
}

/**
  * @brief  USBH_MSC_InterfaceDeInit
  *         The function DeInit the Pipes used for the MSC class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_AISINOCHIP_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  Vender_Class_HandleTypeDef *MSC_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData;

  if (MSC_Handle->OutPipe)
  {
    USBH_ClosePipe(phost, MSC_Handle->OutPipe);
    USBH_FreePipe(phost, MSC_Handle->OutPipe);
    MSC_Handle->OutPipe = 0U;     /* Reset the Channel as Free */
  }

  if (MSC_Handle->InPipe)
  {
    USBH_ClosePipe(phost, MSC_Handle->InPipe);
    USBH_FreePipe(phost, MSC_Handle->InPipe);
    MSC_Handle->InPipe = 0U;     /* Reset the Channel as Free */
  }

  if (phost->pActiveClass->pData)
  {
    USBH_free(phost->pActiveClass->pData);
    phost->pActiveClass->pData = 0U;
  }

  return USBH_OK;
}





/**
  * @brief  USBH_Vendor_Class_SOFProcess
  *         The function is for SOF state
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_Vendor_Class_SOFProcess(USBH_HandleTypeDef *phost)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);

  return USBH_OK;
}

USBH_StatusTypeDef USBH_Vendor_Class_Process(USBH_HandleTypeDef *phost)
{
    return USBH_OK;  
}





/************************ (C) COPYRIGHT AisinoChip *****END OF FILE****/
