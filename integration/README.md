# Integration branch

This integration branch contains the assembled project skeleton for the "smartwatch" product. It includes BSP, HAL wrapper (hal_port), drivers, handlers, services, a minimal LVGL watchface and CI helper scripts.

Structure overview:
- bsp/: board initialization helpers
- hal_port/: thin HAL wrappers used by drivers
- drivers/: device drivers (ST7789, AHT21, MPU6050, RC522, WT588)
- handlers/: higher-level sensor handling tasks
- services/: display/audio/comms services
- app/: watchface and application glue
- bootloader/: complete bootloader implementation (implemented earlier)
- docs/: integration documentation and verification steps

Next steps:
- I will continue to add per-demo full_project directories (demo01..demo14) in batches and run CI builds; monitor progress here.
