# Demo 06: UART Modules (ESP8266 / HC-05 / WT588F02) main.c skeleton

/*
 * Demo06 UART DMA circular receive example
 * - USART1 used for module
 * - RX uses DMA circular buffer; IDLE interrupt triggers frame processing
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

#define UART_RX_BUF_SIZE 512
extern UART_HandleTypeDef huart1;
static uint8_t uart_rx_buf[UART_RX_BUF_SIZE];

osThreadId_t uartTaskHandle;

void UARTTask(void *arg)
{
    // Start DMA RX in circular mode
    HAL_UART_Receive_DMA(&huart1, uart_rx_buf, UART_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    for(;;) {
        // Wait for notification from ISR (use osThreadFlags)
        uint32_t flags = osThreadFlagsWait(0xFF, osFlagsWaitAny, osWaitForever);
        // Process buffer: find new bytes since last index (use DMA NDTR)
        printf("UART event flags=0x%08x\r\n", flags);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t uart_attr = {
        .name = "uart",
        .priority = (osPriority_t)osPriorityAboveNormal,
        .stack_size = 384 * 4
    };
    uartTaskHandle = osThreadNew(UARTTask, NULL, &uart_attr);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();
    while (1) {}
}

// In USART IRQ handler or HAL_UART_RxCpltCallback / IDLE handler, call osThreadFlagsSet(uartTaskHandle, 0x01);
