/**
  ******************************************************************************
  * @file    usbd_core.c
  * @author  MCD Application Team
  * @brief   This file provides all the USBD core functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"



/**
* @brief  USBD_Init
*         Initializes the device stack and load the class driver
* @param  pdev: device instance
* @param  pdesc: Descriptor structure address
* @param  id: Low level core index
* @retval None
*/
USBD_StatusTypeDef USBD_Init(USBD_HandleTypeDef *pdev,
                             USBD_DescriptorsTypeDef *pdesc, uint8_t id)
{
  USBD_StatusTypeDef ret;


  /* Unlink previous class */
  if (pdev->pClass != NULL)
  {
    pdev->pClass = NULL;
  }

  if (pdev->pConfDesc != NULL)
  {
    pdev->pConfDesc = NULL;
  }

  /* Assign USBD Descriptors */
  if (pdesc != NULL)
  {
    pdev->pDesc = pdesc;
  }

  /* Set Device initial State */
  pdev->dev_state = USBD_STATE_DEFAULT;
  pdev->id = id;

  /* Initialize low level driver */
  ret = USBD_LL_Init(pdev);

  return ret;
}

/**
* @brief  USBD_DeInit
*         Re-Initialize th device library
* @param  pdev: device instance
* @retval status: status
*/
USBD_StatusTypeDef USBD_DeInit(USBD_HandleTypeDef *pdev)
{
  USBD_StatusTypeDef ret;

  /* Set Default State */
  pdev->dev_state = USBD_STATE_DEFAULT;

  /* Free Class Resources */
  if (pdev->pClass != NULL)
  {
    pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);
  }

  if (pdev->pConfDesc != NULL)
  {
    pdev->pConfDesc = NULL;
  }

  /* Stop the low level driver  */
  ret = USBD_LL_Stop(pdev);

  if (ret != USBD_OK)
  {
    return ret;
  }

  /* Initialize low level driver */
  ret = USBD_LL_DeInit(pdev);

  return ret;
}

/**
  * @brief  USBD_RegisterClass
  *         Link class driver to Device Core.
  * @param  pDevice : Device Handle
  * @param  pclass: Class handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_RegisterClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass)
{
  uint16_t len = 0U;

  if (pclass == NULL)
  {
#if (USBD_DEBUG_LEVEL > 1U)
    USBD_ErrLog("Invalid Class handle");
#endif
    return USBD_FAIL;
  }

  /* link the class to the USB Device handle */
  pdev->pClass = pclass;

  /* Get Device Configuration Descriptor */
#ifdef USE_USB_FS
  pdev->pConfDesc = (void *)pdev->pClass->GetFSConfigDescriptor(&len);
#else /* USE_USB_HS */
  pdev->pConfDesc = (void *)pdev->pClass->GetHSConfigDescriptor(&len);
#endif /* USE_USB_FS */


  return USBD_OK;
}

/**
  * @brief  USBD_Start
  *         Start the USB Device Core.
  * @param  pdev: Device Handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_Start(USBD_HandleTypeDef *pdev)
{
  /* Start the low level driver  */
  return USBD_LL_Start(pdev);
}

/**
  * @brief  USBD_Stop
  *         Stop the USB Device Core.
  * @param  pdev: Device Handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_Stop(USBD_HandleTypeDef *pdev)
{
  USBD_StatusTypeDef ret;

  /* Free Class Resources */
  if (pdev->pClass != NULL)
  {
    pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);
  }

  if (pdev->pConfDesc != NULL)
  {
    pdev->pConfDesc = NULL;
  }

  /* Stop the low level driver */
  ret = USBD_LL_Stop(pdev);

  return ret;
}

/**
* @brief  USBD_RunTestMode
*         Launch test mode process
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_RunTestMode(USBD_HandleTypeDef  *pdev)
{
  /* Prevent unused argument compilation warning */
  UNUSED(pdev);

  return USBD_OK;
}

/**
* @brief  USBD_SetClassConfig
*        Configure device and start the interface
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status
*/

USBD_StatusTypeDef USBD_SetClassConfig(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  USBD_StatusTypeDef ret = USBD_FAIL;

  if (pdev->pClass != NULL)
  {
    /* Set configuration and Start the Class */
    ret = (USBD_StatusTypeDef)pdev->pClass->Init(pdev, cfgidx);
  }

  return ret;
}

/**
* @brief  USBD_ClrClassConfig
*         Clear current configuration
* @param  pdev: device instance
* @param  cfgidx: configuration index
* @retval status: USBD_StatusTypeDef
*/
USBD_StatusTypeDef USBD_ClrClassConfig(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* Clear configuration and De-initialize the Class process */
  if (pdev->pClass != NULL)
  {
    pdev->pClass->DeInit(pdev, cfgidx);
  }

  return USBD_OK;
}


/**
* @brief  USBD_SetupStage
*         Handle the setup stage
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_SetupStage(USBD_HandleTypeDef *pdev, uint8_t *psetup)
{
  USBD_StatusTypeDef ret;

  USBD_ParseSetupRequest(&pdev->request, psetup);

  pdev->ep0_state = USBD_EP0_SETUP;

  pdev->ep0_data_len = pdev->request.wLength;

  switch (pdev->request.bmRequest & 0x1FU)
  {
    case USB_REQ_RECIPIENT_DEVICE:
      ret = USBD_StdDevReq(pdev, &pdev->request);
      break;

    case USB_REQ_RECIPIENT_INTERFACE:
      ret = USBD_StdItfReq(pdev, &pdev->request);
      break;

    case USB_REQ_RECIPIENT_ENDPOINT:
      ret = USBD_StdEPReq(pdev, &pdev->request);
      break;

    default:
      ret = USBD_LL_StallEP(pdev, (pdev->request.bmRequest & 0x80U));
      break;
  }

  return ret;
}

/**
* @brief  USBD_DataOutStage
*         Handle data OUT stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DataOutStage(USBD_HandleTypeDef *pdev,
                                        uint8_t epnum, uint8_t *pdata)
{
  USBD_EndpointTypeDef *pep;
  USBD_StatusTypeDef ret;

  if (epnum == 0U)
  {
    pep = &pdev->ep_out[0];

    if (pdev->ep0_state == USBD_EP0_DATA_OUT)
    {

    }
    else
    {

    }
  }
  else if ((pdev->pClass->DataOut != NULL) &&
           (pdev->dev_state == USBD_STATE_CONFIGURED))
  {
    ret = (USBD_StatusTypeDef)pdev->pClass->DataOut(pdev, epnum);  

    if (ret != USBD_OK)
    {
      return ret;
    }
  }
  else
  {
    /* should never be in this condition */
    return USBD_FAIL;
  }

  return USBD_OK;
}

/**
* @brief  USBD_DataInStage
*         Handle data in stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DataInStage(USBD_HandleTypeDef *pdev,
                                       uint8_t epnum, uint8_t *pdata)
{

  return USBD_OK;
}

/**
* @brief  USBD_LL_Reset
*         Handle Reset event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Reset(USBD_HandleTypeDef *pdev)
{
  /* Upon Reset call user call back */
  pdev->dev_state = USBD_STATE_DEFAULT;
  pdev->ep0_state = USBD_EP0_IDLE;
  pdev->dev_config = 0U;
  pdev->dev_remote_wakeup = 0U;

  if (pdev->pClassData != NULL)
  {
    pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);
  }

    /* Open EP0 OUT */
  (void)USBD_LL_OpenEP(pdev, 0x00U, USBD_EP_TYPE_CTRL, USB_MAX_EP0_SIZE);
  pdev->ep_out[0x00U & 0xFU].is_used = 1U;

  pdev->ep_out[0].maxpacket = USB_MAX_EP0_SIZE;

  /* Open EP0 IN */
  (void)USBD_LL_OpenEP(pdev, 0x80U, USBD_EP_TYPE_CTRL, USB_MAX_EP0_SIZE);
  pdev->ep_in[0x80U & 0xFU].is_used = 1U;

  pdev->ep_in[0].maxpacket = USB_MAX_EP0_SIZE;

  return USBD_OK;
}

/**
* @brief  USBD_LL_Reset
*         Handle Reset event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_SetSpeed(USBD_HandleTypeDef *pdev,
                                    USBD_SpeedTypeDef speed)
{
  pdev->dev_speed = speed;

  return USBD_OK;
}

/**
* @brief  USBD_Suspend
*         Handle Suspend event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Suspend(USBD_HandleTypeDef *pdev)
{
  pdev->dev_old_state = pdev->dev_state;
  pdev->dev_state = USBD_STATE_SUSPENDED;

  return USBD_OK;
}

/**
* @brief  USBD_Resume
*         Handle Resume event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Resume(USBD_HandleTypeDef *pdev)
{
  if (pdev->dev_state == USBD_STATE_SUSPENDED)
  {
    pdev->dev_state = pdev->dev_old_state;
  }

  return USBD_OK;
}

/**
* @brief  USBD_SOF
*         Handle SOF event
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_SOF(USBD_HandleTypeDef *pdev)
{
  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (pdev->pClass->SOF != NULL)
    {
      pdev->pClass->SOF(pdev);
    }
  }

  return USBD_OK;
}

/**
* @brief  USBD_IsoINIncomplete
*         Handle iso in incomplete event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_IsoINIncomplete(USBD_HandleTypeDef *pdev,
                                           uint8_t epnum)
{
  /* Prevent unused arguments compilation warning */
  UNUSED(pdev);
  UNUSED(epnum);

  return USBD_OK;
}

/**
* @brief  USBD_IsoOUTIncomplete
*         Handle iso out incomplete event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_IsoOUTIncomplete(USBD_HandleTypeDef *pdev,
                                            uint8_t epnum)
{
  /* Prevent unused arguments compilation warning */
  UNUSED(pdev);
  UNUSED(epnum);

  return USBD_OK;
}

/**
* @brief  USBD_DevConnected
*         Handle device connection event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DevConnected(USBD_HandleTypeDef *pdev)
{
  /* Prevent unused argument compilation warning */
  UNUSED(pdev);

  return USBD_OK;
}

/**
* @brief  USBD_DevDisconnected
*         Handle device disconnection event
* @param  pdev: device instance
* @retval status
*/
USBD_StatusTypeDef USBD_LL_DevDisconnected(USBD_HandleTypeDef *pdev)
{
  /* Free Class Resources */
  pdev->dev_state = USBD_STATE_DEFAULT;

  if (pdev->pClass != NULL)
  {
    pdev->pClass->DeInit(pdev, (uint8_t)pdev->dev_config);
  }

  return USBD_OK;
}


/**
* @brief  USBD_LL_Bus_Activity
*         Handle device bus activities, such as suspend/resume/reset/enum done.
* @param  pdev: device instance
* @retval status
*/

USBD_StatusTypeDef USBD_LL_Bus_Activity(USBD_HandleTypeDef *pdev)
{
    uint32_t USBx_BASE;
    PCD_HandleTypeDef * p_PCD; 
    USB_OTG_GlobalTypeDef * p_OTG;  

    p_PCD = (PCD_HandleTypeDef *)(pdev->pData);      
    USBx_BASE = (uint32_t)(p_PCD->Instance);  
    p_OTG = (USB_OTG_GlobalTypeDef * )USBx_BASE;    

    if (0 == ((p_OTG->GINTSTS) & (USB_OTG_GINTSTS_USBSUSP|USB_OTG_GINTSTS_WKUINT|USB_OTG_GINTSTS_USBRST   \
        |USB_OTG_GINTSTS_ENUMDNE|USB_OTG_GINTSTS_SOF)) )   
    {
        return USBD_OK;   
    }
    
    if (p_OTG->GINTSTS & USB_OTG_GINTSTS_USBSUSP)
    {
        p_OTG->GINTSTS = USB_OTG_GINTSTS_USBSUSP;   
    }
      
    if (p_OTG->GINTSTS & USB_OTG_GINTSTS_WKUINT)
    {
        p_OTG->GINTSTS = USB_OTG_GINTSTS_WKUINT;   
    }
      
    if (p_OTG->GINTSTS & USB_OTG_GINTSTS_USBRST)
    {
        int i = 0; 
        USBx_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
        (void)USB_FlushTxFifo(p_PCD->Instance, 0x10U); // fifo num = 16 means all fifo 

        for (i = 0U; i < p_PCD->Init.dev_endpoints; i++)
        {
            USBx_INEP(i)->DIEPINT = 0xFB7FU;
            USBx_INEP(i)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
            USBx_INEP(i)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
            USBx_OUTEP(i)->DOEPINT = 0xFB7FU;
            USBx_OUTEP(i)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
            USBx_OUTEP(i)->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
        }
        USBx_DEVICE->DAINTMSK |= 0x10001U;  // IN EP0 and Out EP0, for device EP inerrupt mask   
        
        USBx_DEVICE->DOEPMSK |= USB_OTG_DOEPMSK_STUPM |
                                USB_OTG_DOEPMSK_XFRCM |
                                USB_OTG_DOEPMSK_EPDM |
                                USB_OTG_DOEPMSK_OTEPSPRM |
                                USB_OTG_DOEPMSK_NAKM;  //transfer completely, EP disabled, setup phase end, status phase received, NAK 

                                 //USB_OTG_DIEPMSK_TOM |  
        USBx_DEVICE->DIEPMSK |=  USB_OTG_DIEPMSK_XFRCM |
                                                         USB_OTG_DIEPMSK_EPDM;  //transfer completely, EP disabled, timeout  
        USBx_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD; 
        
        if (p_PCD->Init.dma_enable == 1U)  
        {
            USBx_OUTEP(0U)->DOEPDMA = (uint32_t)p_PCD->Setup;   
            /* EP enable */
            USBx_OUTEP(0U)->DOEPCTL |= USB_OTG_DOEPCTL_EPENA | USB_OTG_DOEPCTL_USBAEP;
        }
        
        p_OTG->GINTSTS = USB_OTG_GINTSTS_USBRST;   
        USB_PrintfS("Reset\n");  
    }
      
    if (p_OTG->GINTSTS & USB_OTG_GINTSTS_ENUMDNE)
    {
        (void)USB_ActivateSetup(p_PCD->Instance);
        p_PCD->Init.speed = USB_GetDevSpeed(p_PCD->Instance);

        /* Set USB Turnaround time */
        (void)USB_SetTurnaroundTime(p_PCD->Instance,
                                  HAL_RCC_GetHCLKFreq(),
                                  (uint8_t)p_PCD->Init.speed);  
        
         USBD_LL_Reset(pdev);    
         USBD_LL_SetSpeed(p_PCD->pData, p_PCD->Init.speed);  
      
         p_OTG->GINTSTS = USB_OTG_GINTSTS_ENUMDNE;   
    }
      
    if (p_OTG->GINTSTS & USB_OTG_GINTSTS_SOF)
    {
        p_OTG->GINTSTS = USB_OTG_GINTSTS_SOF;     
    }
      
    return USBD_OK;  
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

