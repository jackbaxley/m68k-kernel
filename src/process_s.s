	.text
	.globl run_process
	
	
	
	
	
run_process:
	/* get process header frame */
	/* needs to be updates when struct size changes*/
	clr.l %d0
	move.w cur_process,%d0
	move.l %d0,%d2
	lsl.l #2,%d2
	move.l %d0,%d1
	lsl.l #5,%d1
	move.l %d2,%a0
	add.l %d1,%a0
	add.l %a0,%d0
	add.l %d0,%d0
	move.l %d0,%a0
	add.l #proceses,%a0
	
	/*make exception frame*/
	move.w #0, -(%sp) 
	
	move.l 2(%a0), -(%sp) /*program counter */
	move.w 6(%a0), -(%sp) /*status register*/
	
	/*get page directory address*/
	clr.l %d0
	move.w (%a0), %d0
	move.l %d0, -(%sp)
	jsr page_addr
	addq.l #4, %sp
	
	/*push root pointer contents*/
	move.l %d0, -(%sp)
	move.l #0x80000002, -(%sp)
	
	/*restore registers*/
	move.l 68(%a0), %a1
	move.l %a1, %usp
	/*
	move.l 36(%a0), %d7
	move.l 32(%a0), %d6
	move.l 28(%a0), %d5
	move.l 24(%a0), %d4
	move.l 20(%a0), %d3
	move.l 16(%a0), %d2
	move.l 12(%a0), %d1
	move.l 8(%a0), %d0
	move.l 64(%a0), %a6
	move.l 60(%a0), %a5
	move.l 56(%a0), %a4
	move.l 52(%a0), %a3
	move.l 48(%a0), %a2
	move.l 44(%a0), %a1
	move.l 40(%a0), %a0
	*/
	
	movem.l 8(%a0), %d0-%d7/%a0-%a6
	
	
	/* load root pointer*/
	pmove (%sp), %crp
	addq.l #8 ,%sp
		
	rte
	