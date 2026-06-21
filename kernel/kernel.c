#include "include/kernel.h"
#include "include/console.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pcb.h"

struct pcb* process;
struct pcb* second_process;

void process2()
{
    printk("\nPROCESS 2");
    yield();
}

void process1()
{
    printk("\nPROCESS 1");
    second_process = initNewProcess(2, (uint32_t)process2);
    second_process->state = Running;
    process->state = Ready;
    yield();
    // exec(second_process);
}

void kernel_main()
{
    InitScreen();
    clearScreen();
    printk("Welcome to HuguiniUnix\n\ntype help to see the command list\n\n> ");
    GdtInstall();
    IdtInstall();
    //initMultitasking();
    //yield();

    struct tss tss;
    tss.esp0 = 0x600000;
    tss.ss0 = 0x10;
    uint32_t tss_address = (uint32_t)&tss;
    setSegmentDescriptor(3, sizeof(tss), (tss_address & 0xFFFF), (tss_address >> 16) & 0xFF, 0x89, 0x40, (tss_address >> 24) & 0xFF);

    __asm__ volatile (
        "mov $0x18, %ax\n\t"
        "ltr %ax\n\t"
    );

    process = initNewProcess(1, (uint32_t)process1);
    process->state = Running;
    
    int n = exec(process);
    if (n == 0)
    {
        printk("\nProcess executed with success\n");
    }

    while (1)
    {
        __asm__ volatile ("hlt");
       // yield();
        // execCurrentProcess();
    }
    

}
