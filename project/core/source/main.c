/**
 * Application entry point
 * Pure CMSIS + FreeRTOS (no HAL)
 */
#include <stdint.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOSTasks.h"
#include "gpio.h"
#include "gpio_defs.h"
#include "rcc.h"
#include "stm32f4xx.h"

static void vStartBlinkingTask(void);

int main(void) {
  /**
   * Initial RCC configuration test
   */
  uint32_t system_clock = 0;
  uint32_t hclk = 0;
  uint32_t apb1_clock = 0;
  uint32_t apb2_clock = 0;

  rcc_init();
  system_clock = rcc_get_sysclk_freq();
  hclk = rcc_get_hclk_freq();
  apb1_clock = rcc_get_pclk1_freq();
  apb2_clock = rcc_get_pclk2_freq();

  (void)system_clock;
  (void)hclk;
  (void)apb1_clock;
  (void)apb2_clock;

  /**
   * Update the 'SystemCoreClock' variable required by FreeRTOS
   */
  SystemCoreClockUpdate();

  GPIO_init();
  vStartBlinkingTask();

  // Start the FreeRTOS Scheduler
  vTaskStartScheduler();

  /* Loop forever */
  while (1) {
  }
}

static void vBlinkingTask(void *param) {
  (void)param;
  while (1) {
    GPIO_toggle_pin(&USER_LED);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static void vStartBlinkingTask(void) {
  configASSERT(pdPASS == xTaskCreate(vBlinkingTask, "Blinking Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL))
}

// /**
//  * Creates the startup task used for FreeRTOS initialisations on startup
//  */
// static void startup_task(void *param) {
//   (void)param;
//
//   // Delete startup task after creating other tasks
//   vTaskDelete(NULL);
// }
//
// /**
//  * Create startup task
//  */
// static void startup(void) {
//   configASSERT(pdPASS == xTaskCreate(startup_task, "Startup Task", STARTUP_TASK_STACK_SIZE, NULL, STARTUP_TASK_PRIORITY, NULL));
// }
