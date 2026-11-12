/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <sys/types.h>

void memmove(void *destination, const void *source, u64 count);
void memset(void *destination, u8 data, u64 count);
i32 memcmp(void *location1, void *location2, u64 count);

#endif /* KERNEL_STRING_H */