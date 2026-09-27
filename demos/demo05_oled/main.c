# Demo 05: OLED (SSD1306) - main.c skeleton

/*
 * Demo05 SSD1306 minimal example over I2C
 * - I2C1 PB6=SCL, PB7=SDA
 * - Uses HAL I2C mem write to update display buffer
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t oledTaskHandle;

void OLED_Init(void);
void OLED_DisplayHello(void);

void OLEDTask(void *argument)
{
    OLED_Init();
    for(;;) {
        OLED_DisplayHello();
        osDelay(1000);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t oled_attr = {
        .name = "oled",
        .priority = (osPriority_t)osPriorityLow,
        .stack_size = 256 * 4
    };
    oledTaskHandle = osThreadNew(OLEDTask, NULL, &oled_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
