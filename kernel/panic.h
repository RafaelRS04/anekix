/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include <attributes.h>

NO_RETURN void panic(const char *message);

#endif /* KERNEL_PANIC_H */