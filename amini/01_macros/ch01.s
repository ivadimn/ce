	.file	"ch01.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Before arithmetic: int_ptr: %p, char_ptr: %p\n"
	.align 8
.LC1:
	.string	"After arithmetic: int_ptr: %p, char_ptr: %p\n"
.LC2:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -64(%rbp)
	movq	$0, -48(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	$0, -40(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$4, -48(%rbp)
	addq	$1, -40(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$9, -32(%rbp)
	movl	$22, -28(%rbp)
	movl	$30, -24(%rbp)
	movl	$23, -20(%rbp)
	movl	$18, -16(%rbp)
	movl	$0, -60(%rbp)
	jmp	.L2
.L3:
	movl	-60(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -60(%rbp)
.L2:
	cmpl	$4, -60(%rbp)
	jle	.L3
	leaq	-32(%rbp), %rax
	movq	%rax, -56(%rbp)
.L6:
	movq	-56(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-32(%rbp), %rax
	addq	$16, %rax
	cmpq	%rax, -56(%rbp)
	je	.L10
	addq	$4, -56(%rbp)
	jmp	.L6
.L10:
	nop
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
