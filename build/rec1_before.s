.intel_syntax noprefix

.global _start
_start:
	call	main
	mov	rdi, rax
	mov	rax, 60
	syscall

.global factorial
factorial:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	QWORD PTR [rbp-8], rdi
	mov	QWORD PTR [rbp-8], rdi
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, 2
	cmp	rax, rbx
	setl	al
	movzx	rax, al
	mov	QWORD PTR [rbp-16], rax
	cmp	QWORD PTR [rbp-16], 0
	jne	then_0
	jmp	end_2
then_0:
	mov	rax, 1
	mov	rsp, rbp
	pop	rbp
	ret
end_2:
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, 1
	sub	rax, rbx
	mov	QWORD PTR [rbp-24], rax
	mov	rdi, QWORD PTR [rbp-24]
	call	factorial
	mov	QWORD PTR [rbp-32], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, QWORD PTR [rbp-32]
	imul	rax, rbx
	mov	QWORD PTR [rbp-40], rax
	mov	rax, QWORD PTR [rbp-40]
	mov	rsp, rbp
	pop	rbp
	ret
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	rax, 6
	mov	QWORD PTR [rbp-8], rax
	mov	rdi, QWORD PTR [rbp-8]
	call	factorial
	mov	QWORD PTR [rbp-16], rax
	mov	rax, QWORD PTR [rbp-16]
	mov	QWORD PTR [rbp-24], rax
	mov	rdi, QWORD PTR [rbp-24]
	call	print_int
	mov	QWORD PTR [rbp-32], rax
	lea	rax, [.LCs0 + rip]
	mov	QWORD PTR [rbp-40], rax
	lea	rax, [.LCs0_len + rip]
	mov	rdi, QWORD PTR [rax]
	lea	rax, [.LCs0 + rip]
	mov	rsi, rax
	call	println
	mov	QWORD PTR [rbp-48], rax
	mov	rax, 0
	mov	rsp, rbp
	pop	rbp
	ret
.section .rodata
.LCs0_len:
	.quad 1
.LCs0:
	.string " "
.text
