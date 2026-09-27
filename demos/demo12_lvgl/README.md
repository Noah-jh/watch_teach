# Demo 12 README (LVGL integration)

Purpose
- Integrate LVGL with ST7789 driver
- Show sample GUI: label, button, chart

Requirements
- demo04 ST7789 flush primitive (drv_st7789_flush)
- touch input driver (if touch exists)
- LVGL port files (lv_conf.h, lvgl sources in middleware)

Integration notes
- flush_cb must call lv_disp_flush_ready when DMA completes
- Keep display buffer small and use tiled rendering to conserve RAM
- Single UI task should call lv_timer_handler periodically

Validation
- Show a simple screen with button toggling LED

