#ifndef YMODEM_H
#define YMODEM_H

#include "stm32f4xx_hal.h"

int ymodem_receive_and_write(UART_HandleTypeDef *huart, uint32_t dest_address);

#endif // YMODEM_H
