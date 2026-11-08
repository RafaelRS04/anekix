/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOOT_BIOS_ATA_DISK_H
#define BOOT_BIOS_ATA_DISK_H

#include <sys/types.h>

#define ATA_SECTOR_SIZE 512

void read_disk_sector(void *buffer, u32 logical_sector);

#endif /* BOOT_BIOS_ATA_DISK_H */