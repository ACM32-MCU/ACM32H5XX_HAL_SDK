/******************************************************************************
*@file  : reg_scan.h
*@brief : header file
******************************************************************************/
#ifndef __REG_SCAN_H__
#define __REG_SCAN_H__

#include "hal.h"

//读出寄存器值，与期望值进行比较
uint8_t reg_default_compare(uint32_t addr, uint32_t cdata);

//读出寄存器缺省值，与期望值进行比较，掩码位不比较
uint8_t reg_default_compare_mask(uint32_t addr, uint32_t mdata, uint32_t cdata);

//先写入寄存器值，再读出与期望值进行比较
uint8_t reg_write_compare(uint32_t addr, uint32_t wdata, uint32_t cdata);

//先写入寄存器值，再读出与期望值进行比较，掩码位不写入和比较
uint8_t reg_write_compare_mask(uint32_t addr, uint32_t mdata, uint32_t wdata, uint32_t cdata);

#endif
