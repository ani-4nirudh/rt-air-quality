/**
 * @file rcc.c
 * @brief Source file to configure the Reset & Clock Controller (RCC)
 *
 * @author Anirudh Singh
 * @date 30th September 2026
 */

#include "rcc.h"
#include "flash.h"

/**
 * Get the PLLP division factor based on the maximum VCO frequency of the MCU
 *  - We are basically backtracking inside the clock tree from the SYSCLK
 *  - PLLP can only have 4 values: 2, 4, 6, 8
 *  - The Constraint: SYSCLK * PLLP <= 432 MHz (Max. VCO)
 * @param mcu_hw_mhz HCLK hardware clock frequency to drive the CPU
 * @return PLLP divisor
 */
static uint8_t rcc_get_pllp(uint8_t mcu_hw_mhz) {
  if ((mcu_hw_mhz * 8) <= RCC_MAX_VCO_FREQ) {
    return 8;
  }

  if ((mcu_hw_mhz * 6) <= RCC_MAX_VCO_FREQ) {
    return 6;
  }

  if ((mcu_hw_mhz * 4) <= RCC_MAX_VCO_FREQ) {
    return 4;
  }

  // if ((mcu_hw_mhz * 2) <= RCC_MAX_VCO_FREQ)
  // Return 2 otherwise
  return 2;
}

/**
 * Set the PLLN, PLLM and PLLP registers
 * @param sysclk_freq_mhz System clockspeed the CPU runs at
 */

static void rcc_pll_config(uint8_t sysclk_freq_mhz) {
  uint8_t PLLP = 0;
  uint16_t PLLN = 0;
  uint8_t PLLM = RCC_HSE_MHZ;

  if (sysclk_freq_mhz > RCC_MAX_SYSCLK_MHZ) {
    sysclk_freq_mhz = RCC_MAX_SYSCLK_MHZ;
  }

  // Get the PLLP and PLLN
  PLLP = rcc_get_pllp(sysclk_freq_mhz);
  PLLN = sysclk_freq_mhz * PLLP;

  // Set the PLLP Register
  // This conversion is necessary because 2 is encoded for the binary number 00
  // In PLLM and PLLN, the binary corresponds to the calculated unsigned integer.
  PLLP = (PLLP / 2) - 1;
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
  RCC->PLLCFGR |= PLLP << RCC_PLLCFGR_PLLP_Pos;

  // Set the PLLN Register
  // This register's integer value corresponds to the binary number that it wants to receive. Therefore, no conversion needed.
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
  RCC->PLLCFGR |= PLLN << RCC_PLLCFGR_PLLN_Pos;

  // Set the PLLM Register
  // This register's integer value corresponds to the binary number that it wants to receive. Therefore, no conversion needed.
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
  RCC->PLLCFGR |= PLLM << RCC_PLLCFGR_PLLM_Pos;
}

/**
 * Initialising the RCC register
 * Steps:
 * - Set SYSCLK
 * - Configure flash wait states
 * - Enable the HSE
 * - Disable the PLL
 * - Configure the PLL as the clock source
 * - Enable the PLL
 * - Wait for PLL clock to stabilise
 * - Disable the HSE
 * - Set the peripheral clock
 */
void rcc_init(void) {

  // Set up the max system clock frequency to 180 MHz
  uint8_t sysclk_freq_mhz = RCC_MAX_SYSCLK_MHZ;

  // Set up the wait states for the flash memory
  flash_config_wait_states(sysclk_freq_mhz);

  // Enable HSE clock X1 on the ST-Link Debugger
  rcc_hse_enable();

  // Disable the PLL
  rcc_pll_disable();

  // Set up HSE clock as PLL source
  rcc_pll_source(RCC_PLL_SRC_HSE);

  // Set up the PLL clock source params
  rcc_pll_config(sysclk_freq_mhz);

  // Enable the PLL
  rcc_pll_enable();

  // Set the prescaler for the AHB bus
  rcc_ahb_set_prescaler(RCC_SYSCLK_DIV_1);

  // Switch the system clock source to PLLP
  rcc_sysclk_set_source(RCC_SYSCLK_SRC_PLLP);

  while (RCC_CFGR_SWS_PLL != rcc_sysclk_get_source())
    ;

  rcc_hsi_disable();

  rcc_apb1_set_prescaler(RCC_APB1_DIV_4);
  rcc_apb2_set_prescaler(RCC_APB2_DIV_4);
}
