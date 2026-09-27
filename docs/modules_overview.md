# docs/modules_overview.md

# 模块总览与可行接线策略

## 1. 模块清单（按你上传图片中的名字归类）

### 1.1 传感器类
- AHT21（四根线、I2C）
- GY521（MPU‑6050）
- 土壤温湿度监测
- MQ2 烟雾传感器（模拟 + 加热）
- 二氧化碳监测模块
- HC‑SR04 超声波模块
- RFID 读卡模块（迷你型）

### 1.2 显示类
- LCD屏幕
- OLED屏幕

### 1.3 通信类
- ESP8266
- HC‑05 蓝牙模块
- WT588F02-8S-C 语音模块

### 1.4 控制类
- SG90 舵机（180°）
- 风扇
- PS2 游戏摇杆 / 普通摇杆

### 1.5 电源类
- TP4056 1A 锂电池充电模块（Type-C）
- 18650 3.7V 锂电池

## 2. 模块 -> 接口映射

### 2.1 I2C 类（优先）
- AHT21 -> I2C1 (PB6/PB7)
- GY521 -> I2C1 (PB6/PB7)
- OLED (I2C) -> I2C1 (PB6/PB7)
- 土壤温湿度模块（若为 I2C 型） -> I2C1 (PB6/PB7)

### 2.2 SPI 类
- LCD 屏幕（ST7789 通常为 SPI） -> SPI1
- RC522 RFID -> SPI1 或 SPI2（按硬件/CS 分配）
- SPI OLED -> SPI1 或 SPI2

### 2.3 UART 类
- ESP8266 -> USART1 (PA9/PA10)
- HC‑05 -> USART2 (PA2/PA3) 或 USART1
- WT588F02 -> USART1 或 USART2（按实际模块版型）

### 2.4 ADC 类
- MQ‑2 模拟输出 -> PA1（经分压）
- 摇杆 X/Y -> PA2/PA3（或其它模拟通道）
- 电池分压 -> PA0
- 土壤湿度模拟 -> PA1/PA2/PA3 等可用 ADC 通道

### 2.5 PWM / GPIO 类
- SG90 舵机 -> TIM/PWM（推荐 PA8 或独立 PWM 通道）
- 风扇 -> MOSFET + PWM
- HC‑SR04 Trigger -> GPIO输出
- HC‑SR04 Echo -> TIM 输入捕获或 EXTI + 计时

## 3. 重要硬件层面

### 3.1 5V 模块必须独立供电
- 舵机
- 风扇
- ESP8266（稳定 3.3V 供电，峰值电流大）
- MQ‑2 加热器
- 某些 5V 屏幕

### 3.2 3V3 稳压要求
- STM32F411 的 GPIO 均建议在 3.3V 级别上工作
- PA0 和 PB5 仅支持 3.3V，不能直接接 5V

### 3.3 ADC 电压保护
- 所有模拟输出都需要保证采样电压 < 3.3V
- 若输出 5V，需分压后再接 ADC 输入

### 3.4 GND 共地
- 所有模块必须与 MCU 共地，否则并不能稳定工作

## 4. 最佳实践

推荐模组优先顺序：
1. AHT21 + MPU6050
2. MQ‑2 + 摇杆 + ADC
3. LCD / OLED
4. ESP8266 / HC‑05 / WT588F02
5. HC‑SR04
6. RFID
7. 舵机 / 风扇
8. 最后整合到一体工程

## 5. 实际建议的 demo 绑定

- Demo01: LED + KEY + UART
- Demo02: AHT21 + MPU6050 + OLED（可选）
- Demo03: MQ‑2 + 摇杆 + 电池监测
- Demo04: ST7789 LCD + 背光
- Demo05: OLED（I2C）
- Demo06: ESP8266 / HC‑05 / WT588F02（UART）
- Demo07: HC‑SR04 超声波
- Demo08: RFID
- Demo09: SG90 + 风扇（PWM）
- Demo10: 语音模块 / 处理队列
- Demo11: 统一 sensor driver / handler
- Demo12: LVGL + LCD + 触摸

