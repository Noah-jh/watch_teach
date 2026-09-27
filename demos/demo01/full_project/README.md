# Demo01 full_project README (updated)

This directory contains the user-space files to integrate with a CubeMX-generated Keil project for Demo01 (LED + UART + KEY).

Steps:
1. In CubeMX set target STM32F411CEU6, enable USART1 (PA9/PA10), configure PB5 as output for LED, PA0 as EXTI input for KEY, enable FreeRTOS CMSIS V2. Generate code for Keil.
2. Copy the files in this folder into the generated project: Src/main.c (replace or merge), Src/retarget.c. Add the files to Keil project.
3. Ensure LED_Pin and LED_GPIO_Port are defined in main.h and generated gpio file (CubeMX usually names user LED differently; adjust macros accordingly).
4. Build in Keil and download with J-Link. Open serial at 115200 8N1.

Expected behavior:
- LED toggles every 500ms.
- Pressing the button connected to PA0 prints "KEY pressed!" in the serial terminal.

Serial log template (capture this when testing):
[BOOT] Demo01 start\n
[INFO] LED task started\n
[INFO] Key task started\n
KEY pressed!

