/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_BIOS_PANIC_H
#define BOOT_BIOS_PANIC_H

#include <attributes.h>

NO_RETURN void panic(const char *message);

#endif /* BOOT_BIOS_PANIC_H */