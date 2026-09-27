# Demo 07: HC-SR04 Ultrasonic main.c skeleton

/*
 * HC-SR04 example using EXTI for echo or TIM input capture
 * - Trigger pin: e.g., PB10 (GPIO output)
 * - Echo pin: e.g., PB3 configured as TIM input capture or EXTI with micros measurement
 * - Echo is 5V: must use a resistor divider to 3.3V before MCU
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t hcsrTaskHandle;

void HCSRTask(void *arg)
{
    for(;;) {
        // trigger pulse
        HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

        // wait for capture or poll echo pin
        osDelay(200);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t hcsr_attr = {
        .name = "hcsr",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    hcsrTaskHandle = osThreadNew(HCSRTask, NULL, &hcsr_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
