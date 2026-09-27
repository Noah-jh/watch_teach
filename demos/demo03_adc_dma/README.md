# Demo 03: ADC + DMA (MQ-2 / Joystick / Battery) - README (updated)

## MQ-2 interface clarification
- MQ-2 modules commonly provide an analog voltage (A0) proportional to gas concentration and a digital DO threshold output.
- Most MQ-2 modules are *not* UART devices. Unless your specific MQ-2 module explicitly lists a UART in its datasheet, assume analog output.
- Action: This demo implements MQ-2 as an analog input to ADC (PA1) via a voltage divider and low-pass filter. If your module is a UART variant, tell me the model and I'll replace the demo with the UART parser.

## Wiring summary (assume analog MQ-2)
- MQ-2 VCC -> 5V independent supply (heater)
- MQ-2 GND -> common GND
- MQ-2 A0 -> voltage divider -> PA1 (ADC1_IN1). Ensure Vmax <= 3.3V
- Add RC filter (10k + 10nF) to smooth PWM/AC noise

## CubeMX
- ADC1 multi-channel (PA0 battery, PA1 MQ-2, PA2 joystick X, PA3 joystick Y)
- DMA circular for ADC1
- USART1 debug
- FreeRTOS CMSIS V2

Validation
- ADC values change with joystick movement
- MQ-2 analog changes when exposed to small gas source (cautious testing)

If later your MQ-2 is confirmed UART: provide the repo file or module part number and I will swap this demo to a UART-based parser and sample commands.
