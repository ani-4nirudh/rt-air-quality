/**
 * Application entry point
 * Pure CMSIS + FreeRTOS (no HAL)
 */
#include <stdint.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOSTasks.h"
#include "rcc.h"
#include "stm32f4xx.h"

#define LED_PIN 5U

static volatile uint8_t stage = 0;

/**
 * Start Task Prototype
 */
// static void startup(void);
static void led_init(void);
static void blink(void);

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
   * (gdb) target extended-remote localhost:3333
   * (gdb) monitor reset halt
   * (gdb) load
   * (gdb) break main
   * (gdb) continue
   */

  /**
   * Update the 'SystemCoreClock' variable required by FreeRTOS
   */
  SystemCoreClockUpdate();

  led_init();

  // Create the task scheduler
  // startup();
  blink();

  // Start the FreeRTOS Scheduler
  vTaskStartScheduler();

  /* Loop forever */
  while (1) {
  }
}

static void led_init(void) {

  /**
   * Setup the LED pins
   */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER &= ~(3U << (LED_PIN * 2U));
  GPIOA->MODER |= (1U << (LED_PIN * 2U));
}

static void led_toggle(void) {
  GPIOA->ODR ^= (1U << LED_PIN);
}

void blink_task(void *argument) {
  (void)argument;

  while (1) {
    led_toggle();
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  vTaskDelete(NULL);
}

static void blink(void) {
  configASSERT(pdPASS == xTaskCreate(blink_task, "LED Blinking Task", STARTUP_TASK_STACK_SIZE, NULL, (STARTUP_TASK_PRIORITY - 2), NULL))
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
