#include "include/kernel.h"
#include "include/console.h"

void kernel_main()
{
    InitScreen();
    clearScreen();
    printk("Hello from HuguiniUnix");
}