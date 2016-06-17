	
	.text
	.globl prog1_start
prog1_start:
	clr.l %d0
	clr.l %d1
	move.b #'H' , %d1
	trap #0
	move.b #'e' , %d1
	trap #0
	move.b #'l' , %d1
	trap #0
	move.b #'l' , %d1
	trap #0
	move.b #'o' , %d1
	trap #0
	move.b #' ' , %d1
	trap #0
	move.b #'W' , %d1
	trap #0
	move.b #'o' , %d1
	trap #0
	move.b #'r' , %d1
	trap #0
	move.b #'l' , %d1
	trap #0
	move.b #'d' , %d1
	trap #0
	move.b #'!' , %d1
	trap #0
	move.b #'\n' , %d1
	trap #0
l:
	bra l
	