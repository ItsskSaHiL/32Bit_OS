#include "fat.h"
#include "status.h"
#include "kernel.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "string/string.h"
#include "disk/disk.h"

/* FAT12 specific data */
struct fat_private
{
    struct fat_header header;
    unsigned int fat_read_buffer;
    unsigned int fat_cluster_read_buffer;
    unsigned int cluster_size;
    unsigned int root_dir_sector;
    unsigned int fat_sector;
};

int fat_resolve(struct disk *disk)
{
    int res = 0;
    struct fat_private *fat_private = kzalloc(sizeof(struct fat_private));
    struct fat_header header;

    // Read the boot sector
    if (disk_read_block(disk, 0, 1, &header) != 0)
    {
        res = -EIO;
        goto out;
    }

    // Check for FAT signature (should have 0x55AA at offset 510)
    // For now, we'll assume FAT if bytes per sector is valid
    if (header.bytes_per_sector == 0)
    {
        res = -EFSNOTUS;
        goto out;
    }

    // Save the header
    fat_private->header = header;
    fat_private->cluster_size = header.sectors_per_cluster * header.bytes_per_sector;
    fat_private->fat_sector = header.reserved_sectors;
    fat_private->root_dir_sector = header.reserved_sectors + (header.num_fats * header.sectors_per_fat);

    disk->fs_private = fat_private;
    disk->filesystem = 0;

    res = 0;
out:
    if (res != 0)
    {
        kfree(fat_private);
    }
    return res;
}

void *fat_open(struct disk *disk, struct path_part *path, FILE_MODE mode)
{
    // TODO: Implement file opening
    return 0;
}

struct filesystem *fat_init()
{
    struct filesystem *fs = kzalloc(sizeof(struct filesystem));
    fs->resolve = fat_resolve;
    fs->open = fat_open;
    // Copy filesystem name manually
    const char *name = "FAT";
    int i = 0;
    while (name[i] && i < 19)
    {
        fs->name[i] = name[i];
        i++;
    }
    fs->name[i] = 0;
    return fs;
}
