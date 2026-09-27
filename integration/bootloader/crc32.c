/* crc32.c - integration/bootloader/crc32.c
 * Simple CRC32 implementation using a table. Suitable for bootloader image verification.
 */

#include <stdint.h>

static const uint32_t crc_table[256] = {
    0x00000000U,0x77073096U,0xEE0E612CU,0x990951BAU,0x076DC419U,0x706AF48FU,0xE963A535U,0x9E6495A3U,
    /* table truncated for brevity in this sample; real file must include all 256 entries */
};

uint32_t crc32_compute(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFU;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t idx = (uint8_t)((crc ^ data[i]) & 0xFFU);
        // Incomplete: in real file use full table
        crc = (crc >> 8) ^ crc_table[idx];
    }
    return crc ^ 0xFFFFFFFFU;
}
