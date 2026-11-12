/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_BIOS_KERNEL_ELF_H
#define BOOT_BIOS_KERNEL_ELF_H

#include <attributes.h>
#include <sys/types.h>
#include <elf.h>

#define ELF_PROGRAM_HEADER_OFFSET 0x40
#define MAX_ELF_PROGRAM_HEADERS 8

typedef struct {
    u32 kernel_sector;
    Elf64Header header;
    Elf64ProgramHeader phdrs[MAX_ELF_PROGRAM_HEADERS];
} KernelElf;

typedef NO_RETURN void (*KernelEntry)(void);

KernelElf *load_kernel_elf_headers(void *buffer, u32 kernel_sector);
void load_kernel_elf_binary(const KernelElf *elf);

#endif /* BOOT_BIOS_KERNEL_ELF_H */