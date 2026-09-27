# Update README: note about sector mapping and MCU-specific adjustments

Flash sector mapping used in flash_ops.c assumes STM32F4 medium-density layout (typical for STM32F411):
- sectors 0..3 : 16KB
- sector 4     : 64KB
- sectors 5..  : 128KB

If your MCU uses a different flash geometry, adjust get_sector_from_address() in flash_ops.c accordingly.
