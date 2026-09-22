/**
 * @file flash.c
 * @brief Source file to configure internal flash memory of the CPU
 *
 * @author Anirudh Singh
 * @date 25th September 2026
 */

#include "flash.h"
#include "stm32f4xx.h"

void flash_config_wait_states(uint8_t hclk) {

  // Clear bits in the registerN
  FLASH->ACR &= ~(FLASH_ACR_LATENCY | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_ICRST | FLASH_ACR_DCRST);

  // Set the following bits
  FLASH->ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN);

  /**
  * Calculate the number of wait states (WS)
  * Because at 30Mhz WS should be zero, 1 is subtracted.
  * The division is done by 30 because of the HCLK range in the column 1 of Table 5.
  */
  uint8_t latency = (hclk - 1) / 30;

  // Shift left the latency binary by the position in the register and then mask it with the register
  FLASH->ACR |= (latency << FLASH_ACR_LATENCY_Pos);
}
