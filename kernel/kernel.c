#include "include/kernel.h"
#include "include/console.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pcb.h"

void process1()
{
    printk("JADKLJALKÑJ DASKLDJ ALÑKS JKLASJ DLKj dlkJ DLKÑj lkd lñK KLAJÑ SFADSLKF ASDLK");
    while (1) {__asm__ volatile ("sti");}
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

    struct pcb* process = initNewProcess(1, (uint32_t)process1);
    process->state = Running;
    // addTSS(process);
    
    volatile struct
    {
        uint32_t offset;
        uint16_t selector;

    } __attribute__((packed)) _tmp;
    
    _tmp.offset = 0;
    _tmp.selector = (process->pid + 3) * 8;

        __asm__ volatile ("sti");


    __asm__ volatile ("ljmp %0" :: "m"(_tmp));

    while (1)
    {
        __asm__ volatile ("hlt");
       // yield();
        // execCurrentProcess();
    }
    

}
