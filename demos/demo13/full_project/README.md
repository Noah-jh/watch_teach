# Demo13 full_project README (bootloader detailed steps)

This README describes how to build, flash and validate the bootloader + app workflow on the STM32F411-based watch.

1) Linker setup
- Build bootloader linked at 0x08000000. Bootloader size in example: 0x4000 (16KB). Ensure the bootloader .sct/.ld places vectors at start.
- Build application linked at APP_ADDRESS 0x08004000. The app must not overlap bootloader.

2) Build and flash
- Build bootloader -> generate bootloader.bin
- Build app -> generate app.bin
- Option A (direct test): use J-Link to program bootloader at 0x08000000 and app.bin at 0x08004000 and run; bootloader will check app header and jump.
- Option B (OTA test): program only bootloader. Use YMODEM or host tool to send image to bootloader which writes to staging (OTA_ADDRESS) and then installs.

3) Image creation
- Use integration/ci/flash_image_gen.sh to append a 16-byte header: magic, version, length, crc32. Example:
  ./integration/ci/flash_image_gen.sh app.bin image_with_header.bin
- Send image_with_header.bin via YMODEM to bootloader

4) Verify
- Bootloader prints logs on UART. After successful install, the device reboots into new app and prints app logs.

5) Recovery
- If device becomes non-bootable: connect J-Link and flash a known-good image at 0x08004000, or reflash bootloader+app.

