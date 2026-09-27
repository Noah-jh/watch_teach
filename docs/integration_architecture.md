# 大厂式分层与最终合并方式

## 1. 推荐目录

```text
project/
├── Core/                 # CubeMX 生成：启动、时钟、中断、HAL 初始化
├── bsp/                  # 板级资源：引脚、供电、板级初始化
├── hal_port/             # 对 HAL 的最薄封装
├── drivers/              # 具体器件：aht21、mpu6050、st7789、rc522
├── handlers/             # 业务无关的器件使用策略和状态机
├── middleware/           # FreeRTOS、LVGL、EasyLogger、协议栈
├── services/             # sensor_service、display_service、storage_service
├── applications/         # 页面、业务和演示任务
├── tests/                # 主机单元测试和硬件联调记录
├── docs/                 # 架构、接线、调试、变更记录
└── tools/                # J-Link、构建、静态检查脚本
```

## 2. 调用方向

```text
applications -> services -> handlers -> drivers -> hal_port -> STM32 HAL
                       |                         |
                       +---- middleware ----------+
```

禁止事项：
- `applications` 直接操作 `HAL_I2C_Mem_Read`。
- `drivers` 直接依赖具体的 FreeRTOS 任务句柄。
- 中断服务函数中做 printf、浮点计算、阻塞延时或复杂协议解析。
- 一个全局 I2C/SPI 实例被多个驱动随意修改配置。

## 3. 接口注入

推荐驱动接收接口表，而不是在驱动内部绑定 `hi2c1`：

```c
typedef struct
{
    uint8_t (*pf_write)(void *p_context,
                        uint8_t device_address,
                        const uint8_t *p_data,
                        uint16_t data_length);
    uint8_t (*pf_read)(void *p_context,
                       uint8_t device_address,
                       uint8_t *p_data,
                       uint16_t data_length);
    uint32_t (*pf_get_tick)(void);
    void (*pf_delay_ms)(uint32_t delay_ms);
    void *p_context;
} i2c_interface_t;
```

这样做的目的：
- 驱动可以脱离具体 MCU 测试。
- 未来更换 I2C1/I2C2 只改适配层。
- 可在主机测试中替换为 mock。

## 4. 任务职责

| 任务 | 职责 | 不负责 |
|---|---|---|
| `app_key_task` | 接收按键事件、识别短按/长按 | 直接读取 GPIO 寄存器 |
| `sensor_task` | 调度传感器读取、发送数据 | 绘制 LVGL 控件 |
| `display_task` | 运行 LVGL handler | 直接读取 MPU6050 |
| `uart_rx_task` | 消费 DMA 环形缓冲 | 在 ISR 中解析完整协议 |
| `storage_task` | 批量保存参数 | 阻塞按键 ISR |

## 5. 错误处理

每一个非 void 调用都应检查返回值，错误码应在边界转换：

```c
ret = drv_aht21_read(p_driver, &temperature, &humidity);
if (AHT21_OK != ret)
{
    log_e("aht21 read failed: %d", ret);
    return SERVICE_ERROR;
}
```

## 6. 中断上半部/下半部

ISR 只做：
- 读取最小硬件状态。
- 写入无阻塞环形缓冲。
- 设置线程标志或释放信号量。
- 清除中断标志。

任务中做：
- 状态机。
- 数据解析。
- 日志。
- UI 更新。
- 文件/Flash 写入。

## 7. 合并前检查表

- [ ] 所有模块有独立 `drv_xxx.c/h`。
- [ ] 所有公共接口有返回值和 Doxygen 注释。
- [ ] 没有重复的全局符号。
- [ ] SPI 每个设备有独立 CS。
- [ ] I2C 地址没有冲突。
- [ ] ADC 通道和量程经过计算。
- [ ] 高电流模块使用独立电源。
- [ ] 所有 5V 输出进入 MCU 前已降压/分压。
- [ ] FreeRTOS 任务栈经过观察和调整。
- [ ] HardFault 能记录 PC/LR。
- [ ] 每个 demo 有接线、日志和验收记录。
