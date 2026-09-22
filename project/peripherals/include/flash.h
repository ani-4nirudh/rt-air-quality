/**
 * @file flash.h
 * @brief Header file to configure internal flash memory of the CPU
 *
 * @author Anirudh Singh
 * @date 25th September 2026
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

/**
 * @brief Configure the wait states to access the internal flash memory 
 * @param uint8_t hclk The CPU clock frequency in MHz.
 */
void flash_config_wait_states(uint8_t hclk);

#endif // !FLASH_H
