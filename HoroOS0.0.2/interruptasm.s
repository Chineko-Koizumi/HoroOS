
.set IRQ_BASE, 0x20

.section .text

.extern HandleInterrupt
.extern HelpMe
.extern ip

.global idt_install
.global IgnoreInterruptRequest


.macro MyHandleInterruptRequest num
.global HandleInterruptRequest\num\()

HandleInterruptRequest\num\():
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom

.endm	

MyHandleInterruptRequest 0x00
MyHandleInterruptRequest 0x01
MyHandleInterruptRequest 0x0C


.macro MyHandleException num
.global HandleException\num\()

HandleException\num\():
	movb $\num, (interruptnumber)
	jmp int_bottom

.endm	


idt_install:
	lidt ip
	ret

int_bottom:
	
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	pushl %esp
	push (interruptnumber)
	call HandleInterrupt
	mov %eax, %esp
	
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	
IgnoreInterruptRequest:

	iret 
	
	
.data 
	interruptnumber: .byte 0
