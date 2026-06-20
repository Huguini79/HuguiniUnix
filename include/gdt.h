#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stddef.h>

void setSegmentDescriptor(int num, uint16_t limit, uint16_t base_first_0_15_bits, uint8_t base_15_23_bits, uint8_t access_byte, uint8_t flags, uint8_t base_23_31_bits);
void GdtInstall();

#endif