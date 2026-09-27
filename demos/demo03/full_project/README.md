# Demo03 full_project README

Steps:
1. In CubeMX: enable ADC1, add regular channels for PA0 (Battery), PA1 (MQ-2), PA2 (Joystick X), PA3 (Joystick Y). Enable DMA for ADC1 in circular mode.
2. Generate code for Keil and copy this main.c into Src/. Add DMA init if CubeMX hasn't added it.
3. Wire MQ-2 A0 via voltage divider to PA1, joystick to PA2/PA3, battery divider to PA0.
4. Build and flash. Open serial 115200.

Expected serial output sample:
ADC: 1234 567 2345 1023

Important: ensure voltage dividers keep voltages <= 3.3V
