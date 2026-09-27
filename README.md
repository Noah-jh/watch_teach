# watch_teach

一个基于 STM32F411CEU6 最小系统板 + 面包板 + 跳线方案的模块化学习仓库。

本仓库目标：
- 不把所有模块塞进一个大工程
- 按模块拆成多个独立 demo
- 每个 demo 都能单独验证
- 等各模块都成功跑通后，再统一合并封装

当前已完成：
- Demo1：LED + UART + KEY（最小 FreeRTOS / CMSIS‑RTOS v2）
- Demo2：I2C 传感器（AHT21 + MPU6050）
- Demo3：ADC + DMA（MQ‑2 + 摇杆 + 电池检测）
- 37 章学习路线映射
- 大厂式分层结构说明
- J‑Link 调试与 Stack Trace 文档
- 前端显示 / OLED / LCD / UART / 超声波 / RFID / SG90 / WT588F02 / LVGL / Bootloader 等 demo 说明

本仓库引用的实际开发板引脚（基于你提供的原理图）
-----------------------------------------------------

下表是你当前开发板的真实基础引脚，后续每个模块都必须按这个表分配：

1) 通用电源 / 控制
- 5V: 外部 5V 电源输入
- 3V3: 系统 3.3V 输出/输入
- G: GND
- VB: VBAT，备用电池供电
- PC13: 板载蓝色 LED / RTC 相关功能（需核对具体用法）
- NRST: 复位
- BOOT0 / BOOT1: 启动模式

2) I2C / SPI / UART 常用总线
- I2C1: PB6 (SCL), PB7 (SDA)
- I2C2: PB10 (SCL), PB3/PB9 (SDA)
- I2C3: PA8 (SCL), PB4/PB8 (SDA)

- SPI1: PA5 (SCK), PA6 (MISO), PA7 (MOSI), PA4/PA15 (NSS)
- SPI2: PB13 (SCK), PB14 (MISO), PB15 (MOSI), PB12/PB9 (NSS)

- USART1: PA9 (TX), PA10 (RX)
- USART2: PA2 (TX), PA3 (RX)

3) ADC 与 PWM 常用
- ADC0 ~ ADC8: PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PB0, PB1 等
- TIM / PWM：PA8, PA6, PA7, PB0, PB1, PB2 等（实际要看冲突）

4) 关键注意
- PA0 和 PB5 在 F411 上仅支持 3.3V，不能直接接 5V
- 5V 模块必须经过分压/转换或独立电源
- 18650 + TP4056 仅能充电，不能直接供 MCU，必须稳压后再给 MCU
- 所有模块 GND 必须共地

你当前手头模块列表（按接口分组）
-----------------------------------

A. I2C / 传感器：
- AHT21（四根线、I2C）
- GY521（MPU-6050）
- OLED屏幕（常见为 I2C/OLED）
- 土壤温湿度模块

B. 串口 / 通信：
- ESP8266
- HC‑05蓝牙模块
- WT588F02-8S-C 语音模块

C. 模拟 / 传感：
- MQ2 烟雾传感器
- 二氧化碳监测模块（按型号验证）
- 摇杆（PS2 + 普通摇杆）
- HC‑SR04 超声波模块

D. 显示：
- LCD屏幕
- OLED屏幕

E. 控制 / 执行器：
- SG90 舵机（180°）
- 风扇
- 迷你 RFID 读卡模块
- 扫描模块 / RFID 读写模块

F. 功耗 / 电源：
- TP4056 1A 锂电池充电模块（Type-C）
- 18650 3.7V 锂电池

G. 其他：
- Scan 相关模块
- 小型语音播放模块（WT588F02）

显示器类与 RFID 类模块优先说明
--------------------------------

1) LCD 屏幕
- 主要优先作为 SPI LCD（例如 ST7789）验证
- 接线默认方案：PA5/PA6/PA7 和 PA4/PA7/PA0/PA1 等，按具体模块选择
- 先验证屏幕是否为 SPI 还是并口/8080

2) OLED 屏幕
- 优先以 I2C 版本为主（PB6/PB7）
- 若是 SPI OLED，则使用 SPI1 + CS/DC/RST

3) RFID 模块
- 常见是 RC522（SPI）
- 典型：SPI1 + CS / RST / IRQ
- 需要 3.3V 电源，不能接 5V

4) WiFi / 蓝牙模块
- ESP8266：推荐独立 3.3V 稳压电源，串口通信
- HC‑05：常见串口蓝牙，波特率 9600/38400/115200 视模块而定

5) 超声波模块
- HC‑SR04：Trig GPIO 输出 + Echo 经过电平转换进入 TIM 输入捕获或 EXTI
- Echo 常为 5V，必须分压

6) 舵机 / 风扇 / MQ‑2
- 舵机和风扇最好独立 5V 供电，且 GND 共地
- MQ‑2 需要 5V 加热，模拟输出必须分压后输入 ADC

本仓库的推荐系统化路线
------------------------

优先顺序：
1. Demo1：LED + UART + KEY（验证最小系统板）
2. Demo2：I2C：AHT21 + MPU6050（验证总线与传感器）
3. Demo3：ADC + DMA：MQ‑2 + 摇杆 + 电池检测（验证采样）
4. Demo4：ST7789 LCD（SPI + DMA + 背光）
5. Demo5：OLED（I2C）
6. Demo6：ESP8266 / HC‑05（UART）
7. Demo7：HC‑SR04 超声波（GPIO + TIM/EXTI）
8. Demo8：RFID（SPI）
9. Demo9：SG90 舵机 + 风扇（PWM + MOSFET）
10. Demo10：WT588F02 语音模块
11. Demo11：结构化传感器驱动 HAL/Handler
12. Demo12：LVGL（显示 + 触摸 + 按钮）
13. Demo13：Bootloader/OTA
14. Demo14：TIM/EXTI 状态机（长按/短按/中断）

本仓库中有详细说明：
- docs/wiring.md
- docs/demo_flow.md
- docs/naming_and_style.md
- docs/lesson_mapping.md
- docs/integration_architecture.md
- docs/debug_and_jlink.md
- demos/demo01_led_key/README.md
- demos/demo02_i2c_sensor/README.md
- demos/demo03_adc_dma/README.md
- demos/demo04_st7789/README.md
- demos/demo05_oled/README.md
- demos/demo06_uart_modules/README.md
- demos/demo07_hcsr04/README.md
- demos/demo08_rfid/README.md
- demos/demo09_actuator/README.md
- demos/demo10_wt588f02/README.md
- demos/demo11_sensor_drivers/README.md
- demos/demo12_lvgl/README.md
- demos/demo13_bootloader/README.md
- demos/demo14_irq_timer/README.md

下一步建议：
- 先完全跑通 Demo1
- 然后跑通 Demo2
- 再做 Demo3
- 在每个 demo 里记录：串口日志、接线和引脚表
- 成功后再决定是否整合到总工程

你可以直接使用：
- `git clone https://github.com/Noah-jh/watch_teach.git`
- 然后逐个按照 `demos/demoXX/README.md` 实操

如果某个模块在板子上的具体脚位与这里不一致，请按你自己的原理图修正引脚；本仓库中给出的映射是基于你提供的原理图与你当前模块类型做的最佳默认值。

