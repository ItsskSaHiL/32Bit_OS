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

    ; Remap the Master PIC
    mov al, 00010001b       ;This is ICW1 -> Set them with bit pos
    out 0x20, al            ; Tell Master PIC

    mov al,0x20             ; Inturrupt Vector Offset
    out 0x21, al            ; Out to data port and when the inturrupt ocr the it set ot 0x20

    mov al, 00000001b       ; Set to 8086 PIC controller
    out 0x21, al            ; Set with data port

    ; Here sti remove cause after IDT we should start Inturrupt
    ;sti                     ; Start inturrupt "Check This"

    call kernel_main

    jmp $

; Check IDT
problem:
    int 0       ; Call inturrupt 0

times 512-($ - $$) db 0