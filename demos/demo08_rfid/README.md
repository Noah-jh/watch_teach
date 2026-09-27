# Demo 08 README (RFID RC522)

Purpose
- Read UID from RFID card using MFRC522

CubeMX
- SPI1 (PA5/PA6/PA7)
- GPIO for CS & RST
- USART1 debug

Wiring
- VCC -> 3.3V
- GND -> GND
- SDA/SS -> PB12 (or chosen CS)
- SCK -> PA5
- MOSI -> PA7
- MISO -> PA6
- RST -> PB11

Validation
- Place card on reader, UID printed on serial

Notes
- Use existing open-source MFRC522 HAL driver as reference; adapt SPI transfer functions
