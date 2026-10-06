/**
 * @file flash.h
 * @brief Header file to configure internal flash memory of the CPU
 *
 * @author Anirudh Singh
 * @date 25th September 2026
 */

#ifndef INC_FLASH_H
#define INC_FLASH_H

#include <stdint.h>

/**
 * @brief Configure the wait states to access the internal flash memory
 *
 * - This is necessary because of the relationship between CPU clock frequency and flash memory read time.
 * - To correctly read data from the flash memory, the number of wait states must be correctly programmed in the Access Control Register (FLASH_ACR) of the microcontroller.
 * - The number of these states are decided based upon the frequency of the CPU clock and the supply voltage of the device.
 *
 * @param uint8_t hclk The CPU clock frequency in MHz.
 */
void flash_config_wait_states(uint8_t hclk);

#endif // !FLASH_H
