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

//	lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
//	lv_example_get_started_3();
//	lv_example_anim_3();
//	lv_demo_benchmark();
	lv_demo_transform();
}


#endif	/* PKG_USING_LVGL */

