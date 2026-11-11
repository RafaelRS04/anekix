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
    outsb(DEBUG_PORT, (char*)boot_info.acpi_rsdp_address, 8);
    freeze();
}