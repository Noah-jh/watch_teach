# Demo 09: SG90 Servo + Fan (PWM & MOSFET) main.c skeleton

/*
 * Servo PWM and fan MOSFET control
 * - Servo: TIM PWM output (example TIM1_CH1 -> PA8)
 * - Fan: MOSFET low-side controlled via GPIO / PWM
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t actuatorTaskHandle;

void ActuatorTask(void *arg)
{
    // init PWM and set neutral
    for(;;) {
        // sweep servo
        for (int pos=1000; pos<=2000; pos+=50) {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pos);
            osDelay(20);
        }
        osDelay(500);
        for (int pos=2000; pos>=1000; pos-=50) {
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pos);
            osDelay(20);
        }
        osDelay(1000);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t act_attr = {
        .name = "act",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    actuatorTaskHandle = osThreadNew(ActuatorTask, NULL, &act_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
