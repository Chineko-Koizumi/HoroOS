#ifndef __PORT_H
#define __PORT_H

#define PORT_MODE_8			0
#define PORT_MODE_16		1
#define PORT_MODE_32		2
#define PORT_MODE_8_SLOW	3

#define EOI_M 	0
#define EOI_MS	1

#define HEX_PORT_NUMBER_PIC_MCP			0x20
#define HEX_PORT_NUMBER_PIC_MDP			0x21
#define HEX_PORT_NUMBER_PIC_SCP			0xA0
#define HEX_PORT_NUMBER_PIC_SDP			0xA1
		
#define HEX_PORT_NUMBER_KBMS_DP			0x60
#define HEX_PORT_NUMBER_KBMS_CP			0x64

#define HEX_PORT_NUMBER_PCI_CONFIG_DATA 0xCFC
#define HEX_PORT_NUMBER_PCI_CONFIG_ADDR	0xCF8

#include <stdint.h>

struct InternalPortVariables
{
	uint8_t 	*WritePort8bit;
    uint16_t 	*WritePort16bit;
    uint32_t 	*WritePort32bit;
    
    uint8_t 	*ResutWrapper8bit;
    uint16_t 	*ResutWrapper16bit;
    uint32_t 	*ResutWrapper32bit;
	
	uint8_t v0x20 ;
	
	uint8_t status;
};

extern struct InternalPortVariables PortStruct;

extern uint8_t PIC_MCP, PIC_SCP, PIC_MDP, PIC_SDP, KB_DP, KB_CP, MS_DP, MS_CP, PS2_DP, PS2_CP;

extern void writePort(uint8_t Mode, uint16_t PortNumber, void* data);
extern void readPort(uint8_t Mode, uint16_t PortNumber, void* result);

extern void WaitFor0x60Read();
extern void WaitFor0x60_0x64Write();





#endif
