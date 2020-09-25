#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <stdint.h>
#include <stddef.h>

#include "GDT.h"
#include "port.h"
#include "kernelstdio.h"
#include "Keyboard.h"
#include "Mouse.h"

struct idt_entry
{
	uint16_t handlerAddressLow;
	uint16_t gdt_codeSegmentSelector;
	uint8_t reserved;
	uint8_t access;
	uint16_t handlerAddressHigh; 
	
}__attribute__((packed));

struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
	
}__attribute__((packed));


extern struct idt_entry idt[256];
extern struct idt_ptr ip;

extern void InitInterupts();
extern void ActivateInterrupts();
extern void DeactivateInterrupts();
extern void SetIDTentry(uint8_t INT_number, uint16_t CodeSegmentSelectorOffset, void(*handler)(), uint8_t DescriptorPrivilidgeLevel, uint8_t DescriptorType);


extern uint32_t HandleInterrupt(uint8_t INT_number, uint32_t ESP);

extern void HandleInterruptRequest0x00();
extern void HandleInterruptRequest0x01(); 
extern void HandleInterruptRequest0x0C(); 

extern void IgnoreInterruptRequest();

extern void HelpMe();
extern void idt_install();
#endif
