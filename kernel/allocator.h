/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KERNEL_ALLOCATOR_H
#define KERNEL_ALLOCATOR_H

#include <sys/boot_info.h>

/* Used to poison use-after-free */
#define POISON_FREE 0x52

void allocator_early_init(BootInfo *boot_info);
void kfree(void *vaddress);
void *kalloc(void);

#endif /* KERNEL_ALLOCATOR_H */