/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

SECTIONS {
    . = 0xFFFFFFFF80000000;

    .boot_info : AT(0x200000) {
        KEEP(*(.boot_info))
    }

    . = ALIGN(0x1000);
    PROVIDE(kernel_text = .);

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    . = ALIGN(0x1000);
    PROVIDE(kernel_data = .);

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
    }

    . = ALIGN(0x1000);
    PROVIDE(kernel_end = .);
}