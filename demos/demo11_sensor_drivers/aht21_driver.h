/* driver template: aht21_driver.h */

#ifndef _AHT21_DRIVER_H_
#define _AHT21_DRIVER_H_

#include "sensor_hal.h"
#include <stdint.h>

typedef enum {
    AHT21_OK = 0,
    AHT21_ERROR = 1,
    AHT21_ERR_TIMEOUT = 2,
} aht21_status_t;

typedef struct {
    sensor_hal_if_t hal;
    uint8_t inited;
} aht21_t;

aht21_status_t aht21_init(aht21_t *dev);
aht21_status_t aht21_read(aht21_t *dev, float *temp, float *humi);

#endif // _AHT21_DRIVER_H_
