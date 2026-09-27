# Demo 05: OLED (I2C) - README (updated)

Purpose
- Validate I2C OLED (likely SSD1306) using I2C1 on PB6/PB7
- If the OLED is SPI variant, follow the SPI README in this repo

User note
- You don't know whether your OLED is I2C or SPI and the I2C address is unknown. This demo includes an I2C scanner and automatic fallback instructions.

CubeMX required
- I2C1 (PB6/PB7), 100k or 400k
- USART1 for debug
- FreeRTOS CMSIS V2

Wiring (I2C variant)
- VCC -> 3.3V
- GND -> GND
- SDA -> PB7
- SCL -> PB6

I2C Scanner (put this code in your test app before OLED init)
```c
for (uint8_t addr = 1; addr < 0x7F; addr++) {
    if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 1, 10) == HAL_OK) {
        printf("I2C device found at 0x%02X\r\n", addr);
    }
}
```
- Common SSD1306 addresses: 0x3C or 0x3D. If the scanner reports one of these, use that address.

Fallback if OLED is SPI
- If no I2C device found, try the SPI wiring and use SPI driver (see demo04 for SPI wiring and pins)

Validation
- Run I2C scanner and note reported address
- Initialize display with reported address (or SPI variant) and display "Hello"

