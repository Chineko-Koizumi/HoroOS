#include "PCI.h"

struct PCIData PCIStruct;
struct PCIDescriptorEntity PCIdt[2048];

uint32_t readPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
	PCIStruct.id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    
    writePort(PORT_MODE_32, HEX_PORT_NUMBER_PCI_CONFIG_ADDR, (void*)&PCIStruct.id);
    readPort(PORT_MODE_32, HEX_PORT_NUMBER_PCI_CONFIG_DATA, (void*)&PCIStruct.result);
    
    return PCIStruct.result >> (8*(registeroffset % 4));
}

void writePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t *value)
{
	PCIStruct.id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    
    writePort(PORT_MODE_32, HEX_PORT_NUMBER_PCI_CONFIG_ADDR, (void*)&PCIStruct.id);
    writePort(PORT_MODE_32, HEX_PORT_NUMBER_PCI_CONFIG_DATA, (void*)value);
}

bool isDeviceHasFunctions(uint16_t bus, uint16_t device)
{ 
	return readPCI(bus, device, 0, 0x0E) & (1<<7);	
}

void createPCIdtEntity(uint16_t bus, uint16_t device, uint16_t function)
{
	PCIdt[PCIStruct.DescriptorTableItterator].bus = bus;
    PCIdt[PCIStruct.DescriptorTableItterator].device = device;
    PCIdt[PCIStruct.DescriptorTableItterator].function = function;

    PCIdt[PCIStruct.DescriptorTableItterator].vendor_id = readPCI(bus, device, function, 0x00);
    PCIdt[PCIStruct.DescriptorTableItterator].device_id = readPCI(bus, device, function, 0x02);

    PCIdt[PCIStruct.DescriptorTableItterator].class_id = readPCI(bus, device, function, 0x0b);
    PCIdt[PCIStruct.DescriptorTableItterator].subclass_id = readPCI(bus, device, function, 0x0a);
    PCIdt[PCIStruct.DescriptorTableItterator].interface_id = readPCI(bus, device, function, 0x09);

    PCIdt[PCIStruct.DescriptorTableItterator].revision = readPCI(bus, device, function, 0x08);
    PCIdt[PCIStruct.DescriptorTableItterator].interrupt = readPCI(bus, device, function, 0x3c);
    
}

void InitPCI()
{
	
	printf("\nSearching  for PCI devices\n__________________________________________________________________\n");
	PCIStruct.DescriptorTableItterator = 0;
	
	for(uint8_t bus = 0; bus < 8; bus++)
    {
        for(uint8_t device = 0; device < 32; device++)
        {
            uint8_t numFunctions = isDeviceHasFunctions(bus, device) ? 8 : 1;
            for(uint8_t function = 0; function < numFunctions; function++)
            {
				createPCIdtEntity(bus, device, function);
				
				switch(PCIdt[PCIStruct.DescriptorTableItterator].vendor_id)
				{
					case 0x0000:break;
					case 0xFFFF:break;
					default:
					{
							char temp[100];
						printf("DEVICE ");
						printf(itoa(device & 0xFF, temp, 16));	
						printf(" = VENDOR ");
						printf(itoa((PCIdt[PCIStruct.DescriptorTableItterator].vendor_id & 0xFF00) >> 8, temp, 16));
						printf(itoa(PCIdt[PCIStruct.DescriptorTableItterator].vendor_id & 0xFF, temp, 16));
						
						printf(", CLASS ");
						printf(itoa(PCIdt[PCIStruct.DescriptorTableItterator].class_id, temp, 16));
										
						printf(", SUBCLASS ");
						printf(itoa(PCIdt[PCIStruct.DescriptorTableItterator].subclass_id, temp, 16));			
						
						printf(", IF ");
						printf(itoa(PCIdt[PCIStruct.DescriptorTableItterator].interface_id, temp, 16));
						
						printf(", DEVICE ");
						printf(itoa((PCIdt[PCIStruct.DescriptorTableItterator].device_id & 0xFF00) >> 8, temp, 16));
						printf(itoa(PCIdt[PCIStruct.DescriptorTableItterator].device_id & 0xFF, temp, 16));
						
						printf("\n");
						
						PCIStruct.DescriptorTableItterator++;
						continue;
					}
				}
				break;
            }
        }
    }
    
    printf("__________________________________________________________________\n\n");
}
