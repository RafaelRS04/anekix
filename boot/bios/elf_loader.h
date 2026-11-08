/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_BIOS_ELF_LOADER_H
#define BOOT_BIOS_ELF_LOADER_H

#include <attributes.h>
#include <sys/types.h>
#include <elf.h>

#define ELF_PROGRAM_HEADER_OFFSET 0x40
#define MAX_ELF_PROGRAM_HEADERS 8

typedef struct {
   u32 kernel_sector;
   Elf64Header *header;
   Elf64ProgramHeader *program_headers;
} Elf;

typedef NO_RETURN void (*ElfEntry)(void);

Elf load_elf_headers(u32 kernel_sector, void *buffer);
void load_elf(const Elf *elf);

#endif /* BOOT_BIOS_ELF_LOADER_H */