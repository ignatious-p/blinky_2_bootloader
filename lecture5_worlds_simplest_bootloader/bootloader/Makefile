# Based on https://github.com/libopencm3/libopencm3-template/blob/master/my-project/Makefile

PROJECT = bootloader

SHARED_DIR = inc
CFILES = src/bootloader.c  

# I want to modify the linker script at a later date 
DEVICE =
LDSCRIPT = linkerscript.ld

OPENCM3_LIB = opencm3_stm32f1
OPENCM3_DEFS = -DSTM32F1
ARCH_FLAGS = -mthumb -mcpu=cortex-m3 -msoft-float

# INCLUDES = inc
BUILD_DIR = build
OPT = -Og
CSTD = -std=c2x
OOCD_INTERFACE = jlink
OOCD_TARGET = stm32f1x

VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../../../libopencm3

# include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../../rules.mk
# include $(OPENCM3_DIR)/mk/genlink-rules.mk

# This bit is a janky workaround to run the python padding script after running make.
# #build system woes
%.bin: %.elf
	@printf "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -O binary  $< $@
	$(Q)python3 pad_bootloader.py

