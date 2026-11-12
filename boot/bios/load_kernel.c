/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/memory_layout.h>
#include <sys/boot_info.h>
#include <attributes.h>
#include <sys/types.h>

#include "kernel_elf.h"
#include "panic.h"
#include "e820.h"

/* Free memory above the first 64KB used by bootloader */
#define SCRATCH_AREA ((void*)0x10000)

/*
 * Verify the Root System Descriptor Pointer signature
 */
static bool is_acpi_rsdp(u64 address) {
   const char signature[] = "RSD PTR ";
   char *rsdp = (void*)address;

   for(u8 i = 0; i < 8; i++) {
      if(rsdp[i] != signature[i]) {
         return false;
      }
   }

   return true;
}

/*
 * Verify the Root System Descriptor Pointer checksum
 */
static bool acpi_rsdp_checksum(u64 address) {
   char *rsdp = (void*)address;
   u8 sum = 0;

   for(u8 i = 0; i < 20; i++) {
      sum += rsdp[i];
   }

   return !sum;
}

/*
 * Search for Root System Descriptor Pointer
 */
static u64 find_acpi_rsdp(u64 ebda) {
   /* Search on the first 1KB of Extended BIOS Data Area */
   for(u64 address = ebda; address <= ebda + 1024; address += 16) {
      if(is_acpi_rsdp(address) && acpi_rsdp_checksum(address)) {
         return address;
      }
   }

   /* Search on the BIOS read-only memory */
   for(u64 address = 0xE0000; address <= 0xFFFFF; address += 16) {
      if(is_acpi_rsdp(address) && acpi_rsdp_checksum(address)) {
         return address;
      }
   }

   return NULL;
}

/*
 * Map e820 entry type to Anekix memory region type
 */
static u64 convert_e820_type(u32 e820_type) {
   switch(e820_type) {
   case E820_RAM_TYPE:
      return ANEKIX_USABLE_MEMORY;
   case E820_RESERVED_TYPE:
   case E820_ACPI_TYPE:
   case E820_NVS_TYPE:
      return ANEKIX_UNUSABLE_MEMORY;
   default:
      return ANEKIX_IGNORED_MEMORY;
   }
}

/*
 * Insert memory region into boot info
 */
static void insert_memory_region(BootInfo *boot_info, const MemoryRegion *region) {
   if(boot_info->region_count >= BOOT_INFO_MAX_REGIONS) {
      panic("too many memory regions to insert into boot info!");
   }

   u64 index = boot_info->region_count;
   boot_info->regions[index] = *region;
   boot_info->region_count++;
}

/*
 * Populate boot info memory regions using e820 map entries
 */
static void populate_memory_regions_from_e820(BootInfo *boot_info) {
   E820Table *e820_table = E820_TABLE_BUFFER;
   boot_info->region_count = 0;

   if(e820_table->entry_count > E820_MAX_ENTRIES) {
      panic("e820 entry count is greater than expected!");
   }

   for(u32 i = 0; i < e820_table->entry_count; i++) {
      E820Entry entry = e820_table->entries[i];

      MemoryRegion region = {
         .address = entry.base_address,
         .size = entry.size,
         .type = convert_e820_type(entry.type)
      };

      insert_memory_region(boot_info, &region);
   }
}

SECTION(".entry")
NO_RETURN void load_kernel(u32 kernel_sector, u64 ebda) {
   BootInfo *boot_info = BOOT_INFO_PHYSICAL_BUFFER;
   KernelEntry kernel_entry;
   KernelElf *elf;

   elf = load_kernel_elf_headers(SCRATCH_AREA, kernel_sector);
   load_kernel_elf_binary(elf);

   boot_info->acpi_rsdp_address = find_acpi_rsdp(ebda);
   populate_memory_regions_from_e820(boot_info);

   kernel_entry = (KernelEntry)elf->header.entry;
   kernel_entry();
}