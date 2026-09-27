#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>

void st7789_init(void);
void st7789_reset(void);
void st7789_fill_color(uint16_t color);

#endif // ST7789_H
