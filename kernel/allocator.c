/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/memory_layout.h>
#include <x64/paging.h>

#include "allocator.h"
#include "string.h"
#include "linker.h"
#include "panic.h"

typedef struct FreeList FreeList;

struct FreeList {
    FreeList *next;
};

static FreeList *free_list = NULL;

/*
 * Mark memory regions on MMIO hole as ignored memory
 */
static void ignore_mmio_hole_addresses(BootInfo *boot_info) {
    for(u64 i = 0; i < boot_info->region_count; i++) {
        MemoryRegion *region = &boot_info->regions[i];

        if(region->start >= MMIO_HOLE_BASE
            && region->start < MMIO_HOLE_BASE + MMIO_HOLE_SIZE) {
            region->type = ANEKIX_IGNORED_MEMORY;
        }
    }
}

/*
 * Call kfree for a range of virtual addresses
 */
static void free_range(void *vstart, void *vend) {
    u64 page_start = PAGE_ALIGN_UP(vstart);
    u64 end = PAGE_ALIGN_DOWN(vend);

    while(page_start < end) {
        kfree((void*)page_start);
        page_start += PAGE_SIZE;
    }
}

/*
 * Init allocator with free memory inside early paging ranges
 */
void allocator_early_init(BootInfo *boot_info) {
    ignore_mmio_hole_addresses(boot_info);

    free_range(
        VA(EXTENDED_MEMORY),
        VA(KERNEL_LOAD_ADDRESS)
    );

    free_range(
        SYMBOL_VA(kernel_end),
        VA(KERNEL_LOAD_ADDRESS + KERNEL_MAX_SIZE)
    );
}

/*
 * Free page of physical memory pointed by virtual address
 */
void kfree(void *vaddress) {
    FreeList *head = vaddress;

    if(!IS_PAGE_ALIGNED(vaddress)) {
        panic("Misaligned pointer passed to kfree!");
    }

    memset(vaddress, POISON_FREE, PAGE_SIZE);

    head->next = free_list;
    free_list = head;
}

/*
 * Allocate one page of physical memory from the free list
 * Return a pointer from direct map on success
 */
void *kalloc(void) {
    FreeList *head = free_list;

    if(free_list) {
        free_list = free_list->next;
    }

    return head;
}