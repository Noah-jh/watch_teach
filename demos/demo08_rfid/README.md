# Demo 8：RFID / RC522 SPI

## 目标
验证 SPI 从设备、独立 CS、复位和 RFID UID 读取。模块型号必须先确认是 RC522/MFRC522 还是其它 RFID 芯片。

## 典型 RC522 接线
- 3.3V：3.3V
- GND：GND
- SCK：SPI SCK
- MOSI：SPI MOSI
- MISO：SPI MISO
- SDA：CS
- RST：GPIO
- IRQ：可选 EXTI

## CubeMX
- SPI Master，8-bit，软件 NSS。
- CS/RST 为 GPIO。
- USART1 打印 UID。

## 验收
- 能读到芯片版本寄存器。
- 放入卡片后打印稳定 UID。
- 移开卡片后状态回到 idle。

## 注意
RC522 通常是 3.3V 设备，不要使用 5V 供电或 5V 信号。
