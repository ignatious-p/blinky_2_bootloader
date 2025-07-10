# Based on https://github.com/libopencm3/libopencm3-template/blob/master/my-project/Makefile

PROJECT = lecture3

SHARED_DIR = ../my-common-code
CFILES = app/src/main.c

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
OPENCM3_DIR=../../libopencm3

# include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
# include $(OPENCM3_DIR)/mk/genlink-rules.mk