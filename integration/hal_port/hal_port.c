#include "hal_port.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

int32_t hal_i2c_write(uint8_t dev_addr, const uint8_t *data, size_t len, uint32_t timeout)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(dev_addr<<1), (uint8_t*)data, len, timeout) == HAL_OK) return 0;
    return -1;
}

int32_t hal_i2c_read(uint8_t dev_addr, uint8_t *data, size_t len, uint32_t timeout)
{
    if (HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(dev_addr<<1), data, len, timeout) == HAL_OK) return 0;
    return -1;
}

int32_t hal_spi_tx(const uint8_t *data, size_t len, uint32_t timeout)
{
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)data, len, timeout) == HAL_OK) return 0;
    return -1;
}

int32_t hal_uart_tx(const uint8_t *data, size_t len, uint32_t timeout)
{
    if (HAL_UART_Transmit(&huart1, (uint8_t*)data, len, timeout) == HAL_OK) return 0;
    return -1;
}

int32_t hal_adc_start_dma(uint16_t *buffer, size_t len)
{
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)buffer, len) == HAL_OK) return 0;
    return -1;
}

uint32_t hal_get_tick(void)
{
    return HAL_GetTick();
}

void hal_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
