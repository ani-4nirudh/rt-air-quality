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
 * Configuring the PLL clock source so that SYSCLK runs at 180 MHz
 */

static void rcc_pll_config(void) {

  // Disable the PLL before configuration. i.e. PLLON and PLLRDY are set to 0
  RCC->CR &= ~(RCC_CR_PLLON);
  while (RCC->CR & RCC_CR_PLLRDY)
    ;

  /**
   * We want to run the CPU at 180 MHz. HSE -> PLL -> SYSCLK -> HCLK -> APB1 & APB2 clocks (Sources are left to right)
   * Because HSE is a 8 MHz crystal, and for using PLL as a clock source, 2 MHz is minimum recommended frequency for VCO input to avoid jitter.
   * VCO output frquency is limited to 432 MHz.
   */
  uint32_t PLLM = 4U; // Lower means higher VCO_input frequency
  uint32_t PLLN = 180U;
  uint32_t VCO_input = RCC_HSE_MHZ / PLLM;         // 2 MHz
  uint32_t VCO_output = VCO_input * PLLN;          // 360 MHz < 432 MHz
  uint32_t PLLP = VCO_output / RCC_MAX_SYSCLK_MHZ; // 2 (Permissible values: 2, 4, 6, 8)

  // Set the PLLP Register
  // This conversion is necessary because 2 is encoded for the binary number 00
  // In PLLM and PLLN, the binary corresponds to the calculated unsigned integer.
  PLLP = (PLLP / 2U) - 1U;
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
 * - Configure the PLL as the clock source
 * - Enable the PLL
 * - Wait for PLL clock to stabilise
 * - Disable the HSI
 * - Set the peripheral clock
 */
void rcc_init(void) {

  // Set up the wait states for the flash memory
  flash_config_wait_states(RCC_MAX_SYSCLK_MHZ);

  // Enable HSE clock X1 on the ST-Link Debugger
  rcc_hse_enable();

  // Set up HSE clock as PLL source
  rcc_pll_source(RCC_PLLCFGR_PLLSRC_HSE);

  // Set up the PLL clock source params
  rcc_pll_config();

  // Enable the PLL
  rcc_pll_enable();

  // Set the prescaler for the AHB bus
  rcc_ahb_set_prescaler(RCC_SYSCLK_DIV_1);

  // Switch the system clock source to PLLP
  rcc_sysclk_set_source(RCC_SYSCLK_SRC_PLLP);

  // Wait until the clock source is switched to PLLP
  while (RCC_CFGR_SWS_PLL != rcc_sysclk_get_source())
    ;

  // Disable the HSI clock to save power
  rcc_hsi_disable();

  // Set prescaler values for the APB1 and APB2 peripheral buses
  rcc_apb1_set_prescaler(RCC_APB1_DIV_4);
  rcc_apb2_set_prescaler(RCC_APB2_DIV_4);
}

uint32_t rcc_get_sysclk_freq(void) {
  uint32_t sysclk_freq_mhz = 0U;

  // Initialise the variables that will save values of respective bits from the RCC_PLLCFGR register
  uint32_t PLLP = 0U;
  uint32_t pllp_bits = 0U;
  uint32_t PLLM = 0U;
  uint32_t PLLN = 0U;

  // Verify which clock source is used for system clock
  switch (RCC->CFGR & RCC_CFGR_SWS) {
  case RCC_CFGR_SWS_HSI:
    sysclk_freq_mhz = RCC_HSI_FREQ;
    break;

  case RCC_CFGR_SWS_HSE:
    sysclk_freq_mhz = RCC_HSE_FREQ;
    break;

  case RCC_CFGR_SWS_PLL:

    /**
      * Convert the PLLP factor as it is encoded [(0bxx + 1) * 2]
      * 0b00 -> 0 -> 1 -> 2
      * 0b01 -> 1 -> 2 -> 4
      * 0b10 -> 2 -> 3 -> 6
      * 0b11 -> 3 -> 4 -> 8
      */
    pllp_bits = (RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos;
    PLLP = (pllp_bits + 1U) * 2U;
    PLLM = (RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos;
    PLLN = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;

    // If the PLL source is HSE
    if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE) {
      sysclk_freq_mhz = (RCC_HSE_FREQ * PLLN) / (PLLP * PLLM);
    }

    // If the PLL source is HSI
    if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI) {
      sysclk_freq_mhz = (RCC_HSI_FREQ * PLLN) / (PLLP * PLLM);
    }

    break;

  default:
    sysclk_freq_mhz = 0U;
    break;
  }
  return sysclk_freq_mhz;
}

uint32_t rcc_get_hclk_freq(void) {
  uint32_t sysclk_freq_mhz = rcc_get_sysclk_freq();
  uint16_t ahb_prescaler = 1U;
  switch (RCC->CFGR & RCC_CFGR_HPRE) {
  case RCC_CFGR_HPRE_DIV1:
    ahb_prescaler = 1U;
    break;
  case RCC_CFGR_HPRE_DIV2:
    ahb_prescaler = 2U;
    break;
  case RCC_CFGR_HPRE_DIV4:
    ahb_prescaler = 4U;
    break;
  case RCC_CFGR_HPRE_DIV8:
    ahb_prescaler = 8U;
    break;
  case RCC_CFGR_HPRE_DIV16:
    ahb_prescaler = 16U;
    break;
  case RCC_CFGR_HPRE_DIV64:
    ahb_prescaler = 64U;
    break;
  case RCC_CFGR_HPRE_DIV128:
    ahb_prescaler = 128U;
    break;
  case RCC_CFGR_HPRE_DIV256:
    ahb_prescaler = 256U;
    break;
  case RCC_CFGR_HPRE_DIV512:
    ahb_prescaler = 512U;
    break;

  default:
    return 0U;
  }

  return (sysclk_freq_mhz / ahb_prescaler);
}

uint32_t rcc_get_pclk1_freq(void) {
  uint32_t hwclk_mhz = rcc_get_hclk_freq();
  uint8_t apb1_prescaler = 1U;
  switch (RCC->CFGR & RCC_CFGR_PPRE1) {
  case RCC_CFGR_PPRE1_DIV1:
    apb1_prescaler = 1U;
    break;
  case RCC_CFGR_PPRE1_DIV2:
    apb1_prescaler = 2U;
    break;
  case RCC_CFGR_PPRE1_DIV4:
    apb1_prescaler = 4U;
    break;
  case RCC_CFGR_PPRE1_DIV8:
    apb1_prescaler = 8U;
    break;
  case RCC_CFGR_PPRE1_DIV16:
    apb1_prescaler = 16U;
    break;

  default:
    return 0U;
  }

  return (hwclk_mhz / apb1_prescaler);
}

uint32_t rcc_get_pclk2_freq(void) {
  uint32_t hwclk_mhz = rcc_get_hclk_freq();
  uint8_t apb2_prescaler = 1U;
  switch (RCC->CFGR & RCC_CFGR_PPRE2) {
  case RCC_CFGR_PPRE2_DIV1:
    apb2_prescaler = 1U;
    break;
  case RCC_CFGR_PPRE2_DIV2:
    apb2_prescaler = 2U;
    break;
  case RCC_CFGR_PPRE2_DIV4:
    apb2_prescaler = 4U;
    break;
  case RCC_CFGR_PPRE2_DIV8:
    apb2_prescaler = 8U;
    break;
  case RCC_CFGR_PPRE2_DIV16:
    apb2_prescaler = 16U;
    break;

  default:
    return 0U;
  }

  return (hwclk_mhz / apb2_prescaler);
}
