/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_BOOT_INFO_H
#define SYS_BOOT_INFO_H

#include <sys/types.h>
#include <sys/memory_layout.h>

#define BOOT_INFO_PHYSICAL_BUFFER ((void*)KERNEL_LOAD_ADDRESS)
#define BOOT_INFO_MAX_REGIONS 128

typedef struct {
    u64 acpi_rsdp_address;
    u64 region_count;
    MemoryRegion regions[BOOT_INFO_MAX_REGIONS];
} BootInfo;

#endif /* SYS_BOOT_INFO_H */