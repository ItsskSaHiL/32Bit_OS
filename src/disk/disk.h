#ifndef DISK_H
#define DISK_H

/* Include for File System */
#include "fs/file.h"

typedef unsigned int MYOS_DISK_TYPE;

// Physical Hard Drive Count
#define MYOS_DISK_TYPE_REAL 0

struct disk
{
    MYOS_DISK_TYPE type;
    int sector_size;
    
    /*
    * Add File System In Disk
    */
   struct filesystem *filesystem;
};

void disk_search_and_init();
struct disk *disk_get(int index);
int disk_read_block(struct disk *idisk,unsigned int lba,int total, void *buf);
#endif