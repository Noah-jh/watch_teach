/* driver template: aht21_driver.c */

#include "aht21_driver.h"
#include <string.h>

#define AHT21_ADDR 0x38

static uint8_t default_write(void *ctx, uint8_t addr, const uint8_t *data, uint16_t len) {
    // implement using HAL_I2C_Mem_Write wrapper from ctx
    return 1; // 0 ok, 1 error
}

static uint8_t default_read(void *ctx, uint8_t addr, uint8_t *data, uint16_t len) {
    return 1;
}

aht21_status_t aht21_init(aht21_t *dev) {
    if (dev == NULL) return AHT21_ERROR;
    // ensure hal callbacks set
    if (dev->hal.pf_read == NULL || dev->hal.pf_write == NULL) return AHT21_ERROR;
    dev->inited = 1;
    return AHT21_OK;
}

aht21_status_t aht21_read(aht21_t *dev, float *temp, float *humi) {
    if (dev == NULL || !dev->inited) return AHT21_ERROR;
    uint8_t cmd[] = {0xAC, 0x33, 0x00};
    dev->hal.pf_write(dev->hal.p_context, AHT21_ADDR, cmd, sizeof(cmd));
    dev->hal.pf_delay_ms(80);
    uint8_t buf[6] = {0};
    dev->hal.pf_read(dev->hal.p_context, AHT21_ADDR, buf, 6);
    // parse raw to temp/humi (omitted)
    *temp = 25.0f; *humi = 50.0f;
    return AHT21_OK;
}
