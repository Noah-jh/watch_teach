/* Demo12 LVGL full_project main.c
 * Minimal LVGL integration demonstrating a watchface with ST7789.
 * Assumptions:
 * - CubeMX has generated MX_GPIO_Init(), MX_DMA_Init(), MX_SPI1_Init(), MX_I2C1_Init(), MX_USART1_UART_Init(), MX_TIMx_Init() as needed.
 * - hal_port::hal_spi_tx and st7789 driver exist in integration/drivers
 * - LVGL sources are added to the project (middleware/lvgl) and lv_conf.h configured for 240x280
 */

#include "main.h"
#include "lvgl.h"
#include "st7789.h"
#include "cmsis_os2.h"
#include <stdio.h>

static osThreadId_t lvglTaskHandle;

/* Simple flush implementation for demonstration: blocking SPI transmit.
 * For low RAM panels this should be implemented with tiled buffers and DMA.
 */

static void lvgl_disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // For demo we ignore area and just stream a small test rectangle
    // Production: set column/row window and stream area->size pixels (RGB565)
    (void)disp_drv; (void)area; (void)color_p;
    // call st7789 fill to show that display works
    st7789_fill_color(0xF800); // red
    lv_disp_flush_ready(disp_drv);
}

void lvgl_task(void *arg)
{
    (void)arg;
    lv_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[240 * 10]; // 10 lines buffer
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, sizeof(buf1) / sizeof(lv_color_t));

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = lvgl_disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 280;
    lv_disp_drv_register(&disp_drv);

    // simple watchface
    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "00:00");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *bat = lv_label_create(scr);
    lv_label_set_text(bat, "BAT: --%");
    lv_obj_align(bat, LV_ALIGN_TOP_RIGHT, -10, 10);

    for (;;) {
        lv_timer_handler();
        osDelay(10);
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    // init display hw
    st7789_init();

    osKernelInitialize();
    const osThreadAttr_t attr = { .name = "lvgl", .stack_size = 1024*4, .priority = osPriorityNormal };
    lvglTaskHandle = osThreadNew(lvgl_task, NULL, &attr);
    osKernelStart();

    while (1) {}
}
