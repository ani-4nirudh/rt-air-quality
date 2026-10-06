/**
 * @file gpio.h 
 * @brief Header file to define GPIO pins
 *
 * @author Anirudh Singh
 * @date 6th October 2026
 */

#ifndef INC_GPIO_H
#define INC_GPIO_H

#include "stm32f4xx.h"

/**
 * There are 16 GPIO pins available for configuration
 */
typedef enum {
  GPIO_PIN_0 = 0,
  GPIO_PIN_1,
  GPIO_PIN_2,
  GPIO_PIN_3,
  GPIO_PIN_4,
  GPIO_PIN_5,
  GPIO_PIN_6,
  GPIO_PIN_7,
  GPIO_PIN_8,
  GPIO_PIN_9,
  GPIO_PIN_10,
  GPIO_PIN_11,
  GPIO_PIN_12,
  GPIO_PIN_13,
  GPIO_PIN_14,
  GPIO_PIN_15,
} gpio_pin_num_e;

/**
 * Define the pin mode type
 */
typedef enum {
  GPIO_MODE_INPUT = 0,
  GPIO_MODE_OUTPUT,
  GPIO_MODE_AF,
  GPIO_MODE_ANALOG,
} gpio_pin_mode_e;

/**
 * Define the type of output pin
 */
typedef enum {
  GPIO_OUTPUT_PUSH_PULL = 0,
  GPIO_OUTPUT_OPEN_DRAIN,
} gpio_output_type_e;

/**
 * Define the speed of output pin
 */
typedef enum {
  GPIO_OUTPUT_SPEED_LOW = 0,
  GPIO_OUTPUT_SPEED_MEDIUM,
  GPIO_OUTPUT_SPEED_FAST,
  GPIO_OUTPUT_SPEED_HIGH,
} gpio_output_speed_e;

/**
 * Define whether a pin should activate its pull-up or pull-down resistor
 */
typedef enum {
  GPIO_PULL_NONE = 0,
  GPIO_PULL_UP,
  GPIO_PULL_DOWN,
} gpio_pin_pull_e;

/**
 * Select the type of alternate function to bind to the GPIO pin
 */
typedef enum {
  GPIO_AF_0 = 0,
  GPIO_AF_1,
  GPIO_AF_2,
  GPIO_AF_3,
  GPIO_AF_4,
  GPIO_AF_5,
  GPIO_AF_6,
  GPIO_AF_7,
  GPIO_AF_8,
  GPIO_AF_9,
  GPIO_AF_10,
  GPIO_AF_11,
  GPIO_AF_12,
  GPIO_AF_13,
  GPIO_AF_14,
  GPIO_AF_15,
  GPIO_AF_NONE,
} gpio_alt_fn_e;

/**
 * Set pin state HIGH or LOW (RESET)
 */
typedef enum {
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET,
} gpio_pin_state_e;

/**
 * Creating a data structure type for easier pin configuration
 */
typedef struct {
  GPIO_TypeDef *port;
  gpio_pin_num_e pin;
  gpio_pin_mode_e pin_mode;
  gpio_output_type_e pin_output_type;
  gpio_output_speed_e pin_output_speed;
  gpio_pin_pull_e pin_pull;
  gpio_alt_fn_e pin_alt_fn;
} GPIO_pin_config_s;

/**
 * @brief Initiaise the GPIO pins
 */
void GPIO_init(void);

/**
 * @brief Reset the pin state
 */
void GPIO_reset_pin(GPIO_pin_config_s *gpio);

/**
 * @brief Set the GPIO pin
 */
void GPIO_set_pin(GPIO_pin_config_s *gpio);

/**
 * @brief Check the pin state
 * @return Pin state
 */
gpio_pin_state_e GPIO_read_pin(GPIO_pin_config_s *gpio);

/**
 * @brief Toggle the desired pin (for e.g. the onboard LED)
 */
void GPIO_toggle_pin(GPIO_pin_config_s *gpio);

#endif // !INC_GPIO_H
