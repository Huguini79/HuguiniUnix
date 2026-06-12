ORG 0x7c00
BITS 16

%define PE_BIT 0b00000001
%define KERNEL_CODE_SELECTOR 0x08
%define KERNEL_DATA_SELECTOR 0x10

start:
    ; Disable interrupts
    cli
    ; Initialize data segments to 0
    mov ax, 0
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov sp, 0x7c00 ; Initialize the stack
    
    lgdt [Gdtr] ; Load the Gdt
    ; Enter to protected mode
    mov ebx, cr0 ; Move the cr0 register to ebx
    or ebx, PE_BIT ; Enable the PE_BIT in ebx
    mov cr0, ebx ; Save the changes
    jmp KERNEL_CODE_SELECTOR:ProtectedMode ; FAR JUMP - We jump to 32 bit code

[BITS 32]
ProtectedMode:
    mov eax, 1
    mov ecx, 100 ; Number of sectors to read
    mov edi, 0x100000 ; The start of the kernel

    call lba_read ; Read sectors
    jmp KERNEL_CODE_SELECTOR:0x100000 ; Jump into the kernel

%include "boot/gdt/gdt.asm"
%include "boot/ata/ata.asm"

times 510 - ($ - $$) db 0 ; Fill the rest of the file with zeros
dw 0xAA55 ; Boot signature (this is 2 bytes)