# 37 章学习路线映射

本文把你上传的 37 张作业图片中的知识点，映射到本仓库的独立实验工程。仓库采用“先独立验证、后统一封装”的方式，不要求所有模块同时接线。

## 一、课程路线总览

| 阶段 | 图片中的知识点 | 本仓库实验 |
|---|---|---|
| 工程与规范 | 01 项目流程、02 电路基础、03 代码架构与分层、08 Keil 调试与规范 | docs/naming_and_style.md、docs/integration_architecture.md |
| 调试与下载 | 04 调用栈/stack trace、05 J-Link、17 Bootloader、20 Ozone 单元测试、23 Ozone 调试 | docs/debug_and_jlink.md、demo13_bootloader_notes |
| RTOS 与基础控制 | 07 CubeMX + FreeRTOS HelloWorld、09~13 按键/LED/定时器/中断 | demo01_led_key、demo14_irq_timer |
| 日志与底层驱动 | 14 EasyLog_RTT、16~19 Speaker 原理/driver/handler、22 OTA | demo10_wt588f02、docs/logging.md、demo13_bootloader_notes |
| ADC、DMA、串口 | 15-1 ADC + DMA、16-1 串口不定长 + DMA 环形缓冲、21/22/23/24 中断优化 | demo03_adc_dma、demo06_uart_modules、demo14_irq_timer |
| 传感器驱动 | 06 AHT21 HAL driver、07 AHT21 HAL driver.c、08 Handler.h、10 Handler.c、11 MPU6050 driver.h、12 HAL driver.c、13 handler.h、14 handler.c | demo02_i2c_sensor、demo11_sensor_drivers |
| 显示与存储 | 25 LCD/ST7789 driver、33 显示驱动平台化、34 触摸驱动平台化、36 UI 资源到外部 Flash、37 背光驱动 | demo04_st7789、demo05_oled、demo12_lvgl |
| 项目集成 | 24 OS 层、26~32 项目集成/顺序/BSP/GUI | docs/integration_architecture.md、demo12_lvgl |
| LVGL 基础 | 26 初识 LVGL、28 目录结构、30~36 组件/事件/动画/图表/画布/触摸 | demo12_lvgl、demo05_oled |
| 工程化 | 27~32 项目集成、MCU 外设驱动框架、外部存储器平台化 | docs/integration_architecture.md、demo11_sensor_drivers |

## 二、逐章对应关系

| 章/任务编号 | 学习目标 | 推荐入口 |
|---:|---|---|
| 01 | 大厂项目流程与目录创建 | docs/integration_architecture.md |
| 02 | 电路基础 | docs/wiring.md；所有 demo 的电源章节 |
| 03 | 代码架构与分层 | docs/integration_architecture.md |
| 04 | 调用栈与故障定位 | docs/debug_and_jlink.md |
| 05 | J-Link 下载 | docs/debug_and_jlink.md |
| 06 | AHT21 HAL 层 driver.h | demo02_i2c_sensor、demo11_sensor_drivers |
| 07 | AHT21 HAL 层 driver.c | demo02_i2c_sensor、demo11_sensor_drivers |
| 08 | AHT21 Handler.h | demo11_sensor_drivers |
| 09 | AHT21 基础组件 | demo11_sensor_drivers |
| 10 | AHT21 Handler.c | demo11_sensor_drivers |
| 11 | MPU6050 driver.h | demo11_sensor_drivers |
| 12 | MPU6050 HAL driver.c | demo11_sensor_drivers |
| 13 | MPU6050 handler.h | demo11_sensor_drivers |
| 14 | MPU6050 handler.c | demo11_sensor_drivers |
| 15-1 | ADC + DMA，理解 CPU/DMA 并行 | demo03_adc_dma |
| 16 | Speaker 原理与驱动 | demo10_wt588f02 |
| 17 | Speaker HAL driver.c | demo10_wt588f02 |
| 18 | Speaker handler.h | demo10_wt588f02 |
| 19 | Speaker handler.c | demo10_wt588f02 |
| 20 | 中断理论与优化 | demo14_irq_timer |
| 21 | 中断延时捕获 | demo14_irq_timer |
| 22 | 中断延迟优化 | demo14_irq_timer |
| 23 | 中断上半部优化 | demo14_irq_timer |
| 24 | OS 层项目集成 | docs/integration_architecture.md |
| 25 | ST7789 Driver 层 | demo04_st7789 |
| 26 | 初识 LVGL | demo12_lvgl |
| 27 | 项目架构总览 | docs/integration_architecture.md |
| 28 | LVGL 文件夹结构 | demo12_lvgl/README.md |
| 29 | BSP 层平台化 | docs/integration_architecture.md、demo04_st7789 |
| 30~32 | LVGL 基础组件 | demo12_lvgl |
| 33 | 显示驱动平台化 | demo04_st7789 |
| 34 | 触摸驱动平台化 | demo12_lvgl |
| 35 | J-Link 下载算法/流程 | docs/debug_and_jlink.md |
| 36 | UI 资源迁移到外部 Flash | demo12_lvgl/README.md |
| 37 | ST7789 背光控制集成 | demo04_st7789 |

## 三、推荐验证顺序

1. Demo01：LED、UART、按键、中断、CMSIS-RTOS v2。
2. Demo14：定时器、中断精度、短按/长按状态机。
3. Demo02：AHT21、MPU6050、I2C 驱动分层。
4. Demo03：ADC、DMA、摇杆、MQ-2、土壤湿度、电池分压。
5. Demo06：UART DMA 不定长接收，验证 ESP8266/HC-05。
6. Demo04：ST7789、SPI、DMA、背光。
7. Demo05：OLED。
8. Demo07：HC-SR04 输入捕获。
9. Demo08：RC522/RFID SPI。
10. Demo09：SG90 PWM、风扇 MOSFET。
11. Demo10：WT588F02 语音模块。
12. Demo11：传感器驱动 HAL/handler 模板。
13. Demo12：LVGL、触摸、资源、事件和组件。
14. Demo13：Bootloader/OTA 阅读与独立验证。
15. 最后建立 integration/ 总工程，解决真实硬件引脚和供电冲突。

## 四、重要边界

本仓库中的每个 demo 是“可按说明创建的 CubeMX 工程模板”，不是把未知型号的模块直接假定为同一种芯片。LCD、OLED、RFID、CO2、WT588F02 都需要根据实物丝印和数据手册确认控制器、地址、供电和协议后才能形成最终驱动。
