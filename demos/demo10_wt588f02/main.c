# Demo 10: WT588F02 voice module - main.c skeleton

/*
 * WT588F02 minimal UART/IO control skeleton
 * - Uses UART to send play command or uses trigger pin to play index
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t voiceTaskHandle;

void VoiceTask(void *arg)
{
    for(;;) {
        // Send simple play command example - replace with module's protocol
        uint8_t cmd[] = {0x7E, 0x02, 0x00, 0x7E};
        HAL_UART_Transmit(&huart1, cmd, sizeof(cmd), HAL_MAX_DELAY);
        osDelay(2000);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t v_attr = {
        .name = "voice",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    voiceTaskHandle = osThreadNew(VoiceTask, NULL, &v_attr);
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
