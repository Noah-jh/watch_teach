# Demo02 full_project README

Instructions:
1. CubeMX: select STM32F411CEU6, enable I2C1 on PB6/PB7, enable USART1 (PA9/PA10), enable FreeRTOS CMSIS V2. Generate code for Keil.
2. Copy this main.c into Src/ and add integration/drivers/aht21.c and mpu6050.c into the project (or use drivers from integration/drivers by copying them into project Src/ and Inc/).
3. Build and flash. Connect AHT21 and MPU6050 to PB6/PB7 (3.3V). Open serial @115200.

Expected serial output sample:
AHT21 init OK
MPU6050 init OK
AHT21 T=25.32 C H=45.67%
MPU Accel: 12 -4 16384

Troubleshooting:
- If I2C NACK: check wiring and pull-ups. Use logic analyzer if available.
