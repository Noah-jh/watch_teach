# Demo 06 README (UART modules)

Purpose
- Demonstrate UART DMA circular receive with IDLE detection
- Use to interface ESP8266, HC-05, WT588F02 (connect one at a time)

CubeMX
- USART1 (PA9/PA10)
- DMA for USART1_RX: circular
- Enable IDLE interrupt handling

Wiring
- Module TX -> MCU RX (PA10)
- Module RX -> MCU TX (PA9)
- Module VCC -> appropriate power (ESP8266 3.3V high-current)
- GND common

Validation
- Send "AT" from terminal, expect "OK"
- For WT588F02, send specific command frames per module datasheet

Notes
- Ensure power can supply module; ESP8266 needs ~300-400mA peaks
