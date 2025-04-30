/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/Inc/usbd_desc.h
  * @author  MCD Application Team
  * @brief   Header for usbd_desc.c module
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_DESC_H
#define __USBD_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define         DEVICE_ID1          (0x1FF07A10)
#define         DEVICE_ID2          (0x1FF07A14)
#define         DEVICE_ID3          (0x1FF07A18)

#define  USB_SIZ_STRING_SERIAL       0x1A

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_DescriptorsTypeDef HID_Desc;  

uint8_t * USBD_HID_DeviceDescriptor_ex(uint16_t *length);     

#endif /* __USBD_DESC_H */
 
/************************ (C) COPYRIGHT *** *****END OF FILE****/
