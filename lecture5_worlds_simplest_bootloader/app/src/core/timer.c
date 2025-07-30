#include "core/timer.h"
#include "libopencm3/stm32/common/timer_common_all.h"
#include "libopencm3/stm32/f1/rcc.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

// freq = system_freq /((prescaler -1)*(arr-1))
#define PRESCALER (72)
#define ARR_VALUE (1000)

// Setup timer TIM2 for PWM
void timer_setup(void) {
  // PA15 is connected to TIM2_CH1
  rcc_periph_clock_enable(RCC_TIM2);

  // Timer configuration
  // Refer to 15.4.1 TIMx control register 1 (TIMx_CR1) in the STM32F103
  // reference manual.
  // CKD (Clock division) == 0 (no clock division)
  // CMS (Center-aligned mode selection) == edge aligned
  // DIR (Direction) == up counting
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  // Setup PWM mode
  // Refer 15.4.7 TIMx capture/compare mode register 1 (TIMx_CCMR1) in the
  // STM32F103 reference manual.
  // PWM1 == Channel 1 is active as long as TIMx_CNT < TIMx_CCR1 else inactive
  timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);

  // todo: find what this does
  // timer_enable_break_main_output(TIM2);

  // set the duty cycle of the PWM to be zero 
  timer_set_oc_value(TIM2, TIM_OC1, 0);

  // todo: 
  timer_enable_oc_output(TIM2, TIM_OC1);
  
  // Setup frequency and resolution
  timer_set_prescaler(TIM2, PRESCALER - 1);
  timer_set_period(TIM2, ARR_VALUE - 1);
  
  // Enable PWM output
  timer_enable_counter(TIM2);
}

void timer_pwm_set_duty_cycle(float duty_cycle) {
  // duty cycle = (ccr /arr) * 100
  // ccr = arr * (duty cycle / 100)

  const float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);

  timer_set_oc_value(TIM2, TIM_OC1, (uint32_t)raw_value);
};