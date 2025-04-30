/*
  ******************************************************************************
  * Copyright (c)  2008 - 2021, Shanghai AisinoChip Co.,Ltd .
  * @file    bsp_lcd.c
  * @version V1.0.0
  * @date    2022
  * @author  Aisinochip Firmware Team
  * @brief
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "bsp_lcd.h"

#include <string.h>
#include <stdlib.h>


#include "acm32h5xx_hal_conf.h"
#include "lcd_cfg.h"

#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))

#define ABS(X)  ((X) > 0 ? (X) : -(X))

LTDC_HandleTypeDef  Ltdc_Handler = {0};
DMA2D_HandleTypeDef Dma2d_Handler;

/* Default LCD configuration with LCD Layer 1 */
uint32_t            ActiveLayer = 0;


/**
 * @brief  Initializes the LCD.
 * @param  None
 * @retval None
 */


void Close_Layer(uint16_t LayerIndex)
{
    /* Enable LTDC_Layer by setting LEN bit */
    LTDC_LAYER(&Ltdc_Handler, LayerIndex)->CR &= ~(uint32_t)LTDC_LxCR_LEN;
}


void  Disable_LTDC(void)
{
    __HAL_LTDC_ENABLE(&Ltdc_Handler);
}



/**
 * @brief  初始化控制LCD的IO
 * @param  无
 * @retval 无
 */
static void LCD_GPIO_Config(void)
{


}
/**
  * @brief  时钟配置
  * @retval 无
  */
void LCD_ClockConfig(void)
{
    /* use external 12MHz crystal oscillator, LTDC clock =  (12*200/9)/PLL2Q/DckCfg */
    HAL_RCC_PLL2Config(ENABLE, RCC_PLL_SOURCE_XTH, 9, 200, 2, 15);
    HAL_RCC_PLL2QCLKConfig(ENABLE);

	HAL_LTDC_Clk_Div(RCC_PLL2CR_Q_DIV_10, RCC_DCKCFG_DIV_2);
}


__IO uint8_t  Test_LineITCnt;
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{	
	Test_LineITCnt++;
}

void LTDC_IRQHandler(void)
{
    printfS("I:%x\r\n", LTDC->ISR);
    HAL_LTDC_IRQHandler(&Ltdc_Handler);
}

void LTDC_ERR_IRQHandler(void)
{
    printfS("E:%x\r\n", LTDC->ISR);
    HAL_LTDC_ERR_IRQHandler(&Ltdc_Handler);
}

void DMA2D_IRQHandler(void)
{
    printfS("INT:%x\r\n", DMA2D->ISR);
    HAL_DMA2D_IRQHandler(&Dma2d_Handler);
}



/**
  * @brief  获取LCD当前有效层X轴的大小
  * @retval X轴的大小
  */
uint32_t LCD_GetXSize(void)
{
    return Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  获取LCD当前有效层Y轴的大小
  * @retval Y轴的大小
  */
uint32_t LCD_GetYSize(void)
{
    return Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  设置LCD当前有效层Y轴的大小
  * @param  imageWidthPixels：图像宽度像素个数
  * @retval 无
  */
void LCD_SetXSize(uint32_t imageWidthPixels)
{
    Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  设置LCD当前有效层Y轴的大小
  * @param  imageHeightPixels：图像高度像素个数
  * @retval None
  */
void LCD_SetYSize(uint32_t imageHeightPixels)
{
    Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  初始化LCD层
  * @param  LayerIndex:  前景层(层1)或者背景层(层0)
  * @param  FB_Address:  每一层显存的首地址
  * @param  PixelFormat: 层的像素格式
  * @retval 无
  */
void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address, uint32_t PixelFormat, uint32_t XSize, uint32_t YSize, uint32_t LoadType)
{
    LTDC_LayerCfgTypeDef  layer_cfg;

    /* 层初始化 */
	layer_cfg.WindowX0 = DISP_L1_AREA_X_START;				//窗口起始位置X坐标
    layer_cfg.WindowX1 = (layer_cfg.WindowX0 + XSize);  //窗口结束位置X坐标
	layer_cfg.WindowY0 = DISP_L1_AREA_Y_START;				//窗口起始位置Y坐标
    layer_cfg.WindowY1 = (layer_cfg.WindowY0 + YSize); //窗口结束位置Y坐标
    layer_cfg.PixelFormat = PixelFormat;    //像素格式
    layer_cfg.FBStartAdress = FB_Address; //层显存首地址
    layer_cfg.Alpha = 0;                //用于混合的透明度常量，范围（0—255）0为完全透明
    layer_cfg.Alpha0 = 0;                   //默认透明度常量，范围（0—255）0为完全透明
    layer_cfg.Backcolor.Blue = 0;           //层背景颜色蓝色分量
    layer_cfg.Backcolor.Green = 0;      //层背景颜色绿色分量
    layer_cfg.Backcolor.Red = 0;            //层背景颜色红色分量
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;//层混合系数1
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;//层混合系数2
    layer_cfg.ImageWidth = XSize;   //LCD_GetXSize();//设置图像宽度
    layer_cfg.ImageHeight = YSize;  //LCD_GetYSize();//设置图像高度

    HAL_LTDC_ConfigLayer(&Ltdc_Handler, &layer_cfg, LayerIndex); //设置选中的层参数

    HAL_LTDC_Reload(&Ltdc_Handler, LoadType); //LTDC_RELOAD_VERTICAL_BLANKING   LTDC_RELOAD_IMMEDIATE
}


/**
  * @brief  fill destinate cache buffer
  * @param  LayerIndex: current layer
  * @param  pDst: poiint to destinate cache buffer
  * @param  xSize: cache buffer width
  * @param  ySize: cache buffer height
  * @param  OffLine: offset
  * @param  ColorIndex: current color
  * @retval None
  */
void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
    Dma2d_Handler.Init.Mode         = DMA2D_R2M;
    if (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    {
        Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_RGB565;
    }
    else if (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
    {
        Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    }
    else if (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
    {
        Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_RGB888;
    }
    else if (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
    {
        Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_ARGB1555;
    }
    else if (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
    {
        Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_ARGB4444;
    }
    Dma2d_Handler.Init.OutputOffset = OffLine;

    Dma2d_Handler.Instance = DMA2D;

    //DMA2D initialize
    if (HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&Dma2d_Handler, LayerIndex) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&Dma2d_Handler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
            {
                //polling transfer result
                HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 10);  //delete for test only
            }
        }
    }
}


void LL_mem2mem_layer_cfg(uint32_t LayerIndex, uint32_t OffLine, uint32_t ColorMode, uint32_t Alpha, uint32_t AlphaMode)
{
//  Dma2d_Handler.Init.OutputOffset = OffLine;
    Dma2d_Handler.LayerCfg[LayerIndex].InputOffset = OffLine;
    Dma2d_Handler.LayerCfg[LayerIndex].InputColorMode = ColorMode;
    Dma2d_Handler.LayerCfg[LayerIndex].AlphaMode = AlphaMode;
    Dma2d_Handler.LayerCfg[LayerIndex].InputAlpha = Alpha;
}


void LL_mem2mem(uint32_t OutPixFormat, uint32_t LayerIndex, uint32_t OutOffLine, void *pDst, void *pSrc, uint32_t xSize, uint32_t ySize, uint32_t Mode, DMA2D_CLUTCfgTypeDef *pCLUTCfg)
{
    Dma2d_Handler.Init.Mode         = Mode;
    Dma2d_Handler.Init.ColorMode    = OutPixFormat;

    Dma2d_Handler.Init.OutputOffset = OutOffLine;
    Dma2d_Handler.Instance = DMA2D;

    //DMA2D initialize
    if (HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&Dma2d_Handler, LayerIndex) == HAL_OK)
        {
            if ((pCLUTCfg != NULL) && (Mode == DMA2D_M2M_PFC))
            {
                HAL_DMA2D_EnableCLUT(&Dma2d_Handler, DMA2D_LAYER_FG);
                HAL_DMA2D_CLUTLoad(&Dma2d_Handler, *pCLUTCfg, DMA2D_LAYER_FG);
            }

            if (HAL_DMA2D_Start(&Dma2d_Handler, (uint32_t)pSrc, (uint32_t)pDst, xSize, ySize) == HAL_OK)
            {
                //polling transfer result
                HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 1000);
            }
        }
    }
}


void LL_mem2mem_blend(uint32_t OutPixFormat, uint32_t OutOffLine, void *pDst, void *pFgSrc, void *pBgSrc, uint32_t xSize, uint32_t ySize, uint32_t Mode)
{
    Dma2d_Handler.Init.Mode         = Mode;
    Dma2d_Handler.Init.ColorMode    = OutPixFormat;

    Dma2d_Handler.Init.OutputOffset = OutOffLine;
    Dma2d_Handler.Instance = DMA2D;

    //DMA2D initialize
    if (HAL_DMA2D_Init(&Dma2d_Handler) != HAL_OK)
    {
        return;
    }

    if (HAL_DMA2D_ConfigLayer(&Dma2d_Handler, 0) != HAL_OK)
    {
        return;
    }

    if (HAL_DMA2D_ConfigLayer(&Dma2d_Handler, 1) != HAL_OK)
    {
        return;
    }

    if (HAL_DMA2D_BlendingStart(&Dma2d_Handler, (uint32_t)pFgSrc, (uint32_t)pBgSrc, (uint32_t)pDst, xSize, ySize) != HAL_OK)
    {
        return;
    }

    //polling transfer result
    HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 10);
}



/**
  * @brief  选择LCD层
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @retval 无
  */
void LCD_SelectLayer(uint32_t LayerIndex)
{
    ActiveLayer = LayerIndex;
}

/**
  * @brief  设置LCD层的可视化
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  State: 禁能或者使能
  * @retval 无
  */
void LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
    if (State == ENABLE)
    {
        __HAL_LTDC_LAYER_ENABLE(&Ltdc_Handler, LayerIndex);
    }
    else
    {
        __HAL_LTDC_LAYER_DISABLE(&Ltdc_Handler, LayerIndex);
    }
//  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);
    HAL_LTDC_Reload(&Ltdc_Handler, LTDC_RELOAD_IMMEDIATE);
}

void LCD_SetLayerVisible_ex(uint32_t LayerIndex, FunctionalState State, uint32_t ReloadType)
{
    if (State == ENABLE)
    {
        __HAL_LTDC_LAYER_ENABLE(&Ltdc_Handler, LayerIndex);
    }
    else
    {
        __HAL_LTDC_LAYER_DISABLE(&Ltdc_Handler, LayerIndex);
    }

    HAL_LTDC_Reload(&Ltdc_Handler, ReloadType);
}


/**
  * @brief  设置LCD的透明度常量
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Transparency: 透明度，范围（0—255）0为完全透明
  * @retval 无
  */
void LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
    HAL_LTDC_SetAlpha(&Ltdc_Handler, Transparency, LayerIndex);
}

/**
  * @brief  设置LCD缓冲帧的首地址
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Address: LCD缓冲帧的首地址
  * @retval 无
  */
void LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address, uint32_t ReloadType)
{
    HAL_LTDC_SetAddress_NoReload(&Ltdc_Handler, Address, LayerIndex);

    if (ReloadType == LTDC_RELOAD_IMMEDIATE)
    {
        __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&Ltdc_Handler);
    }
    else if (ReloadType == LTDC_RELOAD_VERTICAL_BLANKING)
    {
        __HAL_LTDC_VERTICAL_BLANKING_RELOAD_CONFIG(&Ltdc_Handler);
    }
}

/**
  * @brief  设置显示窗口
  * @param  LayerIndex: 前景层(层1)或者背景层(层0)
  * @param  Xpos: LCD   X轴起始位置
  * @param  Ypos: LCD   Y轴起始位置
  * @param  Width: LCD  窗口大小
  * @param  Height: LCD 窗口大小
  * @retval None
  */
void LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /* 重新设置窗口大小 */
    HAL_LTDC_SetWindowSize(&Ltdc_Handler, Width, Height, LayerIndex);

    /* 重新设置窗口的起始位置 */
    HAL_LTDC_SetWindowPosition(&Ltdc_Handler, Xpos, Ypos, LayerIndex);
}



/**
  * @brief  使能显示
  * @retval 无
  */
void LCD_DisplayOn(void)
{
	extern void lcd_bl_on(void);
    /* 开显示 */
    __HAL_LTDC_ENABLE(&Ltdc_Handler);
    lcd_bl_on();
}

/**
  * @brief  禁能显示
  * @retval 无
  */
void LCD_DisplayOff(void)
{
	extern void lcd_bl_off(void);
    /* 关显示 */
    __HAL_LTDC_DISABLE(&Ltdc_Handler);
    lcd_bl_off();
}


void lcd_fill_array(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, void *pcolor)
{
//	HAL_GPIO_WritePin(TIME_MEASURE_PORT, TIME_MEASURE_PIN, GPIO_PIN_SET);
	LCD_SetLayerAddress(LTDC_DISP_LAYER1, (uint32_t)pcolor, LTDC_RELOAD_VERTICAL_BLANKING);
//	printfS("==: %d * %d  Addr:0x%08x\r\n", (x_end-x_start+1), (y_end-y_start+1), (uint32_t)pcolor);
	while(HAL_Get_Reload_status(&Ltdc_Handler));
//	HAL_GPIO_WritePin(TIME_MEASURE_PORT, TIME_MEASURE_PIN, GPIO_PIN_RESET);
}


