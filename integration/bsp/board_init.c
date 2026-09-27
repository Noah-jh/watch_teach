#include "board_init.h"
#include "main.h"

// Board_Init should call CubeMX-generated MX_* init functions as appropriate
void Board_Init(void)
{
    // For integration skeleton we expect that SystemClock_Config(), MX_GPIO_Init(), MX_DMA_Init(),
    // MX_SPI1_Init(), MX_I2C1_Init(), MX_USART1_UART_Init(), MX_ADC1_Init(), MX_TIM1_Init() are generated
    // and available from CubeMX project. Board_Init can call them or perform board-level initializations.

    // Example safe call (uncomment in actual project when MX init functions exist):
    // SystemClock_Config();
    // MX_GPIO_Init();
    // MX_DMA_Init();
    // MX_SPI1_Init();
    // MX_I2C1_Init();
    // MX_USART1_UART_Init();
}

void Board_DeInit(void)
{
    // Optional: de-init peripherals
}
