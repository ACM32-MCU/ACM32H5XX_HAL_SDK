/******************************************************************************
*@file  : fpc2042_it.h
*@brief : This file contains the headers of the interrupt handlers  
******************************************************************************/

#ifndef __FPC2042_IT_H
#define __FPC2042_IT_H


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI15_10_IRQHandler(void);

#endif /* __FPC2042_IT_H */


