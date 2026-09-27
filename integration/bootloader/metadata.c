#include "metadata.h"
#include "flash_ops.h"
#include <string.h>

int metadata_read(image_metadata_t *meta)
{
    if (meta == NULL) return -1;
    memcpy(meta, (void*)METADATA_ADDRESS, sizeof(image_metadata_t));
    return 0;
}

int metadata_write(const image_metadata_t *meta)
{
    if (meta == NULL) return -1;
    // Erase metadata sector then program
    if (flash_erase_region(METADATA_ADDRESS, 0x1000) != 0) return -1; // assume 4KB sector

    uint32_t addr = METADATA_ADDRESS;
    const uint8_t *p = (const uint8_t*)meta;
    for (size_t i = 0; i < sizeof(image_metadata_t); i += 2) {
        uint16_t half = p[i];
        if (i+1 < sizeof(image_metadata_t)) half |= (uint16_t)p[i+1] << 8;
        if (flash_program_halfword(addr, half) != 0) return -1;
        addr += 2;
    }
    return 0;
}

int metadata_set_state(image_state_t state)
{
    image_metadata_t meta;
    if (metadata_read(&meta) != 0) return -1;
    meta.state = (uint8_t)state;
    return metadata_write(&meta);
}
