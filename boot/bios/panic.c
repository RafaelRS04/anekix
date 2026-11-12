/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>
#include <debug.h>

#include "panic.h"

static const char prefix[] = "\nBIOS Bootloader Panic: ";

/*
 * Print to debug port and halt execution
 */
NO_RETURN void panic(const char *message) {
   outsb(DEBUG_PORT, prefix, sizeof prefix);

   while(*message != '\0') {
      outb(DEBUG_PORT, *message);
      message++;
   }

   outb(DEBUG_PORT, '\n');

   freeze();
}