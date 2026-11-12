/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/memory_layout.h>
#include <elf.h>
#include <efi.h>
#include <efilib.h>

#include "LoadKernel.h"
#include "Assert.h"

/*
 * Check ELF identification magic number
 */
STATIC BOOLEAN IsElf(
    IN UINT8 *Ident
) {
    return Ident[0] == ELF_MAG0
            && Ident[1] == ELF_MAG1
            && Ident[2] == ELF_MAG2
            && Ident[3] == ELF_MAG3;
}

/*
 * Allocate pages for kernel
 */
VOID AllocateKernelSpace(VOID) {
    EFI_PHYSICAL_ADDRESS Location = KERNEL_LOAD_ADDRESS;

    CheckStatus(
        L"Unable to allocate kernel space",
        BS->AllocatePages(
            AllocateAddress,
            EfiLoaderCode,
            KERNEL_MAX_SIZE / EFI_PAGE_SIZE,
            &Location
        )
    );
}

/*
 * Load kernel executable ELF from file buffer
 */
VOID LoadKernelElfBinary(
    IN VOID *FileBuffer,
    OUT KERNEL_ENTRY *KernelEntry
) {
    Elf64Header *header = FileBuffer;

    if(!IsElf(header->ident)) {
        ExitSystem(
            L"Kernel is not an ELF file",
            EFI_LOAD_ERROR
        );
    }

    Elf64ProgramHeader *phdrs = (VOID*)((UINT8*)header + header->phoff);

    for(UINTN i = 0; i < header->phnum; i++) {
        Elf64ProgramHeader ph = phdrs[i];

        if(ph.type == ELF_LOADABLE_SEGMENT) {
            UINT8 *Source = (UINT8*)header + ph.offset;
            UINT8 *Paddr = (VOID*)ph.paddr;

            CopyMem(Paddr, Source, ph.filesz);
            ZeroMem(Paddr + ph.filesz, ph.memsz - ph.filesz);
        }
    }

    *KernelEntry = (KERNEL_ENTRY)header->entry;
}