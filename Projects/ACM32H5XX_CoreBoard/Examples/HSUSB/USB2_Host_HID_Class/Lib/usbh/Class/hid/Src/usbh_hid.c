/**
  ******************************************************************************
  * @file    usbh_hid.c
  * @author  MCD Application Team
  * @brief   This file includes the HID protocol related functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 AisinoChip.
  * All rights reserved.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_class.h"
#include "usbh_hid.h" 


USBH_StatusTypeDef USBH_HID_Abort(USBH_HandleTypeDef *phost, uint8_t lun, uint8_t dir);  


USBH_URBStateTypeDef USBH_Bulk_Read_Data(USBH_HandleTypeDef *phost, uint32_t length)
{
  USBH_StatusTypeDef   error  = USBH_BUSY;

  USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
  HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t toggle = 0U;
    
  
  switch (HIDClass_Handle->in_state)
  {
    case OP_DATA_IN:
      /* Send first packet */
//      uart_printf("IN\n");  
      HIDClass_Handle->read_length = length;  
      HIDClass_Handle->read_cnt = 0;  
      USBH_InterruptReceiveData(phost, HIDClass_Handle->p_readbuff,  
                           HIDClass_Handle->InEpSize, HIDClass_Handle->InPipe);

      HIDClass_Handle->in_state = OP_DATA_IN_WAIT;  
      HIDClass_Handle->timer = phost->Timer;  

      break;

    case OP_DATA_IN_WAIT:

      URB_Status = USBH_LL_GetURBState(phost, HIDClass_Handle->InPipe);

      if (URB_Status == USBH_URB_DONE)
      {
        HCD_HandleTypeDef * p_hcd;  
        p_hcd = (HCD_HandleTypeDef *)phost->pData;    
      
        HIDClass_Handle->in_state  = OP_DATA_IN_DONE;    
        HIDClass_Handle->read_cnt += p_hcd->hc[HIDClass_Handle->InPipe].xfer_count;   
        uart_printf("IN-CNT:%d\n", HIDClass_Handle->read_cnt); 
            
      }
      else if (URB_Status == USBH_URB_STALL)
      {
        /* This is Data IN Stage STALL Condition */
        HIDClass_Handle->in_state  = OP_ERROR_IN;

      }
      else
      {
          
      }
      break;
      
    case OP_DATA_IN_DONE:
    break;  
   
    case OP_ERROR_IN:  
      break;

    case OP_IN_UNRECOVERED_ERROR:  
      break;

    default:
      break;
  }
  
  return URB_Status; 
}

void USBH_Set_Out_State(USBH_HandleTypeDef *phost, OP_OUT_StateTypeDef state)
{
    HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    HIDClass_Handle->out_state = state;   
}


OP_OUT_StateTypeDef USBH_Get_Out_State(USBH_HandleTypeDef *phost)
{
    HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    return HIDClass_Handle->out_state;  
}


 void USBH_Set_In_State(USBH_HandleTypeDef *phost, OP_IN_StateTypeDef state)
{
    HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    HIDClass_Handle->in_state = state;   
}

OP_IN_StateTypeDef USBH_Get_In_State(USBH_HandleTypeDef *phost)
{
    HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    return HIDClass_Handle->in_state;   
}


USBH_StatusTypeDef USBH_Bulk_Write_Data(USBH_HandleTypeDef *phost, uint32_t length)
{
  USBH_StatusTypeDef   status = USBH_BUSY;
  USBH_StatusTypeDef   error  = USBH_BUSY;

  USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
  HID_Class_HandleTypeDef *HIDClass_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t toggle = 0U;
    
  
  switch (HIDClass_Handle->out_state)
  {
    case OP_DATA_OUT:
    {
        uint32_t write_packet_length;
        
        uart_printf("OUT\n");  
      
        HIDClass_Handle->write_length = length; 
        if (length > HIDClass_Handle->OutEpSize)
        {
            write_packet_length = HIDClass_Handle->OutEpSize;             
        }
        else
        {
            write_packet_length = length;   
        }
        HIDClass_Handle->last_write_cnt = write_packet_length;  
        HIDClass_Handle->write_cnt = 0;  
        USBH_InterruptSendData(phost, HIDClass_Handle->p_writebuff,
                        write_packet_length, HIDClass_Handle->OutPipe);   

        HIDClass_Handle->out_state  = OP_DATA_OUT_WAIT;
        break;
    }

    case OP_DATA_OUT_WAIT:
    {
        uint32_t packet_length;
        URB_Status = USBH_LL_GetURBState(phost, HIDClass_Handle->OutPipe);

        if (URB_Status == USBH_URB_DONE)
        {
            uart_printf("OUT-OK\n");  
          
            HIDClass_Handle->write_cnt += HIDClass_Handle->last_write_cnt;  
            
            if (HIDClass_Handle->write_cnt >=  HIDClass_Handle->write_length)
            {
                HIDClass_Handle->out_state  = OP_DATA_OUT_DONE;  
            }
            else
            {
                if ( (HIDClass_Handle->write_length - HIDClass_Handle->write_cnt) > HIDClass_Handle->OutEpSize)
                {
                    packet_length = HIDClass_Handle->OutEpSize;
                }
                else
                {
                    packet_length = HIDClass_Handle->write_length - HIDClass_Handle->write_cnt;  
                }
                HIDClass_Handle->last_write_cnt = packet_length;  
                USBH_InterruptSendData(phost, HIDClass_Handle->p_writebuff + HIDClass_Handle->write_cnt,
                                   packet_length, HIDClass_Handle->OutPipe); 
            }
        
      }

      else if (URB_Status == USBH_URB_NOTREADY)
      {
        uart_printf("Send Again\n");      
        if ( (HIDClass_Handle->write_length - HIDClass_Handle->write_cnt) > HIDClass_Handle->OutEpSize)
        {
            packet_length = HIDClass_Handle->OutEpSize;
        }
        else
        {
            packet_length = HIDClass_Handle->write_length - HIDClass_Handle->write_cnt;  
        }
        HIDClass_Handle->last_write_cnt = packet_length;  
        USBH_InterruptSendData(phost, HIDClass_Handle->p_writebuff + HIDClass_Handle->write_cnt,
                          packet_length, HIDClass_Handle->OutPipe); 
            
      }

      else if (URB_Status == USBH_URB_STALL)
      {
        HIDClass_Handle->out_state  = OP_ERROR_OUT;

      }
      else
      {
      }
      break;  
    }  
  

    case OP_ERROR_OUT:
    HIDClass_Handle->out_state =  OP_OUT_UNRECOVERED_ERROR;  
    break;


    case OP_OUT_UNRECOVERED_ERROR:
    HIDClass_Handle->out_state =  OP_DATA_OUT_DONE;  
    break; 
    
    case OP_DATA_OUT_DONE:
    break;

    default:      
      break;
  }
  
  return status;
}



/**
  * @brief  USBH_HID_Abort
  *         The function handle the BOT Abort process.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  dir: direction (0: out / 1 : in)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_Abort(USBH_HandleTypeDef *phost, uint8_t lun, uint8_t dir)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(lun);

  USBH_StatusTypeDef status = USBH_FAIL;
  HID_Class_HandleTypeDef *HID_Handle = (HID_Class_HandleTypeDef *) phost->pActiveClass->pData;

  return status;
}





/************************ (C) COPYRIGHT AisinoChip *****END OF FILE****/



