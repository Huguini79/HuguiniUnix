#include "include/kernel.h"
#include "include/console.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pcb.h"

void kernel_main()
{
    InitScreen();
    clearScreen();
    printk("Welcome to HuguiniUnix\n\ntype help to see the command list\n\n> ");
    GdtInstall();
    IdtInstall();
    initMultitasking();
    yield();

    __asm__ volatile ("sti");
    
    while (1)
    {
        __asm__ volatile ("hlt");
        yield();
        // execCurrentProcess();
    }
    

}
