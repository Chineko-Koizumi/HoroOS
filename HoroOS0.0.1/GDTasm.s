.section .text

.global gdt_flush
.extern gp

gdt_flush:

	lgdt gp
	jmp $0x08, $flush2
	
flush2:
	mov $0x10, %eax
	mov %eax,  %ds
	mov %eax,  %es
	mov %eax,  %fs
	mov %eax,  %gs
	mov %eax,  %ss
	ret
