/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_UEFI_LOAD_FILE_H
#define BOOT_UEFI_LOAD_FILE_H

#include <efi.h>

VOID GetFileProtocol(
    OUT EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **FileSystem
);

VOID LoadFile(
    IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem,
    IN CHAR16 *FileName,
    OUT VOID **FileBuffer
);

#endif /* BOOT_UEFI_LOAD_FILE_H */