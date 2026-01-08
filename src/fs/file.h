#ifndef FILE_H
#define FILE_H

/* Includes */
#include "pparser.h"

typedef unsigned int FILE_SEEK_MODE;
enum
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END,
};

typedef unsigned int FILE_MODE;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID,
};

struct disk;
typedef void*(*FS_OPEN_FUNCTION)(struct disk *disk,struct path_part *path,FILE_MODE mode);
typedef int (*FILE_RESOLVE_FUNCTION)(struct disk *disk);

/* Create a Filesystem Structure */
struct filesystem
{
    /*
    * File system Resolve Function should not return 0
    * If the provided disk is use the File System
    * */
    FILE_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;

    /* File System Name*/
    char name[20];
};

/* Create File Descriptor */
struct file_descriptor
{
    /* Descriptor Index */
    int index;
    /* File System */
    struct filesystem *filesystem;
    /* Privet Data For Internal File Descriptor */
    void *private;
    /* Disk That File Descriptor should be use on */
    struct disk *disk;
};

/* Function Declearation */
void fs_init();
int fopen(const char *filename,const char *mode);
/* For */
void fs_insert_filesystem(struct filesystem *filesystem);
struct filesystem *fs_resolve(struct disk *disk);


#endif