BITS 32

section .text

global isr_zero_handler_stub
global isr_pit_handler_stub
global isr_keyboard_handler_stub
global isr_syscall_handler_stub

extern isr_zero_handler
extern isr_pit_handler
extern isr_keyboard_handler
extern isr_syscall_handler

isr_zero_handler_stub:
    pusha
    call isr_zero_handler
    popa
    iretd

isr_pit_handler_stub:
    pusha
    call isr_pit_handler
    popa
    iretd

isr_keyboard_handler_stub:
    pusha
    call isr_keyboard_handler
    popa
    iretd

isr_syscall_handler_stub:
    pusha
    call isr_syscall_handler
    popa
    iretd