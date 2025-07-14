/*
    Adopted from https://github.com/libopencm3/libopencm3-miniblink/blob/master/ld.stm32.basic

    Refer to the STM32F108C8 datasheet, Section 4. "Memory mapping" for the start addresses.

    Also, refer to https://blog.thea.codes/the-most-thoroughly-commented-linker-script/ for more information on linker scripts.

*/


FLASH_SIZE = 0x10000;      /* 64kB */
NVM_SIZE = 0x400;          /* 1kB */


MEMORY
{
	/* this is the original file content*/
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K


	/* I want to split the flash into an NVM area and the flash area*/
	/* rom (rx) : ORIGIN = 0x08000000, LENGTH = FLASH_SIZE - NVM_SIZE
	nvm (rw) : ORIGIN = 0x08000000 + FLASH_SIZE, LENGTH = NVM_SIZE
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K */
}

/* Enforce emmition of the vector table. */
EXTERN (vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define sections. */
SECTIONS
{
	.text : {
		*(.vectors)	/* Vector table */
		*(.text*)	/* Program code */
		. = ALIGN(4);
		*(.rodata*)	/* Read-only data */
		. = ALIGN(4);
	} >rom

	/* C++ Static constructors/destructors, also used for __attribute__
	 * ((constructor)) and the likes */
	.preinit_array : {
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} >rom
	.init_array : {
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
	} >rom
	.fini_array : {
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} >rom

	/*
	 * Another section used by C++ stuff, appears when using newlib with
	 * 64bit (long long) printf support
	 */
	.ARM.extab : {
		*(.ARM.extab*)
	} >rom
	.ARM.exidx : {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} >rom

	. = ALIGN(4);
	_etext = .;

	/* ram, but not cleared on reset. eg:  boot/app comms */
	.noinit (NOLOAD) : {
		*(.noinit)
	} >ram
	. = ALIGN(4);

	.data : {
		_data = .;
		*(.data*)	/* Read-write initialized data */
		. = ALIGN(4);
		*(.ramfunctions) /* functions we need/want to run from ram */
		_edata = .;
	} >ram AT >rom
	_data_loadaddr = LOADADDR(.data);

	.bss : {
		*(.bss*)	/* Read-write zero initialized data */
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} >ram

	/*
	 * The .eh_frame section appears to be used for C++ exception handling.
	 * You may need to fix this if you're using C++.
	 */
	/DISCARD/ : { *(.eh_frame) }

	. = ALIGN(4);
	end = .;
}

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));

