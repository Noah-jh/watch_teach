# Demo 05 README (OLED SSD1306)

Purpose
- Validate I2C OLED (SSD1306) using I2C1 on PB6/PB7

CubeMX required
- I2C1 (PB6/PB7), 100k or 400k
- USART1 for debug
- FreeRTOS CMSIS V2

Wiring
- VCC -> 3.3V
- GND -> GND
- SDA -> PB7
- SCL -> PB6

Validation
- Run I2C scanner to detect 0x3C/0x3D
- Initialize and display "Hello"

Notes
- If OLED is SPI variant, use SPI1 and assign CS/DC/RST accordingly

