	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp2:
	.cfi_def_cfa_offset 16
Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp4:
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movzwl	L_.str(%rip), %edi
	cmpl	$256, %edi              ## imm = 0x100
	setl	%al
	andb	$1, %al
	movzbl	%al, %eax
	popq	%rbp
	ret
	.cfi_endproc

	.section	__TEXT,__const
L_.str:                                 ## @.str
	.asciz	"\000\377"


.subsections_via_symbols
