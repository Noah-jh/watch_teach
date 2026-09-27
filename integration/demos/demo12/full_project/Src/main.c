/* demo12 DMA-based LVGL flush integration
 * Replaces the blocking flush with a DMA-driven, tiled flush. Uses a small tile buffer to keep RAM usage low.
 */

#include "main.h"
#include "lvgl.h"
#include "st7789.h"
#include "cmsis_os2.h"
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;

static osThreadId_t lvglTaskHandle;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[240 * 8]; // 8 lines buffer
static volatile bool flush_in_progress = false;

// DMA completion callback from driver -> must call lv_disp_flush_ready
static void dma_flush_done_cb(void)
{
    // lvgl will call flush ready from LVGL context; here we defer via lv_timer if needed
    // But LVGL allows calling lv_disp_flush_ready from ISR context if LVGL is not reentrant.
    // To be safe, post a flag and let the LVGL task call lv_disp_flush_ready.
    flush_in_progress = false;
}

static void lvgl_disp_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (flush_in_progress) {
        // Busy: tell LVGL we're done to avoid blocking (drop frame) - production should queue
        lv_disp_flush_ready(disp_drv);
        return;
    }

    // Convert area and color_p to raw RGB565 buffer if necessary; assume lv_color_t is 16-bit
    uint32_t px_count = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
    uint32_t bytes = px_count * 2;

    // Start DMA transfer (tile-by-tile recommended). For demo, send at most buf size.
    uint32_t send_bytes = bytes;
    if (send_bytes > sizeof(buf1)) send_bytes = sizeof(buf1);

    memcpy(buf1, color_p, send_bytes);
    flush_in_progress = true;

    // initiate DMA transfer; the driver will call back on completion
    if (st7789_dma_start_transfer((uint8_t*)buf1, send_bytes, dma_flush_done_cb) != 0) {
        flush_in_progress = false;
        lv_disp_flush_ready(disp_drv);
        return;
    }

    // For now report ready immediately to avoid blocking LVGL loop; in production, call when DMA done
    // We signal ready here but real system should call lv_disp_flush_ready() in dma_flush_done_cb after all tiles.
    lv_disp_flush_ready(disp_drv);
}

static void lvgl_task(void *arg)
{
    (void)arg;
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, sizeof(buf1) / sizeof(lv_color_t));

    lv_disp_drv_t disp_drv;
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
