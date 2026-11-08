/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/boot_info.h>
#include <x64/assembly.h>
#include <attributes.h>
#include <debug.h>

SECTION(".boot_info")
BootInfo boot_info;

NO_RETURN void main(void) {
    outsb(DEBUG_PORT, "\nHello, you are on kernel main() function!\n", 44);
    halt();
    UNREACHEABLE();
}