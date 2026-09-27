# Demo 6：ESP8266 / HC-05 / WT588F02 串口 DMA

## 目标
对应图片中的串口不定长数据、DMA 环形缓冲、Speaker 驱动和 handler 分层。

## 单模块验证
不要同时把 ESP8266、HC-05、WT588F02 接到同一个 UART。每次只接一个模块；后续总工程使用不同 UART 或模拟复用。

## 推荐引脚
- PA9：USART1_TX -> 模块 RX
- PA10：USART1_RX <- 模块 TX
- GND 共地

## CubeMX
1. USART1 Asynchronous，115200 8N1。
2. 开启 UART RX DMA，Circular 模式。
3. 开启 IDLE 中断或使用 HAL 的 Receive-To-Idle API（具体 API 以 HAL 版本为准）。
4. 任务中解析数据，ISR/DMA 回调只记录长度并通知任务。

## 验收
- ESP8266 发送 `AT` 返回 `OK`。
- HC-05 能返回模块信息。
- WT588F02 的命令帧按其手册发送并能播放指定语音。

## 电源
ESP8266 需要稳定的 3.3V 大电流供电。不能用 MCU 的普通 GPIO 供电。HC-05 的 VCC/逻辑电平以模块版本为准。
