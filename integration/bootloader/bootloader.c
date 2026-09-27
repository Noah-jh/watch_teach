# Update bootloader to use metadata and APP_OK mechanism (partial)

#include "bootloader.h"
#include "flash_ops.h"
#include "crc32.h"
#include "ymodem.h"
#include "metadata.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

static void uart_log(const char *fmt, ...)
{
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
}

static int verify_image_at(uint32_t addr, image_metadata_t *out_meta)
{
    image_metadata_t meta;
    memcpy(&meta, (void*)addr, sizeof(image_metadata_t));
    if (meta.magic != 0x57465457U) return 0; // 'WTFW'
    uint32_t len = meta.length;
    uint32_t crc_expect = meta.crc;
    uint32_t crc = crc32_compute((uint8_t*)(addr + sizeof(image_metadata_t)), len);
    if (out_meta) *out_meta = meta;
    return crc == crc_expect;
}

static int install_staging(void)
{
    // Read staging metadata
    image_metadata_t meta;
    if (metadata_read(&meta) != 0) return -1;
    if (meta.state != IMAGE_STATE_VALID && meta.state != IMAGE_STATE_PENDING) return -1;

    // Erase app area and copy
    uint32_t src = OTA_ADDRESS;
    uint32_t dst = APP_ADDRESS;
    uint32_t len = meta.length + sizeof(image_metadata_t);
    if (flash_erase_region(dst, len) != 0) return -1;

    for (uint32_t off = 0; off < len; off += 2) {
        uint16_t half = *(uint16_t*)(src + off);
        if (flash_program_halfword(dst + off, half) != 0) return -1;
    }

    // mark metadata active
    meta.state = IMAGE_STATE_ACTIVE;
    if (metadata_write(&meta) != 0) return -1;
    return 0;
}

void bootloader_main_loop(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_USART1_UART_Init();
    uart_log("Bootloader start\r\n");

    // check app slot
    image_metadata_t app_meta;
    if (verify_image_at(APP_ADDRESS, &app_meta)) {
        uart_log("Valid app found. Jumping...\r\n");
        bootloader_jump_to_app();
    }

    uart_log("Checking staging...\r\n");
    image_metadata_t stage_meta;
    if (verify_image_at(OTA_ADDRESS, &stage_meta)) {
        uart_log("Staging image valid. Installing...\r\n");
        if (install_staging() == 0) {
            uart_log("Install complete. Rebooting...\r\n");
            NVIC_SystemReset();
        } else {
            uart_log("Install failed.\r\n");
        }
    }

    uart_log("Enter OTA mode (serial).\r\n");
    if (ota_receive_simple(&huart1, OTA_ADDRESS, 120000) == 0) {
        uart_log("OTA receive OK. Verify and install...\r\n");
        if (verify_image_at(OTA_ADDRESS, &stage_meta)) {
            // mark staging valid
            stage_meta.state = IMAGE_STATE_VALID;
            metadata_write(&stage_meta);
            if (install_staging() == 0) {
                uart_log("Install complete. Rebooting...\r\n");
                NVIC_SystemReset();
            }
        } else {
            uart_log("Received image invalid (CRC).\r\n");
        }
    } else {
        uart_log("OTA receive failed or timed out.\r\n");
    }

    while (1) {
        HAL_Delay(1000);
    }
}
