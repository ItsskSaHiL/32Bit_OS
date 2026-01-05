#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR    0x08            // Use for jump form IDT
#define KERNEL_DATA_SELECTOR    0x10            //
#define TOTAL_INTURRUPT_SUPORTED 512            //Total inturrupt supported by our IDT


/* Malloc Implementation */
#define MYOS_HEAP_SIZE_BYTE     104857600       // Total Heap Size 100MB
#define MYOS_HEAP_BLOCK_SIZE    4096            // One Block Size When allocation happen

#define MYOS_HEAP_ADDRESS       0x01000000      // Start address of Heap
#define MYOS_HEAP_TABLE_ADDRESS 0x00007E00      // Store the Heap Table in address not in array

#define MYOS_SECTOR_SIZE        512             // Hard Disk Sector Size


#endif