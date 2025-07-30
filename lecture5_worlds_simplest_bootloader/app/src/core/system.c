#include "core/system.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/rcc.h>

// why 64 bits? because the ticks value will increment once every
// millisecond. if we wnt with a 32 bit number, it will overflow in 49.7
// days, exactly like it does in Arduino's millis(). with a 64 bit number,
// it will overflow in 213503982334.6 days. If your device manages to last
// that long, the human race will likely no longer exist. However, this is
// not a magic bullet either. We are dealing with a 32 bit architecture and
// on our chip we don't have a 64bit add instruction that can happen withing
// one cycle. We need to do all operations with this value with interrupts
// disabled.
static volatile uint64_t ticks = 0;
void sys_tick_handler(void) {
  // todo: disable interrupts when incrementing ticks
  ticks++;
}

static void rcc_setup(void) {
  // we use the high speed external clock at 72MHz
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE12_72MHZ]);
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);

}

static void systick_setup() {
  systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();
}

// Returns the number of ticks elapsed since startup.
// 1 tick approximately equals 1 millisecond.
uint64_t system_get_ticks(void) { return ticks; }

// setup systick
void system_setup(void) {
  rcc_setup();
  systick_setup();
};
