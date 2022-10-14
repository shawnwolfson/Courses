.globl my_ili_handler

.text
.align 4, 0x90
my_ili_handler:	
	pushq %rbp
	movq %rsp, %rbp
	pushq %r8	
	movq 8(%rbp), %r8 #****r8 holds instruction address
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rax
	pushq %rdi
	pushq %rsi
	pushq %rdx	
	pushq %rcx
	pushq %rbx
	cmpb $0xF ,(%r8)
	jne ONE_BYTE_HW2
	xor %rdi , %rdi
	movb 1(%r8), %dil
	call what_to_do
	jmp HANDLER_FAULT_HW2

ONE_BYTE_HW2:
        xor %rdi,%rdi
	movb (%r8), %dil
	call what_to_do

HANDLER_FAULT_HW2:
	cmp $0,%rax
	je GOT_ZERO_HW2
	jmp GOT_NOT_ZERO_HW2

GOT_ZERO_HW2:
	popq %rbx	
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rax
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	jmp *old_ili_handler
	jmp RET_HW2

GOT_NOT_ZERO_HW2:
	popq %rbx	
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi	
	movq %rax, %rdi
	popq %rax
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	cmpb $0xF ,(%r8)
	jne ONE_BYTE_OFFSET_HW2
	popq %r8
	popq %rbp
	addq $2,(%rsp)
	jmp RET_HW2

ONE_BYTE_OFFSET_HW2:
	popq %r8
	popq %rbp
	addq $1,(%rsp)

RET_HW2:
iretq
