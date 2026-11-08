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

CFLAGS = -fno-omit-frame-pointer -fno-stack-protector -fno-stack-check -fno-unwind-tables
CFLAGS += -fno-asynchronous-unwind-tables -fno-strict-aliasing -fno-common -fno-pic
CFLAGS += -mgeneral-regs-only -mno-red-zone -masm=intel
CFLAGS += -Werror -Wall -O2 -std=c23 -pedantic
CFLAGS += -nostdlib -nostdinc -ffreestanding

ASFLAGS = -w+error=all

################################################### PATHS AND FILES ###################################################

INCLUDE_PATH = -I./include/
SCRIPTS_FOLDER = scripts
BUILD_FOLDER = build

BOOT_BIOS_CSOURCES = $(shell find boot/bios -name "*.c")
KERNEL_ASM_SOURCES = $(shell find kernel -name "*.asm")
KERNEL_CSOURCES = $(shell find kernel -name "*.c")

BOOT_BIOS_HEADERS = $(shell find boot/bios -name "*.h")
NASM_INC_HEADERS = $(shell find include -name "*.inc")
INCLUDE_HEADERS = $(shell find include -name "*.h")
KERNEL_HEADERS = $(shell find kernel -name "*.h")

BOOT_BIOS_OBJS = $(addprefix $(BUILD_FOLDER)/, ${BOOT_BIOS_CSOURCES:.c=.o})
KERNEL_OBJS = $(addprefix $(BUILD_FOLDER)/, ${KERNEL_ASM_SOURCES:.asm=.o})
KERNEL_OBJS += $(addprefix $(BUILD_FOLDER)/, ${KERNEL_CSOURCES:.c=.o})

ANEKIX_BIOS_IMAGE = $(BUILD_FOLDER)/anekix-bios.img

#################################################### PHONY TARGETS ####################################################

.PHONY: all clean

all: $(ANEKIX_BIOS_IMAGE)

clean:
	rm -rf $(BUILD_FOLDER)

#################################################### IMAGE TARGETS ####################################################

$(ANEKIX_BIOS_IMAGE): $(BUILD_FOLDER)/boot/bios/bootloader.bin $(BUILD_FOLDER)/kernel/anekix.elf
	mkdir -p $(@D)
	cat $^ > $@
	truncate -s %512 $@

################################################## BOOT BIOS TARGETS ##################################################

$(BUILD_FOLDER)/boot/bios/bootloader.bin: boot/bios/bootloader.asm $$(@D)/stage2.bin $(NASM_INC_HEADERS)
	mkdir -p $(@D)
	$(AS) $(INCLUDE_PATH) $(ASFLAGS) -dBOOTLOADER_STAGE2_BIN=$(word 2, $^) -f bin -o $@ $<

$(BUILD_FOLDER)/boot/bios/stage2.bin: $(BOOT_BIOS_OBJS) $(SCRIPTS_FOLDER)/stage2.ld
	mkdir -p $(@D)
	$(LD) -T $(SCRIPTS_FOLDER)/stage2.ld -o $@ $(BOOT_BIOS_OBJS)

$(BUILD_FOLDER)/boot/bios/%.o: boot/bios/%.c $(BOOT_BIOS_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

#################################################### KERNEL TARGETS ###################################################

$(BUILD_FOLDER)/kernel/anekix.elf: $(KERNEL_OBJS) $(SCRIPTS_FOLDER)/kernel.ld
	mkdir -p $(@D)
	$(LD) -z separate-code -T $(SCRIPTS_FOLDER)/kernel.ld -o $@ $(KERNEL_OBJS)

$(BUILD_FOLDER)/kernel/%.o: kernel/%.asm $(NASM_INC_HEADERS)
	mkdir -p $(@D)
	$(AS) $(INCLUDE_PATH) $(ASFLAGS) -f elf64 -o $@ $<

$(BUILD_FOLDER)/kernel/%.o: kernel/%.c $(KERNEL_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -mcmodel=kernel -c $< -o $@

######################################################## QEMU ########################################################

QEMU_CPUS ?= 2
QEMU_MEMORY ?= 128M

QEMU_ACCEL = -accel kvm -accel tcg
QEMU_GRAPHIC = -nographic
QEMU_MACHINE = -M pc -global cpu.la57=off
QEMU_DEBUGCON = -debugcon file:/dev/stdout

QEMU_OPTIONS = $(QEMU_MACHINE) $(QEMU_ACCEL) -smp $(QEMU_CPUS) -m $(QEMU_MEMORY) $(QEMU_GRAPHIC) $(QEMU_DEBUGCON)
QEMU_BIOS_BOOT_DRIVE = -drive media=disk,index=0,format=raw,if=ide

qemu-bios: $(ANEKIX_BIOS_IMAGE)
	$(QEMU) $(QEMU_OPTIONS) $(QEMU_BIOS_BOOT_DRIVE),file=$<