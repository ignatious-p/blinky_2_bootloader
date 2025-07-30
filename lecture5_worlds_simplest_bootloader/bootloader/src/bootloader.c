#include "common_defines.h"
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/memorymap.h>

// bootlaoder must fit within 8192 bits
#define BOOTLOADER_SIZE (0x2000U)

#define MAIN_APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)

static void jump_to_main(void) {

  // // My code differs from that in the videos as there is already a predefined
  // // type for this that i can just use

  // // MAIN_APP_START_ADDRESS is the stack pointer
  // // the reset vector pointer is 4 bytes more on a 32-bit arch (exactly one
  // // SYSTEM_POINTER_SIZE more)
  // // I know I can array increment the pointer, but that it is naughty and
  // feels
  // // dirty
  // vector_table_entry_t *reset_vector_entry =
  //     (vector_table_entry_t *)(MAIN_APP_START_ADDRESS +
  //                              sizeof(vector_table_entry_t *));

  // // Gnarly code, I agree, but all it does is dereference reset_vector_entry
  // // pointer and cast it's contents as a vector_table_entry_t pointer
  // vector_table_entry_t reset_vector =
  //     (vector_table_entry_t)(*reset_vector_entry);

  // reset_vector();

  // Or, I could do this.
  vector_table_t *main_vector_table =
      (vector_table_t *)(MAIN_APP_START_ADDRESS);
  main_vector_table->reset();
}

int main(void) {

  jump_to_main();

  // never return
  while (true) {
    __asm__("nop");
  };

  return 0;
};