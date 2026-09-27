# Demo 08: RC522 RFID - main.c skeleton

/*
 * RC522 MFRC522 minimal read UID example over SPI
 * - SPI1: PA5/PA6/PA7
 * - RST, SS: e.g., PB12 (SS), PB11 (RST)
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t rfidTaskHandle;

void RFIDTask(void *arg)
{
    for(;;) {
        // Poll card, if found print UID
        printf("RFID poll...\r\n");
        osDelay(1000);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t rfid_attr = {
        .name = "rfid",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    rfidTaskHandle = osThreadNew(RFIDTask, NULL, &rfid_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}
