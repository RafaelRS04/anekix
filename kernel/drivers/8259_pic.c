/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA0

/*
 * Disable legacy 8259 Programmable Interrupt Controller
 */
void pic_disable(void) {
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}