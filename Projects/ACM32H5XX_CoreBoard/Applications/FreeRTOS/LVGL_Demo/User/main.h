/******************************************************************************
*@file  : main.h
*@brief : main program
******************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H


   
#include "hal.h"
#include "acm32h5xx_coreboard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "./PSRAM/ic_aps6404m.h"
#include "./PSRAM/xccela_opi.h"
#include "task_disp.h"


typedef enum
{
	TASK_PRIORITY_IDLE = 0,		//lowest priority
	
	TASK_PRIORITY_DISP = 10,
}tTASK_PRIORITY;





#endif /* __MAIN_H */
