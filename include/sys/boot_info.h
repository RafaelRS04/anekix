/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_BOOT_INFO_H
#define SYS_BOOT_INFO_H

#include <sys/types.h>
#include <attributes.h>
#include <sys/memory_layout.h>

#define BOOT_INFO_PHYSICAL_ADDRESS KERNEL_LOAD_ADDRESS
#define MAX_RANGES 128

typedef struct {
    u64 start;
    u64 end;
} Range;

typedef struct PACKED {
    u64 acpi_rsdp_address;
    u64 free_ranges_count;
    Range free_memory[MAX_RANGES];
} BootInfo;

#endif /* SYS_BOOT_INFO_H */