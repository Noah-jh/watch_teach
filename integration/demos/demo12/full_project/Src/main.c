/* Update LVGL DMA-driven flush to call lv_disp_flush_ready in task context when DMA completes */

#include "main.h"
#include "lvgl.h"
#include "st7789.h"
#include "cmsis_os2.h"
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;

static osThreadId_t lvglTaskHandle;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[240 * 8]; // 8 lines buffer
static volatile bool dma_flush_done_flag = false;
static lv_disp_drv_t *saved_disp_drv = NULL;

// DMA completion callback from driver
static void dma_flush_done_cb(void)
{
    dma_flush_done_flag = true;
}

static void lvgl_disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (saved_disp_drv != NULL) {
        // previous flush still in progress - signal fail-safe
        lv_disp_flush_ready(disp_drv);
        return;
    }

    // copy into buffer (tile). Production code should implement tiled transfer for large areas.
    uint32_t px_count = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    uint32_t bytes = px_count * 2;
    if (bytes > sizeof(buf1)) {
        // tile too large for buffer - fallback to simple blocking write (not ideal)
        memcpy(buf1, color_p, sizeof(buf1));
        st7789_dma_start_transfer((uint8_t*)buf1, sizeof(buf1), dma_flush_done_cb);
    } else {
        memcpy(buf1, color_p, bytes);
        st7789_dma_start_transfer((uint8_t*)buf1, bytes, dma_flush_done_cb);
    }

    // save disp driver to notify when DMA done
    saved_disp_drv = disp_drv;
}

static void lvgl_task(void *arg)
{
    (void)arg;
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, sizeof(buf1) / sizeof(lv_color_t));

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = lvgl_disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 280;
    lv_disp_drv_register(&disp_drv);

    st7789_init();
    st7789_dma_init();

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
        // if DMA flush completed, notify LVGL in task context
        if (dma_flush_done_flag && saved_disp_drv) {
            dma_flush_done_flag = false;
            lv_disp_flush_ready(saved_disp_drv);
            saved_disp_drv = NULL;
        }
        osDelay(5);
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

    osKernelInitialize();
    const osThreadAttr_t attr = { .name = "lvgl", .stack_size = 1024*4, .priority = osPriorityNormal };
    lvglTaskHandle = osThreadNew(lvgl_task, NULL, &attr);
    osKernelStart();

    while (1) {}
}
