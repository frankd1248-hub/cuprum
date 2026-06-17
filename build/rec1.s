.intel_syntax noprefix

.global _start
_start:
	call	main
	mov	rdi, rax
	mov	rax, 60
	syscall

.global factorial
factorial:
	push	r14
	push	r13
	push	r12
	push	rbp
	mov	rbp, rsp
	mov	r12, rdi
	mov	rax, r12
	mov	r9,  2
	cmp	rax, r9
	setl	al
	movzx	rax, al
	mov	r10, rax
	cmp	r10, 0
	jne	then_0
	jmp	end_2
then_0:
	mov	rax, 1
	mov	rsp, rbp
	pop	rbp
	pop	r12
	pop	r13
	pop	r14
	ret
end_2:
	mov	rax, r12
	mov	rax, r12
	mov	r9,  1
	sub	rax, r9
	mov	r13, rax
	mov	rdi, r13
	call	factorial
	mov	r14, rax
	mov	rax, r12
	mov	rax, r12
	mov	r9,  r14
	imul	rax, r9
	mov	r10, rax
	mov	rax, r10
	mov	rsp, rbp
	pop	rbp
	pop	r12
	pop	r13
	pop	r14
	ret
.global main
main:
	push	r13
	push	r12
	push	rbp
	mov	rbp, rsp
	mov	r12, 12
	mov	rdi, r12
	call	factorial
	mov	r13, rax
	mov	rax, r13
	mov	r12, rax
	mov	rdi, r12
	call	print_int
	mov	r13, rax
	lea	rax, [.LCs0 + rip]
	mov	r12, rax
	lea	rax, [.LCs0_len + rip]
	mov	rdi, QWORD PTR [rax]
	lea	rax, [.LCs0 + rip]
	mov	rsi, rax
	call	println
	mov	r13, rax
	mov	rax, 0
	mov	rsp, rbp
	pop	rbp
	pop	r12
	pop	r13
	ret
.section .rodata
.LCs0_len:
	.quad 1
.LCs0:
	.string " "
.text
