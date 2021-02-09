#ifndef __PS2_H
#define __PS2_H

#include "port.h"
#include "Keyboard.h"
#include "Mouse.h"

struct PS2State 
{
	uint8_t status;
}; 

extern struct PS2State PS2Struct;

extern void InitPS2();

#endif
