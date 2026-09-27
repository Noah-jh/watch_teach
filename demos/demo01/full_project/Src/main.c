/* Demo01 full_project main.c - LED + UART + KEY
 * This file is intended to be copied into the CubeMX-generated project.
 * It assumes CubeMX generates SystemClock_Config, MX_GPIO_Init, MX_USART1_UART_Init
 * and that FreeRTOS CMSIS v2 is enabled.
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

osThreadId_t ledTaskHandle;
osThreadId_t keyTaskHandle;

void LedTask(void *argument)
{
    for(;;) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(500);
    }
}

void KeyTask(void *argument)
{
    for(;;) {
        // Wait for notification from EXTI callback (implemented in stm32xx_it.c)
        osEventFlagsWait(0x1, osFlagsWaitAny, osWaitForever);
        printf("KEY pressed!\r\n");
    }
}

// EXTI callback from HAL should set the event flag used above:
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { osEventFlagsSet(keyTaskHandle, 0x1); }

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    // initialize CMSIS-RTOS
    osKernelInitialize();

    const osThreadAttr_t led_attr = { .name = "led", .priority = osPriorityNormal, .stack_size = 256*4 };
    const osThreadAttr_t key_attr = { .name = "key", .priority = osPriorityAboveNormal, .stack_size = 256*4 };

    ledTaskHandle = osThreadNew(LedTask, NULL, &led_attr);
    keyTaskHandle = osThreadNew(KeyTask, NULL, &key_attr);

    osKernelStart();

    while (1) {}
}
