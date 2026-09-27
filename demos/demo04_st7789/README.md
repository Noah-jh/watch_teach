# Demo 04 README (ST7789 SPI LCD)

Purpose
- Validate ST7789 (or compatible) SPI LCD using SPI1 + DMA
- Provide a flush primitive to integrate LVGL later

CubeMX required
- SPI1 (Master), 8-bit
- DMA for SPI1_TX
- GPIOs: CS (PA4), DC (PB0), RST (PB1), BL (PB2)
- USART1 for debug
- FreeRTOS -> CMSIS V2

Wiring (based on board mapping)
- LCD VCC -> 3.3V (or module specified)
- LCD GND -> GND
- SCL (SCK) -> PA5
- SDA (MOSI) -> PA7
- MISO -> PA6 (if required)
- CS -> PA4
- DC -> PB0
- RST -> PB1
- BL -> PB2 (optional PWM)

Notes
- Confirm module controller is ST7789 (check silk/datasheet). If it's an 8080 parallel interface, adapt accordingly.
- For large resolutions, use tiled DMA transfers to avoid exhausting RAM.

Validation
- Build and flash
- Observe screen fill colors (red/green/blue cycles)
- Check backlight control toggles BL

