# Demo 11: Sensor driver templates (header + source)

/* driver template: sensor_hal.h */

#ifndef _SENSOR_HAL_H_
#define _SENSOR_HAL_H_

#include <stdint.h>

typedef struct {
    uint8_t (*pf_write)(void *context, uint8_t dev_addr, const uint8_t *data, uint16_t len);
    uint8_t (*pf_read)(void *context, uint8_t dev_addr, uint8_t *data, uint16_t len);
    uint32_t (*pf_get_tick)(void);
    void (*pf_delay_ms)(uint32_t ms);
    void *p_context;
} sensor_hal_if_t;

#endif // _SENSOR_HAL_H_
