.global _start

.section .text
_start:
	movq src(%rip) , %rax
	movq dst(%rip) , %rbx
	movq head(%rip), %rsi 
	cmp %rax, %rbx
	je exit_HW1
	test %rsi, %rsi
	je exit_HW1

	xor %rcx, %rcx 
	xor %rdx, %rdx 
	xor %r11,%r11 
	xor %r12,%r12 
	xor %r13,%r13 
	xor %r14,%r14 

	lea (%rsi) , %r9
	cmp (%r9),%rax
	je headIsSource_HW1
	
	cmp (%r9),%rbx
	je headIsDestination_HW1
	jmp checkUniqueVal_HW1

headIsSource_HW1:
	add $1 , %rcx
	movq head, %r12
	jmp checkUniqueVal_HW1

headIsDestination_HW1:
	add $1 , %rdx
	movq head, %r14

checkUniqueVal_HW1:
	movq 8(%r9) , %r10  
	cmpq $0, %r10
	je conditionCheck_HW1
	cmp (%r10),%rax
	je foundSource_HW1
	cmp (%r10),%rbx
	je foundDestination_HW1
        movq 8(%r9), %r9
	jmp checkUniqueVal_HW1

foundSource_HW1:
	add $1 , %rcx
	lea (%r9), %r11
	lea (%r10), %r12
        movq 8(%r9), %r9
	jmp checkUniqueVal_HW1
	
foundDestination_HW1:
	
	add $1 , %rdx
	lea (%r9), %r13
	lea (%r10), %r14
        movq 8(%r9), %r9
	jmp checkUniqueVal_HW1

conditionCheck_HW1:
	cmp $1 , %rcx
	jne exit_HW1
	cmp $1 , %rdx
	jne exit_HW1

swapProcess_HW1:
        test %r11 , %r11
        jz swapDestWithHead_HW1
        test %r13 , %r13
        jz swapSourceWithHead_HW1
	movq %r14, 8(%r11) 
 	movq %r12, 8(%r13) 
	movq 8(%r14), %r15 
	movq 8(%r12), %r8 
	movq %r15, 8(%r12)  
	movq %r8, 8(%r14) 
        jmp exit_HW1
        
swapDestWithHead_HW1:
        movq %r12, 8(%r13) 
	movq 8(%r14), %r15 
	movq 8(%r12), %r8 
	movq %r15, 8(%r12) 
	movq %r8, 8(%r14)
        movq %r14, head
        jmp exit_HW1
        
swapSourceWithHead_HW1:
        movq %r14, 8(%r11) 
	movq 8(%r14), %r15 
	movq 8(%r12), %r8 
	movq %r15, 8(%r12)  
	movq %r8, 8(%r14)  
        movq %r12, head

exit_HW1:


