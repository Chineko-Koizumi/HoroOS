#include "GDT.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	/* Setup the descriptor base address */
		gdt[num].base_low 		= (base & 0xFFFF);
		gdt[num].base_middle	= (base >> 16) & 0xFF;
		gdt[num].base_high		= (base >> 24) & 0xFF;
		
	/* Setup the descriptor limits */
		gdt[num].limit_low 		= (limit & 0xFFFF);
		gdt[num].granularity	= ((limit >> 16) & 0x0F);
		
	/* Finally set up the granularity and access flags */
		gdt[num].granularity |= (gran & 0xF0);
		gdt[num].access = access;
}

void gdt_install()
{		
	
		/* NULL descriptor*/
		gdt_set_gate(0, 0, 0, 0, 0);
		
		/* CODE segment 4GB */
		gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

		/* DATA segment 4GB */
		gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
		
		/*Flush old GDT and aplly new one*/

		gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
		gp.base = gdt;
		
		printf("GDT set.\n");

		gdt_flush();
		printf("GDT loaded.\n");
}
