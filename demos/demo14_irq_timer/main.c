# Demo 14: TIM/EXTI/Key state machine main.c skeleton

/*
 * Demo14 implements short press / long press detection using EXTI + TIM
 * - EXTI for key press/release
 * - TIM for measuring press duration or debouncing
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t keyTaskHandle;
volatile uint32_t key_press_time = 0;

void KeyTask(void *arg) {
    for(;;) {
        uint32_t flags = osThreadFlagsWait(0x03, osFlagsWaitAny, osWaitForever);
        if (flags & 0x01) {
            // short press
            printf("short press\r\n");
        }
        if (flags & 0x02) {
            // long press
            printf("long press\r\n");
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_Pin) {
        // simple state: on falling edge start timer, on rising edge compute duration
        // This callback must be minimal: record timestamp and set flag
        static uint32_t last = 0;
        uint32_t now = HAL_GetTick();
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) {
            // pressed
            last = now;
        } else {
            // released
            uint32_t dt = now - last;
            if (dt > 1000) osThreadFlagsSet(keyTaskHandle, 0x02);
            else osThreadFlagsSet(keyTaskHandle, 0x01);
        }
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t key_attr = {
        .name = "key",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    keyTaskHandle = osThreadNew(KeyTask, NULL, &key_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
