#include "port.h"

struct InternalPortVariables PortStruct;

uint8_t PIC_MCP, PIC_SCP, PIC_MDP, PIC_SDP, KB_DP, KB_CP, MS_DP, MS_CP, PS2_DP, PS2_CP;

void writePort(uint8_t Mode, uint16_t PortNumber, void* data)
{
	switch(Mode)
	{
		case 0://8bit mode
		{
			PortStruct.WritePort8bit = (uint8_t*)data;
			
			asm volatile("outb %0, %1": : "a" (*PortStruct.WritePort8bit), "Nd" (PortNumber));
			
		}break;
		
		case 1://16bit mode
		{
			PortStruct.WritePort16bit= (uint16_t*)data;
			
			asm volatile("outw %0, %1": : "a" (*PortStruct.WritePort16bit), "Nd" (PortNumber));
			
		}break;
		
		case 2://32bit mode
		{
			PortStruct.WritePort32bit = (uint32_t*)data;
			
			asm volatile("outl %0, %1": : "a" (*PortStruct.WritePort32bit), "Nd" (PortNumber));
			
		}break;
		
		case 3://8bit slow mode
		{
			PortStruct.WritePort8bit = (uint8_t*)data;
			
			asm volatile("outb %0, %1 \n jmp 1f \n 1: jmp 1f \n 1:": : "a" (*PortStruct.WritePort8bit), "Nd" (PortNumber));
			
		}break;
	}
	
	
}
void readPort(uint8_t Mode, uint16_t PortNumber, void* result)
{
	switch(Mode)
	{
		case 0://8bit mode
		{
			PortStruct.ResutWrapper8bit = (uint8_t*)result;
			asm volatile("inb %1, %0" : "=a" (*PortStruct.ResutWrapper8bit) : "Nd" (PortNumber));
			
		}break;
		
		case 1://16bit mode
		{
			PortStruct.ResutWrapper16bit = (uint16_t*)result;
			asm volatile("inw %1, %0" : "=a" (*PortStruct.ResutWrapper16bit) : "Nd" (PortNumber));
			
		}break;
		
		case 2://32bit mode
		{
			PortStruct.ResutWrapper32bit = (uint32_t*)result;
			asm volatile("inl %1, %0" : "=a" (*PortStruct.ResutWrapper32bit) : "Nd" (PortNumber));
			
		}break;
		
		case 3://8bit slow mode
		{
			PortStruct.ResutWrapper8bit = (uint8_t*)result;
			asm volatile("inb %1, %0" : "=a" (*PortStruct.ResutWrapper8bit) : "Nd" (PortNumber));
			
		}break;
	}
}

void WaitFor0x60Read()
{
	//could not read from dataport until this
	
	PortStruct.status = 0; 
	while((PortStruct.status & 1) != 1)
	{
		readPort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&PortStruct.status); 
	}
}

void WaitFor0x60_0x64Write()
{
	//could not write data port and command port until this 
	
	PortStruct.status = 0;
	while(PortStruct.status & 2)
	{
		readPort(PORT_MODE_8, HEX_PORT_NUMBER_KBMS_CP, (void*)&PortStruct.status);
	}
}

