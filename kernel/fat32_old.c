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
    
    if (ATA_read_sectors(fs->partition.drive, fat_sector, 1, fs->fat_cache) != 0) {
        return FAT32_CLUSTER_EOC_MAX;
    }
    
    uint32_t entry = *(uint32_t*)((uint8_t*)fs->fat_cache + sector_offset);
    return entry & 0x0FFFFFFF;  // Mask off upper 4 bits (clusters use a 28 bit index)
}

static uint32_t fat32_cluster_to_sector(fat32_fs_t* fs, uint32_t cluster) {
    if (cluster < 2) {
        return INVALID_SECTOR;  // Clusters 0,1 don't exist in data area
    }

    return fs->data_start_sector + (cluster - 2) * fs->boot_sector.sectors_per_cluster;     // Cluster 0 and 1 are reserved and
                                                                                            // don't physically exist in data section
}

int8_t fat32_mount(partition_t* partition, fat32_fs_t* fs) {
    if(memset(fs, 0, sizeof(fat32_fs_t)) == nullptr)            // Clears filesystem structure
        return 0;
    
    fs->partition = *partition;                                 // Copies partition information
    

    if (ATA_read_sectors(partition->drive, partition->start_sector, 1, &fs->boot_sector) != 1)     // Read boot sector
        return 0;
    
    

    if (fs->boot_sector.boot_signature != FAT32_SIGNATURE)     // Validate FAT32 boot sector
        return 0;

    
    if (fs->boot_sector.sectors_per_fat_16 != 0 || fs->boot_sector.root_entries != 0) // Not FAT32
        return 0;  

    
    // Calculate important sectors
    fs->fat_start_sector = partition->start_sector + fs->boot_sector.reserved_sectors;
    fs->data_start_sector = fs->fat_start_sector + (fs->boot_sector.fat_count * fs->boot_sector.sectors_per_fat_32);
    fs->cluster_size_bytes = fs->boot_sector.sectors_per_cluster * fs->boot_sector.bytes_per_sector;
    
    return 1;
}

void fat32_unmount(fat32_fs_t* fs) {
    if (fs->fat_cache) {
        free(fs->fat_cache);
        fs->fat_cache = nullptr;
    }
}

static int fat32_find_dir_in_cluster(fat32_fs_t* fs, uint32_t cluster, const char* name, fat32_dir_entry_t* result) {
    uint8_t* cluster_buffer = malloc(fs->cluster_size_bytes);   // Good because cluster size is already sector aligned (multiple of 512 bytes)
    if (cluster_buffer == nullptr) return 0;
    
    uint32_t sector = fat32_cluster_to_sector(fs, cluster);
    if (ATA_read_sectors(fs->partition.drive, sector, fs->boot_sector.sectors_per_cluster, cluster_buffer) != 1) {
        free(cluster_buffer);
        return 0;
    }
    
    fat32_dir_entry_t* entries = (fat32_dir_entry_t*)cluster_buffer;
    int entries_per_cluster = fs->cluster_size_bytes / sizeof(fat32_dir_entry_t);
    
    for (int i = 0; i < entries_per_cluster; i++) {
        if (entries[i].name[0] == 0) break;  // End of directory
        if (entries[i].name[0] == 0xE5) continue;  // Deleted entry
        if (entries[i].attributes == DIR_ATTR_LONG_NAME) continue;  // LFN entry
        
        // Create filename from 8.3 format
        char filename[13];
        int pos = 0;
        
        // Copy name part
        for (int j = 0; j < 8 && entries[i].name[j] != ' '; j++)
            filename[pos++] = entries[i].name[j];
        
        // Add extension if present
        if (entries[i].extension[0] != ' ') {
            filename[pos++] = '.';
            for (int j = 0; j < 3 && entries[i].extension[j] != ' '; j++) {
                filename[pos++] = entries[i].extension[j];
            }
        }
        filename[pos] = '\0';
        
        if (strcmp(filename, name) == 0) {
            *result = entries[i];
            free(cluster_buffer);
            return 1;
        }
    }
    
    free(cluster_buffer);
    return 0;
}


int fat32_open(fat32_fs_t* fs, const char* path, fat32_dir_t* file) {
    memset(file, 0, sizeof(fat32_dir_t));
    file->fs = fs;
    
    if (strcmp(path, "/") == 0) {
        // Open root directory
        file->current_cluster = fs->boot_sector.root_cluster;
        file->is_directory = true;
        strcpy(file->name, "/");
        return 0;
    }
    
    // Parse path and traverse directory structure
    uint32_t current_cluster = fs->boot_sector.root_cluster;
    char path_copy[256];
    strcpy(path_copy, path);
    
    char* token = strtok(path_copy, "/");
    fat32_dir_entry_t entry;
    
    while (token != NULL) {
        if (fat32_find_dir_in_cluster(fs, current_cluster, token, &entry) != 1) {
            return 0;  // File not found
        }
        
        current_cluster = (uint32_t)entry.cluster_high << 16 | entry.cluster_low;
        token = strtok(NULL, "/");
    }
    
    file->current_cluster = current_cluster;
    file->size = entry.file_size;
    file->is_directory = (entry.attributes & DIR_ATTR_DIRECTORY) != 0;
    strcpy(file->name, path);
    
    return 0;
}

int fat32_read(fat32_dir_t* file, void* buffer, size_t size) {
    if (file->position >= file->size) {
        return 0;  // EOF
    }
    
    size_t to_read = (size > file->size - file->position) ? file->size - file->position : size;
    size_t bytes_read = 0;
    uint8_t* buf = (uint8_t*)buffer;
    
    uint32_t cluster = file->current_cluster;
    uint32_t cluster_offset = file->position % file->fs->cluster_size_bytes;
    
    // Skip to correct cluster based on current position
    uint32_t clusters_to_skip = file->position / file->fs->cluster_size_bytes;
    for (uint32_t i = 0; i < clusters_to_skip; i++) {
        cluster = fat32_get_fat_entry(file->fs, cluster);
        if (cluster >= FAT32_CLUSTER_EOC_MIN) {
            return bytes_read;
        }
    }
    
    uint8_t* cluster_buffer = malloc(file->fs->cluster_size_bytes);
    if (!cluster_buffer) return 0;
    
    while (bytes_read < to_read && cluster < FAT32_CLUSTER_EOC_MIN) {
        uint32_t sector = fat32_cluster_to_sector(file->fs, cluster);
        if (ATA_read_sectors(file->fs->partition.drive, sector, file->fs->boot_sector.sectors_per_cluster, cluster_buffer) != 0) {
            break;
        }
        
        uint32_t bytes_in_cluster = file->fs->cluster_size_bytes - cluster_offset;
        uint32_t bytes_to_copy = (bytes_in_cluster > to_read - bytes_read) ? to_read - bytes_read : bytes_in_cluster;
        
        memcpy(buf + bytes_read, cluster_buffer + cluster_offset, bytes_to_copy);
        bytes_read += bytes_to_copy;
        cluster_offset = 0;  // Only first cluster might have offset
        
        cluster = fat32_get_fat_entry(file->fs, cluster);
    }
    
    free(cluster_buffer);
    file->position += bytes_read;
    return bytes_read;
}

int fat32_readdir(fat32_dir_t* dir, fat32_dir_entry_t* entry) {
    if (!dir->is_directory) {
        return -1;
    }
    
    // This is a simplified version - you'd want to implement proper directory iteration
    // For now, just return error to indicate no more entries
    return -1;
}

void fat32_close(fat32_dir_t* file) {
    memset(file, 0, sizeof(fat32_dir_t));
}