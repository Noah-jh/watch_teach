#ifndef HAL_PORT_H
#define HAL_PORT_H

#include <stdint.h>
#include <stddef.h>

int32_t hal_i2c_write(uint8_t dev_addr, const uint8_t *data, size_t len, uint32_t timeout);
int32_t hal_i2c_read(uint8_t dev_addr, uint8_t *data, size_t len, uint32_t timeout);

int32_t hal_spi_tx(const uint8_t *data, size_t len, uint32_t timeout);

int32_t hal_uart_tx(const uint8_t *data, size_t len, uint32_t timeout);

int32_t hal_adc_start_dma(uint16_t *buffer, size_t len);

uint32_t hal_get_tick(void);
void hal_delay_ms(uint32_t ms);

#endif // HAL_PORT_H
