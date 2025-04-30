/**
  ******************************************************************************
  * @file    usbh_msc.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_msc.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 AisinoChip.
  * All rights reserved.
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_VENDOR_CLASS_H
#define __USBH_VENDOR_CLASS_H  

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"

#include "usbh_bulk.h"



#define USB_READ_BUFFER_LEN    1024

#define USB_WRITE_BUFFER_LEN   1024






typedef struct _Vender_Class
{
  uint8_t              InPipe;
  uint8_t              OutPipe;
  uint8_t              OutEp;
  uint8_t              InEp;
  uint16_t             OutEpSize;
  uint16_t             InEpSize;
  uint8_t              *p_readbuff;
  uint32_t              read_length;
  uint32_t              read_cnt;  
  uint8_t              *p_writebuff;  
  uint32_t              write_length;
    uint32_t            last_write_cnt;
  uint32_t              write_cnt; 
  OP_OUT_StateTypeDef   out_state;   
  OP_IN_StateTypeDef    in_state;     
}
Vender_Class_HandleTypeDef;



#define USB_AISINOCHIP_CLASS                             0xDCU

#define USB_AISINOCHIP_SUBCLASS_CODE               0xA0
#define USB_AISINOCHIP_CLASS_PROTOCOL_CODE         0xB0  



extern USBH_ClassTypeDef  USBH_Aisinochip;
#define USBH_AISINOHCIP_CLASS    &USBH_Aisinochip



#ifdef __cplusplus
}
#endif

#endif  /* __USBH_VENDOR_CLASS_H */


/************************ (C) COPYRIGHT AisinoChip *****END OF FILE****/



