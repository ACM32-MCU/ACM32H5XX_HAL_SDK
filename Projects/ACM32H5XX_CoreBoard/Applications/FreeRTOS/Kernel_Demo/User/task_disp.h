/******************************************************************************
*@file  : task_disp.h
*@brief : 
******************************************************************************/

#include "hal.h"
#include "acm32h5xx_coreboard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "./PSRAM/ic_aps6404m.h"
#include "./LTDC/bsp_lcd.h"
#include "lcd_cfg.h"
#include "lv_port_disp.h"


//task property
#define configTASK_DISP_STACK_SIZE				4096




void vDispTask(void *pvParameters);

