#ifndef __LCD8080_H__
#define __LCD8080_H__


#include "app.h"
#include "fonts.h"


/* 定义 LCD 驱动芯片 ID */
#define LCDID_UNKNOWN                       0x0000
#define LCDID_ILI9341                       0x9341
#define LCDID_ST7789V                       0x8552

#define LINE(x)                             ((x) * (((FONT_SelectTypeDef *)LCD_GetFont())->Height))
#define LINEY(x)                            ((x) * (((FONT_SelectTypeDef *)LCD_GetFont())->Width))

/******************************* ILI9341 显示屏的 FMC 参数定义 ***************************/
//FMC_Bank3_NORSRAM用于LCD命令操作的地址
#define      FMC_Addr_ILI9341_CMD          ((uint32_t) FMC_BANK_LCD)

//FMC_Bank3_NORSRAM用于LCD数据操作的地址      
#define      FMC_Addr_ILI9341_DATA         ((uint32_t) (FMC_BANK_LCD + 2))


//复位引脚 A10地址=PG0
#define      ILI9341_RST_PORT               GPIOG
#define      ILI9341_RST_PIN                GPIO_PIN_0

//背光引脚 A1地址=PF1
#define      ILI9341_BK_PORT                GPIOF
#define      ILI9341_BK_PIN                 GPIO_PIN_1

/*************************************** 调试预用 ******************************************/
#define      DEBUG_DELAY()    

/***************************** ILI934 显示区域的起始坐标和总行列数 ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //起始点的X坐标
#define      ILI9341_DispWindow_Y_Star		    0     //起始点的Y坐标

#define 	 ILI9341_LESS_PIXEL                 240			//液晶屏较短方向的像素宽度
#define 	 ILI9341_MORE_PIXEL                 320			//液晶屏较长方向的像素宽度

//根据液晶扫描方向而变化的XY像素宽度
//调用ILI9341_GramScan函数设置方向时会自动更改
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//液晶屏扫描模式
//参数可选值为0-7
extern uint8_t LCD_SCAN_MODE;

/******************************* 定义 ILI934 显示屏常用颜色 ********************************/
#define      BACKGROUND		                    BLACK   //默认背景颜色

#define      WHITE		 		                0xFFFF	   //白色
#define      BLACK                              0x0000	   //黑色 
#define      GREY                               0xF7DE	   //灰色 
#define      BLUE                               0x001F	   //蓝色 
#define      BLUE2                              0x051F	   //浅蓝色 
#define      RED                                0xF800	   //红色 
#define      MAGENTA                            0xF81F	   //红紫色，洋红色 
#define      GREEN                              0x07E0	   //绿色 
#define      CYAN                               0x7FFF	   //蓝绿色，青色 
#define      YELLOW                             0xFFE0	   //黄色 
#define      BRED                               0xF81F
#define      GRED                               0xFFE0
#define      GBLUE                              0x07FF



/******************************* 定义 ILI934 常用命令 ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define      CMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define      CMD_SetPixel		 		        0x2C	     //填充像素






void FMC_LCD8080_Test(void);





#endif





