# Demo 11：传感器驱动 HAL / Handler 模板

## 目标
综合图片中 AHT21、MPU6050 的 `driver.h`、`driver.c`、`handler.h`、`handler.c` 编写要求。

## 分层
- `ec_bsp_aht21_driver.h/c`：只负责 AHT21 协议和状态。
- `ec_bsp_mpu6050_driver.h/c`：只负责 MPU6050 寄存器和数据换算。
- `sensor_handler.h/c`：负责周期读取、错误重试、队列投递。
- `sensor_task.c`：负责 CMSIS-RTOS v2 任务调度。

## 接口要求
- 构造函数检查所有指针。
- 所有非 void 返回值都检查。
- I2C、tick、delay 通过函数指针注入。
- 驱动不得直接依赖某个 FreeRTOS 任务句柄。
- ISR 不执行 I2C 阻塞访问。

## 验收
- 驱动可以单独编译。
- I2C 错误能返回明确枚举值。
- handler 能统计重试次数。
- 任务能通过队列收到有效传感器数据。
