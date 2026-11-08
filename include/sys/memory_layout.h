/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_MEMORY_LAYOUT_H
#define SYS_MEMORY_LAYOUT_H

#include <sys/types.h>

#define KERNEL_MAP_BASE 0xFFFFFFFF80000000
#define DIRECT_MAP_BASE 0xFFFF800000000000

#define EXTENDED_MEMORY_BASE 0x100000
#define KERNEL_LOAD_ADDRESS 0x200000
#define MMIO_HOLE_BASE 0xC0000000

#define KERNEL_STACK_SIZE 0x2000

#define SYMBOL_PA(va) ((u64)(va) - KERNEL_MAP_BASE + KERNEL_LOAD_ADDRESS)

#define VA(pa) ((void *)((u64)(pa) + DIRECT_MAP_BASE))
#define PA(va) ((u64)(va) - DIRECT_MAP_BASE)

#endif /* SYS_MEMORY_LAYOUT_H */