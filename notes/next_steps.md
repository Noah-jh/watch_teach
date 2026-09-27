# notes/next_steps.md

# 下一步执行清单

## 当前提交
本次补充了：
- 37 章逐章映射
- 大厂式分层和最终合并原则
- J-Link、Keil、Stack Trace 调试说明
- ST7789、OLED、UART DMA、HC-SR04、RFID、舵机/风扇、WT588F02、传感器分层、LVGL、Bootloader、TIM/EXTI 等 demo README

## 你现在不要一次接所有模块
推荐只做：
1. Demo01
2. Demo02
3. Demo03

每个 demo 成功后记录：
- CubeMX 外设配置截图
- Keil 编译输出
- J-Link 下载结果
- 串口日志
- 实际接线
- 结果和问题

## 后续真正生成代码工程时需要补充的信息
- LCD 控制器、分辨率和排针丝印
- OLED 控制器和 I2C/SPI 类型
- RFID 芯片型号
- CO2 模块型号和通信协议
- WT588F02 具体协议版本
- 最小系统板实际晶振和 LED/按键引脚
- ESP8266/HC-05 模块具体版本和供电方式

这些信息确定后，才能生成不会损坏硬件的 CubeMX `.ioc` 和驱动实现。
