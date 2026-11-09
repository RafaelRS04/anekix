/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/memory_layout.h>
#include <sys/boot_info.h>
#include <attributes.h>
#include <sys/types.h>

#include "elf_loader.h"

/* Free memory above the first 64KB used by bootloader */
#define ELF_HEADERS_BUFFER ((void*)0x10000)

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

SECTION(".entry")
NO_RETURN void load_kernel(u64 ebda, u32 kernel_sector) {
   BootInfo *boot_info = (void*)KERNEL_LOAD_ADDRESS;
   ElfEntry kernel_entry;
   Elf elf;

   elf = load_elf_headers(kernel_sector, ELF_HEADERS_BUFFER);
   load_elf(&elf);

   boot_info->acpi_rsdp_address = find_acpi_rsdp(ebda);

   kernel_entry = (ElfEntry)elf.header->entry;
   kernel_entry();
}