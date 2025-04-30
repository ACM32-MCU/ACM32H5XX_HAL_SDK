/******************************************************************************
*@file  : task_disp.c
*@brief : 
******************************************************************************/

#include "task_disp.h"


//disp memory locate to PSRAM
static uint32_t ltdc_layer_buf[LCD_BUF_NUM][LCD_MAX_LAYER_BUF_SIZE/4] OSPI1PSRAM;
#define ltdc_layer1_buf			ltdc_layer_buf[0]
#define ltdc_layer2_buf			ltdc_layer_buf[1]


extern LTDC_HandleTypeDef Ltdc_Handler;
extern DMA2D_HandleTypeDef Dma2d_Handler;



static void simple_delay(uint32_t n)
{
	while(n--);
}

void *get_disp_buf(uint8_t idx)
{
	idx %= LCD_BUF_NUM;
	return (void *)(ltdc_layer_buf[idx]);
}


void lcd_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin       = LCD_RST_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Drive = GPIO_DRIVE_LEVEL4;
	__HAL_RCC_GPIOI_CLK_ENABLE();//
    HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = LCD_BL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_FUNCTION_0;
	GPIO_InitStruct.Drive = GPIO_DRIVE_LEVEL4;
	__HAL_RCC_GPIOH_CLK_ENABLE();//
    HAL_GPIO_Init(LCD_BL_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET);

    NVIC_ClearPendingIRQ(DMA2D_IRQn);
    NVIC_SetPriority(DMA2D_IRQn, 0x00);
	NVIC_EnableIRQ(DMA2D_IRQn);
}

//LCD reset
void lcd_rst(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
	simple_delay(50);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
	simple_delay(50);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
}

//LCD backlight on
void lcd_bl_on(void)
{
	HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);
}

//LCD backlight off
void lcd_bl_off(void)
{
	HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET);
}

void ltdc_init(void)
{
    /* LTDC Initialization -------------------------------------------------------*/

    /* Polarity configuration */
    /* Initialize the horizontal synchronization polarity as active low */
    Ltdc_Handler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    /* Initialize the vertical synchronization polarity as active low */
    Ltdc_Handler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    /* Initialize the data enable polarity as active low */
    Ltdc_Handler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    /* Initialize the pixel clock polarity as input pixel clock */
    Ltdc_Handler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    /* Timing configuration */
    /* Horizontal synchronization width = Hsync - 1 */
    Ltdc_Handler.Init.HorizontalSync = LCD_HSYNC_WIDTH - 1;
    /* Vertical synchronization height = Vsync - 1 */
    Ltdc_Handler.Init.VerticalSync = LCD_VSYNC_HEIGHT - 1;
    /* Accumulated horizontal back porch = Hsync + HBP - 1 */
    Ltdc_Handler.Init.AccumulatedHBP = LCD_HSYNC_WIDTH + LCD_HBP - 1;
    /* Accumulated vertical back porch = Vsync + VBP - 1 */
    Ltdc_Handler.Init.AccumulatedVBP = LCD_VSYNC_HEIGHT + LCD_VBP - 1;
    /* Accumulated active width = Hsync + HBP + Active Width - 1 */
    Ltdc_Handler.Init.AccumulatedActiveW = LCD_HSYNC_WIDTH + LCD_HBP + LCD_WIDTH - 1;
    /* Accumulated active height = Vsync + VBP + Active Heigh - 1 */
    Ltdc_Handler.Init.AccumulatedActiveH = LCD_VSYNC_HEIGHT + LCD_VBP + LCD_HEIGHT - 1;
    /* Total height = Vsync + VBP + Active Heigh + VFP - 1 */
    Ltdc_Handler.Init.TotalHeigh = Ltdc_Handler.Init.AccumulatedActiveH + LCD_VFP;
    /* Total width = Hsync + HBP + Active Width + HFP - 1 */
    Ltdc_Handler.Init.TotalWidth = Ltdc_Handler.Init.AccumulatedActiveW + LCD_HFP;

    /* Configure R,G,B component values for LCD background color */
    Ltdc_Handler.Init.Backcolor.Blue = 0;
    Ltdc_Handler.Init.Backcolor.Green = 0;
    Ltdc_Handler.Init.Backcolor.Red = 0;

    Ltdc_Handler.Instance = LTDC;

	HAL_LTDC_Init(&Ltdc_Handler);

    NVIC_ClearPendingIRQ(LTDC_IRQn);
	HAL_NVIC_SetPriority(LTDC_IRQn, 2, 1);
	
	NVIC_EnableIRQ(LTDC_IRQn);

    NVIC_ClearPendingIRQ(LTDC_ERR_IRQn);
	HAL_NVIC_SetPriority(LTDC_ERR_IRQn, 2, 1);
	NVIC_EnableIRQ(LTDC_ERR_IRQn);
}

void lcd_disp_init(uint32_t x,uint32_t y)
{
	uint32_t i, j;

	/* LCD 第一层初始化 */ 
	LCD_LayerInit(0, (uint32_t)ltdc_layer1_buf,RGB565,x,y, LTDC_RELOAD_IMMEDIATE);
	LCD_LayerInit(1, (uint32_t)ltdc_layer1_buf,RGB565,x,y, LTDC_RELOAD_IMMEDIATE);

	/* 选择LCD第一层 */
	LCD_SelectLayer(0);

	/* 配置第一和第二层的透明度,最小值为0，最大值为255*/
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 0);

	LCD_SetLayerVisible(0, ENABLE);
	LCD_SetLayerVisible(1, DISABLE);

	/* 使能LCD，包括开背光 */ 
	LCD_DisplayOn(); 
}

void lcd_intf_init(void)
{
	lcd_init();
	lcd_rst();
	lcd_bl_on();
	LCD_ClockConfig();
	ltdc_init();
}



void vDispTask(void *pvParameters)
{
	uint32_t color_shft=0;
	uint32_t LayerIdx=0;

	lcd_intf_init();
	
	lv_init();
	lv_port_disp_init();
	
//	lv_port_indev_init();

	lcd_disp_init(DISP_L1_AREA_WIDTH, DISP_L1_AREA_HEIGHT);

	lv_user_gui_init();

	lv_tick_set_cb(HAL_GetTick);
	lv_delay_set_cb(HAL_Delay);
    while(1)
    {
		lv_task_handler();
		vTaskDelay(pdMS_TO_TICKS(5));
    }
}



