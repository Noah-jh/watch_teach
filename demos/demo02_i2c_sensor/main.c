# demos/demo02_i2c_sensor/main.c

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t sensorTaskHandle;

#define AHT21_ADDR 0x38U
#define MPU6050_ADDR 0x68U

void SensorTask(void *argument)
{
    uint8_t aht_buf[6] = {0};
    uint8_t mpu_buf[14] = {0};

    for (;;) {
        // 伪代码：读取 AHT21 温湿度
        // 1. 发送 measure command
        // 2. wait 80 ms
        // 3. read 6 bytes

        // 伪代码：读取 MPU6050
        // 1. wake up or read data registers
        // 2. read 6 or 14 bytes

        printf("[Demo2] AHT21 / MPU6050 data read\r\n");
        osDelay(500);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t sensor_attr = {
        .name = "SensorTask",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    sensorTaskHandle = osThreadNew(SensorTask, NULL, &sensor_attr);
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_FREERTOS_Init();

    osKernelStart();

    while (1) {
    }
}

