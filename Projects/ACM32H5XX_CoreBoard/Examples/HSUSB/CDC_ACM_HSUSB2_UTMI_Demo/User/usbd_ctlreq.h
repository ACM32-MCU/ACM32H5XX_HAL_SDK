/**
  ******************************************************************************
  * @file    usbd_req.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_req.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_REQUEST_H
#define __USB_REQUEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_def.h"




/** @defgroup USBD_REQ
  * @brief header file for the usbd_req.c file
  * @{
  */

/** @defgroup USBD_REQ_Exported_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_REQ_Exported_Types
  * @{
  */
/**
  * @}
  */



/** @defgroup USBD_REQ_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_REQ_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_REQ_Exported_FunctionsPrototype
  * @{
  */

USBD_StatusTypeDef USBD_StdDevReq(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
USBD_StatusTypeDef USBD_StdItfReq(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
USBD_StatusTypeDef USBD_StdEPReq(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

void USBD_CtlError(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
void USBD_ParseSetupRequest(USBD_SetupReqTypedef *req, uint8_t *pdata);
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_REQUEST_H */

/**
  * @}
  */

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
