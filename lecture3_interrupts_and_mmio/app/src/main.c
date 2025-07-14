#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/f1/rcc.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

// LED is at PC13 according to
// https://stm32-base.org/boards/STM32F103C8T6-STM32-Smart-V2.0#User-LED
constexpr uint32_t LED_PORT = GPIOC;
constexpr uint16_t LED_PIN = GPIO13;

#define SYSTICK_FREQ (uint32_t)1'000
#define CPU_FREQ (uint32_t)72'000'000

#define TOGGLE_RATE_HZ 2
constexpr uint64_t TOGGLE_DELAY = (SYSTICK_FREQ / TOGGLE_RATE_HZ);

// why 64 bits? because the ticks value will increment once every
// millisecond. if we wnt with a 32 bit number, it will overflow in 49.7
// days, exactly like it does in Arduino's millis(). with a 64 bit number,
// it will overflow in 213503982334.6 days. If your device manages to last
// that long, the human race will likely no longer exist. However, this is
// not a magic bullet either. We are dealing with a 32 bit architecture and
// on our chip we don't have a 64bit add instruction that can happen withing
// one cycle. We need to do all operations with this value with interrupts
// disabled.
volatile uint64_t ticks = 0;
void sys_tick_handler(void) {
  // todo: disable interrupts when incrementing ticks
  ticks++;
}

// Returns the number of ticks elapsed since startup.
// 1 tick approximately equals 1 millisecond.
static uint64_t get_ticks(void) { return ticks; }

static void rcc_setup(void) {
  // we use the high speed external clock at 72MHz
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE12_72MHZ]);
  rcc_periph_clock_enable(RCC_GPIOC);
}

static void gpio_setup(void) {
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                LED_PIN);
  gpio_set(LED_PORT, LED_PIN);
}

static void systick_setup(void) {
  systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();
}

int main(void) {
  rcc_setup();
  gpio_setup();
  systick_setup();

  uint64_t last_toggled_time = get_ticks();

  while (true) {
    if (get_ticks() - last_toggled_time > TOGGLE_DELAY) {
      gpio_toggle(LED_PORT, LED_PIN);
      last_toggled_time = get_ticks();
    }

    // We are free to do useful work here, wheeee!
  }

  // never return
  return 0;
}
