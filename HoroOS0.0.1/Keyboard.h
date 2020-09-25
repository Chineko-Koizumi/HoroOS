#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "port.h"
#include "kernelstdio.h"

struct KeyboardState
{
	uint8_t Key;
	uint8_t SCAKeyMask;// | L Shift | R Shift | L Ctrl | R Ctrl | L Alt | R Alt | E0 | 0 |
	
	uint8_t status;
};

extern struct KeyboardState KeyboardStruct;


extern void InitKeyboard();
extern void GetDataFromKeyboard();
extern char GetCharFromKeyCode();

#endif
