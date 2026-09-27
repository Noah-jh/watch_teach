/* flash_ops.c - integration/bootloader/flash_ops.c
 * Encapsulate HAL flash erase/program primitives used by bootloader
 */

#include "flash_ops.h"
#include "stm32f4xx_hal.h"

int flash_erase_region(uint32_t address, uint32_t length)
{
    // Note: STM32F4 uses sectors; adapt as needed for your device
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInit;
    EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInit.Sector = 0; // this is a placeholder; compute based on address
    EraseInit.NbSectors = 1; // compute actual count
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
    HAL_FLASH_Lock();
    return 0;
}
