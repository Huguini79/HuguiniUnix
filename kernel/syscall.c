#include "include/kernel.h"
#include "include/console.h"
#include "include/io.h"
#include "include/string.h"
#include "include/pcb.h"

void sys_fork()
{
    fork();
}

void syscall_handler(uint32_t eax, uint32_t ebx, uint32_t edx, uint32_t ecx)
{
    if (eax == 1) {
        printk("sys_exit()");

    } else if (eax == 2)
    {
        sys_fork();

    } else if (eax == 4)
    {
        printk((char*)ecx);
    }
    
    else
    {
        char buf[256];
        itoa(eax, buf, 10);
        printk("EAX: ");
        printk(buf);
        printk("\n");
    }
}