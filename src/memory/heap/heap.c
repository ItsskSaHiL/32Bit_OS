#include "heap.h"
#include "status.h"
#include "kernel.h"
#include <stdbool.h>
#include "idt/idt.h"
#include "memory/memory.h"

/* Validate Heap Table */
static int heap_validate_table(void *ptr,void *end,struct heap_table *table)
{
    int res = 0;

    // Calculate Total Size
    size_t table_size = (size_t)(end - ptr);
    size_t total_block = table_size/MYOS_HEAP_BLOCK_SIZE;

    // Validate the Block Count 
    if(table->total != total_block)
    {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}

/* Check Valid Alignment */
static bool heap_validate_alignment(void *ptr)
{
    return ((unsigned int)ptr % MYOS_HEAP_BLOCK_SIZE) == 0;
}


int heap_create(struct heap* heap,void* ptr,void *end,struct heap_table* table)
{
    // Result Variable
    int res = 0;
    /* Check Start and End Address Both are Allign */
    if(!(heap_validate_alignment(ptr)) || !(heap_validate_alignment(end)))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap,0,sizeof(struct heap));
    heap->saddr = ptr;                          // Init Start Add
    heap->table = table;                        // Init Table Address

    /* Validate Table */


out:
    return res;
}

/* Calculate Total Byte Should Allocting */
static uint32_t heap_allign_value_to_upper(uint32_t val)
{
    if((val%MYOS_HEAP_BLOCK_SIZE) == 0)
    {
        return val;
    }
    val = (val - (val%MYOS_HEAP_BLOCK_SIZE));
    val += MYOS_HEAP_BLOCK_SIZE;
    return val;
}
static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0f;
}

int heap_get_start_block(struct heap *heap,uint32_t total_block)
{
    struct heap_table *table = heap->table;
    int bc = 0;
    int bs = -1;

    for(size_t i = 0;i < table->total;i++)
    {
        if(heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_TABLE_ENTRY_FREE)
        {
            bc = 0;
            bs = -1;
            continue;
        }
        // If It's First Block
        if(bs == -1)
        {
            bs = i;
        }
        bc++;
        if(bc ==  total_block)
        {
            break;
        }
    }
    if(bs == -1)
    {
        return -ENOMEM;
    }

    return bs;
}

void* heap_block_to_address(struct heap *heap,uint32_t block)
{
    return (heap->saddr +(block*MYOS_HEAP_BLOCK_SIZE));
}

void heap_mark_block_taken(struct heap* heap, uint32_t start_block,uint32_t total_block)
{
    uint32_t end_block = (start_block+total_block)-1;   // Get Total Block
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_block > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    for(int i = start_block;i <= end_block;i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_TABLE_ENTRY_TAKEN;
        if(i != end_block-1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void *heap_malloc_blocks(struct heap *heap,uint32_t total_block)
{
    void *address = 0;
    // Get Start Block
    int start_block = heap_get_start_block(heap,total_block);
    if(start_block < 0)
    {
        goto out;
    }
    // Get Address
    address = heap_block_to_address(heap,start_block);

    // Mark Block As Taken
    heap_mark_block_taken(heap,start_block,total_block);
out:
    return address;
}

/* Return Block NO Starting Block */
int heap_address_to_block(struct heap* heap, void *address)
{
    return ((address - heap->saddr) / MYOS_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_free(struct heap* heap,uint32_t starting_block)
{
    struct heap_table *table = heap->table;
    for(int i = starting_block; i < (int)table->total;i++)
    {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_TABLE_ENTRY_FREE;
        if(!(entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

void* heap_malloc(struct heap* heap,size_t size)
{
    // Get Allign Size
    size_t allign_size = heap_allign_value_to_upper(size);
    // Calculate Total Block 
    uint32_t total_block = allign_size / MYOS_HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap,total_block);
}

void heap_free(struct heap* heap,void *ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap,ptr));
}