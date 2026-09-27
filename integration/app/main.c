# Integration main for watch product - integration/app/main.c

/* Integration main for the watch product
 * This file demonstrates how to bring up services and handlers in the integration branch.
 * It assumes CubeMX generated MX_* init functions exist and that FreeRTOS CMSIS v2 is used.
 */

#include "main.h"
#include "board_init.h"
#include "hal_port.h"
#include "handlers/sensor_handler.h"
#include "services/display_service.h"
#include "cmsis_os2.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // CubeMX generated peripheral init
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();

    Board_Init();

    osKernelInitialize();

    // init subsystems
    sensor_handler_init();
    display_service_init();

    // create watchface after display init (ideally within display service)

    osKernelStart();

    while (1) {}
}
