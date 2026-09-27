/* ST7789 driver (minimal) - integration/drivers/st7789.c */

#include "st7789.h"
#include "hal_port.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

// GPIO pins defined according to board mapping

void st7789_reset(void)
{
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    hal_delay_ms(10);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
    hal_delay_ms(10);
}

void st7789_write_cmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    hal_spi_tx(&cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void st7789_write_data(uint8_t *data, uint32_t len)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    hal_spi_tx(data, len, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void st7789_init(void)
{
    st7789_reset();
    // minimal init sequence (depends on panel) - placeholder
    st7789_write_cmd(0x36); // MADCTL
    uint8_t madctl = 0x00;
    st7789_write_data(&madctl, 1);
    st7789_write_cmd(0x3A); // COLMOD
    uint8_t colmod = 0x55; // 16-bit
    st7789_write_data(&colmod, 1);
    // more init commands required per panel datasheet
}

void st7789_fill_color(uint16_t color)
{
    // naive fill implementation using small line buffer
    const int width = 240; // adjust per panel
    uint16_t line[240];
    for (int i = 0; i < width; i++) line[i] = color;

    // set window (omitted)
    for (int y = 0; y < 280; y++) {
        uint8_t *pdata = (uint8_t*)line;
        st7789_write_data(pdata, width * 2);
    }
}
