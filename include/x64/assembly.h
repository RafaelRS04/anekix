/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef X64_ASSEMBLY_H
#define X64_ASSEMBLY_H

#include <attributes.h>
#include <sys/types.h>

#define ASM __asm__

/*
 * Get a byte from the specified I/O Port
 */
static inline u8 inb(u16 port) {
    u8 data;

    ASM volatile("in al, dx" : "=a" (data) : "d" (port));

    return data;
}

/*
 * Get a word from the specified I/O Port
 */
static inline u16 inw(u16 port) {
    u16 data;

    ASM volatile("in ax, dx" : "=a" (data) : "d" (port));

    return data;
}

/*
 * Get a doubleword from the specified I/O Port
 */
static inline u32 ind(u16 port) {
    u32 data;

    ASM volatile("in eax, dx" : "=a" (data) : "d" (port));

    return data;
}

/*
 * Send a byte to the specified I/O Port
 */
static inline void outb(u16 port, u8 data) {
    ASM volatile("out dx, al" : : "a" (data), "d" (port));
}

/*
 * Send a word to the specified I/O Port
 */
static inline void outw(u16 port, u16 data) {
    ASM volatile("out dx, ax" : : "a" (data), "d" (port));
}

/*
 * Send a doubleword to the specified I/O Port
 */
static inline void outd(u16 port, u32 data) {
    ASM volatile("out dx, eax" : : "a" (data), "d" (port));
}

/*
 * Get a string of bytes from the specified I/O Port
 */
static inline void insb(u16 port, void *destination, u64 count) {
    ASM volatile(
        "cld; rep insb"
        : "+D" (destination), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Get a string of words from the specified I/O Port
 */
static inline void insw(u16 port, void *destination, u64 count) {
    ASM volatile(
        "cld; rep insw"
        : "+D" (destination), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Get a string of doublewords from the specified I/O Port
 */
static inline void insd(u16 port, void *destination, u64 count) {
    ASM volatile(
        "cld; rep insd"
        : "+D" (destination), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Send a string of bytes to the specified I/O Port
 */
static inline void outsb(u16 port, const void *source, u64 count) {
    ASM volatile(
        "cld; rep outsb"
        : "+S" (source), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Send a string of words to the specified I/O Port
 */
static inline void outsw(u16 port, const void *source, u64 count) {
    ASM volatile(
        "cld; rep outsw"
        : "+S" (source), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Send a string of doublewords to the specified I/O Port
 */
static inline void outsd(u16 port, const void *source, u64 count) {
    ASM volatile(
        "cld; rep outsd"
        : "+S" (source), "+c" (count)
        : "d" (port)
        : "memory", "cc"
    );
}

/*
 * Fill a memory location with a given byte value
 */
static inline void stosb(void *location, u8 data, u64 count) {
    ASM volatile(
        "cld; rep stosb"
        : "+D" (location), "+c" (count)
        : "a" (data)
        : "memory", "cc"
    );
}

/*
 * Fill a memory location with a given word value
 */
static inline void stosw(void *location, u16 data, u64 count) {
    ASM volatile(
        "cld; rep stosw"
        : "+D" (location), "+c" (count)
        : "a" (data)
        : "memory", "cc"
    );
}

/*
 * Fill a memory location with a given doubleword value
 */
static inline void stosd(void *location, u32 data, u64 count) {
    ASM volatile(
        "cld; rep stosd"
        : "+D" (location), "+c" (count)
        : "a" (data)
        : "memory", "cc"
    );
}

/*
 * Fill a memory location with a given quadword value
 */
static inline void stosq(void *location, u64 data, u64 count) {
    ASM volatile(
        "cld; rep stosq"
        : "+D" (location), "+c" (count)
        : "a" (data)
        : "memory", "cc"
    );
}

/*
 * Disable Interrupts
 */
static inline void cli(void) {
    ASM volatile("cli" : : : "memory");
}

/*
 * Enable Interrupts
 */
static inline void sti(void) {
    ASM volatile("sti" : : : "memory");
}

/*
 * Stop instruction execution and enter HALT state
 */
static inline void halt(void) {
    ASM volatile("hlt" : : : "memory");
}

/*
 * Load Control Register 3 with PML4 physical address
 */
static inline void load_cr3(u64 pml4) {
    ASM volatile("mov cr3, %0" : : "r" (pml4) : "memory");
}

/*
 * Disable interrupts and HALT execution forever
 */
static inline NO_RETURN void freeze(void) {
    ASM volatile("cli; 1: hlt; jmp 1b" : : : "memory");
    UNREACHEABLE();
}

#endif /* X64_ASSEMBLY_H */