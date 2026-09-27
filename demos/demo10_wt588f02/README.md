# Demo 10 README (WT588F02 voice module)

Purpose
- Control WT588F02 module with UART or IO trigger to play audio clips

CubeMX
- USART (for module control) or GPIO triggers
- USART with sufficient baud rate as per module

Wiring
- Module VCC per module (3.3V or 5V), confirm
- TX/RX to MCU UART (with level shifting if needed)
- If module has PLAY pin, can drive via GPIO

Validation
- Able to play sample clip with command
- Handle busy/ack responses

Notes
- Refer to module datasheet for exact command frames
