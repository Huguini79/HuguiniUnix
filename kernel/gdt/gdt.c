#include <stdint.h>
#include <stddef.h>

struct Gdt
{
    uint16_t limit;
    uint16_t base_first_0_15_bits;
    uint8_t base_15_23_bits;
    uint8_t access_byte;
    uint8_t flags;
    uint8_t base_23_31_bits;

} __attribute__((packed));

struct Gdtr
{
    uint16_t limit;
    uint32_t base;

} __attribute__((packed));

struct Gdtr gdtr;

struct Gdt AllDescriptors[8192]__attribute__((aligned(8)));

void setSegmentDescriptor(int num, uint16_t limit, uint16_t base_first_0_15_bits, uint8_t base_15_23_bits, uint8_t access_byte, uint8_t flags, uint8_t base_23_31_bits)
{
    struct Gdt* Gdt = &AllDescriptors[num];
    Gdt->limit = limit;
    Gdt->base_first_0_15_bits = base_first_0_15_bits;
    Gdt->base_15_23_bits = base_15_23_bits;
    Gdt->access_byte = access_byte;
    Gdt->flags = flags;
    Gdt->base_23_31_bits = base_23_31_bits;
}

void GdtInstall()
{
    setSegmentDescriptor(0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); /* Null */
    setSegmentDescriptor(1, 0xFFFF, 0x00, 0x00, 0x9B, 0xCF, 0x00); /* Code */
    setSegmentDescriptor(2, 0xFFFF, 0x00, 0x00, 0x93, 0xCF, 0x00); /* Data */

    setSegmentDescriptor(3, 0, 0, 0, 0, 0, 0); /* Selector 0x18: TSS Actual */
    setSegmentDescriptor(4, 0, 0, 0, 0, 0, 0); /* Selector 0x20: TSS Destino */


    gdtr.limit = sizeof(AllDescriptors) - 1;
    gdtr.base = (uint32_t)AllDescriptors;

    __asm__ volatile ("lgdt %0" :: "m"(gdtr));

}