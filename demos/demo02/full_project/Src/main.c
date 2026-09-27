/* Demo02 full_project main.c - I2C sensors: AHT21 + MPU6050
 * Intended to be used with CubeMX-generated project with I2C1 (PB6/PB7) and USART1.
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "aht21.h"
#include "mpu6050.h"

osThreadId_t sensorTaskHandle;

void SensorTask(void *argument)
{
    float temp=0, humi=0;
    int16_t ax,ay,az;

    if (aht21_init() == 0) printf("AHT21 init OK\r\n"); else printf("AHT21 init FAIL\r\n");
    if (mpu6050_init() == 0) printf("MPU6050 init OK\r\n"); else printf("MPU6050 init FAIL\r\n");

    for(;;) {
        if (aht21_read(&temp, &humi) == 0) {
            printf("AHT21 T=%.2f C H=%.2f%%\r\n", temp, humi);
        }
        if (mpu6050_read_accel(&ax,&ay,&az) == 0) {
            printf("MPU Accel: %d %d %d\r\n", ax, ay, az);
        }
        osDelay(1000);
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    osKernelInitialize();
    const osThreadAttr_t s_attr = { .name = "sensors", .priority = osPriorityNormal, .stack_size = 512*4 };
    sensorTaskHandle = osThreadNew(SensorTask, NULL, &s_attr);
    osKernelStart();
    while (1) {}
}
