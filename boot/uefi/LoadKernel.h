/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_UEFI_LOAD_KERNEL_H
#define BOOT_UEFI_LOAD_KERNEL_H

#include <attributes.h>
#include <efi.h>

typedef NO_RETURN void (*KERNEL_ENTRY)(void);

VOID AllocateKernelSpace(VOID);

VOID LoadKernelElfBinary(
    IN VOID *FileBuffer,
    OUT KERNEL_ENTRY *KernelEntry
);

#endif /* BOOT_UEFI_LOAD_KERNEL_H */