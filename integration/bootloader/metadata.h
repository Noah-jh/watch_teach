#ifndef METADATA_H
#define METADATA_H

#include <stdint.h>

#define METADATA_ADDRESS 0x0803B000U // choose a small sector near end of flash; adjust for your MCU

typedef enum {
    IMAGE_STATE_EMPTY = 0xFF,
    IMAGE_STATE_PENDING = 0x01,
    IMAGE_STATE_VALID = 0x02,
    IMAGE_STATE_ACTIVE = 0x03,
    IMAGE_STATE_BAD = 0x04
} image_state_t;

typedef struct {
    uint32_t magic;      // 'WTFW'
    uint32_t version;
    uint32_t length;
    uint32_t crc;
    uint32_t timestamp;
    uint8_t state;       // image_state_t
    uint8_t reserved[11];
} image_metadata_t;

int metadata_read(image_metadata_t *meta);
int metadata_write(const image_metadata_t *meta);
int metadata_set_state(image_state_t state);

#endif // METADATA_H
