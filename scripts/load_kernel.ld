/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT(binary)

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