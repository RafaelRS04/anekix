/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_MEMORY_LAYOUT_H
#define SYS_MEMORY_LAYOUT_H

#include <sys/types.h>

/* Is mapped and can be used by allocator */
#define ANEKIX_USABLE_MEMORY 0

/* Is mapped and can't be used by allocator */
#define ANEKIX_UNUSABLE_MEMORY 1

/* Isn't mapped and can't be used by allocator */
#define ANEKIX_IGNORED_MEMORY 2

typedef struct {
    u64 start;
    u64 end;
    u64 type;
} MemoryRegion;

#define EXTENDED_MEMORY 0x100000
#define MMIO_HOLE_BASE 0xC0000000
#define MMIO_HOLE_SIZE 0x40000000

#define KERNEL_LOAD_ADDRESS 0x200000
#define KERNEL_MAP_BASE 0xFFFFFFFF80000000
#define DIRECT_MAP_BASE 0xFFFF800000000000
#define KERNEL_STACK_SIZE 0x2000
#define KERNEL_MAX_SIZE 0x200000

#define SYMBOL_PA(symbol) ((u64)(symbol) - KERNEL_MAP_BASE + KERNEL_LOAD_ADDRESS)
#define SYMBOL_VA(symbol) ((void*)(SYMBOL_PA(symbol) + DIRECT_MAP_BASE))

#define VA(pa) ((void*)((u64)(pa) + DIRECT_MAP_BASE))
#define PA(va) ((u64)(va) - DIRECT_MAP_BASE)

#define IS_FROM_DIRECT_MAP(va) ((u64)va >= DIRECT_MAP_BASE && (u64)va < KERNEL_MAP_BASE)
#define IS_FROM_KERNEL_MAP(va) ((u64)va >= KERNEL_MAP_BASE)

#endif /* SYS_MEMORY_LAYOUT_H */