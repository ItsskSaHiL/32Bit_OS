#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

// Includes
#include "disk.h"

struct disk_stream
{
    int pos;            // This is Byte Position
    struct disk *disk;  // Bind With Disk
};

struct disk_stream* diskstreamer_new(int disk_id);
int diskstreamer_seek(struct disk_stream *stream,int pos);
int diskstreamer_read(struct disk_stream *steram,void *out,int total);
void diskstream_close(struct disk_stream *stream);

#endif