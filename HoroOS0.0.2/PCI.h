#ifndef __PCI_H
#define __PCI_H

#include <stdbool.h>

#include "port.h"
#include "kernelstdio.h"

struct PCIDescriptorEntity
{
	uint32_t portBase;
	uint32_t interrupt;
	
	uint16_t bus;
	uint16_t device;
	uint16_t function;
	
	uint16_t vendor_id;
	uint16_t device_id;
	
	uint8_t class_id;
	uint8_t subclass_id;
	uint8_t interface_id;
	
	uint8_t revision;
};

struct PCIData
{
	uint32_t id;
	uint32_t result;
	
	uint8_t DescriptorTableItterator;
};

extern struct PCIData PCIStruct;
extern struct PCIDescriptorEntity PCIdt[2048];

extern void InitPCI();

extern void createPCIdtEntity(uint16_t bus, uint16_t device, uint16_t function);

extern uint32_t readPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
extern void writePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t *value);
extern bool isDeviceHasFunctions(uint16_t bus, uint16_t device);

#endif
