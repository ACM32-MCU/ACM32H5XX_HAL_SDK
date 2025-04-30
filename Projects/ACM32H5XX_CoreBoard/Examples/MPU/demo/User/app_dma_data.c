/*
  ******************************************************************************
  * Copyright (c)  2008 - 2022, Shanghai AisinoChip Co.,Ltd .
  * @file    APP.c 
  * @version V1.0.0
  * @date    2022
  * @author  Aisinochip Firmware Team  
  * @brief   DMA demo source code.
  ******************************************************************************        
*/
 
#include  "app.h"

uint8_t gu8_Tx1_DMA_Buffer[512];  
uint8_t gu8_Tx2_DMA_Buffer[512];   
DMA_LinkTypeDef DMA_List_Node[2];  
volatile uint32_t dma_complete_times;   