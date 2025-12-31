section .asm

extern int21h_handler
extern no_inturrupt_handler

global idt_load
global int21h
global no_inturrupt
global enable_inturrupt
global disable_inturrupt

enable_inturrupt:
    sti
    ret

disable_inturrupt:
    cli
    ret

idt_load:
    push ebp            ; Push the Current Base Pointer to the Stack
    mov ebp, esp        ; Move current Stack pointer to base ponter
    mov ebx, [ebp+8]    ; Move the base pointer to the Caller function give some explanation on this
    ; This is cause the ebp point to the ebp call itself and ebp+4 return address of the calle function ebp point to the Call function
    lidt [ebx]          ; Load the IDT
    pop ebp
    ret

; Make IDT 21 
int21h:
    cli
    pushad              ; Push all the current register in stack
    call int21h_handler ; Call he handler function
    popad               ; Pop all the store register 
    sti
    iret

; No inturrupt set
no_inturrupt:
    cli
    pushad
    call no_inturrupt_handler
    popad
    sti
    iret