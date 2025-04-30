
/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H__
#define __APP_H__

#include "hal.h"
#include "acm32h5xx_coreboard.h"


#define ACM32H5XX_176_EVB



#define LCD_WIDTH           800
#define LCD_HEIGHT          480

#define LCD_BITS_PER_PIXEL  32
#define LCD_BUF_SIZE        (LCD_WIDTH * LCD_HEIGHT * LCD_BITS_PER_PIXEL / 8)
#define LCD_PIXEL_FORMAT    RTGRAPHIC_PIXEL_FORMAT_RGB565

#define LCD_MAX_LAYER_BUF_SIZE					(LCD_WIDTH * LCD_HEIGHT * 4)	//ARGB8888

#define LCD_HSYNC_WIDTH							1
#define LCD_VSYNC_HEIGHT						1


#define LCD_HBP									46
#define LCD_VBP									23
#define LCD_HFP									40
#define LCD_VFP									13


#define PSRAM_RST_PORT							GPIOC
#define PSRAM_RST_PIN							GPIO_PIN_12


#define LCD_RST_PORT							GPIOI
#define LCD_RST_PIN								GPIO_PIN_8
#define LCD_BL_PORT								GPIOH
#define LCD_BL_PIN								GPIO_PIN_15


#define TEST_L1_AREA_WIDTH						LCD_WIDTH	//800
#define TEST_L1_AREA_HEIGHT						LCD_HEIGHT	//480
#define TEST_L1_AREA_X_START					0
#define TEST_L1_AREA_Y_START					0
#define TEST_L1_AREA_X_END						(TEST_L1_AREA_X_START+TEST_L1_AREA_WIDTH)
#define TEST_L1_AREA_Y_END						(TEST_L1_AREA_Y_START+TEST_L1_AREA_HEIGHT)

#define TEST_L2_AREA_WIDTH						LCD_WIDTH	//800
#define TEST_L2_AREA_HEIGHT						LCD_HEIGHT	//480
#define TEST_L2_AREA_X_START					0
#define TEST_L2_AREA_Y_START					0
#define TEST_L2_AREA_X_END						(TEST_L2_AREA_X_START+TEST_L2_AREA_WIDTH)
#define TEST_L2_AREA_Y_END						(TEST_L2_AREA_Y_START+TEST_L2_AREA_HEIGHT)




#define DISP_COLOR_RED							0x00FF0000
#define DISP_COLOR_GREEN						0x0000FF00
#define DISP_COLOR_BLUE							0x000000FF




void APP_Test(void);
void lcd_intf_init(void);
void lcd_disp_init(uint32_t x,uint32_t y);
void lcd_bl_off(void);
void lcd_bl_on(void);

extern uint32_t ltdc_layer1_buf[LCD_MAX_LAYER_BUF_SIZE/4];
extern uint32_t ltdc_layer2_buf[LCD_MAX_LAYER_BUF_SIZE/4];



#endif /* __APP_H__ */

