#include "../lib/stdint.h"
#include "disk.h"



void disk_read_sectors(uint32_t lba, void *buffer) {
    outb(ATA_PRIMARY_IO + 2, 1);                            // Writes 1 (number of sectors to read) on port 0x1F2
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba));               // LBA low
    outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));          // LBA mid
    outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));         // LBA high
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));  // Drive/head
    outb(ATA_PRIMARY_IO + 7, 0x20);                         // Read sectors command

    // Wait for BSY=0 and DRQ=1
    while (1) {
        uint8_t status = inb(ATA_PRIMARY_IO + 7);
        if ((status & 0x08) && !(status & 0x80)) break;
    }

    uint16_t* buf16 = (uint16_t*) buffer;
    for(size_t i = 0; i < ATA_SECTOR_SIZE/2; i)
        buf16[i] = inw(ATA_PRIMARY_IO);
    

}