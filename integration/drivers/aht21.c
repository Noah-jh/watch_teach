/* AHT21 driver minimal - integration/drivers/aht21.c */

#include "aht21.h"
#include "hal_port.h"

#define AHT21_ADDR 0x38

int aht21_init(void)
{
    // simple init sequence - send soft reset or initialize if needed
    uint8_t cmd[] = {0xE1,0x08,0x00};
    if (hal_i2c_write(AHT21_ADDR, cmd, sizeof(cmd), 100) != 0) return -1;
    hal_delay_ms(30);
    return 0;
}

int aht21_read(float *temp, float *humi)
{
    uint8_t cmd[] = {0xAC,0x33,0x00};
    uint8_t buf[6];
    if (hal_i2c_write(AHT21_ADDR, cmd, sizeof(cmd), 100) != 0) return -1;
    hal_delay_ms(80);
    if (hal_i2c_read(AHT21_ADDR, buf, 6, 100) != 0) return -1;
    // parse raw (simple, non-optimized)
    uint32_t rawh = ((uint32_t)(buf[1])<<12) | ((uint32_t)(buf[2])<<4) | ((buf[3]>>4)&0x0F);
    uint32_t rawt = ((uint32_t)(buf[3]&0x0F)<<16) | ((uint32_t)buf[4]<<8) | buf[5];
    *humi = (float)rawh * 100.0f / 1048576.0f;
    *temp = (float)rawt * 200.0f / 1048576.0f - 50.0f;
    return 0;
}
