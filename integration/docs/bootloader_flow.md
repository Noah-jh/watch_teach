# Update docs: integration/docs/bootloader_flow.md (add OTA steps & ymodem usage)

See bootloader/README.md for full details. Added quick YMODEM host example:

YMODEM host (linux) example using lrzsz:
- Install: sudo apt-get install lrzsz
- Send file: sx --ymodem image_with_header.bin > /dev/ttyUSB0 < /dev/ttyUSB0

Windows: use TeraTerm -> File -> Transfer -> YMODEM -> Send -> select image_with_header.bin

Bootloader flow for OTA:
1. Bootloader waits for YMODEM or raw protocol
2. Host triggers YMODEM send; bootloader writes to OTA_ADDRESS and acknowledges blocks
3. After transfer, bootloader computes CRC and if valid installs image

