#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/heap.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "memory/memory.h"
#include "string/string.h"
#include "fs/pparser.h"

// Check IDT 
extern void problem();
// Use for page table
static struct paging_4gb_chunk *kernel_chunk;


uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    
    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}
void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}

void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

void kernel_main()
{
    terminal_initialize();
    print("Hello world!\ntest");

    // Malloc Initilize
    kheap_init();

    // Search and Init Disk
    disk_search_and_init();

    // Initilize IDT
    idt_init();

    // Set up Page table
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    // Switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    // Enable paging
    enable_paging();

    //    // Geting data form HDD 
    // char buf[512];
    // disk_read_sector(0,1,buf); 

    // Enable Inturrupt after IDT initilize
    enable_inturrupt();

    // Implement PathParser
    struct path_root *root_path = pathparser_parse("1:/home/desktop/personal/My_Os",NULL);

    if(root_path)
    {
        
    }
                                                                                              
    // Call IDT                    
    // problem();

    // Implement the paging Example                               
    // char *ptr = (char*)kzalloc(4096);
    // // Set the Page things
    // paging_set(paging_4gb_chunk_get_directory(kernel_chunk),(void*)0x1000,(uint32_t)ptr | PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    // // set the things
    // char *ptr1 = (char *)0x1000;
    // ptr1[0] = 'A';
    // ptr1[1] = 'B';
    // ptr1[2] = 'C';

    // print(ptr1);

    // print("\n");

    // print(ptr);

    //outb(0x60,0xff);
//     void *ptr = kmalloc(50);
//     void *ptr1 = kmalloc(5000);
//     void *ptr2 = kmalloc(5600);
//     kfree(ptr);
//     void *ptr3 = kmalloc(50);

//     if(ptr || ptr1 || ptr2 || ptr3)
//     {

//     }
}