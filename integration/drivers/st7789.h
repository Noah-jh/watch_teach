#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>

void st7789_init(void);
void st7789_reset(void);
void st7789_fill_color(uint16_t color);

// DMA API
void st7789_dma_init(void);
int st7789_dma_start_transfer(const uint8_t *data, uint32_t len, void (*done_cb)(void));
void st7789_dma_on_tx_complete(void);

#endif // ST7789_H
