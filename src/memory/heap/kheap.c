#include "kheap.h"
#include "config.h"
#include "heap.h"
#include "kernel.h"


/* Global Variable */
struct heap kernel_heap;    
struct heap_table kernel_heap_table;

void kheap_init()
{
    int total_table_entries = MYOS_HEAP_SIZE_BYTE/MYOS_HEAP_BLOCK_SIZE;                 // Get Total Entries
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(MYOS_HEAP_TABLE_ADDRESS);     // Initilize the Start address of Heap Table
    kernel_heap_table.total = total_table_entries;                                      // Total Table Entries

    void *end = (void*)(MYOS_HEAP_ADDRESS+MYOS_HEAP_SIZE_BYTE);                         // Calclulate End Address of Heap
    int res = heap_create(&kernel_heap,(void*)(MYOS_HEAP_ADDRESS),end,&kernel_heap_table);
    if(res < 0)
    {
        print("Failed To Creat Heap ###---\n");
    }
}

void* kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap,size);
}

void kfree(void *ptr)
{
    heap_free(&kernel_heap,ptr);
}