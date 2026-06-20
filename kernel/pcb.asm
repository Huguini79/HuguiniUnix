global getCurrentEIP

getCurrentEIP:
    call get_pc

get_pc:
    mov eax, [esp]
    ret