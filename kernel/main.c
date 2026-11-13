/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/boot_info.h>
#include <x64/assembly.h>
#include <attributes.h>
#include <debug.h>

#include "drivers/8259_pic.h"
#include "allocator.h"

SECTION(".boot_info")
BootInfo boot_info;

NO_RETURN void main(void) {
    allocator_early_init(&boot_info);
    pic_disable();

    while(kalloc() != NULL) {
        outb(DEBUG_PORT, '+');
    }

    outsb(DEBUG_PORT, "\nOut of memory successfully\n", 28);

    freeze();
}