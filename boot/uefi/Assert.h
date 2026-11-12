/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_UEFI_ASSERT_H
#define BOOT_UEFI_ASSERT_H

#include <efi.h>

VOID ExitSystem(
    IN CHAR16 *ErrorMessage,
    IN EFI_STATUS Status
);

VOID CheckStatus(
    IN CHAR16 *ErrorMessage,
    IN EFI_STATUS Status
);

#endif /* BOOT_UEFI_ASSERT_H */