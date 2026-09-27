/* WT588 driver skeleton (UART) - integration/drivers/wt588.c */

#include "wt588.h"
#include "hal_port.h"

int wt588_play_index(uint16_t idx)
{
    // Example: send frame to WT588 to play index (depends on module framing)
    uint8_t cmd[4];
    // Simple placeholder - real module requires actual CRC/format
    cmd[0] = 0x7E;
    cmd[1] = (uint8_t)(idx & 0xFF);
    cmd[2] = 0x00;
    cmd[3] = 0x7E;
    if (hal_uart_tx(cmd, sizeof(cmd), 1000) != 0) return -1;
    return 0;
}
