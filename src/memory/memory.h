#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/* Own Memset Function */
void *memset(void *ptr,int c,int size);
int memcmp(void *c1,void *c2,int count);
#endif