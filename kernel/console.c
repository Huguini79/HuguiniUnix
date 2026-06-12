#include "include/string.h"

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

void InitScreen()
{
    vga.framebuffer = (uint16_t*)0xB8000;
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
        if (str[i] == '\n')
        {
            vga.x = 0;
            vga.y++;
            put_c(' ', vga.x, vga.y);
        }

        if (vga.x >= VGA_WIDTH)
        {
            vga.x = 0;
            vga.y++;
        }

        if (vga.y >= VGA_HEIGHT)
        {
            vga.x = 0;
            vga.y = 0;
        }

        put_c(str[i], vga.x, vga.y);
        vga.x++;
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