#ifndef IDT_H
#define IDT_H

void isr_zero_handler_stub();
void isr_pit_handler_stub();
void isr_keyboard_handler_stub();
void isr_syscall_handler_stub();
void IdtInstall();

#endif