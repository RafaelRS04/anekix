/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>

#include "kernel_elf.h"
#include "ata_disk.h"
#include "panic.h"

/*
 * Check ELF identification magic number
 */
static bool is_elf(const u8 *ident) {
   return ident[0] == ELF_MAG0
            && ident[1] == ELF_MAG1
            && ident[2] == ELF_MAG2
            && ident[3] == ELF_MAG3;
}

/*
 * Parse and assert kernel ELF header structure
 */
static void parse_kernel_elf_header(KernelElf *elf) {
   if(!is_elf(elf->header.ident)) {
      panic("kernel is not an ELF file!");
   }

   if(elf->header.type != ELF_EXECUTABLE_FILE) {
      panic("kernel is not an executable ELF file!");
   }

   if(elf->header.phoff != ELF_PROGRAM_HEADER_OFFSET) {
      panic("unexpected ELF program header offset!");
   }

   if(elf->header.phnum > MAX_ELF_PROGRAM_HEADERS) {
      panic("kernel has more ELF program headers than expected!");
   }
}

/*
 * Load a kernel ELF segment from program header into memory
 */
static void load_kernel_elf_segment(const KernelElf *elf, u16 index) {
   if(index >= MAX_ELF_PROGRAM_HEADERS) {
      panic("ELF program header not loaded!");
   }

   Elf64ProgramHeader ph = elf->phdrs[index];

   if(ph.type != ELF_LOADABLE_SEGMENT) {
      return;
   }

   if(ph.offset % ATA_SECTOR_SIZE) {
      panic("expected a sector aligned offset");
   }

   u32 logical_sector = ph.offset / ATA_SECTOR_SIZE;
   logical_sector += elf->kernel_sector;

   u64 paddr = ph.paddr;

   /*
    * It may read more bytes than needed on the last sector,
    * but loading on ascending order avoids overlaps.
    */
   while(paddr < ph.paddr + ph.filesz) {
      read_disk_sector((void*)paddr, logical_sector);
      paddr += ATA_SECTOR_SIZE;
      logical_sector++;
   }

   /* Fill with zeros if memory size is bigger than file size */
   if(ph.memsz > ph.filesz) {
      u8 *location = (void*)(ph.paddr + ph.filesz);
      u64 count = ph.memsz - ph.filesz;
      stosb(location, 0, count);
   }
}

/*
 * Load kernel ELF headers from kernel sector
 */
KernelElf *load_kernel_elf_headers(void *buffer, u32 kernel_sector) {
   KernelElf *elf = buffer;
   elf->kernel_sector = kernel_sector;

   read_disk_sector(&elf->header, kernel_sector);
   parse_kernel_elf_header(elf);

   return elf;
}

/*
 * Load kernel executable ELF into memory
 */
void load_kernel_elf_binary(const KernelElf *elf) {
   u16 phnum = elf->header.phnum;

   for(u16 i = 0; i < phnum; i++) {
      load_kernel_elf_segment(elf, i);
   }
}