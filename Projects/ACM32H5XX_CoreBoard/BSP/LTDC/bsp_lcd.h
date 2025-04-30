/*
  ******************************************************************************
  * Copyright (c)  2008 - 2022, Shanghai AisinoChip Co.,Ltd .
  * @file    bsp_lcd.h
  * @version V1.0.0
  * @date    2022
  * @author  Aisinochip Firmware Team  
  * @brief  
  ******************************************************************************        
*/

#ifndef __BSP_LCD_H
#define	__BSP_LCD_H


/* Includes ------------------------------------------------------------------*/
#include "acm32h5xx_hal_conf.h"


/* LCD 层像素格式*/
#define ARGB8888 	LTDC_PIXEL_FORMAT_ARGB8888  /*!< ARGB8888 LTDC像素格式 */
#define RGB888 		LTDC_PIXEL_FORMAT_RGB888    /*!< RGB888 LTDC像素格式   */
#define RGB565 		LTDC_PIXEL_FORMAT_RGB565    /*!< RGB565 LTDC像素格式   */
#define ARGB1555 	LTDC_PIXEL_FORMAT_ARGB1555  /*!< ARGB1555 LTDC像素格式 */
#define ARGB4444 	LTDC_PIXEL_FORMAT_ARGB4444  /*!< ARGB4444 LTDC像素格式 */


#define DMA2D_LAYER_FG							1
#define DMA2D_LAYER_BG							0

#define LTDC_DISP_LAYER1						0
#define LTDC_DISP_LAYER2						1


#define DMA2D_FG_CLUT_ADDR						(DMA2D_BASE_ADDR+0x0400)
#define DMA2D_BG_CLUT_ADDR						(DMA2D_BASE_ADDR+0x0800)



typedef struct
{
	int16_t X;
	int16_t Y;
}Point, * pPoint;

/**
  * @brief  字体对齐模式
  */
typedef enum
{
	CENTER_MODE = 0x01,    /* 居中对齐 */
	RIGHT_MODE = 0x02,    /* 右对齐   */
	LEFT_MODE = 0x03     /* 左对齐   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
/**
  * @brief  LCD status structure definition
  */
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

  /**
	* @brief  LCD FB_StartAddress
	*/
//#define LCD_FB_START_ADDRESS       ((uint32_t)0xC0000000)
	/**
	  * @brief  LCD color
	  */
#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)
#define TRANSPARENCY 			      ((uint32_t)0x00FFFFFF)	//透明

	  /**
		* @brief LCD default font
		*/
#define LCD_DEFAULT_FONT        Font24 



void  LCD_Init(void);
uint8_t  LCD_DeInit(void);
uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);
void     LCD_SetXSize(uint32_t imageWidthPixels);
void     LCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     LCD_LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address);
void     LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address, uint32_t ReloadType);
void     LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
void     LCD_ResetColorKeying(uint32_t LayerIndex);
void     LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     LCD_SelectLayer(uint32_t LayerIndex);
void     LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);


void     LCD_DisplayOff(void);
void     LCD_DisplayOn(void);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void     LCD_MspInit(LTDC_HandleTypeDef* hltdc, void* Params);
void     LCD_MspDeInit(LTDC_HandleTypeDef* hltdc, void* Params);
void     LCD_ClockConfig(void);

void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address, uint32_t PixelFormat, uint32_t XSize, uint32_t YSize, uint32_t LoadType);

void  Disable_LTDC(void);

void LL_mem2mem_layer_cfg(uint32_t LayerIndex, uint32_t OffLine, uint32_t ColorMode, uint32_t Alpha, uint32_t AlphaMode);
void LL_mem2mem(uint32_t OutPixFormat, uint32_t LayerIndex, uint32_t OutOffLine, void *pDst, void *pSrc, uint32_t xSize, uint32_t ySize, uint32_t Mode, DMA2D_CLUTCfgTypeDef *pCLUTCfg);
void LL_mem2mem_blend(uint32_t OutPixFormat, uint32_t OutOffLine, void *pDst, void *pFgSrc, void *pBgSrc, uint32_t xSize, uint32_t ySize, uint32_t Mode);
void LCD_LayerInit1(uint16_t LayerIndex, uint32_t FB_Address, uint32_t PixelFormat, uint32_t XSize, uint32_t YSize, uint32_t LoadType);
void LCD_LayerInit2(uint16_t LayerIndex, uint32_t FB_Address, uint32_t PixelFormat, uint32_t XSize, uint32_t YSize, uint32_t LoadType);
void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);



#endif /* __BSP_LCD_H */
