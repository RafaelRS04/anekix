/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ELF_H
#define ELF_H

#include <sys/types.h>

typedef struct {
    u8 e_ident[16];  /* ELF identification */
    u16 e_type;      /* Object file type */
    u16 e_machine;   /* Machine type */
    u32 e_version;   /* Object file version */
    u64 e_entry;     /* Entry point address */
    u64 e_phoff;     /* Program header offset */
    u64 e_shoff;     /* Section header offset */
    u32 e_flags;     /* Processor-specific flags */
    u16 e_ehsize;    /* ELF header size */
    u16 e_phentsize; /* Size of program header entry */
    u16 e_phnum;     /* Number of program header entries */
    u16 e_shentsize; /* Size of section header entry */
    u16 e_shnum;     /* Number of section header entries */
    u16 e_shstrndx;  /* Section name string table index */
} Elf64Header;

typedef struct {
    u32 p_type;      /* Type of segment */
    u32 p_flags;     /* Segment attributes */
    u64 p_offset;    /* Offset in file */
    u64 p_vaddr;     /* Virtual address in memory */
    u64 p_paddr;     /* Reserved */
    u64 p_filesz;    /* Size of segment in file */
    u64 p_memsz;     /* Size of segment in memory */
    u64 p_align;     /* Alignment of segment */
} Elf64ProgramHeader;

#endif /* ELF_H */