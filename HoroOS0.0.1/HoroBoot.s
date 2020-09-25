.set MAGIC, 	0x1badb002
.set FLAGS, 	(1<<0 | 1<<1)
.set CHECKSUM,	-(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.section .text

.extern HoroKernel

.global loader


loader: 
	cli
	mov $kernel_stack, %esp
	push %eax
	push %ebx
	call HoroKernel

_stop:				
	cli
	hlt
	jmp _stop
	
.section .bss
.space 64*1024*1024	; # 2MiB
kernel_stack:
	
