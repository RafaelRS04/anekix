/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <efi.h>
#include <efilib.h>

#include "LoadKernel.h"
#include "LoadFile.h"
#include "BootInfo.h"
#include "Assert.h"

EFI_STATUS EfiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    KERNEL_ENTRY KernelEntry;
    VOID *FileBuffer;
    UINTN MapKey;

    InitializeLib(ImageHandle, SystemTable);

    AllocateKernelPages();
    GetFileProtocol(&FileSystem);
    LoadFile(FileSystem, L"anekix.elf", &FileBuffer);
    LoadKernelElfBinary(FileBuffer, &KernelEntry);
    FreePool(FileBuffer);
    FillBootInfo(&MapKey);

    CheckStatus(
        L"Failed to exit Boot Services",
        BS->ExitBootServices(ImageHandle, MapKey)
    );

    KernelEntry();
}