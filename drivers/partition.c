#include "partition.h"
#include "disk.h"

int8_t partition_read_mbr(uint8_t drive, mbr_t* mbr) {
    return ATA_read_sectors(drive, 0, 1, mbr);
}

int8_t partition_enumerate(uint8_t drive, partition_t* partitions, int max_partitions) {
    mbr_t mbr;
    int count = 0;
    
    if (partition_read_mbr(drive, &mbr) != 0) {
        return -1;
    }
    
    if (mbr.signature != 0xAA55) {
        return -1;  // Invalid MBR
    }
    
    for (int i = 0; i < 4 && count < max_partitions; i++) {
        partition_entry_t* entry = &mbr.partitions[i];
        
        if (entry->type != 0 && entry->sector_count > 0) {
            partitions[count].drive = drive;
            partitions[count].partition_index = i;
            partitions[count].start_sector = entry->lba_start;
            partitions[count].sector_count = entry->sector_count;
            partitions[count].type = entry->type;
            count++;
        }
    }
    
    return count;
}