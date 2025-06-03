#include "fat32.h"
#include "../drivers/disk.h"
#include "../lib/string.h"
#include "../kernel/memory.h"

static uint32_t fat32_get_fat_entry(fat32_fs_t* fs, uint32_t cluster) {
    // Check if cluster is above the number of available clusters 
    if (cluster >= (fs->boot_sector.total_sectors_32 - fs->data_start_sector) / fs->boot_sector.sectors_per_cluster) {
        return FAT32_CLUSTER_EOC_MAX;
    }
    
    // Calculate sector and offset within sector
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fs->fat_start_sector + (fat_offset / fs->boot_sector.bytes_per_sector);
    uint32_t sector_offset = fat_offset % fs->boot_sector.bytes_per_sector;
    
    // Read FAT sector if not cached
    if (!fs->fat_cache) {
        fs->fat_cache = malloc(fs->boot_sector.bytes_per_sector);
        if (!fs->fat_cache) return FAT32_CLUSTER_EOC_MAX;
    }
    
    if (disk_read_sectors(fs->partition.drive, fat_sector, 1, fs->fat_cache) != 1) {
        return FAT32_CLUSTER_EOC_MAX;
    }
    
    uint32_t entry = *(uint32_t*)((uint8_t*)fs->fat_cache + sector_offset);
    return entry & 0x0FFFFFFF;  // Mask off upper 4 bits
}