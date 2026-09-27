# Demo 7：HC-SR04 超声波

## 目标
使用 GPIO 触发、定时器输入捕获测量 Echo 高电平时间，完成中断/定时器精确测量。

## 接线
- TRIG：MCU GPIO 输出
- ECHO：MCU 定时器输入捕获
- VCC：按模块要求通常 5V
- GND：共地

ECHO 常为 5V，进入 STM32 前必须使用电阻分压或电平转换。

## CubeMX
- 一个 GPIO 输出作为 TRIG。
- 一个 TIM Channel 配置 Input Capture。
- 使能 TIM 中断。
- 任务中计算距离：`distance_cm ≈ echo_us / 58`。

## 验收
- 触发周期不小于模块要求。
- Echo 脉宽随距离变化。
- 使用尺子对比，误差在合理范围。

## 注意
不要在中断中 printf 或调用阻塞延时；将捕获值放入变量后通知任务。
