/* RC522 skeleton (SPI) - integration/drivers/rc522.c */

#include "rc522.h"
#include "hal_port.h"

int rc522_init(void)
{
    // perform soft reset and basic init via SPI
    // This is a placeholder, use MFRC522 sequences in production
    return 0;
}

int rc522_read_uid(uint8_t *uid, uint8_t *len)
{
    // Placeholder: poll for card and return UID when found
    return -1;
}
