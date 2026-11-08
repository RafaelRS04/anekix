/*
 * Copyright (c) 2026 Rafael Rodrigues Sanches.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <x64/assembly.h>
#include "ata_disk.h"

#define ATA_REG_DATA 0x1F0
#define ATA_REG_SECTOR_COUNT 0x1F2
#define ATA_REG_LBA_LOW 0x1F3
#define ATA_REG_LBA_MID 0x1F4
#define ATA_REG_LBA_HIGH 0x1F5
#define ATA_REG_DRIVE 0x1F6
#define ATA_REG_COMMAND 0x1F7
#define ATA_REG_STATUS 0x1F7

#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_BSY 0x08
#define ATA_CMD_READ 0x20

#define LBA_BYTE0(lba) ((lba) & 0xFF)
#define LBA_BYTE1(lba) (((lba) >> 8) & 0xFF)
#define LBA_BYTE2(lba) (((lba) >> 16) & 0xFF)
#define LBA_BYTE3(lba) (((lba) >> 24) & 0x0F)
#define ATA_LBA_MODE 0xE0

#define CHECK_MASK (ATA_STATUS_BSY | ATA_STATUS_DRQ)

/*
 * Check if the drive have PIO data to transfer
 */
static bool is_busy(void) {
    return (inb(ATA_REG_STATUS) & CHECK_MASK) != ATA_STATUS_DRQ;
}

/*
 * Wait until disk is ready to transfer data
 */
static void wait_disk(void) {
    while(is_busy())
        ;
}

/*
 * Read ATA disk sector from logical sector into buffer using PIO
 */
void read_disk_sector(void *buffer, u32 logical_sector) {

    /* Send a read command to boot drive (Master) */
    outb(ATA_REG_SECTOR_COUNT, 1);
    outb(ATA_REG_LBA_LOW, LBA_BYTE0(logical_sector));
    outb(ATA_REG_LBA_MID, LBA_BYTE1(logical_sector));
    outb(ATA_REG_LBA_HIGH, LBA_BYTE2(logical_sector));
    outb(ATA_REG_DRIVE, ATA_LBA_MODE | LBA_BYTE3(logical_sector));
    outb(ATA_REG_COMMAND, ATA_CMD_READ);

    /* Poll disk and read data into buffer */
    wait_disk();
    insd(ATA_REG_DATA, buffer, ATA_SECTOR_SIZE / 4);
}