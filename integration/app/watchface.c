/* app watchface - integration/app/watchface.c */

#include "lvgl.h"
#include <stdio.h>

void watchface_create(void)
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "--:--");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *bat = lv_label_create(scr);
    lv_label_set_text(bat, "BAT: --%");
    lv_obj_align(bat, LV_ALIGN_TOP_RIGHT, -10, 10);
}

void watchface_update_time(const char *time_str)
{
    // In real code, update the time label object; placeholder here
    printf("Time update: %s\r\n", time_str);
}
