/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_BIOS_E820_H
#define BOOT_BIOS_E820_H

#include <attributes.h>
#include <sys/types.h>

#define E820_TABLE_BUFFER ((void*)0x4000)
#define E820_MAX_ENTRIES 128

#define E820_RAM_TYPE 1
#define E820_RESERVED_TYPE 2
#define E820_ACPI_TYPE 3
#define E820_NVS_TYPE 4

typedef struct PACKED {
    u64 base_address;
    u64 size;
    u32 type;
} E820Entry;

typedef struct {
    u32 entry_count;
    E820Entry entries[E820_MAX_ENTRIES];
} E820Table;

#endif /* BOOT_BIOS_E820_H */