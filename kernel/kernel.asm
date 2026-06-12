BITS 32

%define KERNEL_DATA_SELECTOR 0x10

global _start
extern kernel_main

_start:
    ; Ok, we are in protected mode, but we need to reload the data segments with the KERNEL_DATA_SELECTOR
    mov ax, KERNEL_DATA_SELECTOR
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov ds, ax
    mov gs, ax
    mov ss, ax

    call kernel_main

    jmp $