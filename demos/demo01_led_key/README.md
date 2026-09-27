# demos/demo01_led_key/README.md

# Demo 1：LED + UART + KEY（最小 FreeRTOS / CMSIS‑V2）

## 1. 目标
验证以下功能：
- CubeMX 能生成工程
- Keil 能编译并下载
- J‑Link 能连接并烧录
- FreeRTOS / CMSIS‑V2 任务能启动
- 串口 printf 重定向有效
- 按键通过 EXTI 中断唤醒线程

## 2. 模块清单
- 1 个 LED（例如 PB5）
- 1 个按键（例如 PC13）
- 1 个串口模块（USB‑TTL / CP2102）

## 3. CubeMX 配置步骤
1. 新建工程：STM32F411CEU6
2. RCC 配置：保持默认
3. GPIO：
   - PB5 = GPIO_Output（LED）
   - PC13 = GPIO_EXTI13（按键）
4. USART1：
   - Asynchronous
   - TX = PA9
   - RX = PA10
5. Middleware → FreeRTOS：
   - 使用 CMSIS‑RTOS v2
6. Project Manager → Toolchain/IDE：Keil MDK‑ARM
7. Generate Code

## 4. 接线
- PA9 -> USB‑TTL RX
- PA10 -> USB‑TTL TX
- LED -> PB5 via 330Ω -> GND
- KEY -> PC13 -> GND（按键归零）或依据实际板子配置上拉
- GND -> 共地

## 5. 关键代码
### 5.1 printf 重定向
```c
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
```

### 5.2 CMSIS v2 线程创建（示例）
```c
#include "cmsis_os2.h"

osThreadId_t ledTaskHandle;
osThreadId_t keyTaskHandle;

void LEDTask(void *argument) {
    for (;;) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(500);
    }
}

void KeyTask(void *argument) {
    for (;;) {
        uint32_t flags = osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
        if (flags & 0x01) {
            printf("KEY pressed!\r\n");
        }
    }
}
```

### 5.3 EXTI 回调
```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == KEY_Pin) {
        osThreadFlagsSet(keyTaskHandle, 0x01);
    }
}
```

## 6. 验收标准
- 编译通过
- 下载成功
- 串口打印正常
- LED 每 500ms 闪烁一次
- 按下按键后串口打印 `KEY pressed!`

## 7. 常见问题
- 串口没有输出：检查 PA9/PA10 是否正确，波特率 115200
- LED 不闪：检查 GPIO 配置和脚对脚接线
- 按键无响应：检查 PC13 EXTI 和上拉/下拉配置

## 8. 下一步
验证通过后，再继续下一任务：
- Demo2：AHT21 + MPU6050
- Demo3：ADC + DMA

