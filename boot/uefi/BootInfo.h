/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_UEFI_BOOT_INFO_H
#define BOOT_UEFI_BOOT_INFO_H

#include <efi.h>

VOID FillBootInfo(
    OUT UINTN *MapKey
);

#endif /* BOOT_UEFI_BOOT_INFO_H */