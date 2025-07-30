#ifndef SYSTEM_H
#define SYSTEM_H

#include "common_defines.h"

#define CPU_FREQ (uint32_t)(64'000'000)
#define SYSTICK_FREQ (uint32_t)(1'000)

// setup systick
void system_setup(void);

// get ticks since reset
uint64_t system_get_ticks(void);

#endif /* SYSTEM_H */
