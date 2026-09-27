/* ota_serial.c - simple framed OTA receiver (robust raw protocol)
 * Purpose: provide a deterministic, timeout-aware, chunked transfer protocol for firmware
 * This is a simpler and smaller alternative to full YMODEM for embedded devices.
 * Frame format (simple):
 *  [SOH 0x01][4-byte len little-endian][4-byte CRC32][payload ...][EOT 0x04]
 * Chunking: Host may send payload in blocks; device writes incrementally and ACKs progress.
 * ACK: single byte 0x06, NAK: 0x15
 */

#include "ota_serial.h"
#include "flash_ops.h"
#include "crc32.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15

int ota_receive_simple(UART_HandleTypeDef *huart, uint32_t dest_address, uint32_t timeout_ms)
{
    uint8_t b;
    uint32_t start = HAL_GetTick();

    // wait for SOH
    while (1) {
        if (HAL_UART_Receive(huart, &b, 1, 100) == HAL_OK) {
            if (b == SOH) break;
        }
        if ((HAL_GetTick() - start) > timeout_ms) return -1;
    }

    // read len (4 bytes)
    uint8_t lenbuf[4];
    if (HAL_UART_Receive(huart, lenbuf, 4, 1000) != HAL_OK) return -1;
    uint32_t len = lenbuf[0] | (lenbuf[1]<<8) | (lenbuf[2]<<16) | (lenbuf[3]<<24);

    // read crc
    uint8_t crcbuf[4];
    if (HAL_UART_Receive(huart, crcbuf, 4, 1000) != HAL_OK) return -1;
    uint32_t crc_expect = crcbuf[0] | (crcbuf[1]<<8) | (crcbuf[2]<<16) | (crcbuf[3]<<24);

    // Erase dest region
    if (flash_erase_region(dest_address, len + 32) != 0) return -1;

    uint32_t bytes_received = 0;
    uint32_t write_addr = dest_address + 16; // leave space for header (we expect host may send header separately)
    uint8_t payload_buf[256];

    while (bytes_received < len) {
        uint32_t to_read = (len - bytes_received) > sizeof(payload_buf) ? sizeof(payload_buf) : (len - bytes_received);
        if (HAL_UART_Receive(huart, payload_buf, to_read, 5000) != HAL_OK) {
            HAL_UART_Transmit(huart, (uint8_t*)&NAK, 1, 100);
            return -1;
        }

        // program payload in halfword units
        for (uint32_t i = 0; i < to_read; i += 2) {
            uint16_t half = payload_buf[i];
            if (i+1 < to_read) half |= (uint16_t)payload_buf[i+1] << 8;
            if (flash_program_halfword(write_addr, half) != 0) {
                HAL_UART_Transmit(huart, (uint8_t*)&NAK, 1, 100);
                return -1;
            }
            write_addr += 2;
        }
        bytes_received += to_read;
        uint8_t ack = ACK;
        HAL_UART_Transmit(huart, &ack, 1, 100);
    }

    // expect EOT
    if (HAL_UART_Receive(huart, &b, 1, 2000) != HAL_OK) return -1;
    if (b != EOT) return -1;

    // verify CRC
    uint32_t crc = crc32_compute((uint8_t*)(dest_address + 16), len);
    if (crc != crc_expect) return -1;

    return 0;
}
