# Demo 04: ST7789 SPI LCD - main.c skeleton

/*
 * Demo04 ST7789 minimal skeleton
 * - Uses SPI1 (PA5=SCK, PA7=MOSI, PA6=MISO)
 * - CS = PA4, DC = PB0, RST = PB1, BL = PB2 (PWM optional)
 * - SPI Tx uses DMA for frame transfer
 * - HAL + CMSIS-RTOS v2 style
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

osThreadId_t dispTaskHandle;

void ST7789_InitSequence(void);
void ST7789_SendCmd(uint8_t cmd);
void ST7789_SendData(uint8_t *data, uint32_t len);
void ST7789_FillScreenDMA(uint16_t color);

void DispTask(void *argument)
{
    // one-time init
    ST7789_InitSequence();

    for(;;) {
        // Example: fill screen red
        ST7789_FillScreenDMA(0xF800);
        osDelay(1000);
        ST7789_FillScreenDMA(0x07E0);
        osDelay(1000);
        ST7789_FillScreenDMA(0x001F);
        osDelay(1000);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t disp_attr = {
        .name = "disp",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 512 * 4
    };
    dispTaskHandle = osThreadNew(DispTask, NULL, &disp_attr);
}

// Minimal send functions: command vs data
void ST7789_SendCmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void ST7789_SendData(uint8_t *data, uint32_t len)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_DMA(&hspi1, data, len);
    // caller must wait for DMA completion or use callback
}

void ST7789_FillScreenDMA(uint16_t color)
{
    // Simple block-fill: allocate small buffer and stream repeatedly
    static uint16_t lineBuf[240]; // max width, adjust per screen
    for (int i=0;i<240;i++) lineBuf[i] = color;

    // set column/row windows here (omitted - use driver)

    for (int row=0; row<240; row++) {
        // send one line as 240 pixels (16-bit)
        ST7789_SendData((uint8_t*)lineBuf, 240*2);
        // wait for DMA complete - in production use semaphore from callback
        HAL_Delay(2);
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();

    MX_FREERTOS_Init();
    osKernelStart();

    while (1) {}
}
