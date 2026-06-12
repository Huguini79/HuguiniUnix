; Logical Block Addressing (LBA)

lba_read:
    mov ebx, eax
    shr eax, 24
    or eax, 0xE0 ; Select master drive
    mov dx, 0x1F6
    out dx, al

    ; Number of sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al

    mov eax, ebx
    mov dx, 0x1F3
    out dx, al

    mov dx, 0x1F4
    mov eax, ebx
    shr eax, 8
    out dx, al

    mov dx, 0x1F5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1F7 ; Configure operation
    mov al, 0x20 ; Read operation
    out dx, al

.next_sector:
    push ecx

.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
        jz .try_again

    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ret