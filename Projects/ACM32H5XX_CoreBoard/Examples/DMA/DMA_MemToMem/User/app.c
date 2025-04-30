
/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"   


#define TRANSFER_SIZE_TEST   0x4000  

DMA_HandleTypeDef DMA1_Ch0Handle;

volatile uint32_t tick1, tick2;
 
/******************************************************************************
*@brief : app test
*@param : none
*@return: none
******************************************************************************/
void APP_Test(void)
{
    uint32_t i, delta;   
    uint32_t Error;
      
    printfS("DMA MemToMem Demo\r\n\r\n");
    
	BSP_LED_Init();
	BSP_LED_On();
    HAL_Delay(500);
    
	DMA1_Ch0Handle.Instance             = DMA1_Channel0;
	DMA1_Ch0Handle.Init.Mode            = DMA_MODE_NORMAL;
	DMA1_Ch0Handle.Init.DataFlow        = DMA_DATAFLOW_M2M;
	DMA1_Ch0Handle.Init.ReqID           = DMA1_REQ_M2M;
	DMA1_Ch0Handle.Init.SrcIncDec       = DMA_SRCINCDEC_INC;
	DMA1_Ch0Handle.Init.DestIncDec      = DMA_DESTINCDEC_INC;
	DMA1_Ch0Handle.Init.SrcWidth        = DMA_SRCWIDTH_BYTE;
	DMA1_Ch0Handle.Init.DestWidth       = DMA_SRCWIDTH_BYTE;  
	DMA1_Ch0Handle.Init.SrcBurst        = DMA_SRCBURST_128;
	DMA1_Ch0Handle.Init.DestBurst       = DMA_DESTBURST_128; 
	DMA1_Ch0Handle.Init.Lock            = DISABLE; 
	DMA1_Ch0Handle.Init.SrcMaster       = DMA_SRCMASTER_1;
	DMA1_Ch0Handle.Init.DestMaster      = DMA_DESTMASTER_2;  
	
	HAL_DMA_Init(&DMA1_Ch0Handle);
	       
    HAL_DMA_Start(&DMA1_Ch0Handle, (uint32_t)0x20038000, (uint32_t)0x4000, TRANSFER_SIZE_TEST);  //16KB, from ITCM to SRAM2    
    
    tick1 = DWT->CYCCNT;    
    
    while (HAL_DMA_GetFlag(&DMA1_Ch0Handle, DMA_FLAG_RTC) == 0);
    
    tick2 = DWT->CYCCNT;      
    
    delta = tick2 - tick1; 
    
    printfS("delta cnt: %u, time:%u us\r\n", delta, delta/220);      
    
	Error = 0;
    if (memcmp((void *)0x20038000, (void *)0x4000, TRANSFER_SIZE_TEST) != 0)    
    {
        Error = 1;  
    }
    
    if (Error == 0)
    {
        printfS("test result: successful!\r\n");
    }
    else
    {
        printfS("test result: failed!\r\n");
    }
      
}

