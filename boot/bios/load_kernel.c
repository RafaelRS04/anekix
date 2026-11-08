/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/memory_layout.h>
#include <attributes.h>
#include <sys/types.h>

#include "elf_loader.h"

/* Free memory above the first 64KB used by bootloader */
#define ELF_HEADERS_BUFFER ((void*)0x10000)

SECTION(".entry")
NO_RETURN void load_kernel(u32 kernel_sector) {
   Elf elf = load_elf_headers(kernel_sector, ELF_HEADERS_BUFFER);
   ElfEntry kernel_entry = (ElfEntry)elf.header->entry;
   load_elf(&elf);
   kernel_entry();
}