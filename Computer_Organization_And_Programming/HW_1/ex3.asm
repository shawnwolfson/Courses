.global _start

.section .text
_start:
	xor %rax, %rax
	xor %rbx, %rbx
	xor %rcx, %rcx
        xor %rdx, %rdx
	lea (array1), %rax
	lea (array2), %rbx
	lea (mergedArray), %rcx
loop_HW1:
	cmpl $0, (%rax)
	je completeArr2_HW1
	cmpl $0, (%rbx)
	je completeArr1_HW1
	cmpl (%rax), %edx
	je skipArr1_HW1 
	cmpl (%rbx), %edx
	je skipArr2_HW1
	movl (%rbx), %r9d
	cmpl (%rax), %r9d
	jbe addArr1_HW1
	jmp addArr2_HW1

completeArr1_HW1:
	cmpl $0, (%rax)
	je exit_HW1
	movl (%rax), %r9d
	cmpl %edx, %r9d
	je skipArr1_HW1
	movl %r9d, (%rcx)
        movl %r9d, %edx
	add $4 , %rax
	add $4 , %rcx
	jmp completeArr1_HW1

completeArr2_HW1:
	cmpl $0, (%rbx)
	je exit_HW1
	movl (%rbx), %r9d
	cmpl %edx, %r9d
	je skipArr2_HW1
	movl %r9d, (%rcx)
        movl %r9d, %edx
	add $4 , %rbx
	add $4 , %rcx
	jmp completeArr2_HW1

addArr1_HW1:
	movl (%rax), %r9d
	movl %r9d, (%rcx)
        movl %r9d, %edx
	add $4 , %rax
	add $4 , %rcx
	jmp loop_HW1

addArr2_HW1:
	movl (%rbx), %r9d
	movl %r9d, (%rcx)
        movl %r9d, %edx
	add $4 , %rbx
	add $4 , %rcx
	jmp loop_HW1

skipArr1_HW1:
	add $4 , %rax
	jmp loop_HW1
skipArr2_HW1:
	add $4 , %rbx
	jmp loop_HW1
exit_HW1:
	movl $0, (%rcx)

