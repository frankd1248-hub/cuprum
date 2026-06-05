.intel_syntax noprefix

.global _start
_start:
	call	main
	mov	rdi, rax
	mov	rax, 60
	syscall

.global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80
	mov	rax, 1
	mov	QWORD PTR [rbp-8], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	QWORD PTR [rbp-16], rax
	xor	eax, eax
	mov	QWORD PTR [rbp-24], rax
	mov	rax, QWORD PTR [rbp-16]
	mov	rbx, QWORD PTR [rbp-24]
	cmp	rax, rbx
	sete	al
	movzx	rax, al
	mov	QWORD PTR [rbp-32], rax
	cmp	QWORD PTR [rbp-32], 0
	jne	then_0
	jmp	else_1
then_0:
	mov	rax, 10
	mov	QWORD PTR [rbp-40], rax
	mov	rax, QWORD PTR [rbp-40]
	mov	QWORD PTR [rbp-16], rax
	jmp	end_2
else_1:
	mov	rax, 1
	mov	QWORD PTR [rbp-48], rax
	mov	rax, QWORD PTR [rbp-16]
	mov	rbx, QWORD PTR [rbp-48]
	cmp	rax, rbx
	sete	al
	movzx	rax, al
	mov	QWORD PTR [rbp-56], rax
	cmp	QWORD PTR [rbp-56], 0
	jne	then_3
	jmp	else_4
then_3:
	mov	rax, 20
	mov	QWORD PTR [rbp-64], rax
	mov	rax, QWORD PTR [rbp-64]
	mov	QWORD PTR [rbp-16], rax
	jmp	end_5
else_4:
	mov	rax, 30
	mov	QWORD PTR [rbp-72], rax
	mov	rax, QWORD PTR [rbp-72]
	mov	QWORD PTR [rbp-16], rax
end_5:
end_2:
	mov	rax, QWORD PTR [rbp-16]
	mov	rsp, rbp
	pop	rbp
	ret
