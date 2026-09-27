# integration/docs/verification.md

Integration and demo verification checklist

Demo01:
- Build demo01_full project in Keil, flash via J-Link
- Open serial 115200; expect LED toggles and "KEY pressed" messages

Demo02:
- Build demo02_full, flash
- Connect AHT21 and MPU6050 to PB6/PB7, power 3.3V
- Serial should print temperature/humidity and accel values every second

Demo03:
- Build demo03_full, flash
- Connect MQ-2 A0 via divider to PA1, joystick to PA2/PA3, battery divider to PA0
- Serial should show ADC samples every second

Integration watchface + bootloader/OTA:
- Build integration app (CubeMX project) to link at APP_ADDRESS (0x08004000)
- Build bootloader to occupy 0x08000000..0x08003FFF (16KB)
- Flash bootloader, then flash app to APP_ADDRESS via J-Link
- Start device: bootloader should detect valid app and jump
- Verify serial prints from app and display shows watchface

OTA tests (YMODEM):
- Use YMODEM host to send generated firmware image (integration/ci/flash_image_gen.sh produces image with header)
- Bootloader should receive, write to OTA area, verify CRC and mark valid
- Reboot and verify new image runs

