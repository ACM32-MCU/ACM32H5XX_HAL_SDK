/**
  ******************************************************************************
  * @file    usbh_msc_bot.c
  * @author  MCD Application Team
  * @brief   This file includes the BOT protocol related functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 AisinoChip.
  * All rights reserved.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usbh_vendor_class.h"
#include "usbh_bulk.h" 


USBH_StatusTypeDef USBH_MSC_BOT_Abort(USBH_HandleTypeDef *phost, uint8_t lun, uint8_t dir);  

extern void printf_buff_byte(volatile uint8_t* buff, uint32_t length);  


/**
  * @brief  USBH_MSC_BOT_Process
  *         The function handle the BOT protocol.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @retval USBH Status
  */
USBH_URBStateTypeDef USBH_Bulk_Read_Data(USBH_HandleTypeDef *phost, uint32_t length)
{
  USBH_StatusTypeDef   error  = USBH_BUSY;

  USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
  Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t toggle = 0U;
    
  
  switch (VendorClass_Handle->in_state)
  {
    case OP_DATA_IN:
      /* Send first packet */
      uart_printf("IN\n");  
      VendorClass_Handle->read_length = length;  
      VendorClass_Handle->read_cnt = 0;  
      USBH_BulkReceiveData(phost, VendorClass_Handle->p_readbuff,  
                           VendorClass_Handle->InEpSize, VendorClass_Handle->InPipe);

      VendorClass_Handle->in_state = OP_DATA_IN_WAIT;  

      break;

    case OP_DATA_IN_WAIT:

      URB_Status = USBH_LL_GetURBState(phost, VendorClass_Handle->InPipe);

      if (URB_Status == USBH_URB_DONE)
      {
        HCD_HandleTypeDef * p_hcd;  
        uart_printf("IN-OK\n");  
        p_hcd = (HCD_HandleTypeDef *)phost->pData;    
        if ( (p_hcd->hc[VendorClass_Handle->InPipe].last_xfer_count < p_hcd->hc[VendorClass_Handle->InPipe].max_packet) || (p_hcd->hc[VendorClass_Handle->InPipe].xfer_count >= VendorClass_Handle->read_length) )
        {
            VendorClass_Handle->in_state  = OP_DATA_IN_DONE;    
            VendorClass_Handle->read_cnt += p_hcd->hc[VendorClass_Handle->InPipe].xfer_count;   
            uart_printf("IN-CNT:%d\n", VendorClass_Handle->read_cnt); 
        }
        else
        { 
            VendorClass_Handle->read_cnt += p_hcd->hc[VendorClass_Handle->InPipe].xfer_count;   
            USBH_BulkReceiveData(phost, VendorClass_Handle->p_readbuff + VendorClass_Handle->read_cnt,      
                                 VendorClass_Handle->InEpSize, VendorClass_Handle->InPipe);   
        }
        
      }
      else if (URB_Status == USBH_URB_STALL)
      {
        /* This is Data IN Stage STALL Condition */
        VendorClass_Handle->in_state  = OP_ERROR_IN;

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
    Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    VendorClass_Handle->out_state = state;   
}


OP_OUT_StateTypeDef USBH_Get_Out_State(USBH_HandleTypeDef *phost)
{
    Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    return VendorClass_Handle->out_state;  
}


 void USBH_Set_In_State(USBH_HandleTypeDef *phost, OP_IN_StateTypeDef state)
{
    Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    VendorClass_Handle->in_state = state;   
}

OP_IN_StateTypeDef USBH_Get_In_State(USBH_HandleTypeDef *phost)
{
    Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData; 
    
    return VendorClass_Handle->in_state;   
}


USBH_StatusTypeDef USBH_Bulk_Write_Data(USBH_HandleTypeDef *phost, uint32_t length)
{
  USBH_StatusTypeDef   status = USBH_BUSY;
  USBH_StatusTypeDef   error  = USBH_BUSY;

  USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
  Vender_Class_HandleTypeDef *VendorClass_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t toggle = 0U;
    
  
  switch (VendorClass_Handle->out_state)
  {
    case OP_DATA_OUT:
    {
        uint32_t write_packet_length;
        
        uart_printf("OUT\n");  
      
        VendorClass_Handle->write_length = length; 
        if (length > VendorClass_Handle->OutEpSize)
        {
            write_packet_length = VendorClass_Handle->OutEpSize;             
        }
        else
        {
            write_packet_length = length;   
        }
        VendorClass_Handle->last_write_cnt = write_packet_length;  
        VendorClass_Handle->write_cnt = 0;  
        USBH_BulkSendData(phost, VendorClass_Handle->p_writebuff,
                        write_packet_length, VendorClass_Handle->OutPipe, 1U);   

        VendorClass_Handle->out_state  = OP_DATA_OUT_WAIT;
        break;
    }

    case OP_DATA_OUT_WAIT:
    {
        uint32_t packet_length;
        URB_Status = USBH_LL_GetURBState(phost, VendorClass_Handle->OutPipe);

        if (URB_Status == USBH_URB_DONE)
        {
            uart_printf("OUT-OK\n");  
          
            VendorClass_Handle->write_cnt += VendorClass_Handle->last_write_cnt;  
            
            if (VendorClass_Handle->write_cnt >=  VendorClass_Handle->write_length)
            {
                VendorClass_Handle->out_state  = OP_DATA_OUT_DONE;  
            }
            else
            {
                if ( (VendorClass_Handle->write_length - VendorClass_Handle->write_cnt) > VendorClass_Handle->OutEpSize)
                {
                    packet_length = VendorClass_Handle->OutEpSize;
                }
                else
                {
                    packet_length = VendorClass_Handle->write_length - VendorClass_Handle->write_cnt;  
                }
                VendorClass_Handle->last_write_cnt = packet_length;  
                USBH_BulkSendData(phost, VendorClass_Handle->p_writebuff + VendorClass_Handle->write_cnt,
                                   packet_length, VendorClass_Handle->OutPipe, 1U); 
            }
        
      }

      else if (URB_Status == USBH_URB_NOTREADY)
      {
            
        if ( (VendorClass_Handle->write_length - VendorClass_Handle->write_cnt) > VendorClass_Handle->OutEpSize)
        {
            packet_length = VendorClass_Handle->OutEpSize;
        }
        else
        {
            packet_length = VendorClass_Handle->write_length - VendorClass_Handle->write_cnt;  
        }
        VendorClass_Handle->last_write_cnt = packet_length;  
        USBH_BulkSendData(phost, VendorClass_Handle->p_writebuff + VendorClass_Handle->write_cnt,
                          packet_length, VendorClass_Handle->OutPipe, 1U); 
            
      }

      else if (URB_Status == USBH_URB_STALL)
      {
        VendorClass_Handle->out_state  = OP_ERROR_OUT;

      }
      else
      {
      }
      break;  
    }  
  

    case OP_ERROR_OUT:
    VendorClass_Handle->out_state =  OP_OUT_UNRECOVERED_ERROR;  
    break;


    case OP_OUT_UNRECOVERED_ERROR:
    VendorClass_Handle->out_state =  OP_DATA_OUT_DONE;  
    break; 
    
    case OP_DATA_OUT_DONE:
    break;

    default:      
      break;
  }
  
  return status;
}



/**
  * @brief  USBH_MSC_BOT_Abort
  *         The function handle the BOT Abort process.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  dir: direction (0: out / 1 : in)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_BOT_Abort(USBH_HandleTypeDef *phost, uint8_t lun, uint8_t dir)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(lun);

  USBH_StatusTypeDef status = USBH_FAIL;
  Vender_Class_HandleTypeDef *MSC_Handle = (Vender_Class_HandleTypeDef *) phost->pActiveClass->pData;

  return status;
}





/************************ (C) COPYRIGHT AisinoChip *****END OF FILE****/



