#include "../lib/stdint.h"
#include "disk.h"



int8_t disk_read_sectors(int8_t drive, uint32_t start_sector, uint32_t count, void* buffer) {
    if(!(drive == 0 || drive == 1))
        return 0;

    uint8_t last_byte = ((start_sector >> 24) & 0b00001111) | 0b11100000 | (drive  << 4);


    outb(ATA_PRIMARY_IO + 2, count);                            // Writes 1 (number of sectors to read) on port 0x1F2
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(start_sector));               // LBA low
    outb(ATA_PRIMARY_IO + 4, (uint8_t)(start_sector >> 8));          // LBA mid
    outb(ATA_PRIMARY_IO + 5, (uint8_t)(start_sector >> 16));         // LBA high
    outb(ATA_PRIMARY_IO + 6, last_byte);
    outb(ATA_PRIMARY_IO + 7, 0x20);                         // Read sectors command


    for(uint32_t sector = 0; sector < count; sector++) {
        // Wait for drive to be ready for this sector
        while (1) {
            uint8_t status = inb(ATA_PRIMARY_IO + 7);
            if (status & 0b00000001) return 0; // Checks for error bit, returns 0 if set
            if ((status & 0b00001000) && !(status & 0x10000000)) break; // Checks if data is ready (DRQ bit set) and drive is not busy (BSY bit clear)
        }
        
        // Read one sector
        for(size_t i = 0; i < (ATA_SECTOR_SIZE / 2); i++) {
            buf16[sector * (ATA_SECTOR_SIZE / 2) + i] = inw(ATA_PRIMARY_IO);
        }
    }
    
    return 1;
}