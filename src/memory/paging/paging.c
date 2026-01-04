#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"


// Defination and Declearation
static uint32_t *current_directory = 0;
void paging_load_directory(uint32_t *directory);


/* @brief
    * This function give a continous linear memory
    *
*/
/* 4Gb Chunk */
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags)
{
    // Local
    uint32_t offset = 0;
    /* Get 4Kb Directory */
    uint32_t *directory = kzalloc(sizeof(uint32_t)*PAGING_TOTAL_ENTRY_PER_TABLE);
    /* Itterate and Initilize */
    for(int i = 0;i < PAGING_TOTAL_ENTRY_PER_TABLE;i++)
    {
        /* Get Page Table each 4Kb */
        uint32_t *entry = kzalloc(sizeof(uint32_t)*PAGING_TOTAL_ENTRY_PER_TABLE);
        for(int b = 0; b < PAGING_TOTAL_ENTRY_PER_TABLE; b++)
        {
            /* Use for calculate the next address 
                * 0 + 0*4096 = >> 0
                * */
            entry[b] = (offset + (b*PAGING_PAGE_SIZE)) | flags;
        }
        /* Assign next page table address to Page Directory 
        *   page directorey  = 1
        *   offset = 1 *1024*4096 -> 0x400000 
        *   Which is store in next page table address
        * */
        offset += (PAGING_TOTAL_ENTRY_PER_TABLE*PAGING_PAGE_SIZE);

        /*
        *   Set the Directory to page table address
        *   Set the directory is writible so that only the directory is writible.
        */
       directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITABLE;
    }

    /* Create a variable to return the address */
    struct paging_4gb_chunk *chunk_4gb = (struct paging_4gb_chunk*)kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}

bool paging_is_alligned(void *addr)
{
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
}

// Getting the index for the provided virtual address 
int page_get_indexes(void *virtual_address,uint32_t *directory_index_out,uint32_t *table_index_out)
{
    int res = 0;
    if(!paging_is_alligned(virtual_address))
    {
        res = -EINVARG;
        goto out;
    }
    // Get the Page Table Directory Index
    *directory_index_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRY_PER_TABLE * PAGING_PAGE_SIZE));
    // Get the Page Table Indes
    *table_index_out = (((uint32_t)virtual_address % (PAGING_TOTAL_ENTRY_PER_TABLE * PAGING_PAGE_SIZE)) / PAGING_PAGE_SIZE);

out:
    return res;
}

int paging_set(uint32_t *directory, void *virt, int val)
{
    int res = 0;
    if(!paging_is_alligned(virt))
    {
        res = -EINVARG;
        return res;
    }

    uint32_t directory_index = 0;
    uint32_t table_index = 0;

    res = page_get_indexes(virt,&directory_index,&table_index);
    if(res < 0)
    {
        return res;
    }

    // Get Directory 
    uint32_t entry = directory[directory_index];
    // Get the table address 
    uint32_t *table = (uint32_t*)(entry & 0xfffff000);

    // set the virtual table to the main address
    table[table_index] = val;

    return res;
}


void paging_switch(uint32_t* directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk)
{
    return chunk->directory_entry;
}