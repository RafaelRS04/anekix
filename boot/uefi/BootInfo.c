/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/boot_info.h>
#include <efi.h>
#include <efilib.h>

#include "BootInfo.h"
#include "Assert.h"

/*
 * Search for Root System Descriptor Pointer
 */
STATIC UINT64 FindAcpiRsdp(VOID) {
    EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;
    EFI_GUID Acpi1TableGuid = ACPI_TABLE_GUID;
    VOID *RsdpAddress = NULL;

    LibGetSystemConfigurationTable(
        &Acpi2TableGuid,
        &RsdpAddress
    );

    if(RsdpAddress == NULL) {
        LibGetSystemConfigurationTable(
            &Acpi1TableGuid,
            &RsdpAddress
        );
    }

    return (UINT64)RsdpAddress;
}

/*
 * Get UEFI memory map
 */
STATIC EFI_MEMORY_DESCRIPTOR *GetMemoryMap(
    OUT UINTN *EntryCount,
    OUT UINTN *MapKey,
    OUT UINTN *DescriptorSize
) {
    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    UINT32 DescriptorVersion;

    MemoryMap = LibMemoryMap(
        EntryCount,
        MapKey,
        DescriptorSize,
        &DescriptorVersion
    );

    if(MemoryMap == NULL) {
        ExitSystem(
            L"Unable to get memory map",
            EFI_OUT_OF_RESOURCES
        );
    }

    return MemoryMap;
}

/*
 * Map Efi memory type to Anekix memory region type
 */
STATIC UINT64 ConvertEfiMemoryType(
    IN UINT32 EfiMemoryType
) {
    switch(EfiMemoryType) {
    case EfiLoaderCode:
    case EfiLoaderData:
    case EfiBootServicesCode:
    case EfiBootServicesData:
    case EfiConventionalMemory:
        return ANEKIX_USABLE_MEMORY;
    case EfiACPIReclaimMemory:
    case EfiACPIMemoryNVS:
        return ANEKIX_UNUSABLE_MEMORY;
    default:
        return ANEKIX_IGNORED_MEMORY;
    }
}

/*
 * Try to merge a memory region from descriptor into boot info
 */
STATIC BOOLEAN MergeMemoryRegion(
    OUT BootInfo *boot_info,
    IN EFI_MEMORY_DESCRIPTOR *Descriptor
) {
    UINT64 Type = ConvertEfiMemoryType(Descriptor->Type);
    UINT64 Count = boot_info->region_count;

    for(UINTN i = 0; i < Count; i++) {
        MemoryRegion *region = &boot_info->regions[i];
        UINT64 RegionEnd = region->address + region->size;

        if(region->type == Type && Descriptor->PhysicalStart == RegionEnd) {
            region->size += Descriptor->NumberOfPages * EFI_PAGE_SIZE;
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Insert memory region from descriptor into boot info
 */
STATIC VOID InsertMemoryRegion(
    OUT BootInfo *boot_info,
    IN EFI_MEMORY_DESCRIPTOR *Descriptor
) {
    if(boot_info->region_count >= BOOT_INFO_MAX_REGIONS) {
        ExitSystem(
            L"Memory map to long to put on boot info",
            EFI_LOAD_ERROR
        );
    }

    if(!MergeMemoryRegion(boot_info, Descriptor)) {
        UINT64 Index = boot_info->region_count;
        MemoryRegion *region = &boot_info->regions[Index];

        region->address = Descriptor->PhysicalStart;
        region->size = Descriptor->NumberOfPages * EFI_PAGE_SIZE;
        region->type = ConvertEfiMemoryType(Descriptor->Type);

        boot_info->region_count++;
    }
}

/*
 * Fill boot info with RSDP and memory map
 */
VOID FillBootInfo(
    OUT UINTN *MapKey
) {
    BootInfo *boot_info = BOOT_INFO_PHYSICAL_BUFFER;
    boot_info->acpi_rsdp_address = FindAcpiRsdp();
    boot_info->region_count = 0;

    UINTN EntryCount;
    UINTN DescriptorSize;

    EFI_MEMORY_DESCRIPTOR *Descriptor = GetMemoryMap(
        &EntryCount,
        MapKey,
        &DescriptorSize
    );

    for(UINTN i = 0; i < EntryCount; i++) {
        InsertMemoryRegion(boot_info, Descriptor);
        Descriptor = NextMemoryDescriptor(Descriptor, DescriptorSize);
    }
}