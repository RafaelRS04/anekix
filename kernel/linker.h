/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KERNEL_LINKER_H
#define KERNEL_LINKER_H

/* First address of kernel .text section */
extern char kernel_text[];

/* First address of kernel .data section */
extern char kernel_data[];

/* First address after loaded kernel */
extern char kernel_end[];

#endif /* KERNEL_LINKER_H */