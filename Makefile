# Copyright (c) 2026 Rafael Rodrigues Sanches.
# SPDX-License-Identifier: BSD-3-Clause

.SECONDEXPANSION:

######################################################## TOOLS ########################################################

TOOL_PREFIX = x86_64-elf
QEMU = qemu-system-x86_64

CC = $(TOOL_PREFIX)-gcc
LD = $(TOOL_PREFIX)-ld
AS = nasm

######################################################## FLAGS ########################################################

CFLAGS = -fno-omit-frame-pointer -fno-stack-protector -fno-stack-check
CFLAGS += -mgeneral-regs-only -mno-red-zone -masm=intel
CFLAGS += -fno-strict-aliasing -fno-common -fno-pic
CFLAGS += -Werror -Wall -O2 -std=c11 -pedantic
CFLAGS += -nostdlib -nostdinc -ffreestanding

################################################### PATHS AND FILES ###################################################

INCLUDE_PATH = -I./include/
SCRIPTS_FOLDER = scripts
BUILD_FOLDER = build

BOOT_BIOS_CSOURCES = $(shell find boot/bios -name "*.c")
KERNEL_CSOURCES = $(shell find kernel -name "*.c")

BOOT_BIOS_HEADERS = $(shell find boot/bios -name "*.h")
NASM_INC_HEADERS = $(shell find include -name "*.inc")
INCLUDE_HEADERS = $(shell find include -name "*.h")
KERNEL_HEADERS = $(shell find kernel -name "*.h")

BOOT_BIOS_OBJS = $(addprefix $(BUILD_FOLDER)/, ${BOOT_BIOS_CSOURCES:.c=.o})
KERNEL_OBJS = $(addprefix $(BUILD_FOLDER)/, ${KERNEL_CSOURCES:.c=.o})

################################################## BOOT BIOS TARGETS ##################################################

$(BUILD_FOLDER)/boot/bios/bootloader.bin: boot/bios/bootloader.asm $$(@D)/load_kernel.bin $(NASM_INC_HEADERS)
	mkdir -p $(@D)
	$(AS) $(INCLUDE_PATH) -dBOOTLOADER_STAGE2_BIN=$(word 2, $^) -fbin -o $@ $<

$(BUILD_FOLDER)/boot/bios/load_kernel.bin: $(BOOT_BIOS_OBJS)
	mkdir -p $(@D)
	$(LD) -m elf_x86_64 -T $(SCRIPTS_FOLDER)/load_kernel.ld -o $@ $^

$(BUILD_FOLDER)/boot/bios/%.o: boot/bios/%.c $(BOOT_BIOS_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_FOLDER)