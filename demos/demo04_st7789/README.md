# Demo 04 README (ST7789 SPI LCD)

Purpose
- Validate ST7789 (1.69" SPI LCD, 240x280 RGB) using SPI1 + DMA
- Provide a flush primitive to integrate LVGL later

Confirmed by user:
- Panel size: 1.69 inch
- Interface: SPI
- Resolution: 240 x 280 RGB (use 16-bit 565 color mode)

CubeMX required
- SPI1 (Master), 8-bit
- DMA for SPI1_TX
- GPIOs: CS (PA4), DC (PB0), RST (PB1), BL (PB2)
- USART1 for debug
- FreeRTOS -> CMSIS V2

Wiring (based on board mapping)
- LCD VCC -> 3.3V (confirm module supports 3.3V)
- LCD GND -> GND
- SCL (SCK) -> PA5
- SDA (MOSI) -> PA7
- MISO -> PA6 (if required)
- CS -> PA4
- DC -> PB0
- RST -> PB1
- BL -> PB2 (optional PWM)

Driver notes
- Use 16-bit color (RGB565) when sending pixels
- Panel rotation: check MADCTL if image appears rotated
- For 240x280 use partial windowing with tiled DMA transfers to avoid large framebuffers

Validation
- Build and flash
- Observe screen fill colors (red/green/blue cycles)
- Check backlight control toggles BL

