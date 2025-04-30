/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-07     liYony       The first version (FT6336)
 * 2022-07-08     liYony       Add FT6206
 */
#include <lvgl.h>
#include <rtdevice.h>
#include <touch.h>

#define DBG_TAG    "LVGL.port.indev"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

/* RT-Thread touch device name */
#define TOUCH_DEV_NAME "xpt0"

lv_indev_t * touch_indev;
rt_device_t touch_dev;
struct rt_touch_data *read_data;

static void input_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    rt_memset(read_data, 0, sizeof(struct rt_touch_data));
    if(1 == rt_device_read(touch_dev, 0, read_data, 1))
    {
        if (read_data->event == RT_TOUCH_EVENT_NONE)
            return;
       rt_kprintf("=2=%d %d %d %d %d\n",
            read_data->event,
            read_data->x_coordinate,
            read_data->y_coordinate,
            read_data->timestamp,
            read_data->width);
        if (read_data->event == RT_TOUCH_EVENT_DOWN)
            data->state = LV_INDEV_STATE_PR;
        if (read_data->event == RT_TOUCH_EVENT_MOVE)
            data->state = LV_INDEV_STATE_PR;
        if (read_data->event == RT_TOUCH_EVENT_UP)
            data->state = LV_INDEV_STATE_REL;
        data->point.x = read_data->x_coordinate;
        data->point.y = read_data->y_coordinate;
    }
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = input_read;

    /*Register the driver in LVGL and save the created input device object*/
    touch_indev = lv_indev_drv_register(&indev_drv);
}

static int lv_hw_touch_init(void)
{
    touch_dev = rt_device_find(TOUCH_DEV_NAME);
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        LOG_E("Can't open touch device:%s", TOUCH_DEV_NAME);
        return -RT_ERROR;
    }

    read_data = (struct rt_touch_data *)rt_calloc(1, sizeof(struct rt_touch_data));

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(lv_hw_touch_init);
