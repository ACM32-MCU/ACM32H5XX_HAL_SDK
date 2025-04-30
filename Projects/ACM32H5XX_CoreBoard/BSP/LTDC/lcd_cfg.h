/*
  ******************************************************************************
  * Copyright (c)  2008 - 2022, Shanghai AisinoChip Co.,Ltd .
  * @file    lcd_cfg.h
  * @version V1.0.0
  * @date    2022
  * @author  Aisinochip Firmware Team  
  * @brief  
  ******************************************************************************        
*/

#ifndef __LCD_CFG_H__
#define	__LCD_CFG_H__



//LCD property
#define LCD_WIDTH           800
#define LCD_HEIGHT          480


#define LCD_RST_PORT							GPIOI
#define LCD_RST_PIN								GPIO_PIN_8
#define LCD_BL_PORT								GPIOH
#define LCD_BL_PIN								GPIO_PIN_15

#define LCD_BUF_NUM								2


#define LCD_HSYNC_WIDTH							1
#define LCD_VSYNC_HEIGHT						1

#define LCD_HBP									46
#define LCD_VBP									23
#define LCD_HFP									40
#define LCD_VFP									13


#define DISP_L1_AREA_WIDTH						LCD_WIDTH	//800
#define DISP_L1_AREA_HEIGHT						LCD_HEIGHT	//480
#define DISP_L1_AREA_X_START					0
#define DISP_L1_AREA_Y_START					0
#define DISP_L1_AREA_X_END						(DISP_L1_AREA_X_START+DISP_L1_AREA_WIDTH)
#define DISP_L1_AREA_Y_END						(DISP_L1_AREA_Y_START+DISP_L1_AREA_HEIGHT)

#define DISP_L2_AREA_WIDTH						LCD_WIDTH	//800
#define DISP_L2_AREA_HEIGHT						LCD_HEIGHT	//480
#define DISP_L2_AREA_X_START					0
#define DISP_L2_AREA_Y_START					0
#define DISP_L2_AREA_X_END						(DISP_L2_AREA_X_START+DISP_L2_AREA_WIDTH)
#define DISP_L2_AREA_Y_END						(DISP_L2_AREA_Y_START+DISP_L2_AREA_HEIGHT)


#define LCD_MAX_LAYER_BUF_SIZE					(LCD_WIDTH * LCD_HEIGHT * 4)	//ARGB8888




#endif /* __LCD_CFG_H__ */
