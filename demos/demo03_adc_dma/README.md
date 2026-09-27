# demos/demo03_adc_dma/README.md

# Demo 3：ADC + DMA（MQ‑2 / 摇杆 / 电池监测）

## 1. 目标
验证：
- ADC 多通道采样
- DMA 连续采样
- 任务中读取数据并处理
- 模拟信号随输入变化而变化

## 2. 模块说明
- MQ‑2：模拟气体检测，通常需要 5V 加热
- 此模块模拟输出需分压后输入 STM32 ADC
- 摇杆：模拟 X/Y 输出
- 电池检测：分压后测量

## 3. 典型接线
### 3.1 MQ‑2
- VCC -> 5V（独立电源）
- GND -> GND
- A0 -> 分压 -> PA1
- 注意：输出不能直接超过 3.3V

### 3.2 摇杆
- X -> PA2
- Y -> PA3
- VCC -> 3.3V
- GND -> GND

### 3.3 电池
- 电池 -> 分压电阻 -> PA0
- 目标是使电压 <= 3.3V

## 4. CubeMX 配置
1. ADC1 多通道启用
2. 为通道 0/1/2/3 配置 `ADC1_IN0`、`ADC1_IN1`、`ADC1_IN2`、`ADC1_IN3`
3. DMA 连续请求：ENABLE
4. USART1：串口输出
5. Middleware → FreeRTOS → CMSIS v2

## 5. 关键代码
```c
volatile uint16_t adc_buf[4];
extern osThreadId_t adcTaskHandle;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    osThreadFlagsSet(adcTaskHandle, 0x01);
}

void ADCTask(void *argument) {
    for (;;) {
        osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
        printf("ADC0=%d ADC1=%d ADC2=%d ADC3=%d\r\n",
               adc_buf[0], adc_buf[1], adc_buf[2], adc_buf[3]);
    }
}
```

## 6. 验收标准
- 摇杆移动时，ADC 数值变化
- 电池分压采样稳定
- MQ‑2 输出随着环境变化而变化

## 7. 安全提醒
- MQ‑2 需要通风测试
- 大电流模块最好独立供电
- 分压和滤波需严格做，否则 ADC 易损坏

