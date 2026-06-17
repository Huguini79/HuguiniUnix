#include "include/io.h"
#include "include/console.h"
#include "include/string.h"
#include "include/pcb.h"

#define MAX_SIZE_BUFFER 1048576

char keyboard_buffer[MAX_SIZE_BUFFER];
int pos = 0;

void isr_zero_handler()
{
    printk("KERNEL PANIC: DIVIDE BY ZERO ERROR");
    while (1) {}
}

volatile uint32_t ticks = 0;

void isr_pit_handler()
{
    ticks += 1;
    outb(0x20, 0x20);
}

void addCharacter(char c)
{
    keyboard_buffer[pos++] = c;
    keyboard_buffer[pos] = '\0';
}

void isr_keyboard_handler()
{
    uint8_t scancode = insb(0x60);

    if (scancode == 0x1E) {printk("a"); addCharacter('a');}
    if (scancode == 0x30) {printk("b"); addCharacter('b');}
    if (scancode == 0x2E) {printk("c"); addCharacter('c');}
    if (scancode == 0x20) {printk("d"); addCharacter('d');}
    if (scancode == 0x12) {printk("e"); addCharacter('e');}
    if (scancode == 0x21) {printk("f"); addCharacter('f');}
    if (scancode == 0x22) {printk("g"); addCharacter('g');}
    if (scancode == 0x23) {printk("h"); addCharacter('h');}
    if (scancode == 0x17) {printk("i"); addCharacter('i');}
    if (scancode == 0x24) {printk("j"); addCharacter('j');}
    if (scancode == 0x25) {printk("k"); addCharacter('k');}
    if (scancode == 0x26) {printk("l"); addCharacter('l');}
    if (scancode == 0x32) {printk("m"); addCharacter('m');}
    if (scancode == 0x31) {printk("n"); addCharacter('n');}
    if (scancode == 0x18) {printk("o"); addCharacter('o');}
    if (scancode == 0x19) {printk("p"); addCharacter('p');}
    if (scancode == 0x10) {printk("q"); addCharacter('q');}
    if (scancode == 0x13) {printk("r"); addCharacter('r');}
    if (scancode == 0x1F) {printk("s"); addCharacter('s');}
    if (scancode == 0x14) {printk("t"); addCharacter('t');}
    if (scancode == 0x16) {printk("u"); addCharacter('u');}
    if (scancode == 0x2F) {printk("v"); addCharacter('v');}
    if (scancode == 0x11) {printk("w"); addCharacter('w');}
    if (scancode == 0x2D) {printk("x"); addCharacter('x');}
    if (scancode == 0x15) {printk("y"); addCharacter('y');}
    if (scancode == 0x2C) {printk("z"); addCharacter('z');}

    if (scancode == 0x39) {printk(" "); addCharacter(' ');}

    if (scancode == 0x1A) {printk("["); addCharacter('[');}
    if (scancode == 0x1B) {printk("]"); addCharacter(']');}
    if (scancode == 0x0D) {printk("="); addCharacter('=');}
    if (scancode == 0x28) {printk("'"); addCharacter('"');}
    if (scancode == 0x29) {printk("`"); addCharacter('`');}
    if (scancode == 0x0C) {printk("-"); addCharacter('-');}
    if (scancode == 0x34) {printk("."); addCharacter('.');}
    if (scancode == 0x33) {printk(","); addCharacter(',');}
    if (scancode == 0x35) {printk("/"); addCharacter('/');}
    if (scancode == 0x27) {printk(";"); addCharacter(';');}
    if (scancode == 0x0F) {printk("     "); addCharacter(' ');}

    if (scancode == 0x1C) {
        if (strncmp(keyboard_buffer, "help", 4) == 0)
        {
            printk("\nclear                     help\necho                        ps\n");

        } else if (strncmp(keyboard_buffer, "", 1) == 0)
        {
            printk("\n");

        } else if (strncmp(keyboard_buffer, "clear", 5) == 0)
        {
            clearScreen();
        }

        else if (strncmp(keyboard_buffer, "fork", 4) == 0)
        {
            fork();
            yield();
            printk("\n");
        }

        else if (strncmp(keyboard_buffer, "echo", 4) == 0)
        {
            printk("\n");
            char ret[2];
            int pos = 5;
            ret[1] = '\0';
            while (keyboard_buffer[pos] != '\0')
            {
                ret[0] = keyboard_buffer[pos];
                printk(ret);
                pos++;
            }
            printk("\n");
        }

        else if (strncmp(keyboard_buffer, ";", 1) == 0)
        {
            printk("\n");
        }

        else if (strncmp(keyboard_buffer, "ps", 2) == 0)
        {
            printk("\n");
            showAllProcesses();
            printk("\n");
        }
        
        else
        {
            printk("\n");
            printk(keyboard_buffer);
            printk(": command not found\n");
        }
        pos = 0;
        keyboard_buffer[pos] = '\0';
        printk("> ");
    }
    outb(0x20, 0x20);
}

void isr_syscall_handler()
{
    printk("SYSCALL CALLED");
}