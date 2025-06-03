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
    uint32_t fat_offset = cluster * 4;                                                              // Relative FAT entry start
    uint32_t fat_sector = fs->fat_start_sector + (fat_offset / fs->boot_sector.bytes_per_sector);   // Sector of FAT entry
    uint32_t sector_offset = fat_offset % fs->boot_sector.bytes_per_sector;                         // Byte in secotr of fat entry
    
    // Read FAT sector if not cached
    if (!fs->fat_cache) {
        fs->fat_cache = malloc(fs->boot_sector.bytes_per_sector);
        if (!fs->fat_cache) return FAT32_CLUSTER_EOC_MAX;
    }
    
    if (ATA_read_sectors(fs->partition.drive, fat_sector, 1, fs->fat_cache) != 1) {
        return FAT32_CLUSTER_EOC_MAX;
    }
    
    uint32_t entry = *(uint32_t*)((uint8_t*)fs->fat_cache + sector_offset);
    return entry & 0x0FFFFFFF;  // Mask off upper 4 bits (clusters2 use a 28 bit index)
}

static uint32_t fat32_cluster_to_sector(fat32_fs_t* fs, uint32_t cluster) {
    if (cluster < 2) {
        return INVALID_SECTOR;  // Clusters 0,1 don't exist in data area
    }

    return fs->data_start_sector + (cluster - 2) * fs->boot_sector.sectors_per_cluster;     // Cluster 0 and 1 are reserved and
                                                                                            // don't physically exist in data section
}

int8_t fat32_mount(partition_t* partition, fat32_fs_t* fs) {
    if(memset(fs, 0, sizeof(fat32_fs_t)) == nullptr){
        return 0;
    }
    fs->partition = *partition;
    
    // Read boot sector
    if (disk_read_sectors(partition->drive, partition->start_sector, 1, &fs->boot_sector) != 1) {
        return -1;
    }
    
    // Validate FAT32 boot sector
    if (fs->boot_sector.boot_signature != FAT32_SIGNATURE) {
        return -1;
    }
    
    if (fs->boot_sector.sectors_per_fat_16 != 0 || fs->boot_sector.root_entries != 0) {
        return -1;  // Not FAT32
    }
    
    // Calculate important sectors
    fs->fat_start_sector = partition->start_sector + fs->boot_sector.reserved_sectors;
    fs->data_start_sector = fs->fat_start_sector + (fs->boot_sector.fat_count * fs->boot_sector.sectors_per_fat_32);
    fs->cluster_size_bytes = fs->boot_sector.sectors_per_cluster * fs->boot_sector.bytes_per_sector;
    
    return 0;
}