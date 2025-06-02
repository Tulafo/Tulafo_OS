#pragma once

#include "../lib/stdbool.h"
#include "../lib/stdint.h"

#define MAX_MOUNT_POINTS    16
#define MAX_PATH_LENGTH     256

typedef enum {
    FS_TYPE_FAT32,

} fs_type_t;

typedef struct vfs_node {
    char name[64];
    uint32_t inode;
    uint32_t size;
    uint32_t flags;
    struct vfs_node* parent;
    struct vfs_node* children;
    struct vfs_node* sibling;
    void* fs_specific;        // Filesystem-specific data
} vfs_node_t;

typedef struct {
    char mount_point[64];
    fs_type_t type;
    void* fs_data;           // Points to fat32_fs_t, ext2_fs_t, etc.
    vfs_node_t* root_node;
} mount_point_t;

typedef struct {
    mount_point_t mount_points[MAX_MOUNT_POINTS];
    int mount_count;
    vfs_node_t* root;        // Root of the entire filesystem tree
} vfs_t;


int vfs_init(vfs_t* vfs);


int vfs_mount(vfs_t* vfs, const char* mount_point, fs_type_t type, void* fs_data);
int vfs_unmount(vfs_t* vfs, const char* mount_point);

int vfs_open(vfs_t* vfs, const char* path, int flags);
int vfs_read(int fd, void* buffer, size_t size);
int vfs_write(int fd, const void* buffer, size_t size);
int vfs_close(int fd);