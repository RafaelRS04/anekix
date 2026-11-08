/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:x86-64)

ENTRY(load_kernel)

SECTIONS {
    . = 0x7E00;

    .text : {
        *(.entry)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
    }
}