/**
  ******************************************************************************
  * @file    usbd_ums.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_hid_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_UMS_H
#define __USB_UMS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for usbd_hid.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */
#define UMS_EPIN_ADDR                              0x81U
#define UMS_EPOUT_ADDR                             0x01U
#define UMS_EPIN_SIZE                              0x40U  
#define UMS_EPOUT_SIZE                             0x40U

#define USB_UMS_CONFIG_DESC_SIZ                    32U

#ifndef UMS_HS_BINTERVAL
#define UMS_HS_BINTERVAL                           0x02U
#endif /* UMS_HS_BINTERVAL */

#ifndef UMS_FS_BINTERVAL
#define UMS_FS_BINTERVAL                           0x01U
#endif /* UMS_FS_BINTERVAL */

#define UMS_CBW_LENGTH                             31  


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


//typedef enum 
//{
//    SCSI_COMM_STATE_IDLE = 0,
//    SCSI_COMM_STATE_CMD,   // received cmd
//    SCSI_COMM_STATE_CMD_DATA_TO_SEND, 
//    SCSI_COMM_STATE_DATA_SENT,  // Data has been sent  
//    SCSI_COMM_STATE_CMD_DATA_TO_RECEIVE,
//    SCSI_COMM_STATE_CMD_DATA_RECEIVED, 
//    SCSI_COMM_STATE
//}SCSI_COMM_STATE;

/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_UMS;
#define USBD_UMS_CLASS &USBD_UMS
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_UMS_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report,uint16_t len);
uint32_t USBD_UMS_GetPollingInterval(USBD_HandleTypeDef *pdev);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_HID_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
