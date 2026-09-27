# CRC32 implementation (integration/bootloader/crc32.c) - table-less bitwise

#include <stdint.h>

uint32_t crc32_compute(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFU;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1U) crc = (crc >> 1) ^ 0xEDB88320U;
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFFFFFFU;
}
