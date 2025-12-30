#ifndef HEAP_H
#define HEAP_H

/* Add Includes */
#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_TABLE_ENTRY_TAKEN  0x01        // Represent Block is Allocated
#define HEAP_BLOCK_TABLE_TABLE_ENTRY_FREE   0x00        // Represent Block is Free or Not

#define HEAP_BLOCK_HAS_NEXT     0b10000000              // First Bit Represent more than one 4096 byte allocated
#define HEAP_BLOCK_IS_FIRST     0b01000000              // Second Bit Represent First Block of Allocated Memory

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;           // Easy Read 

/* Access Heap Table */
struct heap_table
{
    HEAP_BLOCK_TABLE_ENTRY *entries;
    size_t total;
};

struct heap
{
    /* Address of Heap Table */
    struct heap_table* table;
    /* Start Address of Heap Pool */
    void *saddr;
};

/* Function Decleartion */
int heap_create(struct heap* heap,void* ptr,void *end,struct heap_table* table);
void* heap_malloc(struct heap* heap,size_t size);
void heap_free(struct heap* heap,void *ptr);

#endif