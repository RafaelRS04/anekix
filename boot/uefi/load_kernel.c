/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <efi.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    UINTN EventIndex;

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI!");
    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &EventIndex);

    return EFI_SUCCESS;
}