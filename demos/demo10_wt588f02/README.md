# Demo 10 README (WT588F02 voice module) - updated

Confirmed model: WT588F02-8F-C (serial control)

Purpose
- Control WT588F02 module with UART to play audio clips

CubeMX
- USART (for module control) - use USART1 or USART2 depending on availability

Wiring
- Module VCC per module (confirm whether 3.3V or 5V). Many WT588 boards accept 5V; check your board.
- TX/RX to MCU UART (with level shifting if needed)
- If module has PLAY pin, can drive via GPIO as simple trigger

Validation
- Able to play sample clip with command (per module manual)
- Handle module ACK/Busy responses
