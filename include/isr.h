#ifndef ISR_H
#define ISR_H

#include <stdint.h>
#include <stddef.h>

void isr_zero_handler();
void isr_pit_handler();
void isr_keyboard_handler();
void isr_syscall_handler(uint32_t eax, uint32_t ebx, uint32_t edx, uint32_t ecx);

#endif