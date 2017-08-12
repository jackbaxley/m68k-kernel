#include "serial.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include "process.h"

extern int k_end;
extern int s_stack_top;
extern int i_stack_top;

extern int prog1_start;



void kmain(){
	serial_init();
	serial_clear();
	init_processes();
	printf("%d pages free\n",init_memory() );
	printf("Kernel Started\n");
	printf("k_end %X\n",&k_end);
	printf("s_stack_end %X\n",&s_stack_top);
	printf("i_stack_end %X\n",&i_stack_top);
	printf("int test %d!\n",sizeof(uint64_t));
	
	load_small_process(&prog1_start);
	printf("Program Loaded\n");
	run_process();//NOT A FUNCTION. WILL NEVER RETURN
}

int syscall(int d0, int d1, int d2, int d3){
	if(d0==0){
		printf("%c",d1);
	}
	/*
	printf("Syscall!\n");
	printf("%%D0: %d\n",d0);
	printf("%%D1: %d\n",d1);
	printf("%%D2: %d\n",d2);
	printf("%%D3: %d\n",d3);
	*/
}