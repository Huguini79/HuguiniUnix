BITS 32

section .text

global GdtInstall

GdtStart:
    null:
        dq 0
    
    kernel_code:
        dw 0xFFFF
        dw 0
        db 0
        db 0x9B
        db 0xCF
        db 0

    kernel_data:
        dw 0xFFFF
        dw 0
        db 0
        db 0x93
        db 0xCF
        db 0

GdtEnd:

Gdtr:
    dw GdtEnd - GdtStart - 1
    dd GdtStart

GdtInstall:
    lgdt [Gdtr]
    jmp 0x08:reload_segments

reload_segments:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov es, ax
    ret