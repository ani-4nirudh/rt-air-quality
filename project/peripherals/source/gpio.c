/**
 * @file gpio.c
 * @brief Source file to initialise GPIO pins
 *
 * @author Anirudh Singh
 * @date 6th October 2026
 */

#include "gpio.h"
#include "gpio_defs.h"
#include "stm32f4xx.h"

static void GPIO_set_pin_mode(GPIO_pin_config_s *gpio) {
  // Clear the 2 bit field by using an inverted mask
  gpio->port->MODER &= ~(3U << (2U * gpio->pin));

  // Set the pin mode except for input pins
  if (gpio->pin_mode != GPIO_MODE_INPUT) {
    gpio->port->MODER |= gpio->pin_mode << (2U * gpio->pin);
  }
}

static void GPIO_set_output_type(GPIO_pin_config_s *gpio) {
  // Clear the bit for a push-pull type
  gpio->port->OTYPER &= ~(1U << gpio->pin);

  // Set the OPEN_DRAIN output type
  if (gpio->pin_output_type == GPIO_OUTPUT_OPEN_DRAIN) {
    gpio->port->OTYPER |= gpio->pin_output_type << gpio->pin;
  }
}

static void GPIO_set_output_speed(GPIO_pin_config_s *gpio) {
  // Clear the bit field by setting it to low speed
  gpio->port->OSPEEDR &= ~(3U << (2U * gpio->pin));

  if (gpio->pin_mode == GPIO_MODE_OUTPUT) {
    gpio->port->OSPEEDR |= (gpio->pin_output_speed << (2U * gpio->pin));
  }
}

static void GPIO_set_pull_resistor(GPIO_pin_config_s *gpio) {
  // Clear the bit field to set no pull-up or pull-down resistor
  gpio->port->PUPDR &= ~(3U << (2 * gpio->pin));

  // Set the pull-up or pull-down resistor
  gpio->port->PUPDR |= (gpio->pin_pull << (2 * gpio->pin));
}

static void GPIO_set_alt_fn(GPIO_pin_config_s *gpio) {
  if (gpio->pin <= GPIO_PIN_7) {
    // Clear the bit field
    gpio->port->AFR[0] &= ~(15U << (4 * gpio->pin));

    // Set the alternate function
    gpio->port->AFR[0] |= (gpio->pin_alt_fn << (4 * gpio->pin));
  } else {
    // Clear the bit field and subtract 8 as bit field for pin 8 is [3:0]
    gpio->port->AFR[1] &= ~(15U << (4 * (gpio->pin - 8U)));

    // Set the alternate function
    gpio->port->AFR[1] |= (gpio->pin_alt_fn << (4 * (gpio->pin - 8U)));
  }
}

static void GPIO_set_config(GPIO_pin_config_s *gpio) {
  GPIO_set_pin_mode(gpio);
  GPIO_set_output_type(gpio);
  GPIO_set_output_speed(gpio);
  GPIO_set_pull_resistor(gpio);

  if ((gpio->pin_mode == GPIO_MODE_AF) && (gpio->pin_alt_fn != GPIO_AF_NONE)) {
    GPIO_set_alt_fn(gpio);
  }
}

void GPIO_init(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIO_set_config(&USER_LED);
}

void GPIO_set_pin(GPIO_pin_config_s *gpio) {
  gpio->port->BSRR = (1U << gpio->pin); // Set the bit inside the BSRR BSy bit field
}

void GPIO_reset_pin(GPIO_pin_config_s *gpio) {
  gpio->port->BSRR = (1U << (gpio->pin + 16U)); // Set the bit inside the BSRR BRy bit field
}

gpio_pin_state_e GPIO_read_pin(GPIO_pin_config_s *gpio) {
  gpio_pin_state_e pin_state;
  pin_state = (gpio->port->IDR & (1U << gpio->pin)); // Mask the register
  if (pin_state != 0) {                              // If the value after the masking is non-zero then that means the bit is set
    return GPIO_PIN_SET;
  }
  return GPIO_PIN_RESET;
}

void GPIO_toggle_pin(GPIO_pin_config_s *gpio) {
  gpio_pin_state_e pin_state = GPIO_read_pin(gpio);
  if (pin_state == GPIO_PIN_SET) {
    GPIO_reset_pin(gpio);
  } else {
    GPIO_set_pin(gpio);
  }
}
