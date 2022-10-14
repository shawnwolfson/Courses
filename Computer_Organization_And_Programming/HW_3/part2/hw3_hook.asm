.global hook

.section .data
msg: .ascii "This code was hacked by Ella Lee's gang\n"
endmsg:

.section .text
hook:
	movb $0xc3, %al
	movb %al , _start +0x1e
	leaq cont_HW3(%rip), %rax
	push %rax
	jmp _start
cont_HW3:
	pushq %rax
	pushq %rdi
	pushq %rsi
	mov $0x1, %rdi
	mov $msg , %rsi
	mov $0x1 , %rax
	mov $endmsg-msg , %rdx
  	syscall
	movb $0x90, %al
	movb %al , _start +0x1e
	popq %rax
	popq %rdi
	popq %rsi
	jmp _start + 0x26
