#include "include/console.h"
#include "include/idt.h"
#include "include/isr.h"

#include <stdint.h>
#include <stddef.h>

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define MAX_INTERRUPTS 256

struct Idt
{
    uint16_t isr_offset_0_15_bits; 
    uint16_t selector;
    uint8_t reserved;
    uint8_t type_attributes;
    uint16_t isr_offset_16_31_bits;
    
} __attribute__((packed));

struct Idtr
{
    uint16_t limit;
    uint32_t base;

} __attribute__((packed));

struct Idtr Idtr;

struct Idt Interrupt_Descriptor[MAX_INTERRUPTS];

void setInterruptGate(int vector, void* isr)
{
    struct Idt* Idt = &Interrupt_Descriptor[vector]; /* A pointer to the IDT Descriptor */
    Idt->isr_offset_0_15_bits = (uint32_t)isr & 0xFFFF; /* First 16 bits of the ISR */
    Idt->selector = KERNEL_CODE_SELECTOR; /* KERNEL CODE SELECTOR - 16 BITS VALUE */
    Idt->reserved = 0b00000000; /* RESERVED, ALWAYS 0 */
    Idt->type_attributes = 0x8E; /* 32 bit Interrupt Gate */
    Idt->isr_offset_16_31_bits = (uint32_t)isr >> 16 & 0xFFFF; /* Last 16 bits of the ISR */
}

void setSoftwareTrap(int vector, void* isr)
{
    struct Idt* Idt = &Interrupt_Descriptor[vector]; /* A pointer to the IDT Descriptor */
    Idt->isr_offset_0_15_bits = (uint32_t)isr & 0xFFFF; /* First 16 bits of the ISR */
    Idt->selector = KERNEL_CODE_SELECTOR; /* KERNEL CODE SELECTOR - 16 BITS VALUE */
    Idt->reserved = 0b00000000; /* RESERVED, ALWAYS 0 */
    Idt->type_attributes = 0x8F; /* 32 bit Software Trap */
    Idt->isr_offset_16_31_bits = (uint32_t)isr >> 16 & 0xFFFF; /* Last 16 bits of the ISR */
}

void IdtInstall()
{
    setInterruptGate(0x00, isr_zero_handler_stub);
    setInterruptGate(0x20, isr_pit_handler_stub);
    setInterruptGate(0x21, isr_keyboard_handler_stub);
    
    setSoftwareTrap(0x80, isr_syscall_handler_stub);
    Idtr.limit = sizeof(Interrupt_Descriptor) - 1;
    Idtr.base = (uint32_t)Interrupt_Descriptor;

    __asm__ volatile ("lidt %0" :: "m"(Idtr));
}