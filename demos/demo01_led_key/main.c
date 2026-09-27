# demos/demo01_led_key/main.c

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

/*
 * Demo 1：最小系统板 + FreeRTOS (CMSIS‑OS v2)
 * 目标：串口输出 + LED 闪烁 + 按键中断唤醒任务
 */

osThreadId_t ledTaskHandle;
osThreadId_t keyTaskHandle;

void LEDTask(void *argument)
{
    for (;;) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(500);
    }
}

void KeyTask(void *argument)
{
    for (;;) {
        uint32_t flags = osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
        if (flags & 0x01) {
            printf("KEY pressed!\r\n");
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        }
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t led_attr = {
        .name = "LED_Task",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 128 * 4
    };
    ledTaskHandle = osThreadNew(LEDTask, NULL, &led_attr);

    const osThreadAttr_t key_attr = {
        .name = "KEY_Task",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    keyTaskHandle = osThreadNew(KeyTask, NULL, &key_attr);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_Pin) {
        osThreadFlagsSet(keyTaskHandle, 0x01);
    }
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_FREERTOS_Init();

    osKernelStart();

    while (1) {
        // 不应在这里写死循环；FreeRTOS 会接管
    }
}

