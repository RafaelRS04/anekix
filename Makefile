# Copyright (c) 2026 Rafael Rodrigues Sanches.
# SPDX-License-Identifier: BSD-3-Clause

.SECONDEXPANSION:

######################################################## TOOLS ########################################################

TOOL_PREFIX = x86_64-elf
QEMU = qemu-system-x86_64

CC = $(TOOL_PREFIX)-gcc
LD = $(TOOL_PREFIX)-ld
AS = nasm

EFI_OBJCOPY = objcopy

######################################################## FLAGS ########################################################

CFLAGS = -fno-omit-frame-pointer -fno-stack-protector -fno-stack-check -fno-unwind-tables
CFLAGS += -fno-asynchronous-unwind-tables -fno-strict-aliasing -fno-common -fno-pic
CFLAGS += -mgeneral-regs-only -mno-red-zone -masm=intel
CFLAGS += -Werror -Wall -O2 -std=c23 -pedantic
CFLAGS += -nostdlib -nostdinc -ffreestanding

LDFLAGS = -z separate-code -z noexecstack

EFI_CFLAGS = -fno-stack-protector -fno-stack-check -ffreestanding -fshort-wchar -fpic
EFI_CFLAGS += -mno-red-zone -maccumulate-outgoing-args -DGNU_EFI_USE_MS_ABI
EFI_CFLAGS += -Werror -Wall -O2 -std=c23 -pedantic

EFI_LDFLAGS = -shared -Bsymbolic -z noexecstack --defsym=efi_main=EfiMain
EFI_LIBRARIES = -lgnuefi -lefi

EFI_OBJCOPY_FLAGS = -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym
EFI_OBJCOPY_FLAGS += -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc
EFI_OBJCOPY_FLAGS += --output-target efi-app-x86_64 --subsystem=10

ASFLAGS = -w+error=all

################################################### PATHS AND FILES ###################################################

INCLUDE_PATH = include
SCRIPTS_FOLDER = scripts
BUILD_FOLDER = build

OVMF_IMAGE_PATH = /usr/share/OVMF/OVMF_CODE_4M.fd
EFI_INCLUDE_PATH = /usr/include/efi
EFI_LIB_PATH = /usr/lib

BOOT_BIOS_CSOURCES = $(shell find boot/bios -name "*.c")
BOOT_UEFI_CSOURCES = $(shell find boot/uefi -name "*.c")
KERNEL_ASM_SOURCES = $(shell find kernel -name "*.asm")
KERNEL_CSOURCES = $(shell find kernel -name "*.c")

BOOT_BIOS_HEADERS = $(shell find boot/bios -name "*.h")
BOOT_UEFI_HEADERS = $(shell find boot/uefi -name "*.h")
NASM_INC_HEADERS = $(shell find include -name "*.inc")
INCLUDE_HEADERS = $(shell find include -name "*.h")
KERNEL_HEADERS = $(shell find kernel -name "*.h")

BOOT_BIOS_OBJS = $(addprefix $(BUILD_FOLDER)/, ${BOOT_BIOS_CSOURCES:.c=.o})
BOOT_UEFI_OBJS = $(addprefix $(BUILD_FOLDER)/, ${BOOT_UEFI_CSOURCES:.c=.o})
KERNEL_OBJS = $(addprefix $(BUILD_FOLDER)/, ${KERNEL_ASM_SOURCES:.asm=.o})
KERNEL_OBJS += $(addprefix $(BUILD_FOLDER)/, ${KERNEL_CSOURCES:.c=.o})

ANEKIX_BIOS_IMAGE = $(BUILD_FOLDER)/anekix-bios.img
ANEKIX_UEFI_PARTITION = $(BUILD_FOLDER)/anekix-uefi
ANEKIX_UEFI_IMAGE = $(ANEKIX_UEFI_PARTITION)/anekix.elf

#################################################### PHONY TARGETS ####################################################

.PHONY: all bios uefi clean

all: $(ANEKIX_BIOS_IMAGE) $(ANEKIX_UEFI_IMAGE)

bios: $(ANEKIX_BIOS_IMAGE)

uefi: $(ANEKIX_UEFI_IMAGE)

clean:
	rm -rf $(BUILD_FOLDER)

#################################################### IMAGE TARGETS ####################################################

$(ANEKIX_BIOS_IMAGE): $(BUILD_FOLDER)/boot/bios/bootloader.bin $(BUILD_FOLDER)/kernel/anekix.elf
	mkdir -p $(@D)
	cat $^ > $@
	truncate -s %512 $@

$(ANEKIX_UEFI_IMAGE): $(BUILD_FOLDER)/kernel/anekix.elf $(ANEKIX_UEFI_PARTITION)/efi/boot/bootx64.efi
	mkdir -p $(@D)
	cp $< $@

################################################## BOOT BIOS TARGETS ##################################################

$(BUILD_FOLDER)/boot/bios/bootloader.bin: boot/bios/bootloader.asm $$(@D)/stage2.bin $(NASM_INC_HEADERS)
	mkdir -p $(@D)
	$(AS) -I$(INCLUDE_PATH) $(ASFLAGS) -dBOOTLOADER_STAGE2_BIN=$(word 2, $^) -f bin -o $@ $<

$(BUILD_FOLDER)/boot/bios/stage2.bin: $(BOOT_BIOS_OBJS) $(SCRIPTS_FOLDER)/stage2.ld
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) -T $(SCRIPTS_FOLDER)/stage2.ld -o $@ $(BOOT_BIOS_OBJS)

$(BUILD_FOLDER)/boot/bios/%.o: boot/bios/%.c $(BOOT_BIOS_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) -I$(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

################################################## BOOT UEFI TARGETS ##################################################

$(ANEKIX_UEFI_PARTITION)/efi/boot/bootx64.efi: $(BUILD_FOLDER)/boot/uefi/bootx64.so
	mkdir -p $(@D)
	$(EFI_OBJCOPY) $(EFI_OBJCOPY_FLAGS) $< $@

$(BUILD_FOLDER)/boot/uefi/bootx64.so: $(BOOT_UEFI_OBJS)
	mkdir -p $(@D)
	$(LD) $(EFI_LDFLAGS) -L$(EFI_LIB_PATH) -L$(EFI_LIB_PATH)/gnuefi -T$(EFI_LIB_PATH)/elf_x86_64_efi.lds \
		$(EFI_LIB_PATH)/crt0-efi-x86_64.o $(BOOT_UEFI_OBJS) -o $@ $(EFI_LIBRARIES)

$(BUILD_FOLDER)/boot/uefi/%.o: boot/uefi/%.c $(BOOT_UEFI_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) -I$(INCLUDE_PATH) -I$(EFI_INCLUDE_PATH) $(EFI_CFLAGS) -c $< -o $@

#################################################### KERNEL TARGETS ###################################################

$(BUILD_FOLDER)/kernel/anekix.elf: $(KERNEL_OBJS) $(SCRIPTS_FOLDER)/kernel.ld
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) -T $(SCRIPTS_FOLDER)/kernel.ld -o $@ $(KERNEL_OBJS)

$(BUILD_FOLDER)/kernel/%.o: kernel/%.asm $(NASM_INC_HEADERS)
	mkdir -p $(@D)
	$(AS) -I$(INCLUDE_PATH) $(ASFLAGS) -f elf64 -o $@ $<

$(BUILD_FOLDER)/kernel/%.o: kernel/%.c $(KERNEL_HEADERS) $(INCLUDE_HEADERS)
	mkdir -p $(@D)
	$(CC) -I$(INCLUDE_PATH) $(CFLAGS) -mcmodel=kernel -c $< -o $@

######################################################## QEMU ########################################################

QEMU_CPUS ?= 2
QEMU_MEMORY ?= 128M

QEMU_ACCEL = -accel kvm -accel tcg
QEMU_GRAPHIC = -nographic
QEMU_MACHINE = -M pc,max-ram-below-4g=3G -global cpu.la57=off
QEMU_DEBUGCON = -debugcon file:/dev/stdout

QEMU_OPTIONS = $(QEMU_MACHINE) $(QEMU_ACCEL) -smp $(QEMU_CPUS) -m $(QEMU_MEMORY) $(QEMU_GRAPHIC) $(QEMU_DEBUGCON)
QEMU_BOOT_DRIVE = -drive media=disk,index=0,format=raw,if=ide

QEMU_OVMF_DRIVE = -drive readonly=on,format=raw,if=pflash
QEMU_OVMF = $(QEMU_OVMF_DRIVE),file=$(OVMF_IMAGE_PATH)

qemu-bios: $(ANEKIX_BIOS_IMAGE)
	$(QEMU) $(QEMU_OPTIONS) $(QEMU_BOOT_DRIVE),file=$<

qemu-uefi: $(ANEKIX_UEFI_IMAGE)
	$(QEMU) $(QEMU_OPTIONS) $(QEMU_OVMF) $(QEMU_BOOT_DRIVE),file=fat:rw:$(<D)