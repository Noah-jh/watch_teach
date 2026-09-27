/* MPU6050 driver skeleton - integration/drivers/mpu6050.c */

#include "mpu6050.h"
#include "hal_port.h"

#define MPU6050_ADDR 0x68

int mpu6050_init(void)
{
    // wake up device
    uint8_t data[2] = {0x6B, 0x00};
    if (hal_i2c_write(MPU6050_ADDR, data, 2, 100) != 0) return -1;
    hal_delay_ms(10);
    return 0;
}

int mpu6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t reg = 0x3B;
    uint8_t buf[6];
    if (hal_i2c_write(MPU6050_ADDR, &reg, 1, 100) != 0) return -1;
    if (hal_i2c_read(MPU6050_ADDR, buf, 6, 100) != 0) return -1;
    *ax = (int16_t)((buf[0]<<8)|buf[1]);
    *ay = (int16_t)((buf[2]<<8)|buf[3]);
    *az = (int16_t)((buf[4]<<8)|buf[5]);
    return 0;
}
