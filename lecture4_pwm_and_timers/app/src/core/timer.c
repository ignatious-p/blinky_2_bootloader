#include "core/timer.h"
#include "libopencm3/stm32/common/timer_common_all.h"
#include "libopencm3/stm32/f1/rcc.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

// freq = system_freq /((prescaler -1)*(arr-1))

constexpr uint32_t PRESCALER = 1;
constexpr uint32_t OC_VALUE = 200;
constexpr uint32_t ARR_VALUE = 1000;

// Setup timer TIM1 for PWM
void timer_setup(void) {

  uint32_t timer = TIM1;

  // Enable Timer 1 and Alternate Function
  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_AFIO);

  // Timer configuration
  // Refer to 15.4.1 TIMx control register 1 (TIMx_CR1) in the STM32F103
  // reference manual.
  // CKD (Clock division) == 0 (no clock division)
  // CMS (Center-aligned mode selection) == center aligned
  // DIR (Direction) == up counting
  timer_set_mode(timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
                 TIM_CR1_DIR_UP);

  // Setup PWM mode
  // Refer 15.4.7 TIMx capture/compare mode register 1 (TIMx_CCMR1) in the
  // STM32F103 reference manual.
  // PWM1 == Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else inactive
  timer_set_oc_mode(timer, TIM_OC1, TIM_OCM_PWM1);
  timer_enable_oc_output(timer, TIM_OC1);

  // Note that for the advanced timers the break functionality must be enabled
  // before the signal will appear at the output, even though break is not being
  // used.
  // Refer to the example in stm32/common/timer_common_all.c
  timer_enable_break_main_output(timer);

  timer_set_oc_value(timer, TIM_OC1, OC_VALUE);

  // Setup frequency and resolution
  // freq = system_freq /((prescaler -1)*(arr-1))
  timer_set_prescaler(timer, PRESCALER - 1);
  timer_set_period(timer, ARR_VALUE - 1);

  // Enable PWM output
  timer_enable_counter(timer);
}

void timer_pwm_set_duty_cycle(float duty_cycle) {
  // duty cycle = (ccr /arr) * 100
  // ccr = arr * (duty cycle / 100)

  float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);

  timer_set_oc_value(TIM1, TIM_OC1, (uint32_t)raw_value);
};