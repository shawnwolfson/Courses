.global _start
.section .text
_start:

        xor %rdx,%rdx
        xor %rbx,%rbx
	xor %rax, %rax
        movq num, %rax
	
LOOP_HW1:
        cmpq $0x40, %rdx
        je Stop_HW1
        addq $1, %rdx
        cmc
        shrq %rax
        jc ADD_HW1
        jmp LOOP_HW1
	
ADD_HW1:
	addq $1, %rbx
	jmp LOOP_HW1


Stop_HW1:
	movl %ebx, CountBits(%rip)
