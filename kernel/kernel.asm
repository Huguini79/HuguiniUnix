BITS 32

%define KERNEL_DATA_SELECTOR 0x10

global _start
global remapPIC

extern kernel_main

_start:
    cli
    ; Ok, we are in protected mode, but we need to reload the data segments with the KERNEL_DATA_SELECTOR
    mov ax, KERNEL_DATA_SELECTOR
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov ds, ax
    mov gs, ax
    mov ss, ax

    call remapPIC

    call kernel_main

    hlt

remapPIC:
    ; ICW 1
    mov al, 0x11
    out 0x20, al ; Master PIC
    out 0xA0, al ; Slave PIC

    ; ICW 2
    mov al, 0x20 ; IRQ 0..7 mapped at interrupt vector 0x20
    out 0x21, al ; Master PIC

    mov al, 0x28 ; IRQ 8..15 mapped at interrupt vector 0x28
    out 0xA1, al ; Slave PIC

    ; ICW 3
    mov al, 0x4 ; IR Line 2
    out 0x21, al ; Master PIC

    mov al, 0x2 ; IR Line 2
    out 0xA1, al ; Slave PIC

    ; ICW 4
    mov al, 1 ; 80x86 mode
    out 0x20, al ; Master PIC
    out 0xA0, al ; Slave PIC

    ret