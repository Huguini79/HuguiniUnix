; Gdt
; The size of each descriptor is 64 bits

GdtStart:
    null:
        ; Fill all the descriptor with zeros
        dq 0

    code:
        dw 0xFFFF ; Limit (0..15 bits)
        dw 0 ; Base (0..15 bits)
        db 0 ; Base (16..23 bits)
        db 0x9B ; Access byte
        db 0xCF ; Flags
        db 0 ; Base (24..31 bits)

    data:        
        dw 0xFFFF ; Limit (0..15 bits)
        dw 0 ; Base (0..15 bits)
        db 0 ; Base (16..23 bits)
        db 0x93 ; Access byte
        db 0xCF ; Flags
        db 0 ; Base (24..31 bits)

GdtEnd:

; Gdt register
Gdtr:
    dw GdtEnd - GdtStart - 1 ; Limit
    dd GdtStart ; Base