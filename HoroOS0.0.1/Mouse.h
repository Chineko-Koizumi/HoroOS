#ifndef __MOUSE_H
#define __MOUSE_H

#include "port.h"
#include "kernelstdio.h"

struct MouseState
{

	uint8_t mouse_data[3];
	uint8_t mouse_data_itterator;
	int8_t mouse_X, mouse_Y;
	
	// byte 0 -> | Y overflow | X overflow | Y sign bit | X sign bit | 1 | Middle button | Right button | Left button |
	// byte 1 ->  X movement
	// byte 2 ->  Y movement
		uint8_t status;
};

extern struct MouseState MouseStruct;

extern void InitMouse();
extern void GetDataFromMouse();

#endif
