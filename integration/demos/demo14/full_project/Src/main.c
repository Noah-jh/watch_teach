/* Integrate debounce into Demo14: polling-based debouncer in task context
 * This file replaces EXTI-only approach with sampled determinism and uses debounce.c
 */

#include "main.h"
#include "cmsis_os2.h"
#include "debounce.h"
#include <stdio.h>

static osThreadId_t keyTaskHandle;
static debounce_t key_db;

void KeyTask(void *arg)
{
    (void)arg;
    debounce_init(&key_db, 50); // 50ms stable
    bool last_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET;
    while (1) {
        bool raw = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET;
        uint32_t now = HAL_GetTick();
        if (debounce_update(&key_db, raw, now)) {
            bool st = key_db.state;
            if (st) {
                // pressed
                // store press time
                static uint32_t press_ts = 0;
                press_ts = now;
            } else {
                // released
                static uint32_t press_ts = 0;
                uint32_t dt = now - press_ts;
                if (dt >= 2000) printf("very long press\r\n");
                else if (dt >= 1000) printf("long press\r\n");
                else printf("short press\r\n");
            }
        }
        osDelay(10); // sample every 10ms
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    osKernelInitialize();
    const osThreadAttr_t attr = { .name = "key", .stack_size = 256*4, .priority = osPriorityNormal };
    keyTaskHandle = osThreadNew(KeyTask, NULL, &attr);
    osKernelStart();
    while (1) {}
}
