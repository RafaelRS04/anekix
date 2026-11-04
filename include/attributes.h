/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#define ALIGNED(alignment) __attribute__((__aligned__(alignment)))
#define SECTION(section) __attribute__((__section__(section)))
#define NO_RETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))

#endif /* ATTRIBUTES_H */