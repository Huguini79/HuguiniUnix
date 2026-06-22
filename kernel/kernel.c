#include "include/kernel.h"
#include "include/console.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pcb.h"
#include "include/string.h"

struct pcb* process;
struct pcb* second_process;

void process2()
{
    char buff[16];
    printk("Hello, I am the second process PID=");
    itoa(getCurrentPID(), buff, 10);
    printk(buff);
    printk("\n");
    char* buf = "BUF";
    size_t count = 3;
    __asm__ volatile (
        "mov $4, %%eax\n\t"
        "mov $1, %%ebx\n\t"
        "mov %0, %%ecx\n\t"
        "mov %1, %%edx\n\t"
        "int $0x80\n\t"
        :
        : "r" (buf), "r"(count)
    );
    // yield()
}

void process1()
{
    char buf[16];
    printk("\nHello, I am the first process PID=");
    itoa(getCurrentPID(), buf, 10);
    printk(buf);
    printk("\n");
    second_process = initNewProcess(2, (uint32_t)process2);
    second_process->state = Running;
    process->state = Ready;
    // yield();
}

void displayRegisters()
{
    uint32_t eax, ebx, edx, ecx, edi, esi, esp;
    __asm__ volatile (
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%edx, %2\n\t"
        "movl %%ecx, %3\n\t"
        "movl %%edi, %4\n\t"
        "movl %%esi, %5\n\t"
        "movl %%esp, %6\n\t"
        : "=r"(eax), "=r"(ebx), "=r"(edx), "=r"(ecx), "=r"(edi), "=r"(esi), "=r"(esp)
        ::
    );

    printk("\n----------REGISTERS----------\n");

    char buf[256];

    itoa(eax, buf, 10);
    printk("EAX: ");
    printk(buf);
    printk("\n");
    
    itoa(ebx, buf, 10);
    printk("EBX: ");
    printk(buf);
    printk("\n");

    itoa(edx, buf, 10);
    printk("EDX: ");
    printk(buf);
    printk("\n");

    itoa(ecx, buf, 10);
    printk("ECX: ");
    printk(buf);
    printk("\n");

    itoa(edi, buf, 10);
    printk("EDI: ");
    printk(buf);
    printk("\n");

    itoa(esi, buf, 10);
    printk("ESI: ");
    printk(buf);
    printk("\n");

    itoa(esp, buf, 10);
    printk("ESP: ");
    printk(buf);
    printk("\n");

    printk("-----------------------------");
    
}

void kernel_main()
{
    InitScreen();
    clearScreen();
    printk("Welcome to HuguiniUnix\n\ntype help to see the command list\n\n> ");
    GdtInstall();
    IdtInstall();

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
    
    int n;
    n = exec(process);
    if (n == 0)
    {
        printk("\nProcess executed with success\n");
        displayRegisters();
        printk("\n\n> ");
    }

    while (1)
    {
        __asm__ volatile ("hlt");
    }
    

}
