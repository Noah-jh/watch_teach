/* sensor handler task - integration/handlers/sensor_handler.c */

#include "cmsis_os2.h"
#include "aht21.h"
#include "mpu6050.h"
#include <stdio.h>

static osThreadId_t sensorTaskHandle;

void sensor_task(void *arg)
{
    float temp, humi;
    int16_t ax,ay,az;

    aht21_init();
    mpu6050_init();

    for(;;) {
        if (aht21_read(&temp, &humi) == 0) {
            printf("AHT21 T=%.2f C H=%.2f%%\r\n", temp, humi);
        }
        if (mpu6050_read_accel(&ax, &ay, &az) == 0) {
            printf("MPU Accel: %d %d %d\r\n", ax, ay, az);
        }
        osDelay(1000);
    }
}

void sensor_handler_init(void)
{
    const osThreadAttr_t attr = { .name = "sensor", .stack_size = 512*4, .priority = osPriorityNormal };
    sensorTaskHandle = osThreadNew(sensor_task, NULL, &attr);
}
