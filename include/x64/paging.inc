; Copyright (c) 2026 Rafael Rodrigues Sanches.
; SPDX-License-Identifier: BSD-3-Clause

%ifndef X64_PAGING_INC
%define X64_PAGING_INC

PAGE_MAP_ENTRY_SIZE equ 8

BASE_PAGE_MASK equ 0x000FFFFFFFFFF000
LARGE_PAGE_MASK equ 0x000FFFFFFFE00000
PAGE_FLAGS_MASK equ 0xFFFF

PAGE_PRESENT_FLAG equ 1 << 0
PAGE_WRITE_FLAG equ 1 << 1
PAGE_SIZE_FLAG equ 1 << 7

%define BASE_PAGE_ENTRY(addr, flags) \
    (((addr) & BASE_PAGE_MASK) | (((flags) & ~PAGE_SIZE_FLAG) & PAGE_FLAGS_MASK))

%define LARGE_PAGE_ENTRY(addr, flags) \
    (((addr) & LARGE_PAGE_MASK) | (((flags) | PAGE_SIZE_FLAG) & PAGE_FLAGS_MASK))

%define BASE_PAGE_ENTRY(addr) \
    BASE_PAGE_ENTRY(addr, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG)

%define LARGE_PAGE_ENTRY(addr) \
    LARGE_PAGE_ENTRY(addr, PAGE_PRESENT_FLAG | PAGE_WRITE_FLAG)

%endif