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
	sub	rsp, 32
	movss	xmm0, DWORD PTR [.LCf0 + rip]
	movq	rax, xmm0
	mov	QWORD PTR [rbp-8], rax
	mov	rax, QWORD PTR [rbp-8]
	mov	QWORD PTR [rbp-16], rax
	mov	rax, QWORD PTR [rbp-16]
	movq	xmm0, rax
	cvttss2si	eax, xmm0
	mov	QWORD PTR [rbp-24], rax
	mov	rax, QWORD PTR [rbp-24]
	mov	rsp, rbp
	pop	rbp
	ret
.section .rodata
.LCf0:
	.long 1081711002
.text
