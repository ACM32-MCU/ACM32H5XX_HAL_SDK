/**
  ******************************************************************************
  * @file    usbh_msc_bot.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_msc_bot.c
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
#ifndef __USBH_BULK_H__
#define __USBH_BULK_H__    

#ifdef __cplusplus  
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */


typedef enum
{
  OP_OUT_NOTREADY,   
  OP_DATA_OUT,
  OP_DATA_OUT_WAIT,
  OP_DATA_OUT_DONE,
  OP_ERROR_OUT,
  OP_OUT_UNRECOVERED_ERROR   
}
OP_OUT_StateTypeDef; 

typedef enum
{
  OP_IN_NOTREADY,   
  OP_DATA_IN,
  OP_DATA_IN_WAIT,
  OP_DATA_IN_DONE,
  OP_ERROR_IN,
  OP_IN_UNRECOVERED_ERROR 
}
OP_IN_StateTypeDef; 





#ifdef __cplusplus
}
#endif

#endif  /* __USBH_BULK_H__ */  



/************************ (C) COPYRIGHT AisinoChip *****END OF FILE****/

