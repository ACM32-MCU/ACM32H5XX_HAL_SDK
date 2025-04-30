/******************************************************************************
*@file  : lv_demo.c
*@brief : 
******************************************************************************/

#include <lvgl.h>

//#ifdef PKG_USING_LVGL
#if 1

//#include "lv_demo_benchmark.h"

void lv_user_gui_init(void)
{
    /* display demo; you may replace with your LVGL application at here */

//	lv_demo_stress();
//	lv_demo_keypad_encoder();
	lv_demo_benchmark();
}


#endif	/* PKG_USING_LVGL */

