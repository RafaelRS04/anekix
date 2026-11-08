/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_TYPES_H
#define SYS_TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;

#define ASSERT_SIZE(type, size) \
    static_assert(sizeof(type) == size, "Expected " #type " to have " #size " bytes")

ASSERT_SIZE(u8, 1);
ASSERT_SIZE(i8, 1);
ASSERT_SIZE(u16, 2);
ASSERT_SIZE(i16, 2);
ASSERT_SIZE(u32, 4);
ASSERT_SIZE(i32, 4);
ASSERT_SIZE(u64, 8);
ASSERT_SIZE(i64, 8);

#undef ASSERT_SIZE

#endif /* SYS_TYPES_H */