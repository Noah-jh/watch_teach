# demos/demo02_i2c_sensor/README.md

# Demo 2：I2C 传感器（AHT21 + MPU6050）

## 1. 目标
验证：
- I2C 总线初始化
- AHT21 温湿度读取
- MPU6050 读取数据
- 线程中读取并串口输出

## 2. 硬件
- AHT21（4线 I2C）
- GY‑521（MPU6050）
- STM32F411CEU6 掌控
- 面包板 + 跳线

## 3. 接线建议
### 3.1 I2C1
- PB6 = I2C1_SCL
- PB7 = I2C1_SDA
- VCC = 3.3V
- GND = GND

### 3.2 传感器连接
- AHT21 VCC -> 3.3V
- AHT21 GND -> GND
- AHT21 SDA -> PB7
- AHT21 SCL -> PB6

- GY‑521 VCC -> 3.3V
- GY‑521 GND -> GND
- GY‑521 SDA -> PB7
- GY‑521 SCL -> PB6

若模块没有上拉：
- 需要在 SDA / SCL 上各加 4.7k 上拉电阻到 3.3V

## 4. CubeMX 配置
1. 新建工程：STM32F411CEU6
2. GPIO / USART1：保留 LED 和 UART 用于 printf
3. I2C1：主模式，Fast Mode（400kHz）
4. Middleware → FreeRTOS → CMSIS‑RTOS v2
5. Generate code

## 5. 关键代码思路
- 通过 HAL_I2C_Mem_Read / HAL_I2C_Mem_Write 读取 AHT21 和 MPU6050
- 在任务或轮询中读取并打印
- 若 AHT21 或 MPU6050 地址冲突，检查实际模块地址和数据手册

示例伪代码：
```c
// AHT21 measure
uint8_t cmd[3] = {0xAC, 0x33, 0x00};
HAL_I2C_Master_Transmit(&hi2c1, AHT21_ADDR << 1, cmd, 3, HAL_MAX_DELAY);
HAL_Delay(80);
HAL_I2C_Master_Receive(&hi2c1, AHT21_ADDR << 1, buf, 6, HAL_MAX_DELAY);

// MPU6050 wakeup and read register 0x3B
uint8_t pwr = 0x00;
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR << 1, 0x6B, 1, &pwr, 1, HAL_MAX_DELAY);
HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR << 1, 0x3B, 1, data, 6, HAL_MAX_DELAY);
```

## 6. 验收标准
- AHT21 温湿度数据稳定
- MPU6050 返回加速度或角速度值
- 串口可以打印数据
- I2C 总线无 NACK / Timeout

## 7. 常见问题
- 温湿度读数全 0：检查 I2C 地址、上拉、接线
- MPU6050 读不到：确认 `WHO_AM_I` 或 `PWR_MGMT_1` 是否正确配置

