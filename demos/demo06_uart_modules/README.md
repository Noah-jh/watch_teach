# Demo 06 README (UART modules) - update for ESP-01S

Purpose
- Demonstrate UART DMA circular receive with IDLE detection
- Use to interface ESP8266 (ESP-01S), HC-05, WT588F02 (connect one at a time)

ESP-01S specifics
- ESP-01S pins: VCC, GND, TX, RX, CH_PD/EN (must be pulled high), RST, GPIO0/2
- Power: ESP-01S requires stable 3.3V regulator capable of supplying 300-500mA peaks
- Wiring:
  - ESP TX -> MCU RX (PA10)
  - ESP RX -> MCU TX (PA9)
  - CH_PD/EN -> 3.3V via 10k pull-up
  - VCC -> dedicated 3.3V supply

WT588F02
- Confirmed model: WT588F02-8F-C (serial control). Use USART frame format per datasheet.

Validation
- ESP: send `AT` via terminal, expect `OK` reply
- WT588: send module-specific command frames to trigger playback

Notes
- Do not power ESP from MCU on-board regulator unless it is rated for the current
