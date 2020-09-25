
#include "interrupt.h"

struct idt_entry idt[256];
struct idt_ptr ip;

void InitInterupts()
{
	const uint8_t IDT_INT_GATE = 0xE;
	
	uint16_t CodeSegment;
	
	asm volatile("mov %%CS, %0": "=r"(CodeSegment) :);
	
	
	for(uint16_t i=0; i<256; ++i)
	{
		SetIDTentry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INT_GATE);
	}
	
	SetIDTentry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INT_GATE);
	SetIDTentry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INT_GATE);
	SetIDTentry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INT_GATE);
	
	printf("IDT set.\n");
	
	ip.limit = (sizeof(struct idt_entry) * 256) - 1;
	ip.base = idt;
	
	idt_install();
	printf("IDT loaded.\n");
	InitPIC();
	printf("PIC set and active.\n");
}
void ActivateInterrupts()
{
		printf("Interrupts activated!!!\n");
		asm volatile("sti");
}
void DeactivateInterrupts()
{
		asm volatile("cli");
}

void SetIDTentry(uint8_t INT_number, uint16_t CodeSegmentSelectorOffset, void(*handler)(), uint8_t DescriptorPrivilidgeLevel, uint8_t DescriptorType)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;
	
	idt[INT_number].handlerAddressLow 		= ((uint32_t)handler) & 0xFFFF;
	idt[INT_number].handlerAddressHigh 		= ((uint32_t)handler >> 16) & 0xFFFF;
	idt[INT_number].gdt_codeSegmentSelector = CodeSegmentSelectorOffset;
	idt[INT_number].access 					= IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilidgeLevel & 3) << 5);
	idt[INT_number].reserved 				= 0;
}

uint32_t HandleInterrupt(uint8_t INT_number, uint32_t ESP)
{
	switch(INT_number)
	{
		case 0x20:
		{
				EOI(EOI_M);
		}break;	
		case 0x21://---------------------------------------------------------------------keyboard
		{	
			GetDataFromKeyboard();
			
			char c_key[2];
			c_key[0] = GetCharFromKeyCode();
			c_key[1] ='\0';
			switch(c_key[0])
			{
				case '\0': 
				{
					    
				}break;
				
				default: printf(c_key);
			}
			EOI(EOI_M);	
		}break;	
		case 0x22:
		{
				EOI(EOI_M);
		}break;
		case 0x23:
		{
				EOI(EOI_M);
		}break;
		case 0x24:
		{
				EOI(EOI_M);
		}break;
		case 0x25:
		{
				EOI(EOI_M);
		}break;
		case 0x26:
		{
				EOI(EOI_M);
		}break;
		case 0x27:
		{
				EOI(EOI_M);
		}break;
		case 0x28:
		{
				EOI(EOI_MS);
		}break;
		case 0x29:
		{
				EOI(EOI_MS);
		}break;
		case 0x2A:
		{
				EOI(EOI_MS);
		}break;
		case 0x2B:
		{
				EOI(EOI_MS);
		}break;
		case 0x2C://---------------------------------------------------------------------mouse
		{
			GetDataFromMouse();
			switch(MouseStruct.mouse_data_itterator)
			{
				case 0:
				{
					char temp [100];
					printf("x:[");
					printf(itoa(MouseStruct.mouse_X, temp, 10));
					printf("] Y:[");
					printf(itoa(MouseStruct.mouse_Y, temp, 10));
					printf("]\n");
				}break;
				default: break;
			}
			EOI(EOI_MS);
		}break;
		case 0x2D:
		{
				EOI(EOI_MS);
		}break;
		case 0x2E:
		{
				EOI(EOI_MS);
		}break;
		case 0x2F:
		{
				EOI(EOI_MS);
		}break;
	}
	return ESP;	
}

void HelpMe()
{
		printf("Help\n");
}
