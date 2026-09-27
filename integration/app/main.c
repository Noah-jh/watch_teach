# Update integration app main to start supervisor and register heartbeats

#include "main.h"
#include "board_init.h"
#include "hal_port.h"
#include "handlers/sensor_handler.h"
#include "services/display_service.h"
#include "cmsis_os2.h"
#include "watchdog.h"
#include "supervisor.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    // MX_IWDG_Init(); // ensure IWDG is configured via CubeMX

    watchdog_init(5000);

    Board_Init();

    osKernelInitialize();

    supervisor_init();
    sensor_handler_init();
    display_service_init();

    osKernelStart();

    while (1) {}
}
