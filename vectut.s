	.file	"vectut.cpp"
	.text
	.p2align 4,,15
	.globl	_Z3sumP3Foo
	.type	_Z3sumP3Foo, @function
_Z3sumP3Foo:
.LFB0:
	.cfi_startproc
	pxor	%xmm0, %xmm0
	leaq	400(%rdi), %rax
	pxor	%xmm4, %xmm4
	.p2align 4,,10
	.p2align 3
.L3:
	movdqa	(%rdi), %xmm1
	addq	$16, %rdi
	movdqa	%xmm4, %xmm2
	cmpq	%rax, %rdi
	movdqa	%xmm1, %xmm5
	pcmpgtd	%xmm1, %xmm2
	punpckldq	%xmm2, %xmm5
	punpckhdq	%xmm2, %xmm1
	paddq	%xmm5, %xmm0
	paddq	%xmm1, %xmm0
	jne	.L3
	movdqa	%xmm0, %xmm6
	psrldq	$8, %xmm6
	paddq	%xmm6, %xmm0
	movdqa	%xmm0, -24(%rsp)
	movq	-24(%rsp), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z3sumP3Foo, .-_Z3sumP3Foo
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
