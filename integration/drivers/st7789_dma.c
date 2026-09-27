/* st7789_dma.c - DMA-based ST7789 driver
 * Uses HAL SPI TX DMA to send pixel data in tiles. On DMA complete, calls user callback.
 */

#include "st7789.h"
#include "hal_port.h"
#include "main.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

static volatile const uint8_t *dma_buf_ptr = NULL;
static volatile uint32_t dma_buf_len = 0;
static void (*dma_done_cb)(void) = NULL;

void st7789_dma_init(void)
{
    // Ensure DMA is configured for SPI1 TX by CubeMX (MX_DMA_Init)
}

int st7789_dma_start_transfer(const uint8_t *data, uint32_t len, void (*done_cb)(void))
{
    if (data == NULL || len == 0) return -1;
    dma_buf_ptr = data;
    dma_buf_len = len;
    dma_done_cb = done_cb;

    // Ensure DC pin is set for data
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)dma_buf_ptr, dma_buf_len) != HAL_OK) {
        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
        return -1;
    }
    return 0;
}

void st7789_dma_on_tx_complete(void)
{
    // Called from HAL_SPI_TxCpltCallback when SPI DMA completes
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    if (dma_done_cb) dma_done_cb();
}
