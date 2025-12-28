section .asm

global idt_load

idt_load:
    push ebp            ; Push the Current Base Pointer to the Stack
    mov ebp, esp        ; Move current Stack pointer to base ponter
    mov ebx, [ebp+8]    ; Move the base pointer to the Caller function give some explanation on this
    ; This is cause the ebp point to the ebp call itself and ebp+4 return address of the calle function ebp point to the Call function
    lidt [ebx]          ; Load the IDT
    pop ebp
    ret
    