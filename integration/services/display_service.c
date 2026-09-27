/* display service (LVGL minimal) - integration/services/display_service.c */

#include "lvgl.h"
#include "st7789.h"
#include "cmsis_os2.h"
#include <stdio.h>

static osThreadId_t lvglTaskHandle;

void lvgl_task(void *arg)
{
    lv_init();
    st7789_init();
    // setup display driver and buffer here (omitted)
    for(;;) {
        lv_timer_handler();
        osDelay(5);
    }
}

void display_service_init(void)
{
    const osThreadAttr_t attr = { .name = "lvgl", .stack_size = 1024*4, .priority = osPriorityNormal };
    lvglTaskHandle = osThreadNew(lvgl_task, NULL, &attr);
}
