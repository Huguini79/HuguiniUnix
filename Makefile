ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

LD_FLAGS = -m elf_i386 -g -relocatable
ASM_FLAGS = -f bin
ASM_FLAGS2 = -f elf32
CC_FLAGS = -I./ -c -m32 -g -ffreestanding -fno-pie -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -Os -Iinc
HEAD = boot/head.asm -o build/head.bin
KERNEL_ASM = kernel/kernel.asm -o build/kernel.asm.o
KERNEL_C = kernel/kernel.c -o build/kernel.o
CONSOLE = kernel/console.c -o build/console.o
STRING = libc/string.c -o build/string.o

OBJECT_FILES = build/kernel.asm.o build/kernel.o build/console.o build/string.o

all:
	clear
	$(ASM) $(ASM_FLAGS) $(HEAD)
	$(ASM) $(ASM_FLAGS2) $(KERNEL_ASM)
	$(CC) $(CC_FLAGS) $(KERNEL_C)
	$(CC) $(CC_FLAGS) $(CONSOLE)
	$(CC) $(CC_FLAGS) $(STRING)
	$(LD) $(LD_FLAGS) $(OBJECT_FILES) -o build/kernelfull.o
	$(CC) -m32 -T linker/linker.ld -o build/kernel.bin -ffreestanding -Os -nostdlib build/kernelfull.o
	dd if=build/head.bin > HuguiniUnix.bin
	dd if=build/kernel.bin >> HuguiniUnix.bin
	dd if=/dev/zero bs=1048576 count=16 >> HuguiniUnix.bin
	$(QEMU) HuguiniUnix.bin

clean:
	clear
	rm -rf build/*.o build/*.asm.o