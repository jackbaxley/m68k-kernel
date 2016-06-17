#NO_APP
	.file	"kernel.c"
	.section	.rodata
.LC0:
	.string	"%d pages free\n"
.LC1:
	.string	"Kernel Started"
.LC2:
	.string	"k_end %X\n"
.LC3:
	.string	"s_stack_end %X\n"
.LC4:
	.string	"i_stack_end %X\n"
.LC5:
	.string	"int test %d!\n"
.LC6:
	.string	"Program Loaded"
	.text
	.align	2
	.globl	kmain
	.type	kmain, @function
kmain:
	link.w %fp,#0
	jsr serial_init
	jsr serial_clear
	jsr init_processes
	jsr init_memory
	move.w %d0,%d0
	and.l #65535,%d0
	move.l %d0,-(%sp)
	pea .LC0
	jsr printf
	addq.l #8,%sp
	pea .LC1
	jsr puts
	addq.l #4,%sp
	pea k_end
	pea .LC2
	jsr printf
	addq.l #8,%sp
	pea s_stack_top
	pea .LC3
	jsr printf
	addq.l #8,%sp
	pea i_stack_top
	pea .LC4
	jsr printf
	addq.l #8,%sp
	pea 8.w
	pea .LC5
	jsr printf
	addq.l #8,%sp
	pea prog1_start
	jsr load_small_process
	addq.l #4,%sp
	pea .LC6
	jsr puts
	addq.l #4,%sp
	nop
	unlk %fp
	rts
	.size	kmain, .-kmain
	.section	.rodata
.LC7:
	.string	"Syscall!"
	.text
	.align	2
	.globl	syscall
	.type	syscall, @function
syscall:
	link.w %fp,#0
	pea .LC7
	jsr puts
	addq.l #4,%sp
	nop
	unlk %fp
	rts
	.size	syscall, .-syscall
	.ident	"GCC: (GNU) 5.3.0"
