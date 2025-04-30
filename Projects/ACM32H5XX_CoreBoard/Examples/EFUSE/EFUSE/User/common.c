/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : common.c
 * Description : conmmon source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"

uint32_t g_data_buf[1024];


#ifdef DEBUG
/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         uint8_t* buff: buff
 *         uint32_t length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(uint8_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.2x ",buff[i]);	
	}
	printf("\n");
}

/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         uint8_t* buff: buff
 *         uint32_t length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(uint32_t* buff, uint32_t length)
{
	uint32_t i;

	for(i=0;i<length;i++)
	{
		printf("%.8x ",buff[i]);	
	}
	printf("\n");
}

#endif

void delay(uint32_t count)
{
    while(count--);
}

//一个字内大小端转换
void reverse_DWORD(uint32_t *var)
{
    uint8_t *P = (uint8_t *)var;
    uint8_t tmp;

    tmp = P[0];
    P[0] = P[3];
    P[3] = tmp;
    tmp = P[1];
    P[1] = P[2];
    P[2] = tmp;
}

////add by chenbailing
//void delay_ms(uint32_t ms) //
//{
//	delay(ms * FCLK * 1000 / 6);  //delay函数需要6个系统时钟周期
//}

//整批数据前后大小端转换
void reverse_memory(uint8_t *buff, uint32_t length)
{
    uint8_t temp;
    uint8_t *buff_start = buff;
    uint8_t *buff_end = buff + length - 1;

    while(buff_end > buff_start)
    {
        temp = *buff_start;
        *buff_start++ = *buff_end;
        *buff_end-- = temp;
    }
}

uint8_t addr_default_compare(uint32_t addr, uint32_t com_data)
{
	uint32_t rdata;
	rdata = (*(volatile uint32_t *)addr); //读出数据
	if(rdata == com_data)
	{
//		printf("addr default pass :0x%8x = 0x%8x\n", addr, com_data);
		return 0;
	}
	else
	{
		printf("addr default error:0x%8x = 0x%8x ? 0x%8x\n", addr, rdata, com_data);
		return 1;
	}
}

uint8_t addr_write_compare(uint32_t addr, uint32_t wdata, uint32_t com_data)
{
	uint32_t rdata;
	(*(volatile uint32_t *)addr) = wdata; //写入数据

	rdata = (*(volatile uint32_t *)addr); //读出数据
	if(rdata == com_data)
	{
		//printfS("addr write read :0x%8x:0x%8x = 0x%8x\n", addr, wdata, com_data);
		return 0;
	}
	else
	{
		printf("addr write error:0x%8x:0x%8x = 0x%8x ? 0x%8x\n", addr, wdata, rdata, com_data);
		return 1;
	}
}

