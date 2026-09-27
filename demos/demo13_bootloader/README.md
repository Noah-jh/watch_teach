# Demo 13: Bootloader notes (README)

Purpose
- Provide safe guidance for creating a bootloader and OTA workflow

Contents
- Partitioning guidance, vector table relocation, CRC check workflow
- Minimal recommended layout (main flash: bootloader at 0x08000000 small, app at 0x08004000)

Warnings
- Test bootloader on spare MCU or use SWD to recover
- Always keep J-Link handy to reflash in case of bad image

