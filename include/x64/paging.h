/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef X64_PAGING_H
#define X64_PAGING_H

#include <sys/types.h>

#define LARGE_PAGE_SIZE 0x200000
#define PAGE_SIZE 0x1000

#define PAGE_MAP_ENTRIES 512

#define PAGE_PRESENT_FLAG (1 << 0)
#define PAGE_WRITE_FLAG (1 << 1)
#define PAGE_WRITE_THROUGH_FLAG (1 << 3)
#define PAGE_CACHE_DISABLE_FLAG (1 << 4)
#define PAGE_SIZE_FLAG (1 << 7)

#define PML4_INDEX(va) (((u64)(va) >> 39) & 0x1FF)
#define PML3_INDEX(va) (((u64)(va) >> 30) & 0x1FF)
#define PML2_INDEX(va) (((u64)(va) >> 21) & 0x1FF)
#define PML1_INDEX(va) (((u64)(va) >> 12) & 0x1FF)

#define PAGE_ALIGN_UP(x) (((u64)(x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_DOWN(x) ((u64)(x) & ~(PAGE_SIZE - 1))

#define IS_LARGE_PAGE_ALIGNED(x) ((u64)(x) % LARGE_PAGE_SIZE == 0)
#define IS_PAGE_ALIGNED(x) ((u64)(x) % PAGE_SIZE == 0)

/* Page Map Entry type */
typedef u64 pme_t;

#endif /* X64_PAGING_H */