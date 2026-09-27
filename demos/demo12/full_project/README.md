# Demo12 LVGL full_project README

Purpose
- Provide a minimal LVGL integration using ST7789 display. Shows a simple watchface label and battery placeholder.

Prerequisites
- Add LVGL sources (v8 or matching version) into project middleware and provide lv_conf.h configured for 240x280.
- CubeMX must enable SPI1 (with DMA if you adapt DMA flush), I2C1 (optional), USART1 for debug, FreeRTOS CMSIS v2.

How to use
1. Generate project in CubeMX with required peripherals.
2. Copy Src/main.c into project Src/ and add LVGL sources into project.
3. Build and flash.
4. Expected: display fills red briefly (demo flush) then shows "00:00" label and "BAT: --%".

Notes
- This demo uses a blocking flush for simplicity. For production use implement DMA tiled flush and call lv_disp_flush_ready from DMA complete callback.
