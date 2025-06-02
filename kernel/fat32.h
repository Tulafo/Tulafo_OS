#pragma once

#include "../lib/stdint.h"
#include "../lib/stdbool.h"
#include "../drivers/partition.h"

#define FAT32_SIGNATURE         0xAA55
#define FAT32_CLUSTER_FREE      0x00000000
#define FAT32_CLUSTER_BAD       0x0FFFFFF7
#define FAT32_CLUSTER_EOC_MIN   0x0FFFFFF8
#define FAT32_CLUSTER_EOC_MAX   0x0FFFFFFF

#define DIR_ATTR_READ_ONLY      0x01
#define DIR_ATTR_HIDDEN         0x02
#define DIR_ATTR_SYSTEM         0x04
#define DIR_ATTR_VOLUME_ID      0x08
#define DIR_ATTR_DIRECTORY      0x10
#define DIR_ATTR_ARCHIVE        0x20
#define DIR_ATTR_LONG_NAME      (DIR_ATTR_READ_ONLY | DIR_ATTR_HIDDEN | DIR_ATTR_SYSTEM | DIR_ATTR_VOLUME_ID)

typedef struct {
    uint8_t jump_code[3];
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_entries;        // 0 for FAT32
    uint16_t total_sectors_16;    // 0 for FAT32
    uint8_t media_descriptor;
    uint16_t sectors_per_fat_16;  // 0 for FAT32
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    
    // FAT32 specific
    uint32_t sectors_per_fat_32;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t signature;
    uint32_t serial_number;
    char volume_label[11];
    char filesystem_type[8];
    uint8_t boot_code[420];
    uint16_t boot_signature;
} __attribute__((packed)) fat32_boot_sector_t;

typedef struct {
    char name[8];
    char extension[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time_tenth;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t cluster_high;
    uint16_t modification_time;
    uint16_t modification_date;
    uint16_t cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat32_dir_entry_t;


typedef struct {
    partition_t partition;
    fat32_boot_sector_t boot_sector;
    uint32_t fat_start_sector;
    uint32_t data_start_sector;
    uint32_t cluster_size_bytes;
    uint32_t* fat_cache;          // Cache for FAT table
    bool fat_cache_dirty;
} fat32_fs_t;

typedef struct {
    fat32_fs_t* fs;
    uint32_t current_cluster;
    uint32_t position;
    uint32_t size;
    bool is_directory;
    char name[256];
} fat32_file_t;

int fat32_mount(partition_t* partition, fat32_fs_t* fs);
void fat32_unmount(fat32_fs_t* fs);

int fat32_open(fat32_fs_t* fs, const char* path, fat32_file_t* file);
int fat32_read(fat32_file_t* file, void* buffer, size_t size);
int fat32_write(fat32_file_t* file, const void* buffer, size_t size);
void fat32_close(fat32_file_t* file);

int fat32_readdir(fat32_file_t* dir, fat32_dir_entry_t* entry);
