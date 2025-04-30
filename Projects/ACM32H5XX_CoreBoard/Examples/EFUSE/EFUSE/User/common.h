/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : common.h
 * Description : common header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

#include  "stdio.h"	   //printf .....
#include  "string.h"   //strlen ,memset,strcmp,memcmp,strcpy .....
#include  "stdint.h"	   //typedef .....
#include  "hal.h"


#define SWAP(x)             ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define max(a, b)		    (((a) > (b)) ? (a) : (b))
#define min(a, b)		    (((a) < (b)) ? (a) : (b))

//寄存器值比较
#define REG_COMPARE(reg, value)\
{\
   if((reg) == (value))\
     printf("reg pass :0x%8x = 0x%8x\n", &(reg), (reg));\
   else\
     printf("reg error:0x%8x = 0x%8x ? 0x%8x\n", &(reg), (reg), (value));\
}

//寄存器写比较
#define REG_COMPARE_WRITE(reg, wvalue, value)\
{\
    (reg) = (wvalue);\
    if((reg) == (value))\
            printf("reg write read :0x%8x:0x%8x = 0x%8x\n", &(reg), (wvalue), (reg));\
    else\
            printf("reg write error:0x%8x:0x%8x = 0x%8x ? 0x%8x\n", &(reg), (wvalue), (reg), (value)); \
}

extern uint32_t g_data_buf[];
#define DATABUF             ((uint8_t *)g_data_buf)

/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         uint8_t* buff: buff
 *         uint32_t length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(uint8_t* buff, uint32_t length);

/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         uint8_t* buff: buff
 *         uint32_t length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(uint32_t* buff, uint32_t length);

void delay(uint32_t count);
void reverse_DWORD(uint32_t *var);
void reverse_memory(uint8_t *buff, uint32_t length);
void delay_ms(uint32_t ms);

void delay_ms(uint32_t ms);
uint8_t addr_default_compare(uint32_t addr, uint32_t com_data);
uint8_t addr_write_compare(uint32_t addr, uint32_t wdata, uint32_t com_data);


//#define SET_BIT(REG, BIT)      ((REG) |= (BIT))
//#define CLEAR_BIT(REG, BIT)    ((REG) &= ~(BIT))
//#define READ_BIT(REG, BIT)     ((REG) & (BIT))
//#define CLEAR_REG(REG)         ((REG) = (0x0))
//#define WRITE_REG(REG, VAL)    ((REG) = (VAL))
//#define READ_REG(REG)          ((REG))
//#define MODIFY_REG(REG,MASK,BITS) ((REG) = (((REG)&(~(MASK)))|((BITS)&(MASK))))

#endif

