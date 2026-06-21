#include "include/io.h"
#include "include/console.h"
#include "include/string.h"
#include "include/pcb.h"
#include "include/syscall.h"
#include "include/keyboard.h"

void isr_zero_handler()
{
    printk("KERNEL PANIC: DIVIDE BY ZERO ERROR");
    while (1) {}
}

volatile uint32_t ticks = 0;

void isr_pit_handler()
{
    ticks += 1;
    outb(0x20, 0x20);
}

void isr_keyboard_handler()
{
    keyboard_handler();
}

void isr_syscall_handler(uint32_t eax, uint32_t ebx, uint32_t edx, uint32_t ecx)
{
    syscall_handler(eax, ebx, edx, ecx);
}