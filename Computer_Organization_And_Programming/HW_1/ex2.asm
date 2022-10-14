.global _start

.section .text
_start:
	xor %rsi, %rsi
        movl num, %eax
        lea source, %rbx
	lea destination, %rcx
	cmp %rbx, %rcx
	je stop_HW1 
	cdq
	cmp $0, %edx
	jne swap_HW1

compare_HW1:
	cmp %rbx, %rcx	
	jb countFromStart_HW1
	add %rax , %rbx
	add %rax , %rcx
        dec %rbx
        dec %rcx
	jmp countFromEnd_HW1

swap_HW1:
	lea (%rbx) , %rdx
	lea (%rcx) , %rbx
	lea (%rdx) , %rcx
	imul $-1 , %eax
	jmp compare_HW1
	
countFromStart_HW1: 
	cmp %esi ,%eax
	je stop_HW1
	movb (%rbx), %r9b 
	movb %r9b, (%rcx) 
	add $1, %esi 
        add $1, %rbx 
	add $1, %rcx
	jmp countFromStart_HW1

countFromEnd_HW1: 
	cmp $0,%eax
	je stop_HW1
	movb (%rbx), %r9b 
	movb %r9b, (%rcx)
	sub $1, %eax
        sub $1, %rbx
	sub $1, %rcx
	jmp countFromEnd_HW1

stop_HW1:

