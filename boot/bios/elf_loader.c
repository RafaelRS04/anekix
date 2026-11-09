/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>

#include "elf_loader.h"
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
 * Parse ELF header and fill program headers pointer
 */
static void parse_elf_header(Elf *elf) {
   if(!is_elf(elf->header->ident)) {
      panic("Panic: kernel is not an ELF file!");
   }

   if(elf->header->type != ELF_EXECUTABLE_FILE) {
      panic("Panic: kernel is not an executable ELF file!");
   }

   if(elf->header->phoff != ELF_PROGRAM_HEADER_OFFSET) {
      panic("Panic: unexpected ELF program header offset!");
   }

   if(elf->header->phnum > MAX_ELF_PROGRAM_HEADERS) {
      panic("Panic: kernel has more ELF program headers than expected!");
   }

   u8 *program_headers = (u8*)elf->header + elf->header->phoff;
   elf->program_headers = (void*)program_headers;
}

/*
 * Load an ELF segment from program header into memory ignoring flags
 */
static void load_elf_segment(const Elf *elf, u16 index) {
   if(index >= MAX_ELF_PROGRAM_HEADERS) {
      panic("Panic: ELF program header not loaded!");
   }

   Elf64ProgramHeader ph = elf->program_headers[index];

   if(ph.type != ELF_LOADABLE_SEGMENT) {
      return;
   }

   if(ph.offset % ATA_SECTOR_SIZE) {
      panic("Panic: expected a sector aligned offset");
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
 * Load ELF headers from kernel sector into buffer
 */
Elf load_elf_headers(u32 kernel_sector, void *buffer) {
   if((u64)buffer & 0x07) {
      panic("Panic: buffer should be 8 byte aligned!");
   }

   read_disk_sector(buffer, kernel_sector);

   Elf elf = { kernel_sector, buffer, NULL };
   parse_elf_header(&elf);

   return elf;
}

/*
 * Load ELF executable into memory
 */
void load_elf(const Elf *elf) {
   u16 phnum = elf->header->phnum;

   for(u16 i = 0; i < phnum; i++) {
      load_elf_segment(elf, i);
   }
}