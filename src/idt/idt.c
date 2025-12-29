#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

/* IDT Structure Defination */
struct idt_desc idt_descriptor[TOTAL_INTURRUPT_SUPORTED]; 
struct idtr_desc idtr_descriptor;

// IDT Load Global scope
extern void idt_load(struct idtr_desc *ptr);
extern void int21h();
extern void no_inturrupt();

// Keyboard Inturrupt
void int21h_handler()
{
    print("Keyboard Press !!!!\n");
    outb(0x20,0x20);
}

// No Inturrupt settinf
void no_inturrupt_handler()
{
    outb(0x20,0x20);
}

void idt_zero(void)
{
    print("Devide By Zero !!\n");
}

void idt_set(uint16_t inturrupt_no,void *address)
{
    struct idt_desc *desc = &idt_descriptor[inturrupt_no];
    desc->offset_1 = (uint32_t)address & 0x0000ffff;        // Get first 2 Byte address
    desc->selector = KERNEL_CODE_SELECTOR;                  // Segment Selector
    desc->zero = 0x00;                                      // Default is Zero
    desc->type_attr = 0xEE;         // 0xEE = 11101110 -> First one P = 1(Define Inturrupt), DPL = 3(0b11)(Define preveledge level ring 3) and 0xE -> define 32 bit Inturrupt gate
    desc->offset_2 = (uint32_t)address >> 16;               // Store the highest 16 bit of the address in offset 2
}

void idt_init(void)
{
    memset(idt_descriptor,0,sizeof(idtr_descriptor));       // Clear Memroy of IDT Descriptor
    idtr_descriptor.limit = sizeof(idt_descriptor) - 1;     // Total Size of IDT
    idtr_descriptor.base = (uint32_t)idt_descriptor;                  // Base address Of IDT

    // Init No Inturrupt
    for(int i = 0;i < TOTAL_INTURRUPT_SUPORTED;i++)
    {
        idt_set(i,no_inturrupt);
    }
    // Add Function Call
    idt_set(0,idt_zero);
    // Call the Keyboard Press Inturrupt
    idt_set(0x21,int21h);

    // Load IDT
    idt_load(&idtr_descriptor);

}