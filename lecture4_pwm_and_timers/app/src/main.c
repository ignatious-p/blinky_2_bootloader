#include "common_defines.h"
#include "core/system.h"
#include "core/timer.h"
#include "libopencm3/stm32/f1/gpio.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

// LED is at PC13 according to
// https://stm32-base.org/boards/STM32F103C8T6-STM32-Smart-V2.0#User-LED

// According to the datasheet, PC13 to PC15 "must not be used as a current
// source (e.g. to drive a LED)" as they can supply only 3mA. But the smart
// chinese designer who made my board realised that this means there is no need
// for a current limiting resistor and went with it anyway!

// Unfortunately, there is no hardware timer attached to this, therefore I
// cannot do PWM on it. No worries though, I will connect an external LED and
// resistor instead.
#define LED_PORT (uint32_t)GPIOC
#define LED_PIN (uint16_t)GPIO13

// This is an externally attached LED that is there as the inbuilt one cannot do
// PWM (at least not without requiring some Dmitry Grinberg-esque hackery).
#define MY_LED_PORT (uint32_t)GPIOA
#define MY_LED_PIN (uint16_t)GPIO15

constexpr uint64_t INCREMENT_MILLIS = 10;

static void gpio_setup(void) {
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, LED_PIN);
}

int main(void) {
  system_setup();
  timer_setup();
  gpio_setup();

  uint64_t start_time = system_get_ticks();
  float duty_cycle = 0.0f;

  while (true) {
    if (system_get_ticks() - start_time > INCREMENT_MILLIS) {
      // increment the duty cycle gradually and reset it once it is maxed out
      duty_cycle += 1.0f;
      if (duty_cycle >= 100.0f) {
        duty_cycle = 0.0f;
      }
      timer_pwm_set_duty_cycle(duty_cycle);
      start_time = system_get_ticks();
    }

    // We are free to do useful work here, wheeee!
  }

  // never return
  return 0;
}
