/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>

#include "string.h"

/*
 * Move bytes from source to destination considering a possible overlap
 */
void memmove(void *destination, const void *source, u64 count) {
    const u8 *src = source;
    u8 *dest = destination;

    if(src < dest) {
        while(count-- != 0) {
            dest[count] = src[count];
        }
    } else {
        while(count-- != 0) {
            *dest++ = *src++;
        }
    }
}

/*
 * Fill destination with a given byte value
 */
void memset(void *destination, u8 data, u64 count) {
    stosb(destination, data, count);
}

/*
 * Compare bytes from the given locations
 */
i32 memcmp(void *location1, void *location2, u64 count) {
    u8 *s1 = location1;
    u8 *s2 = location2;

    while(count-- != 0) {
        if(*s1 != *s2) {
            return *s1 - *s2;
        }
        s1++, s2++;
    }

    return 0;
}