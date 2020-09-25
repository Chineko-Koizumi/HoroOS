#include "Mouse.h"

struct MouseState MouseStruct;

void InitMouse()
{	
	MouseStruct.mouse_data[0] = 0;
	MouseStruct.mouse_data[1] = 0;
	MouseStruct.mouse_data[2] = 0;
	MouseStruct.mouse_X = 0;
	MouseStruct.mouse_Y = 0;
	MouseStruct.mouse_data_itterator = 1;
		
		WaitFor0x60_0x64Write();
	MS_CP = 0xD4;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&MS_CP);
	
		WaitFor0x60_0x64Write();
	MS_DP = 0xF6;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_DP, (void*)&MS_DP);
	
		WaitFor0x60_0x64Write();
	MS_CP = 0xD4;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&MS_CP);
	
		WaitFor0x60_0x64Write();
	MS_DP = 0xF4;
	writePort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_DP, (void*)&MS_DP);
		
		printf("Mouse ready.\n");
}

void GetDataFromMouse()
{
			readPort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_KBMS_CP, (void*)&MouseStruct.status);
			switch(MouseStruct.status & 0x20)
			{
				case 0x20:break;
				default: return;
			}
			WaitFor0x60Read();
			readPort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_DP, (void*)&MouseStruct.mouse_data[MouseStruct.mouse_data_itterator]);
	
			switch(MouseStruct.mouse_data_itterator)
			{
				case 0:
				{
					MouseStruct.mouse_X += MouseStruct.mouse_data[1];
					MouseStruct.mouse_Y -= MouseStruct.mouse_data[2];
					
					/*char temp[100];
					printf("X->[");
					printf(itoa(MouseStruct.mouse_X, temp, 10));
					printf("]  Y->[");
					printf(itoa(MouseStruct.mouse_Y, temp, 10));
					printf("]\n");
					*/
					
					MouseStruct.mouse_data_itterator++;
				}break;
				case 1:
				{
					MouseStruct.mouse_data_itterator++;
				}break;
				case 2:
				{
					MouseStruct.mouse_data_itterator = 0;
				}break;
			}	
}

