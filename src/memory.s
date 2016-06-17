#NO_APP
	.file	"memory.c"
	.comm	page_stack,1024,2
	.comm	page_stack_ptr,2,2
	.comm	page_process_ll,1024,2
	.text
	.align	2
	.globl	get_page
	.type	get_page, @function
get_page:
	link.w %fp,#-4
	move.w page_stack_ptr,%d0
	subq.w #1,%d0
	move.w %d0,page_stack_ptr
	move.w page_stack_ptr,%d0
	move.w %d0,%d0
	and.l #65535,%d0
	move.w page_stack(%d0.l*2),-2(%fp)
	clr.l %d0
	move.w -2(%fp),%d0
	clr.w page_process_ll(%d0.l*2)
	move.w -2(%fp),%d0
	unlk %fp
	rts
	.size	get_page, .-get_page
	.align	2
	.globl	return_page
	.type	return_page, @function
return_page:
	link.w %fp,#-4
	move.l 8(%fp),%d0
	move.w %d0,-2(%fp)
	move.w page_stack_ptr,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,page_stack_ptr
	move.w %d0,%d0
	and.l #65535,%d0
	move.w -2(%fp),page_stack(%d0.l*2)
	nop
	unlk %fp
	rts
	.size	return_page, .-return_page
	.align	2
	.globl	get_page_link
	.type	get_page_link, @function
get_page_link:
	link.w %fp,#-4
	move.l 8(%fp),%d0
	move.w %d0,-4(%fp)
	move.w page_stack_ptr,%d0
	subq.w #1,%d0
	move.w %d0,page_stack_ptr
	move.w page_stack_ptr,%d0
	move.w %d0,%d0
	and.l #65535,%d0
	move.w page_stack(%d0.l*2),-2(%fp)
	clr.l %d0
	move.w -2(%fp),%d0
	move.w -4(%fp),page_process_ll(%d0.l*2)
	move.w -2(%fp),%d0
	unlk %fp
	rts
	.size	get_page_link, .-get_page_link
	.align	2
	.globl	return_list
	.type	return_list, @function
return_list:
	link.w %fp,#-4
	move.l 8(%fp),%d0
	move.w %d0,-2(%fp)
	jra .L7
.L8:
	move.w page_stack_ptr,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,page_stack_ptr
	move.w %d0,%d0
	and.l #65535,%d0
	move.w -2(%fp),page_stack(%d0.l*2)
	clr.l %d0
	move.w -2(%fp),%d0
	move.w page_process_ll(%d0.l*2),-2(%fp)
.L7:
	tst.w -2(%fp)
	jne .L8
	nop
	unlk %fp
	rts
	.size	return_list, .-return_list
	.align	2
	.globl	page_addr
	.type	page_addr, @function
page_addr:
	link.w %fp,#-4
	move.l 8(%fp),%d0
	move.w %d0,-2(%fp)
	clr.l %d0
	move.w -2(%fp),%d0
	add.l #262144,%d0
	moveq #12,%d1
	lsl.l %d1,%d0
	unlk %fp
	rts
	.size	page_addr, .-page_addr
	.align	2
	.globl	init_memory
	.type	init_memory, @function
init_memory:
	link.w %fp,#-36
	movem.l #15360,-(%sp)
	clr.w page_stack_ptr
	move.l #k_end,%d0
	add.l #-1073741824,%d0
	moveq #12,%d1
	lsr.l %d1,%d0
	move.l %d0,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,-2(%fp)
	jra .L12
.L13:
	move.w page_stack_ptr,%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,page_stack_ptr
	move.w %d0,%d0
	and.l #65535,%d0
	move.w -2(%fp),page_stack(%d0.l*2)
	move.w -2(%fp),%d0
	move.w %d0,%d1
	addq.w #1,%d1
	move.w %d1,-2(%fp)
.L12:
	cmp.w #511,-2(%fp)
	jls .L13
	jsr get_page
	move.w %d0,-6(%fp)
	jsr get_page
	move.w %d0,-8(%fp)
	jsr get_page
	move.w %d0,-10(%fp)
	clr.l %d0
	move.w -6(%fp),%d0
	move.l %d0,-(%sp)
	jsr page_addr
	addq.l #4,%sp
	move.l %d0,-14(%fp)
	clr.l %d0
	move.w -8(%fp),%d0
	move.l %d0,-(%sp)
	jsr page_addr
	addq.l #4,%sp
	move.l %d0,-18(%fp)
	clr.l %d0
	move.w -10(%fp),%d0
	move.l %d0,-(%sp)
	jsr page_addr
	addq.l #4,%sp
	move.l %d0,-22(%fp)
	clr.w -4(%fp)
	jra .L14
.L15:
	move.w -4(%fp),%d0
	ext.l %d0
	lsl.l #2,%d0
	move.l -14(%fp),%a0
	add.l %d0,%a0
	clr.l (%a0)
	move.w -4(%fp),%d0
	ext.l %d0
	lsl.l #2,%d0
	move.l -22(%fp),%a0
	add.l %d0,%a0
	clr.l (%a0)
	move.w -4(%fp),%d0
	addq.w #1,%d0
	move.w %d0,-4(%fp)
.L14:
	cmp.w #1023,-4(%fp)
	jle .L15
	move.l -14(%fp),%a0
	lea (1024,%a0),%a0
	move.l -18(%fp),%d0
	addq.l #2,%d0
	move.l %d0,(%a0)
	move.l -14(%fp),%a0
	lea (2048,%a0),%a0
	move.l -22(%fp),%d0
	addq.l #2,%d0
	move.l %d0,(%a0)
	clr.w -4(%fp)
	jra .L16
.L17:
	move.w -4(%fp),%d0
	ext.l %d0
	lsl.l #2,%d0
	move.l -18(%fp),%a0
	add.l %d0,%a0
	move.w -4(%fp),%a1
	move.l %a1,%d0
	add.l #262144,%d0
	moveq #12,%d1
	lsl.l %d1,%d0
	addq.l #1,%d0
	move.l %d0,(%a0)
	move.w -4(%fp),%d0
	addq.w #1,%d0
	move.w %d0,-4(%fp)
.L16:
	cmp.w #63,-4(%fp)
	jle .L17
	jra .L18
.L19:
	move.w -4(%fp),%d0
	ext.l %d0
	lsl.l #2,%d0
	move.l -18(%fp),%a0
	add.l %d0,%a0
	clr.l (%a0)
	move.w -4(%fp),%d0
	addq.w #1,%d0
	move.w %d0,-4(%fp)
.L18:
	cmp.w #1023,-4(%fp)
	jle .L19
	move.l -22(%fp),%a0
	move.l #-2147483647,(%a0)
	move.l -14(%fp),%d0
	move.l %d0,%d3
	clr.l %d2
	moveq #2,%d4
	or.l %d2,%d4
	clr.l %d5
	or.l %d3,%d5
	move.l %d4,%d0
	move.l %d5,%d1
	move.l %d0,-30(%fp)
	move.l %d1,-26(%fp)
#APP
| 84 "memory.c" 1
	pmove -30(%fp), %srp
| 0 "" 2
#NO_APP
	move.l #-2101302784,-34(%fp)
#APP
| 90 "memory.c" 1
	pmove -34(%fp), %tcr
| 0 "" 2
#NO_APP
	move.w page_stack_ptr,%d0
	movem.l -52(%fp),#60
	unlk %fp
	rts
	.size	init_memory, .-init_memory
	.ident	"GCC: (GNU) 5.3.0"
