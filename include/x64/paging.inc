; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%ifndef X64_PAGING_INC
%define X64_PAGING_INC

LARGE_PAGE_ADDRESS_MASK equ 0x000FFFFFFFE00000
PAGE_ADDRESS_MASK equ 0x000FFFFFFFFFF000
PAGE_FLAGS_MASK equ 0xFFFF

PAGE_PRESENT_FLAG equ 1 << 0
PAGE_WRITE_FLAG equ 1 << 1
PAGE_SIZE_FLAG equ 1 << 7

; Reserve bytes for a page map
; %1 = Page map name
%macro RESERVE_PAGE_MAP 1
    global %1:data hidden
    align 0x1000
    %1:
        resb 0x1000
%endmacro

%define PML4_OFFSET(addr) ((((addr) >> 39) & 0x1FF) * 8)
%define PML3_OFFSET(addr) ((((addr) >> 30) & 0x1FF) * 8)
%define PML2_OFFSET(addr) ((((addr) >> 21) & 0x1FF) * 8)
%define PML1_OFFSET(addr) ((((addr) >> 12) & 0x1FF) * 8)

%define PAGE_ENTRY(addr, flags) \
    (((addr) & PAGE_ADDRESS_MASK) | (((flags) & ~PAGE_SIZE_FLAG) & PAGE_FLAGS_MASK))

%define LARGE_PAGE_ENTRY(addr, flags) \
    (((addr) & LARGE_PAGE_ADDRESS_MASK) | (((flags) | PAGE_SIZE_FLAG) & PAGE_FLAGS_MASK))

%define PAGE_ENTRY(addr) \
    PAGE_ENTRY(addr, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG)

%define LARGE_PAGE_ENTRY(addr) \
    LARGE_PAGE_ENTRY(addr, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG)

%endif