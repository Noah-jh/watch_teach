#ifndef FLASH_OPS_H
#define FLASH_OPS_H

#include <stdint.h>

int flash_erase_region(uint32_t address, uint32_t length);
int flash_program_halfword(uint32_t address, uint16_t data);

#endif // FLASH_OPS_H
