/* flash_ops.c - improved flash operations for STM32F4 family
 * Provides erase and program helpers with verification. Adjust sector mapping for your MCU.
 */

#include "flash_ops.h"
#include "stm32f4xx_hal.h"
#include <string.h>

// For STM32F4 (medium/medium-density) typical sector layout:
// Sector 0..3: 16 KB each
// Sector 4: 64 KB
// Sector 5..11: 128 KB each

static uint32_t get_sector_from_address(uint32_t address)
{
    // Addresses are in 0x08000000..
    uint32_t offset = address - FLASH_BASE;
    if (offset < 0x10000) {
        // first 64KB -> sectors 0..3 (16KB each)
        return offset / 0x4000;
    }
    if (offset < 0x20000) {
        // sector 4: 64KB
        return 4;
    }
    // sector 5..: 128KB blocks
    return 5 + ((offset - 0x20000) / 0x20000);
}

int flash_erase_region(uint32_t address, uint32_t length)
{
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInit;
    EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    uint32_t start_sector = get_sector_from_address(address);
    uint32_t end_sector = get_sector_from_address(address + length - 1);
    EraseInit.Sector = start_sector;
    EraseInit.NbSectors = (end_sector - start_sector) + 1;
    uint32_t SectorError = 0;
    if (HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return -1;
    }
    HAL_FLASH_Lock();
    return 0;
}

int flash_program_halfword(uint32_t address, uint16_t data)
{
    HAL_FLASH_Unlock();
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data) != HAL_OK) {
        HAL_FLASH_Lock();
        return -1;
    }
    // verify
    uint16_t readback = *(uint16_t*)address;
    HAL_FLASH_Lock();
    if (readback != data) return -1;
    return 0;
}
