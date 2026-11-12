/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/boot_info.h>
#include <x64/assembly.h>
#include <attributes.h>

#include "drivers/8259_pic.h"

SECTION(".boot_info")
BootInfo boot_info;

NO_RETURN void main(void) {
    pic_disable();
    freeze();
}