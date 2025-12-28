#ifndef IDT_H
#define IDT_H

/* Includes */
#include <stdint.h>

// Create structure for IDT Entry
struct idt_desc
{
    uint16_t offset_1; // Offset bits 0 - 15
    uint16_t selector; // Selector thats in our GDT
    uint8_t zero; // Does nothing, unused set to zero
    uint8_t type_attr; // Descriptor type and attributes gate, DPL, P field
    uint16_t offset_2; // Offset bits 16-31 
}__attribute__((packed));

struct idtr_desc
{
    uint16_t limit; // Size of IDT - 1 -> give the size
    uint32_t base;  //  Start Address if IDT 
}__attribute__((packed));

void idt_init(void);


#endif