# Demo 12: LVGL minimal main.c skeleton

/*
 * LVGL minimal integration with ST7789 flush and CMSIS-RTOS task
 * - Requires demo04 st7789 driver flush function: drv_st7789_flush()
 */

#include "main.h"
#include "lvgl.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t lvglTaskHandle;

void lvgl_task(void *arg) {
    lv_init();
    /* init display driver and input driver here */
    for(;;) {
        lv_timer_handler();
        osDelay(5);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t lv_attr = {
        .name = "lvgl",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 1024 * 4
    };
    lvglTaskHandle = osThreadNew(lvgl_task, NULL, &lv_attr);
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

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
