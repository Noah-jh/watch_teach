# Bootloader flow document

See integration/bootloader/README.md for full implementation details.

This file gives a flow diagram and step checklist for bootloader and OTA validation.

1) On boot: initialize UART for logs -> check APP at APP_ADDRESS -> if valid jump
2) If invalid: check staging area -> if valid copy/activate -> jump
3) If no valid app: enter OTA receive mode (YMODEM or ESP relay)
4) After receiving: write to staging, verify CRC, mark VALID, reboot
5) On new image activation: wait for APP_OK flag written by app, else rollback

Validation checklist
- Confirm bootloader prints on UART with version info
- Confirm app at APP_ADDRESS runs and writes APP_OK metadata
- Test receiving via YMODEM: run ymodem host to send compiled image
- Test power-loss: interrupt power during write and verify bootloader recovers

