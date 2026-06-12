#ifndef CONSOLE_H
#define CONSOLE_H

void InitScreen();
void put_c(char c, int x, int y);
void printk(const char* str);
void clearScreen();

#endif