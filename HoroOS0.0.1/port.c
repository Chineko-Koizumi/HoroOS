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

void InitPIC()
{	
	PortStruct.v0x20 = 0x20;
	uint8_t hardwareInterruptOffset = 0x20;

	PIC_MCP = 0x11;
	PIC_SCP = 0x11;
	
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MCP, (void*)&PIC_MCP);
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SCP, (void*)&PIC_SCP);
	
	PIC_MDP = hardwareInterruptOffset;
	PIC_SDP = hardwareInterruptOffset + 8;
	
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MDP, (void*)&PIC_MDP);
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SDP, (void*)&PIC_SDP);
	
	PIC_MDP = 0x04;
	PIC_SDP = 0x02;
	
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MDP, (void*)&PIC_MDP);
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SDP, (void*)&PIC_SDP); 
	
	PIC_MDP = 0x01;
	PIC_SDP = 0x01;
	
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MDP, (void*)&PIC_MDP);
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SDP, (void*)&PIC_SDP);
	
	PIC_MDP = 0x00;
	PIC_SDP = 0x00;
	
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MDP, (void*)&PIC_MDP);
	writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SDP, (void*)&PIC_SDP);
	
}

void EOI(uint8_t type)
{
	switch(type)
	{
		case EOI_M:
		{
			writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MCP, (void*)&PortStruct.v0x20);
		}break;
		case EOI_MS:
		{
			writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_MCP, (void*)&PortStruct.v0x20);
			writePort(PORT_MODE_8_SLOW, HEX_PORT_NUMBER_PIC_SCP, (void*)&PortStruct.v0x20);
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

