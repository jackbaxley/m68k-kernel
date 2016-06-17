	.text
	
	.globl serial_irq
serial_irq:
	/*move.b 0x40 , 0x80000003*/
	
	movem.l %d0-%d7/%a0-%a6, -(%sp)
	jsr serial_interrupt
	movem.l (%sp)+, %d0-%d7/%a0-%a6
	
	rte
	