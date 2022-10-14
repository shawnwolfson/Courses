.global _start

.section .text
_start:
        movq $new_node, %rax	
        cmpq $0, root
	je initializeTree_HW1	
	movq root, %rbx
        movq new_node, %rcx
        xor %rdx, %rdx
stepInto_HW1:	
	cmpq (%rbx), %rcx
	je end_HW1
	jb leftSon_HW1

rightSon_HW1:
        movq %rbx , %rdx
        movq 16(%rbx), %rbx
	cmpq $0, %rbx
	je insertRight_HW1
	jmp stepInto_HW1
	
leftSon_HW1:
        movq %rbx , %rdx
        movq 8(%rbx), %rbx
	cmpq $0, %rbx
	je insertLeft_HW1
	jmp stepInto_HW1
	

insertRight_HW1:
	mov %rax,16(%rdx)	
	jmp end_HW1

insertLeft_HW1:
	mov %rax,8(%rdx)	
	jmp end_HW1

initializeTree_HW1:
	mov %rax, root(%rip)	

end_HW1:

