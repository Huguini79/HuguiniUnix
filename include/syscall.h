#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <stddef.h>

void syscall_handler(uint32_t eax, uint32_t ebx, uint32_t edx, uint32_t ecx);

#endif