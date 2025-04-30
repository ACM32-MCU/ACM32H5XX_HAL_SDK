/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/

#include "main.h"
#include "sdram.h"
#include "bsp_lcd.h"
#include "app.h"
#include "xccela_opi.h"



/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
	uint32_t color_shft=0;
	uint32_t LayerIdx=0;

    HAL_Init();

	SystemClock_Config(SYSCLK_220M_SRC_XTH_12M, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);
    BSP_UART_Init(USART1, 115200);

    printfS("\r\n\r\n");
    printfS("************************************************************\r\n\r\n");
    printfS("system startup\r\n");

	OSPI_PSRAM_Init();

    printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
    printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
    printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
    printfS("************************************************************\r\n\r\n");

	lcd_intf_init();
	lcd_disp_init(TEST_L1_AREA_WIDTH, TEST_L1_AREA_HEIGHT);

	while(1)
	{
		if(LayerIdx)
		{
			LL_FillBuffer(0, ltdc_layer1_buf, LCD_WIDTH, LCD_HEIGHT, 0, (0xFF<<(8*color_shft)));
			LCD_SetLayerAddress(LTDC_DISP_LAYER1, (uint32_t)ltdc_layer1_buf, LTDC_RELOAD_VERTICAL_BLANKING);
		}
		else
		{
			LL_FillBuffer(0, ltdc_layer2_buf, LCD_WIDTH, LCD_HEIGHT, 0, (0xFF<<(8*color_shft)));
			LCD_SetLayerAddress(LTDC_DISP_LAYER1, (uint32_t)ltdc_layer2_buf, LTDC_RELOAD_VERTICAL_BLANKING);
		}

		LayerIdx = ((0==LayerIdx) ? 1 : 0);

		color_shft++;
		if(color_shft>2)
		{
			color_shft=0;
		}
		HAL_DelayMs(500);
	}
}














