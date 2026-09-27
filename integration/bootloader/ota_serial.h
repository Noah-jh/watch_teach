#ifndef OTA_SERIAL_H
#define OTA_SERIAL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

int ota_receive_simple(UART_HandleTypeDef *huart, uint32_t dest_address, uint32_t timeout_ms);

#endif // OTA_SERIAL_H
