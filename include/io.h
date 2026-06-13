#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stddef.h>

uint8_t insb(uint16_t port);
uint16_t insw(uint16_t port);
uint32_t inl(uint16_t port);

void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
void outl(uint16_t port, uint32_t data);

#endif