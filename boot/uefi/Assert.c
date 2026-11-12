/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <efi.h>
#include <efilib.h>

#include "Assert.h"

/*
 * Print error message and shutdown system
 */
VOID ExitSystem(
    IN CHAR16 *ErrorMessage,
    IN EFI_STATUS Status
) {
    Print(L"[UEFI %r] %s\n", Status, ErrorMessage);
    RT->ResetSystem(EfiResetShutdown, Status, 0, NULL);
}

/*
 * Exit system on error status
 */
VOID CheckStatus(
    IN CHAR16 *ErrorMessage,
    IN EFI_STATUS Status
) {
    if(Status != EFI_SUCCESS) {
        ExitSystem(ErrorMessage, Status);
    }
}