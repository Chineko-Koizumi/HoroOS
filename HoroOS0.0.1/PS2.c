#include "PS2.h"

struct PS2State PS2Struct;

void InitPS2()
{
	PS2Struct.status = 0b01000111;
	
		WaitFor0x60_0x64Write();
	PS2_CP = 0xAE;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&PS2_CP);
	
		WaitFor0x60_0x64Write();
	PS2_CP = 0xA8;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&PS2_CP);
		
		WaitFor0x60_0x64Write();
	PS2_CP = 0x60;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&PS2_CP);
	
		WaitFor0x60_0x64Write();
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_DP, (void*)&PS2Struct.status);
	
	printf("PS2 ready.\n");
}
