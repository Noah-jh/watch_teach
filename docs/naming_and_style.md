# docs/naming_and_style.md

# 编码规范（按照用户给定规范）

本文件用于约束 C 语言开发风格，保证代码可读性、统一性和工程扩展性。

## 1. 基本格式

### 1.1 缩进
- 每级缩进使用 4 个空格
- 不使用 tab
- 1 tab = 4 spaces

### 1.2 每行长度
- 代码行宽度建议不超过 80 列
- 如果参数很多，换行对齐

### 1.3 注释
- 函数必须有注释说明作用、参数、返回值
- 每个重要步骤必须写注释
- 头文件必须有版权/作者/文件说明注释

## 2. 命名规则

### 2.1 枚举类型
- 枚举类型名：小写 + 下划线结尾，如 `aht21_status_t`
- 枚举项：全部大写，如 `AHT21_OK`

```c
typedef enum {
    AHT21_OK = 0,
    AHT21_ERROR = 1,
    AHT21_TIMEOUT = 2,
} aht21_status_t;
```

### 2.2 宏定义
- 全部大写，用下划线隔开

```c
#define OS_SUPPORTING
#define AHT21_MEASURE_WAITING_TIME 80
```

### 2.3 全局变量
- 统一以 `g_` 开头

```c
static int8_t g_inited = 0;
static uint8_t g_device_id = 0;
```

### 2.4 局部变量
- 全部小写 + 下划线分隔

```c
int8_t ret = 0;
uint8_t device_count = 0;
```

### 2.5 指针变量
- 以 `p_` 开头

```c
bsp_aht21_driver_t *p_aht21_instance = NULL;
```

### 2.6 函数指针变量
- 以 `pf_` 开头

```c
uint8_t (*pf_iic_init)(void *);
```

### 2.7 函数名
- 统一小写 + 下划线分隔
- 应具有动作语义

```c
static uint8_t aht21_read_temp(float *temp, float *humi);
```

### 2.8 结构体名
- 结构体名建议小写并以 `_t` 结尾

```c
typedef struct {
    uint8_t id;
    uint8_t status;
} sensor_info_t;
```

## 3. 判断与比较

### 3.1 常量放左边，变量放右边
正确：
```c
if (0 == g_inited) {
    return AHT21_ERROR;
}
```

### 3.2 指针判空
```c
if (NULL == p_handler_instance) {
    return AHT21_ERRORPARAMETER;
}
```

### 3.3 返回值检查
所有函数调用都应该检查返回值，除非函数本身是 void。

```c
int8_t ret = 0;
ret = create_queue();
if (ret) {
    return AHT21_ERRORRESOURCE;
}
```

## 4. 头文件结构模板

```c
#ifndef _EC_BSP_AHT21_DRIVER_H_
#define _EC_BSP_AHT21_DRIVER_H_

#include "ec_bsp_aht21_reg.h"
#include <stdint.h>
#include <stdio.h>

// typedefs
// enums
// function declarations

#endif /* __EC_BSP_AHT21_DRIVER_H__ */
```

## 5. 函数注释模板

```c
/**
 * @brief Instantiates the bsp_led_handler_t target.
 *
 * Steps:
 *  1. Adds Core interfaces into bsp_led_driver instance target.
 *  2. Adds OS interfaces into bsp_led_driver instance target.
 *  3. Adds timebase interfaces into bsp_led_driver instance target.
 *
 * @param[in] self        : Pointer to the target of handler.
 * @param[in] os_delay    : Pointer to the os_delay_interface.
 * @param[in] os_queue    : Pointer to the os_queue_interface.
 * @param[in] os_thread   : Pointer to the os_thread_interface.
 * @param[in] time_base   : Pointer to the time_base_interface.
 *
 * @return Led_handler_status_t : Status of the function.
 */
```

## 6. 代码组织要求

推荐分层：
- `bsp/`：板级初始化与外设基础
- `drv/`：驱动层（LCD / I2C / SPI / ADC / 按键）
- `app/`：业务逻辑
- `service/`：统一的服务层
- `middleware/`：LVGL / FreeRTOS / 日志 / OTA

## 7. C 语言代码示例

```c
static uint8_t aht21_read_temp(bsp_aht21_driver_t *const paht21_instance,
                               float *const temp,
                               float *const humi)
{
    if (NULL == paht21_instance) {
        return AHT21_ERRORPARAMETER;
    }

    if (NULL == temp || NULL == humi) {
        return AHT21_ERRORPARAMETER;
    }

    return AHT21_OK;
}
```

## 8. 结论

统一命名、统一注释、统一层次、统一风格，有助于：
- 代码更可读
- 更容易复用和合并
- 更容易协作和 review
- 更容易维护和扩展

