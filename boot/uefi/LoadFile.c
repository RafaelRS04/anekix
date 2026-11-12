/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <efi.h>
#include <efilib.h>

#include "LoadFile.h"
#include "Assert.h"

/*
 * Locate EFI simple file protocol
 */
VOID GetFileProtocol(
    OUT EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **FileSystem
) {
    CheckStatus(
        L"Unable to EFI simple file protocol",
        LibLocateProtocol(
            &gEfiSimpleFileSystemProtocolGuid,
            (VOID**)FileSystem
        )
    );
}

/*
 * Load a file from file system into buffer
 */
VOID LoadFile(
    IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem,
    IN CHAR16 *FileName,
    OUT VOID **FileBuffer
) {
    EFI_FILE_PROTOCOL *RootNode;
    EFI_FILE_PROTOCOL *File;

    CheckStatus(
        L"Unable to open file system Root Node",
        FileSystem->OpenVolume(FileSystem, &RootNode)
    );

    CheckStatus(
        L"Unable to open file",
        RootNode->Open(
            RootNode,
            &File,
            FileName,
            EFI_FILE_MODE_READ,
            EFI_FILE_READ_ONLY
        )
    );

    EFI_FILE_INFO *FileInfo = LibFileInfo(File);

    if(FileInfo == NULL) {
        ExitSystem(
            L"Unable to get file info",
            EFI_OUT_OF_RESOURCES
        );
    }

    UINTN FileSize = FileInfo->FileSize;
    FreePool(FileInfo);

    *FileBuffer = AllocatePool(FileSize);

    if(FileBuffer == NULL) {
        ExitSystem(
            L"Unable allocate file buffer",
            EFI_OUT_OF_RESOURCES
        );
    }

    CheckStatus(
        L"Unable to read file",
        File->Read(File, &FileSize, *FileBuffer)
    );

    File->Close(File);
    RootNode->Close(RootNode);
}