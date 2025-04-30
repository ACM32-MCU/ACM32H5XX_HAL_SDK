/******************************************************************************
*@file  : task_fatfs.h
*@brief : 
******************************************************************************/

#ifndef _TASK_FATFS_DEFINED
#define _TASK_FATFS_DEFINED


#include "hal.h"
#include "acm32h5xx_coreboard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ff.h"

//////////////////////////////////////////////////////////////////////////////////

#define configTASK_FATFS_STACK_SIZE   2048

//f_typetell返回的类型定义
//根据表FILE_TYPE_TBL获得.
#define F_BIN		0X00	//bin文件
#define F_LRC		0X10	//lrc文件
#define F_NES		0X20	//nes文件
#define F_TEXT		0X30	//.txt文件
#define F_C			0X31	//.c文件
#define F_H			0X32    //.h文件
#define F_FLAC		0X4C	//flac文件
#define F_BMP		0X50	//bmp文件
#define F_JPG		0X51	//jpg文件
#define F_JPEG		0X52	//jpeg文件		 
#define F_GIF		0X53	//gif文件  
#define F_MUSIC		0X40  //音乐文件

void SD_Fatfs_Init(void);
void Fatfs_Test(void);
extern uint8_t f_typetell(uint8_t *fname);						//识别文件类型
extern uint8_t mf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free);	//得到磁盘总容量和剩余容量
extern uint32_t f_fdsize(uint8_t *fdname);						  //得到文件夹大小
extern FRESULT f_scanfiles(char* path,char *strbuf,uint8_t mode); //扫描文件夹里面的文件名称
extern uint16_t f_getflietnum(char *path,uint8_t ftpye);          //获取文件夹中文件总数量
extern uint8_t f_getfileindext(char *path,uint16_t *flieindextbl,uint8_t ftpye); //获取文件夹中文件索引
extern FRESULT f_getfile_spec_data(char *path,uint8_t *outbuf,uint32_t offset,uint32_t fsize);//读取文件的指定位置指定长度的数据

int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);







#endif