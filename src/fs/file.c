#include "file.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"

/* Decleare Total File System Supportd By My_OS */
struct filesystem *filesystems[MYOS_MAX_FILESYSTEM];
struct file_descriptor *file_descriptors[MY_OS_MAX_FILE_DESCRIPTOR];

/*
* This Function give Free FileSystem Buffer 
*/
static struct filesystem** fs_get_free_filesystem(void)
{
    int i = 0;
    for(i = 0;i < MYOS_MAX_FILESYSTEM;i++)
    {
        if(filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }
    return 0;
}

/*
* Inserting a File System to MY_OS File System Buffer 
*/
void fs_insert_filesystem(struct filesystem *filesystem)
{
    struct filesystem **fs;
    fs = fs_get_free_filesystem();
    if(!fs)
    {
        print("Problem Inserting File System\n");
        while(1) {}
    }   
    *fs = filesystem;
}

/*
* Load File System To Buffer
*/
static void fs_static_load(void)
{
    //fs_insert_filesystem(fat16_init());
}

/*
* Load Init Function
*/
void fs_load(void)
{
    memset(filesystems,0,sizeof(filesystems));
    fs_static_load();
}

/*
* File System Init
*/
void fs_init(void)
{
    memset(file_descriptors,0,sizeof(file_descriptors));
    fs_load();
}

/* 
* With This we can Open 512 different File 
* This File Descriptor Help to arrange that
*/
static int file_new_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;
    for(int i = 0;i < MY_OS_MAX_FILE_DESCRIPTOR;i++)
    {
        if(file_descriptors[i] == 0)
        {
            struct file_descriptor *dest = kzalloc(sizeof(struct file_descriptor));
            /*
            * File Descriptor Start at 1
            * Implement + 1 for that
            */
            dest->index = i + 1;
            file_descriptors[i] = dest;
            *desc_out = dest;
            res = 0;
            break;
        }
    }
    return res;
}

/*
* Get File Desctipor With FD value
*/
static struct file_descriptor* file_get_descriptor(int fd)
{
    int index = 0;
    if(fd <= 0 || fd >= MY_OS_MAX_FILE_DESCRIPTOR)
    {
        return 0;
    }

    /*
    * Using the same Logic as we give 
    * File Descriptor start form 1
    */
    index = fd - 1;
    return file_descriptors[index];
}

struct filesystem *fs_resolve(struct disk *disk)
{
    struct filesystem *fs = 0;
    for(int i = 0;i < MYOS_MAX_FILESYSTEM;i++)
    {
        if(filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            fs = filesystems[i];
            break;
        }
    }
    return fs;
}

int fopen(const char *filename,const char *mode)
{
    return -EIO;
}