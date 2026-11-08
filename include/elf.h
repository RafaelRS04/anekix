/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ELF_H
#define ELF_H

#include <sys/types.h>

#define ELF_MAG0 '\x7F'
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'

#define ELF_EXECUTABLE_FILE 2
#define ELF_LOADABLE_SEGMENT 1

typedef struct {
    u8 ident[16];  /* ELF identification */
    u16 type;      /* Object file type */
    u16 machine;   /* Machine type */
    u32 version;   /* Object file version */
    u64 entry;     /* Entry point address */
    u64 phoff;     /* Program header offset */
    u64 shoff;     /* Section header offset */
    u32 flags;     /* Processor-specific flags */
    u16 ehsize;    /* ELF header size */
    u16 phentsize; /* Size of program header entry */
    u16 phnum;     /* Number of program header entries */
    u16 shentsize; /* Size of section header entry */
    u16 shnum;     /* Number of section header entries */
    u16 shstrndx;  /* Section name string table index */
} Elf64Header;

typedef struct {
    u32 type;      /* Type of segment */
    u32 flags;     /* Segment attributes */
    u64 offset;    /* Offset in file */
    u64 vaddr;     /* Virtual address in memory */
    u64 paddr;     /* Physical address in memory */
    u64 filesz;    /* Size of segment in file */
    u64 memsz;     /* Size of segment in memory */
    u64 align;     /* Alignment of segment */
} Elf64ProgramHeader;

#endif /* ELF_H */