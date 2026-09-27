/* Demo13 bootloader - improved bootloader.c
 * This bootloader implements:
 * - Read image header in staging or app slot
 * - Verify CRC32 (software implementation)
 * - Simple activation protocol: if staging image valid, copy to APP slot or mark active
 * - Jump to app if valid
 * - If no valid app, wait for OTA receive command over UART
 *
 * NOTE: Full YMODEM receive is complex; this implementation expects a host tool to send raw image with a small header
 * The repo contains a ymodem skeleton and guidance to use lrzsz or similar tools. This bootloader is intentionally
 * conservative: it will not erase app slot unless a complete and verified image has been received into staging.
 */

#include "bootloader.h"
#include "flash_ops.h"
#include "crc32.h"
#include "ymodem.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

#define UART_BOOT HAL_UART_1

static void uart_log(const char *fmt, ...)
{
    // minimal uart log using HAL UART; assumes huart1 exists
    extern UART_HandleTypeDef huart1;
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
}

static int verify_image_at(uint32_t addr)
{
    // read header (first 16 bytes): magic(4), version(4), length(4), crc(4)
    uint8_t hdr[16];
    memcpy(hdr, (void*)addr, sizeof(hdr));
    if (hdr[0] != 'W' || hdr[1] != 'T' || hdr[2] != 'F' || hdr[3] != 'W') return 0;
    uint32_t len = *((uint32_t*)&hdr[8]);
    uint32_t crc_expect = *((uint32_t*)&hdr[12]);
    uint32_t crc = crc32_compute((uint8_t*)(addr + 16), len);
    return crc == crc_expect;
}

static void copy_staging_to_app(void)
{
    // Very simple copy routine; production should use page-wise erase and copy with verification.
    uint32_t src = OTA_ADDRESS;
    uint32_t dst = APP_ADDRESS;
    uint32_t hdr[4];
    memcpy(hdr, (void*)src, sizeof(hdr));
    uint32_t len = hdr[2]; // bytes len

    // erase destination region as needed
    flash_erase_region(dst, len + 32);

    // program in halfword chunks
    uint32_t remain = len + 16; // include header
    uint32_t offset = 0;
    while (remain > 0) {
        uint16_t half = *(uint16_t*)(src + offset);
        flash_program_halfword(dst + offset, half);
        offset += 2;
        remain -= 2;
    }
}

void bootloader_main_loop(void)
{
    HAL_Init();
    SystemClock_Config();

    // init UART for logs
    MX_USART1_UART_Init();
    uart_log("Bootloader start\r\n");

    if (verify_image_at(APP_ADDRESS)) {
        uart_log("Valid app found at APP slot. Jumping...\r\n");
        bootloader_jump_to_app();
    }

    uart_log("No valid app. Checking staging...\r\n");
    if (verify_image_at(OTA_ADDRESS)) {
        uart_log("Valid image in OTA staging. Installing...\r\n");
        copy_staging_to_app();
        uart_log("Install complete. Jumping to app...\r\n");
        bootloader_jump_to_app();
    }

    uart_log("Entering OTA receive mode. Send image via YMODEM or raw protocol.\r\n");
    // receive image into OTA_ADDRESS
    extern UART_HandleTypeDef huart1;
    if (ymodem_receive_and_write(&huart1, OTA_ADDRESS) == 0) {
        uart_log("Receive complete. Verifying...\r\n");
        if (verify_image_at(OTA_ADDRESS)) {
            uart_log("Staging image valid. Installing...\r\n");
            copy_staging_to_app();
            uart_log("Install finished. Rebooting...\r\n");
            NVIC_SystemReset();
        } else {
            uart_log("Staging image invalid (CRC mismatch).\r\n");
        }
    } else {
        uart_log("Receive failed or timed out.\r\n");
    }

    while (1) {
        HAL_Delay(1000);
    }
}
