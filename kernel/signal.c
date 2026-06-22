#include "include/signal.h"
#include "include/pcb.h"

#include <stdint.h>
#include <stddef.h>

void signal(uint32_t signal)
{
    __asm__ volatile ("cli"); // Disable interrupts
    struct pcb* currentProcess = returnCurrentProcess(); /* Get current process */
    currentProcess->signal |= 1U << (signal); /* Perform a bit in the process signals */
    __asm__ volatile ("sti"); // Enable interrupts
}