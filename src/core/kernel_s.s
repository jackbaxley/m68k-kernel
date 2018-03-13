	.globl s_stack_top
	.globl i_stack_top
	
	.section .stack, "aw", @nobits
	s_stack_bottom:
	.skip 8192
	s_stack_top:
	i_stack_bottom:
	.skip 4096
	i_stack_top:
	
	.section .text.init
_start:
	/* master mode*/
	or.w #0x1000, %sr
	
	/*flush cache*/
	move.l #0x00000808 , %d0
	movec.l %d0 , %cacr
	
	/*enable cache*/
	#move.l #0x00000101 , %d0
	#movec.l %d0 , %cacr
	
	/*initilize ssp*/
	move.l #s_stack_top, %sp
	
	/*initilize isp*/
	move.l #i_stack_top, %d0
	movec.l %d0, %isp
	
	/*initilize vbr*/
	move.l #vector_start, %d0
	movec.l %d0, %vbr
	
	/*disable irq on duart*/
	move.b #0x00, 0x80000005
	
	/*enable interrupt*/
	and.w #0xF8FF, %sr
		
	jsr kmain
	
	
end:
	bra end
	
	.text
	.globl syscall_hdl
syscall_hdl:
	or.w #0x1000, %sr /* master mode*/
	
	movem.l %d0-%d3, -(%sp) /*push 4 registers*/
	movem.l %d4-%d7/%a0-%a6, -(%sp) /*push 11 registers*/
	
	move.l %usp, %a0 /*load usp*/
	move.l %a0, -(%sp) /*push usp*/
	
	
	
	movec %isp, %a0
	clr %d0
	move.w (%a0)+, %d0 /*pop user status register*/
	move.l %d0, -(%sp) /*push user sr*/
	move.l (%a0)+, -(%sp) /* push pc*/
	addq.l #2, %a0 /* remove exception stack frame */
	movec %a0, %isp
	
	jsr save_process /* save registers to process frame structure*/
	
	add.l #56, %sp /*remove resgisters from stack (except d0-d4)*/
	
	
	
	jsr syscall /* do the syscall*/
	
	add.l #16, %sp /* take registers off stack*/
	
	jmp run_process /* go back to running process*/
	
	.section .rodata
bus_err_str:
	.string "Bus Error!\nStatus Register: %X\nProgram Counter: %X\n"
	
	.text
	.globl bus_err
bus_err:

	move.w #0, -(%sp) /* pad status register */
	
	move.l #bus_err_str, -(%sp)
	jsr printf
	addq.l #4, %sp	
be_loop:
	bra be_loop
	
	
	