# docs/wiring.md

# 电源 / 接线总纲（STM32F411CEU6 + 面包板）

本文件说明最小系统板接线逻辑。所有模块均建议按“单独 demo 验证”的方式接线，避免多模块同时导致引脚冲突。

## 1. 基本供电

### 1.1 MCU 供电
- MCU 工作电压：3.3V
- 不建议直接把 18650 电池直接接到 MCU
- 必须使用：
  - 3.3V 稳压模块
  - 或者 5V -> 3.3V 线性/开关稳压板
  - 或者使用成熟的电池管理 + DC‑DC 电源链

### 1.2 高电流模块供电
以下模块不建议直接从 MCU 电源引出：
- ESP8266（峰值电流大）
- MQ‑2 加热器（通常需要 5V）
- 舵机 SG90（瞬时大电流）
- 风扇（大电流）
- 语音模块 WT588F02

建议做法：
- MCU / 逻辑部分：独立 3.3V 稳压
- 高电流部分：独立 5V 电源（建议 2A 以上）
- 所有 GND 必须共地

### 1.3 TP4056 + 18650
- TP4056 只是充电板，不是稳定工作电源
- 18650 直接输出通常 3.7~4.2V，需要做稳压处理后给 MCU
- 若同时给高功耗模块供电，需要单独设计 DC/DC 或 booster

## 2. 常用总线说明

### 2.1 I2C
适用于：
- AHT21
- MPU6050
- OLED 128x64

建议默认：
- I2C1_SCL = PB6
- I2C1_SDA = PB7
- 供电 3.3V
- 若模块上没有上拉电阻，需外部加 4.7k 至 3.3V（多数模块已经带上拉）

### 2.2 SPI
适用于：
- ST7789 LCD
- RC522 RFID

建议默认：
- SPI1_SCK = PA5
- SPI1_MOSI = PA7
- SPI1_MISO = PA6（若模块需要）
- CS = PA4
- DC = PB0
- RST = PB1
- BL = PB2 或其它 PWM/IO

### 2.3 UART
适用于：
- ESP8266
- HC‑05
- WT588F02

建议默认：
- USART1_TX = PA9
- USART1_RX = PA10

注意：
- ESP8266 需要 3.3V 电压，且负载大
- 模块 TX/RX 必须互相连接，且 GND 共地

### 2.4 ADC
适用于：
- MQ‑2 模拟输出
- 摇杆模拟输出
- 土壤湿度模拟输出
- 电池电压分压读取

建议默认：
- ADC_IN0 = PA0
- ADC_IN1 = PA1
- ADC_IN2 = PA2
- ADC_IN3 = PA3

### 2.5 PWM / TIM
适用于：
- SG90 舵机
- 风扇 PWM 调速
- 背光 PWM
- Speaker / buzzer

建议默认：
- TIM3_CH1 = PA6 / speaker
- TIM4_CH1 = PB6 等（按实际板定）

### 2.6 EXTI / GPIO
适用于：
- 按键
- HC‑SR04 Echo（需经过电平转换）
- 给系统事件/中断唤醒任务

建议默认：
- KEY = PC13
- LED = PB5

## 3. 常见模块特别提醒

### 3.1 AHT21 / MPU6050 / OLED
- 这类 I2C 设备通常工作在 3.3V
- 若模块是 5V 板，需要 I2C 级别转换或使用 3.3V 版本

### 3.2 MQ‑2
- 工作电流大、热电阻需要 5V
- 模拟输出需要分压：0~5V -> 0~3.3V
- 先做实验时一定要保证通风

### 3.3 HC‑SR04
- Echo 输出 5V，需要电平分压或转换。不能直接接 STM32 5V tolerant IO
- Trigger 直接接 MCU GPIO 输出即可

### 3.4 ESP8266
- 需要稳定 3.3V 电源且具备大电流能力
- 串口信号需要 3.3V 级别，不要直接接 5V TTL

### 3.5 SG90 舵机
- 需要独立 5V 电源
- PWM 信号可直接接 3.3V 的 MCU PWM 输出，但共地必须接好
- 舵机启动电流较大，面包板不建议直接承载

### 3.6 LCD / Screen / OLED
- 若模块背光是 5V，一定要使用 3.3V/5V 兼容接法或 MOSFET 驱动

## 4. 面包板接线推荐方式

建议为每个 demo 独立接线，不要一条面包板上塞所有模块。这样便于：
- 把问题定位在某个模块上
- 更容易替换引脚
- 减少 pin 冲突

建议分配：
- 试验板 1：LED + KEY + UART
- 试验板 2：I2C（AHT21 / MPU6050 / OLED）
- 试验板 3：SPI（LCD / RC522）
- 试验板 4：ADC（MQ‑2 / 摇杆 / Battery）
- 试验板 5：PWM（SG90 / 风扇 / speaker）

## 5. 工程中常见 pinmap 模板

```csv
demo_id,module,mcu_pin,signal,notes
1,LED,PB5,GPIO_OUTPUT,
1,KEY,PC13,GPIO_EXTI,
1,UART_TX,PA9,USART1_TX,
1,UART_RX,PA10,USART1_RX,
2,AHT21,SCL,PB6,I2C1_SCL
2,AHT21,SDA,PB7,I2C1_SDA
2,MPU6050,SCL,PB6,I2C1_SCL
2,MPU6050,SDA,PB7,I2C1_SDA
3,ADC1,PA0,ADC1_IN0,battery divider
3,ADC2,PA1,ADC1_IN1,MQ2 analog
3,ADC3,PA2,ADC1_IN2,joystick x
3,ADC4,PA3,ADC1_IN3,joystick y
4,ST7789,SCK,PA5,SPI1_SCK
4,ST7789,MOSI,PA7,SPI1_MOSI
4,ST7789,CS,PA4,GPIO
4,ST7789,DC,PB0,GPIO
4,ST7789,RST,PB1,GPIO
4,ST7789,BL,PB2,GPIO or PWM
6,ESP8266_TX,PA10,USART1_RX,
6,ESP8266_RX,PA9,USART1_TX,
```

## 6. 验收清单（每个模块都要验证）
每个 demo 完成后需要确认：
- 编译通过
- 连接成功
- 有串口输出 / 数据变化 / 工作状态变化
- 不带来硬件发热、掉电、损坏风险

## 7. 小结
如果你要一步一步验证：
1. 先做 LED + 按键 + UART
2. 再做 I2C（AHT21 / MPU6050）
3. 再做 ADC / DMA / 气体 & 摇杆
4. 再做 LCD / OLED
5. 再做 UART 模块（ESP8266 / HC‑05）
6. 再做超声波/舵机/RFID/语音等

只要每一步都独立跑通，就可以放心合并成总工程。

