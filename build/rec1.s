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
	sub	rsp, 64
	mov	QWORD PTR [rbp-8], rdi
	mov	QWORD PTR [rbp-8], rdi
	mov	rax, 2
	mov	QWORD PTR [rbp-16], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, QWORD PTR [rbp-16]
	cmp	rax, rbx
	setl	al
	movzx	rax, al
	mov	QWORD PTR [rbp-24], rax
	cmp	QWORD PTR [rbp-24], 0
	jne	then_0
	jmp	end_2
then_0:
	mov	rax, 1
	mov	QWORD PTR [rbp-32], rax
	mov	rax, QWORD PTR [rbp-32]
	mov	rsp, rbp
	pop	rbp
	ret
	jmp	end_2
end_2:
	mov	rax, 1
	mov	QWORD PTR [rbp-40], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, QWORD PTR [rbp-40]
	sub	rax, rbx
	mov	QWORD PTR [rbp-48], rax
	mov	rdi, QWORD PTR [rbp-48]
	call	factorial
	mov	QWORD PTR [rbp-56], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	rbx, QWORD PTR [rbp-56]
	imul	rax, rbx
	mov	QWORD PTR [rbp-64], rax
	mov	rax, QWORD PTR [rbp-64]
	mov	rsp, rbp
	pop	rbp
	ret
.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	rax, 5
	mov	QWORD PTR [rbp-8], rax
	mov	rdi, QWORD PTR [rbp-8]
	call	factorial
	mov	QWORD PTR [rbp-16], rax
	mov	rax, QWORD PTR [rbp-16]
	mov	rsp, rbp
	pop	rbp
	ret
.section .rodata
.text
