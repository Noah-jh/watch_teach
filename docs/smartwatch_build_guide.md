# Smartwatch Build & Integration Guide — From Demos to Working Product

This guide explains, step-by-step, how to assemble the hardware and merge the demo modules in this repository into a single working "smartwatch" firmware you can compile, flash and run with a J‑Link and Keil MDK‑ARM.

Scope
- Hardware assembly and safe power design
- Pinmap & how to avoid conflicts on your STM32F411 board
- Per‑module CubeMX + Keil generation checklist
- How to merge demos into a single project (drivers → handlers → services → app)
- LVGL quick integration for a basic watchface
- How to flash and verify using J‑Link and Keil
- Acceptance tests and troubleshooting

Important safety notes (read first)
- PA0 and PB5 are only 3.3V tolerant on your F411 board — never connect 5V signals directly to these pins.
- High‑current modules: ESP‑01S, MQ‑2 heater, SG90 and fans MUST use a dedicated 3.3V/5V supply (do not power them from on‑board 3.3V regulator unless rated for the current). Always common the grounds.
- TP4056 + 18650: TP4056 charges the battery. DO NOT use battery output as MCU supply without a regulator/ protection board. Use a proper buck/boost to generate 3.3V/5V rails.

What I have added to the repo now
- `docs/smartwatch_build_guide.md` (this file) in the project root
- Per-demo README and skeletons already present in `demos/` for demo01..demo14

A. Required toolchain & hardware (before you start)
- PC with Windows (recommended for Keil) or Linux for ARM-GCC experiments
- STM32CubeMX 6.8.1
- Keil MDK‑ARM 5.38a (or GCC/CMake alternative) — we provide Keil instructions
- SEGGER J‑Link and drivers
- USB‑TTL adapter for serial logs (or use ST‑Link Virtual COM if board supports)
- Multimeter, oscilloscope (optional but very helpful), logic level shifter, breadboard, decoupling caps
- Power supplies: USB 5V for bench supply, 3.3V regulator (able to supply ESP current) and 5V supply for servos/heater

B. Hardware assembly (wiring, power topology)
1. Power topology (recommended):
   - Bench USB 5V -> 5V rail for servos/heater/fans (use 2A+ supply)
   - Bench USB 5V -> DC‑DC buck to 3.3V (capable 600mA+) -> MCU 3.3V pin
   - Battery (18650) and TP4056 only used for battery tests; if battery used to power board, route battery -> protection board -> boost/regulator -> 3.3V and 5V rails.
   - Always use star ground: connect each supply ground to a single ground point on the breadboard; ensure MCU GND tied to module GNDs.
2. Signal and level handling
   - Any 5V signal must be level shifted to 3.3V before MCU. For HC‑SR04 Echo (5V), use a resistor divider or a proper level shifter.
   - MQ‑2 analog output often ranges up to 5V — ALWAYS use a resistor divider before ADC input and low‑pass filter.
3. Periphery wiring (summary)
   - I2C1: PB6 = SCL, PB7 = SDA (AHT21, MPU6050, I2C OLED)
   - SPI1: PA5 = SCK, PA6 = MISO, PA7 = MOSI, CS = PA4, DC = PB0, RST = PB1, BL = PB2 (ST7789)
   - UART1: PA9 = TX, PA10 = RX (ESP‑01S / WT588 / debug)
   - ADC: PA0 = battery divider, PA1 = MQ‑2 after divider, PA2/PA3 = joystick X/Y
   - PWM/Servo: TIM1_CH1 -> PA8 (Servo signal) (servos powered by 5V rail)
   - Onboard button: PA0 (make sure configured as EXTI) and Onboard LED: PC13

C. Per‑demo CubeMX checklist (generate a separate CubeMX .ioc for each demo first)
- I strongly recommend you generate and build each demo as a separate project and verify it on hardware, then merge.
- For each demo: open CubeMX, set the part STM32F411CEU6, configure pins per demo README, enable FreeRTOS (CMSIS‑RTOS V2), set system clock (HSE if present; default HSI with PLL -> 84MHz is fine), configure DMA/peripheral priorities.

Key configuration notes per demo
1) Demo01 (LED + UART + KEY)
   - USART1 Async 115200 (PA9/PA10)
   - GPIO PB5 output for LED
   - EXTI for PA0 (user button)
   - FreeRTOS CMSIS V2 enabled
2) Demo02 (I2C sensors)
   - I2C1 PB6/PB7 Fast Mode 400 kHz (or 100k)
   - Check "Internal Pull‑up" off if modules have external pull‑ups; otherwise set Pull‑Up
3) Demo03 (ADC + DMA)
   - ADC1 multichannel: IN0/IN1/IN2/IN3 mapped to PA0..PA3
   - DMA in circular mode for ADC1
4) Demo04 (ST7789 SPI)
   - SPI1 master; enable SPI1_TX DMA
   - Configure CS, DC, RST GPIOs
   - If using BL PWM, assign PB2 to TIMx channel
5) Demo05 (OLED)
   - I2C1, same bus as sensors; run I2C scanner before commit
6) Demo06 (UART modules)
   - USART1 RX DMA circular + enable IDLE interrupt
7) Demo07 (HC‑SR04)
   - TRIG as GPIO push‑pull output
   - ECHO to TIM input capture (choose a TIM channel that maps to the pin you wired; or EXTI + HAL_GetTick-based timing)
8) Demo08 (RC522)
   - SPI1 with separate CS (e.g., PB12)
9) Demo09 (Servo & Fan)
   - TIM PWM channel for servo; MOSFET gate GPIO for fan
10) Demo10 (WT588F02)
   - USART at the module expected baud rate
11) Demo12 (LVGL)
   - LVGL middleware included; small display buffer (tile approach), flush function maps to demo04 driver
12) Demo13 Bootloader
   - No hardware dependencies — this is software layout and build steps
13) Demo14 (TIM/EXTI key state machine)
   - EXTI on PA0, TIM for timing/debounce

D. How to merge demos into one product (step‑by‑step)
Goal: incremental merging so you retain working state at every step.

1) Prepare repository structure (we already have it):
   - Core (CubeMX-generated project files)
   - bsp/ (board init: power rail control, pinmux helpers)
   - hal_port/ (small wrappers for HAL interfaces that drivers call)
   - drivers/ (device drivers like aht21.c, mpu6050.c, st7789.c, rc522.c, wt588.c)
   - handlers/ (higher‑level wrappers implementing retries, state machines, thresholds)
   - services/ (sensor_service, display_service, audio_service, comms_service)
   - app/ (watchface, menus)

2) Start from a working base project
   - Choose Demo01 CubeMX project as the base (it already has FreeRTOS + UART + EXTI + GPIO). This keeps the debug chain simple.

3) Add `hal_port` layer
   - Implement functions: i2c_write/read, spi_tx, uart_tx, adc_start_dma, pwm_set, gpio_read/write, get_tick, delay_ms.
   - Each driver calls these abstracted functions via function pointers (interface injection). This avoids having drivers depend on `hi2c1` or `hspi1` directly.

4) Add drivers one‑by‑one and unit test
   - Add `drivers/aht21.c` with `aht21_init()` and `aht21_read()` using hal_port I2C functions.
   - Add `handlers/sensor_handler.c` which periodically invokes aht21_read and posts to a `sensor_queue`.
   - Build, flash, and test.
   - Repeat for `mpu6050`, `st7789` (driver first), `ssd1306` (if I2C), `rc522`, `wt588`.

5) Add display service (display_service)
   - The display service owns LVGL and the display driver flush. It receives messages (e.g., sensor data) and updates widgets.
   - This removes direct calls from handlers into LVGL.

6) Add comms service
   - UART service that owns UART DMA buffer and uses IDLE to signal complete packets to a parser task.
   - For ESP‑01S, implement an AT wrapper in `comms_service` to send AT commands and receive responses.

7) Add audio service
   - Simple queueing: app posts `audio_play(index)` messages to audio_service which commands WT588 via UART.

8) Power & battery management
   - Implement ADC monitoring (battery voltage), low battery threshold, charging detection (if TP4056 CHG pin visible). Show battery on screen and disable noncritical modules under low battery.

9) Merge LVGL
   - Implement a small watchface page: time (RTC), battery, heart icon, sensor values.
   - UI task is single thread calling `lv_timer_handler()`; other tasks post messages into thread-safe queue for UI.

10) Test iteratively
   - After each driver/service addition, build and test. Keep a working tag in git (e.g., v0.1‑demo01, v0.2‑sensors, etc.)

E. LVGL Watchface quick start (minimal)
1) LVGL configuration
   - Download LVGL and put it into middleware/ or use CubeMX middleware if available.
   - Configure `LV_HOR_RES_MAX = 240`, `LV_VER_RES_MAX = 280` (match your 1.69" screen orientation) in `lv_conf.h`.
   - Create a small 240×40 frame buffer or use tiled flush to reduce RAM.

2) Implement `disp_flush_cb` that uses `drv_st7789_flush(x1,y1,x2,y2,color_p)`; when DMA completes, call `lv_disp_flush_ready(disp);` from DMA callback via RTOS-safe mechanism (semaphore or osThreadFlags).

3) UI design
   - A top label with time: update every second
   - A battery icon on the top right: update every 5–10s
   - A center area showing sensor values (temp, accel) updated via queue
   - A bottom area with small notifications

4) Touch (optional)
   - If you have touch, create input driver `touch_read_cb` to feed LVGL with point data.

F. Building and flashing with Keil + J‑Link (step‑by‑step)
1) Open demo base project (start from Demo01 main CubeMX generated project) in Keil.
2) Project → Options for Target → Debug → Select `J‑Link` and `Settings` → SWD, speed 1000 kHz.
3) Build (Rebuild all target files). Fix compile errors (usually missing includes or HAL config when merging code — resolve by adding source files to Keil project and include paths).
4) Connect J‑Link: VTref to 3.3V, SWDIO -> PA13, SWCLK -> PA14, GND -> GND. Optionally connect NRST.
5) Download (Download to Flash). Check Output window for success.
6) Open a serial terminal to the USB‑TTL or virtual COM (PA9/PA10) at 115200 to observe `printf` logs.

G. Acceptance tests (what to verify on hardware)
1) Boot and UART log
   - On reset the system prints: "Watch boot OK — FreeRTOS started" and task list.
2) Button
   - Short press toggles LED or toggles backlight; long press opens menu.
3) Sensor readings
   - AHT21 reports temperature/humidity every second (values reasonable).
   - MPU6050 reports acceleration values detectable when moving the watch.
4) Display
   - Boot splash on ST7789, then watchface with time and battery.
5) Wi‑Fi / BT
   - AT command `AT` returns `OK` from ESP‑01S.
6) Audio
   - WT588 plays a small sound on demand.
7) ADC
   - MQ‑2 analog responds to gas, joystick values change when moving.

H. Troubleshooting checklist
- I2C NACK: check SDA/SCL pull‑ups; check 7‑bit vs 8‑bit address in HAL calls (HAL expects 7‑bit left shifted when calling certain functions). Use I2C scanner to confirm address.
- HardFault: enable semihosting off, use Keil to inspect stack, check task stacks and increase size if necessary.
- DMA corruption on SPI: ensure buffers are not modified until DMA callback signals completion.
- Flickering display: ensure BL PWM or DC lines are stable and use proper backlight voltage.
- No UART response from ESP: verify CH_PD/EN pulled high and proper 3.3V supply with enough current.

I. Git workflow & tags (recommended)
- Create a branch `integration` for the merged big project. Keep `demos/` untouched.
- Commit after each successful integration step; tag stable points (`v0.1-demo01`, `v0.2-sensors`, `v0.3-display`, ...)

J. If you want, I will (next):
1. Generate and push full CubeMX .ioc + Keil projects for demos 01..14 into `demos/demoXX/full_project/` (this will take time and I will commit in batches). — I already created demo skeletons; I can complete full projects per your A selection.
2. Create an `integration` branch with the merged base project and add `bsp/ hal_port/ drivers/ handlers/ services/app` skeleton files with working glue code and one working watchface. This is the full product you asked for.

Which do you want me to do next (pick one)?
- 1) Generate full CubeMX .ioc + Keil projects for all demos 01..14 and push (I will commit per‑demo). Time: ~4–7 days total.
- 2) Start building the `integration` branch and produce the working watch product blueprint + initial merged code (bsp + hal_port + drivers + display_service + lvgl watchface). Time: ~2–4 days.
- 3) Both: do (1) and (2) — full completion (time 5–10 days).

Reply with "Next: 1" or "Next: 2" or "Next: 3" and I'll begin. If you choose 2 or 3, I will immediately create the integration branch and push the initial merged base with driver glue code and LVGL minimal watchface skeleton.

