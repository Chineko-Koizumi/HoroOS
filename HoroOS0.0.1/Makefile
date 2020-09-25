CURRRENT_DIR := $(shell pwd)
IMGPATH	:= $(CURRRENT_DIR)/image
BINPATH := $(CURRRENT_DIR)/image/isodir/boot

CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -lgcc

objects = HoroBoot.o kernelstdio.o GDTasm.o GDT.o port.o interruptasm.o interrupt.o PS2.o Keyboard.o Mouse.o HoroKernel.o
 
HoroOS.bin: HoroLinker.ld $(objects)
	i686-elf-gcc $(CFLAGS) -T HoroLinker.ld -o $(BINPATH)/HoroOS.bin $(objects)

%.o: %.c
	i686-elf-gcc -std=gnu99 $(CFLAGS) -c -o $@ $<

%.o: %.s
	i686-elf-as --32 -o $@ $<
	
.PHONY: Clean Iso Run Packet
	
Clean: 
	-rm -f $(BINPATH)/*.bin $(CURRRENT_DIR)/*.o $(IMGPATH)/*.iso

Iso: 
	grub-mkrescue -o $(IMGPATH)/HoroOS.iso $(IMGPATH)/isodir

Run:
	qemu-system-i386 -d cpu_reset,in_asm -no-reboot -no-shutdown -cdrom $(IMGPATH)/HoroOS.iso

Packet: HoroOS.bin Iso Run
