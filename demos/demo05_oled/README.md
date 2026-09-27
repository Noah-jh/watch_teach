# Demo 5：OLED（I2C/SPI）

## 目标
验证 OLED 的控制器、总线和基本字符显示。常见模块是 SSD1306，但必须以实物丝印为准。

## I2C 版本建议
- PB6：SCL
- PB7：SDA
- 3.3V：VCC
- GND：GND

使用 I2C 扫描确认地址，常见是 0x3C 或 0x3D；HAL 调用时使用左移一位后的地址。

## CubeMX
- I2C1 Master，100 kHz 起步。
- USART1 调试。
- FreeRTOS CMSIS-RTOS v2。

## 验收
1. 扫描到 OLED 地址。
2. 初始化成功。
3. 显示 `Hello STM32F411`。
4. 周期刷新计数器。

## 常见问题
- 无显示：检查屏幕控制器、分辨率、地址和初始化命令。
- 乱码：检查页寻址/列地址和字体数据。
- I2C 超时：检查上拉和电平。
