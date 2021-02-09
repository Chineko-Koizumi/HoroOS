
#include "Keyboard.h"

struct KeyboardState KeyboardStruct;

void InitKeyboard()
{	
			WaitFor0x60_0x64Write();
		KB_DP = 0xF4;
		writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_KBMS_DP, (void*)&KB_DP);
		
		printf("Keyboard ready.\n");
}

void GetDataFromKeyboard()
{
	// | L Shift | R Shift | L Ctrl | R Ctrl | L Alt | R Alt | E0 | 0 | <---- Mask
	
	WaitFor0x60Read();
	readPort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_DP, (void*)&KeyboardStruct.Key);
	
	switch(KeyboardStruct.SCAKeyMask & 2UL)
	{
		case 0:
		switch(KeyboardStruct.Key)
		{
		
			case 0x2A: KeyboardStruct.SCAKeyMask |= 1UL << 7;		break;	//Left shift down
			case 0xAA: KeyboardStruct.SCAKeyMask &= ~(1UL << 7);	break;	//Left shift up
				
			case 0x36: KeyboardStruct.SCAKeyMask |= 1UL << 6;		break;	//Right shift down
			case 0xB6: KeyboardStruct.SCAKeyMask &= ~(1UL << 6);	break;	//Right shift up
		
			case 0x1D: KeyboardStruct.SCAKeyMask |= 1UL << 5;		break; 	//left ctrl down
			case 0x9D: KeyboardStruct.SCAKeyMask &= ~(1UL << 5);	break;	//left ctrl up

			case 0x38: KeyboardStruct.SCAKeyMask |= 1UL << 3;		break;	//left alt down
			case 0xB8: KeyboardStruct.SCAKeyMask &= ~(1UL << 3);	break;	//left alt up
					
			case 0xE0: KeyboardStruct.SCAKeyMask |= 1UL << 1;		break;	//E0 flag set
		}
		break;
		
		case 2:
		switch(KeyboardStruct.Key)
		{
			case 0x1D:
			{
				KeyboardStruct.SCAKeyMask |= 1UL << 4;						//Right ctrl down
				KeyboardStruct.SCAKeyMask &= ~(1UL << 1);					//E0 flag clear
			}break;
			
			case 0x9D:
			{ 
				KeyboardStruct.SCAKeyMask &= ~(1UL << 4);					//right ctrl up	
				KeyboardStruct.SCAKeyMask &= ~(1UL << 1);					//E0 flag clear
			}break;
			
			case 0x38:
			{ 
				KeyboardStruct.SCAKeyMask |= 1UL << 2;						//right alt down	
				KeyboardStruct.SCAKeyMask &= ~(1UL << 1);					//E0 flag clear
			}break;
			
			case 0xB8:
			{ 
				KeyboardStruct.SCAKeyMask &= ~(1UL << 2);					//right alt up	
				KeyboardStruct.SCAKeyMask &= ~(1UL << 1);					//E0 flag clear
			}break;
		}
		break;
	}
		
}

char GetCharFromKeyCode()
{
	// | L Shift | R Shift | L Ctrl | R Ctrl | L Alt | R Alt | E0 | 0 | <---- Mask
	char out;
	switch(KeyboardStruct.SCAKeyMask & 0xC0)	
	{
		case 0x80: case 0xC0: case 0x40:
			
		switch(KeyboardStruct.Key)
		{
			case 0x29: out = '~'; break;
			case 0x2: out = '!'; break;
			case 0x3: out = '@'; break;
			case 0x4: out = '#'; break;
			case 0x5: out = '$'; break;
			case 0x6: out = '%'; break;
			case 0x7: out = '^'; break;
			case 0x8: out = '&'; break;
			case 0x9: out = '*'; break;
			case 0xA: out = '('; break;
			case 0xB: out = ')'; break;
			case 0xC: out = '_'; break;
			case 0xD: out = '+'; break;
				
			case 0x10: out = 'Q'; break;
			case 0x11: out = 'W'; break;
			case 0x12: out = 'E'; break;
			case 0x13: out = 'R'; break;
			case 0x14: out = 'T'; break;
			case 0x15: out = 'Y'; break;
			case 0x16: out = 'U'; break;
			case 0x17: out = 'I'; break;
			case 0x18: out = 'O'; break;
			case 0x19: out = 'P'; break;
			case 0x1A: out = '{'; break;
			case 0x1B: out = '}'; break;
			
			case 0x1E: out = 'A'; break;
			case 0x1F: out = 'S'; break;
			case 0x20: out = 'D'; break;
			case 0x21: out = 'F'; break;
			case 0x22: out = 'G'; break;
			case 0x23: out = 'H'; break;
			case 0x24: out = 'J'; break;
			case 0x25: out = 'K'; break;
			case 0x26: out = 'L'; break;
			case 0x27: out = ':'; break;
			case 0x28: out = '\"'; break;
			case 0x2B: out = '|'; break;
			
			case 0x2C: out = 'Z'; break;
			case 0x2D: out = 'X'; break;
			case 0x2E: out = 'C'; break;
			case 0x2F: out = 'V'; break;
			case 0x30: out = 'B'; break;
			case 0x31: out = 'N'; break;
			case 0x32: out = 'M'; break;
			case 0x33: out = '<'; break;
			case 0x34: out = '>'; break;
			case 0x35: out = '?'; break;
			
			case 0x1C: out = '\n'; break;
			case 0x39: out = ' '; break;
			
			
			default:
			{
				out = '\0';	
			}
		}
		break;
		
		default:
		switch(KeyboardStruct.Key)
		{
			case 0x29: out = '`'; break;
			case 0x2: out = '1'; break;
			case 0x3: out = '2'; break;
			case 0x4: out = '3'; break;
			case 0x5: out = '4'; break;
			case 0x6: out = '5'; break;
			case 0x7: out = '6'; break;
			case 0x8: out = '7'; break;
			case 0x9: out = '8'; break;
			case 0xA: out = '9'; break;
			case 0xB: out = '0'; break;
			case 0xC: out = '-'; break;
			case 0xD: out = '='; break;
				
			case 0x10: out = 'q'; break;
			case 0x11: out = 'w'; break;
			case 0x12: out = 'e'; break;
			case 0x13: out = 'r'; break;
			case 0x14: out = 't'; break;
			case 0x15: out = 'y'; break;
			case 0x16: out = 'u'; break;
			case 0x17: out = 'i'; break;
			case 0x18: out = 'o'; break;
			case 0x19: out = 'p'; break;
			case 0x1A: out = '['; break;
			case 0x1B: out = ']'; break;
			
			case 0x1E: out = 'a'; break;
			case 0x1F: out = 's'; break;
			case 0x20: out = 'd'; break;
			case 0x21: out = 'f'; break;
			case 0x22: out = 'g'; break;
			case 0x23: out = 'h'; break;
			case 0x24: out = 'j'; break;
			case 0x25: out = 'k'; break;
			case 0x26: out = 'l'; break;
			case 0x27: out = ';'; break;
			case 0x28: out = '\''; break;
			case 0x2B: out = '\\'; break;
			
			case 0x2C: out = 'z'; break;
			case 0x2D: out = 'x'; break;
			case 0x2E: out = 'c'; break;
			case 0x2F: out = 'v'; break;
			case 0x30: out = 'b'; break;
			case 0x31: out = 'n'; break;
			case 0x32: out = 'm'; break;
			case 0x33: out = ','; break;
			case 0x34: out = '.'; break;
			case 0x35: out = '/'; break;
			
			case 0x1C: out = '\n'; break;
			case 0x39: out = ' '; break;
			
			
			default:
			{
				out = '\0';	
			}
	}
		break;
	}
	
	return out;
}
