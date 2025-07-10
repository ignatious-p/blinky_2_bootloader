#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

// LED is at PC13 according to
// https://stm32-base.org/boards/STM32F103C8T6-STM32-Smart-V2.0#User-LED
constexpr uint32_t LED_PORT = GPIOC;
constexpr uint16_t LED_PIN = GPIO13;

constexpr uint32_t LITTLE_BIT = 1000000;

static void rcc_setup(void) {
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);
  rcc_periph_clock_enable(RCC_GPIOC);
}

static void gpio_setup(void) {
  gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                LED_PIN);
  gpio_set(LED_PORT, LED_PIN);
}

int main(void) {

  rcc_setup();
  gpio_setup();

  // wait for a little bit, lolol
  while (1) {
    for (uint32_t i = 0; i < LITTLE_BIT; i++) {

      // Ewww, we are busy waiting!
      __asm__("nop");
    }
    gpio_toggle(LED_PORT, LED_PIN);
  }

  // never return
  return 0;
}
