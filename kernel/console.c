#include "include/string.h"
#include "include/io.h"
#include "include/console.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_FONT 0x0F

struct vga
{
    uint16_t* framebuffer;
    int x;
    int y;
};

struct vga vga;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (insb(0x3D5) & 0xC0) | cursor_start);
    
    outb(0x3D4, 0x0B);
    outb(0x3D5, (insb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
    uint16_t pos = (y * VGA_WIDTH) + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void InitScreen()
{
    vga.framebuffer = (uint16_t*)0xB8000;
    enable_cursor(0, 15);
}

void put_c(char c, int x, int y)
{
    vga.framebuffer[(y * VGA_WIDTH) + x] = (DEFAULT_FONT << 8) | c;
}

void printk(const char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        char c = str[i];
        if (str[i] == '\n')
        {
            vga.x = 0;
            vga.y++;
            update_cursor(vga.x, vga.y);
        }

        else if (vga.x >= VGA_WIDTH)
        {
            vga.x = 0;
            vga.y++;
            update_cursor(vga.x+1, vga.y);
        }

        else if (vga.y >= VGA_HEIGHT)
        {
            clearScreen();
            printk(">");
        }

        else
        {
            put_c(c, vga.x, vga.y);
            update_cursor(vga.x + 1, vga.y);
            vga.x++;
        }
    }
}

void clearScreen()
{
    vga.x = 0;
    vga.y = 0;
    for (int y = 0; y < VGA_HEIGHT; ++y)
    {
        for (int x = 0; x < VGA_WIDTH; ++x)
        {
            put_c(' ', x, y);
        }
    }
}