#ifndef FAT_H
#define FAT_H

#include "file.h"

/* FAT Boot Sector Structure */
struct fat_header
{
    unsigned char jmp[3];
    unsigned char oem[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sectors;
    unsigned char num_fats;
    unsigned short root_dir_entries;
    unsigned short total_sectors_small;
    unsigned char media_descriptor;
    unsigned short sectors_per_fat;
    unsigned short sectors_per_track;
    unsigned short heads;
    unsigned int hidden_sectors;
    unsigned int total_sectors_large;
} __attribute__((packed));

/* FAT Directory Entry */
struct fat_directory_item
{
    unsigned char filename[8];
    unsigned char ext[3];
    unsigned char attr;
    unsigned char reserved;
    unsigned char creation_time_ms;
    unsigned short creation_time;
    unsigned short creation_date;
    unsigned short access_date;
    unsigned short high_cluster;
    unsigned short write_time;
    unsigned short write_date;
    unsigned short low_cluster;
    unsigned int filesize;
} __attribute__((packed));

/* FAT File Descriptor Private Data */
struct fat_file_descriptor
{
    unsigned int cluster;
    unsigned int position;
    unsigned int filesize;
};

/* Function Declarations */
struct filesystem *fat_init();
int fat_resolve(struct disk *disk);
void *fat_open(struct disk *disk, struct path_part *path, FILE_MODE mode);

#endif
