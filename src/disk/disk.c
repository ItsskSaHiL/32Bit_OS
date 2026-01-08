#include "disk.h"
#include "io/io.h"
#include "memory/memory.h"
#include "config.h"
#include "status.h"

// Variable 
struct disk disk;

int disk_read_sector(int lba, int total, void *buf)
{
    /* Don't Forgot LBA - > Logical Block Address HDD */
    /* Ready IDE Driver */
    outb(0x1F6,(lba >> 24) | 0xE0);             // Drive select + LBA bits
    outb(0x1F2, total);                         // Number of sectors
    outb(0x1F3, (unsigned char)(lba & 0xff));   // LBA bits 0–7
    outb(0x1F4,(unsigned char)(lba >> 8));      // LBA bits 8–15
    outb(0x1F5, (unsigned char)(lba >> 16));    // LBA bits 16–23
    outb(0x1F7, 0x20);                          // Device status

    // Local
    unsigned short *ptr = (unsigned short *)buf;

    // Read Total no of Block
    for(int b = 0;b < total;b++)
    {
        // wait for buffer to be ready
        char c = insb(0x1F7);
        while (!(c & 0x08))
        {
            c = insb(0x1F7);
        }

        // Copy for HDD to buffer(memroy)
        for(int i = 0;i < 256;i++)
        {
            *ptr = insw(0x1F0);     //Sector data (16-bit)
            ptr++;
        }
        
    }
    return 0;
}


// 
void disk_search_and_init()
{
    memset(&disk,0,sizeof(struct disk));
    disk.type = MYOS_DISK_TYPE_REAL;
    disk.sector_size = MYOS_SECTOR_SIZE;
    /*
    * Add This For File System 
    */
   disk.filesystem = fs_resolve(&disk);
}

struct disk *disk_get(int index)
{
    if(index != 0)
    {
        return 0;
    }
    return &disk;
}

int disk_read_block(struct disk *idisk,unsigned int lba,int total, void *buf)
{
    if(idisk != &disk)
    {
        return -EIO;
    }
    return disk_read_sector(lba,total,buf);
}