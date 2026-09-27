# watch_teach

一个面向 STM32F411CEU6 + CubeMX 6.8.1 + Keil MDK-ARM 5.38a 的学习型多模块实验仓库。

目标：
- 不把所有模块塞进一个大工程
- 按模块拆成多个独立 demo
- 每个 demo 都可单独验证、调试、烧录
- 等模块全部跑通后，再合并到总工程

适合对象：
- 初学嵌入式软件开发者
- 想学习 FreeRTOS (CMSIS‑RTOS v2)
- 想学习 I2C / UART / ADC / DMA / SPI / PWM / LVGL / OTA 等
- 想按“工程化规范”组织代码

本仓库遵循的原则：
- 每个 demo 独立运行，不相互污染引脚
- 任务使用 CMSIS‑RTOS v2 API
- 代码命名与注释统一规范（你给出的命名规范）
- 每个 demo 都包含：目标、接线、CubeMX 配置、关键代码、验收标准

目录结构：

```text
watch_teach/
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── wiring.md
│   ├── demo_flow.md
│   └── naming_and_style.md
├── demos/
│   ├── demo01_led_key/
│   │   ├── README.md
│   │   └── main.c
│   ├── demo02_i2c_sensor/
│   │   ├── README.md
│   │   └── main.c
│   └── demo03_adc_dma/
│       ├── README.md
│       └── main.c
└── notes/
    └── TODO.md
```

当前已补充：
- Demo1：LED + UART + KEY（最小 FreeRTOS/CMSIS v2）
- Demo2：I2C 传感器（AHT21 + MPU6050）
- Demo3：ADC + DMA（MQ‑2 + 摇杆 + 电池检测）

后续将补充：
- Demo4：SPI LCD（ST7789）
- Demo5：OLED
- Demo6：ESP8266 / HC‑05 串口模块
- Demo7：HC‑SR04 超声波
- Demo8：RC522 RFID
- Demo9：SG90 舵机
- Demo10：WT588F02 语音模块
- Demo11：风扇 / MOSFET 驱动
- Demo12：LVGL 最小示例

本仓库的学习路线：
1. 先验证最小系统板是否能帮你跑起来（LED / UART / 按键）
2. 验证总线协议：I2C / SPI / UART / ADC
3. 验证传感器与简单功能模块
4. 验证显示与 GUI（LVGL）
5. 再做蓝牙/OTA/通信模块
6. 最后合并成总工程

注意事项：
- 面包板模块和外设电压差异大，MCU 为 3.3V，5V 模块需分压或独立电源
- 18650 电池必须经过稳压或电源管理，不要直接接 MCU
- MQ‑2 需要 5V 加热，且需要独立供电和分压
- HC‑SR04 Echo 是 5V 级别，需要电平转换/分压

你可以直接从这些 README 开始：
- demos/demo01_led_key/README.md
- demos/demo02_i2c_sensor/README.md
- demos/demo03_adc_dma/README.md

下一步：
- 按 demo 顺序验证
- 若某个 demo 没跑通，把错误日志贴给我，我会逐步调
- 你验证通过后，我继续补充下一个 demo

仓库总览：
- 这不是一个大而全的单工程，而是一个“分模块学习工程集合”
- 目标是让你对每个模块都能独立运行、独立调试、独立验证

如何打开：
1. 使用 GitHub -> Clone / Download ZIP
2. 用 CubeMX 打开 .ioc（如果你后续我补上）
3. 用 Keil 打开生成的 .uvprojx
4. 使用 J‑Link 下载并调试

欢迎继续补充，逐个模块跑通。