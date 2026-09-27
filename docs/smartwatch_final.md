# Smartwatch final specification and integration plan (smartwatch_final.md)

## Goal
- Deliver a "big-company" style embedded project that runs on your STM32F411-based watch hardware and integrates sensors, display, UI (LVGL), connectivity (ESP/BT), audio, and power management. This is the final integration target after all individual demos pass.

## Architecture
- Core: CubeMX-generated HAL + SystemClock + Startup
- BSP: board_init, power management, pinmux, battery monitor
- HAL Port: thin wrappers for I2C/SPI/UART/ADC/DMA/TIM
- Drivers: aht21, mpu6050, st7789, ssd1306, mfrc522, wt588f02
- Handlers/Services: sensor_service, display_service, storage_service, audio_service, comms_service
- Application: watch app, UI pages (watchface, settings, notifications)
- Middleware: FreeRTOS (CMSIS v2), LVGL, EasyLogger

## Power topology
- Battery (18650) -> Protection board -> TP4056 for charging -> 5V boost (optional) -> 3.3V regulator for MCU
- High-current devices (ESP, servo, heater) powered from dedicated rails
- All GNDs common

## Build & CI
- Keil build locally; add GitHub Actions in repo to run static checks and ARM GCC build (CMake) for CI or test builds
- Add cppcheck, clang-format checks

## Release & OTA
- Dual-bank firmware layout recommended for OTA; Bootloader validates CRC before swapping
- Use YMODEM / HTTP / custom protocol for image transfer

## UI & LVGL
- Main watchface shows time, steps, HR, battery
- Background tasks publish sensor data into queue; UI consumes and updates widget
- Touch input mapped to LVGL input device

## Acceptance criteria
- Boot to watchface within 2s
- UI runs at target 30 FPS without freezing under load
- Bluetooth/WiFi connect & retrieve simple data
- Battery monitoring and low-battery warning

## Next steps
- After per-demo validation: integrate drivers into drivers/ and handlers/ and implement services
- Conduct stress tests and power profiling
- Finalize bootloader and OTA path

