# Demo 4：ST7789 LCD（SPI + DMA + 背光）

## 目标
对应图片中的 ST7789 Driver 层、显示驱动平台化、背光控制和 SPI DMA。此 demo 不直接绑定 LVGL，先验证“屏能亮、能复位、能刷色”。

## 必须确认
仅凭“LCD 屏幕”无法确定控制器和分辨率。开始前确认排针丝印是否为 `VCC/GND/SCL/SDA/RES/DC/CS/BL`，以及控制器确实是 ST7789。

## 建议引脚
- PA5：SPI1_SCK -> SCL
- PA7：SPI1_MOSI -> SDA
- PA4：CS
- PB0：DC
- PB1：RES
- PB2：BL（先 GPIO，确认正常后再换 PWM）
- PA9/PA10：USART1 调试

## CubeMX
1. SPI1 Master，8-bit，软件 NSS。
2. 初始 SPI 分频设置保守，不要一开始使用最高速率。
3. SPI1_TX 配置 DMA，Normal 模式先验证，成功后再考虑循环/分块。
4. CS/DC/RES/BL 配置为 GPIO 输出。
5. 保留 USART1 和 CMSIS-RTOS v2。

## 接线
LCD 的逻辑电压必须确认。3.3V 屏直接接 3.3V；带 5V 电平转换的模块按模块说明接电源，但 MCU 信号仍要确认兼容。

## 验收
- 复位时序正确。
- 屏幕背光可开关。
- 全屏红/绿/蓝填充成功。
- SPI DMA 完成回调能够释放传输锁。

## 重要实现原则
- DMA 传输期间不能修改发送缓冲区。
- DMA 完成回调中只置位标志/释放信号量，不做复杂绘图。
- CS 必须在整个命令/数据事务期间保持有效。
- 后续接入 LVGL 时，将底层刷屏封装成 `drv_st7789_flush()`。
