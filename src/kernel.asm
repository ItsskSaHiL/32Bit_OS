[BITS 32]

global _start
extern kernel_main
global problem      ;Define the IDT in Global Scope

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    call kernel_main

    jmp $

; Check IDT
problem:
    int 0       ; Call inturrupt 0

times 512-($ - $$) db 0