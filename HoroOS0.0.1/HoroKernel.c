#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernelstdio.h"
#include "GDT.h"
#include "interrupt.h"
#include "PS2.h"
#include "port.h"
#include "Keyboard.h"


void HoroKernel(void* multiBootStructure, uint32_t magicnumber)
{
	terminal_initialize();
	gdt_install();
	InitInterupts();
	InitPS2();
	InitKeyboard();
	InitMouse();
	
	
	ActivateInterrupts();
	printf("\nAwooooooooo");
	
	while(1);
}
