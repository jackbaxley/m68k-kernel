#include "serial.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include "process.h"
#include "buildtime.h"

extern char k_end;
extern char s_stack_top;
extern char i_stack_top;

extern char prog1_start;



void kmain(){
	serial_init();
	serial_interface* serial_interface_A = serial_get_interface('A');
	//serial_interface* serial_interface_B = serial_get_interface('B');
	serial_clear(serial_interface_A);
	set_std_si(serial_interface_A);
	
	char c;
	if(serial_interface_A!=0){
		
		c=(*serial_interface_A).id;


		printf("Got serial interface id: %c\n",c);
	}else{
		printf("Could not get serial interface\n");
	}
	
	
	#ifdef NO_IRQ
		printf("Serial IRQ off.\n");
	#else
		printf("Serial IRQ on.\n");
	#endif
	init_processes();
	printf("%d frames free\n",init_memory() );

	printf("Kernel Started\n");
	printf("Built: %s\n",get_buildtime());
	printf("k_end %X\n",(uint32_t)&k_end);
	printf("s_stack_end %X\n",(uint32_t)&s_stack_top);
	printf("i_stack_end %X\n",(uint32_t)&i_stack_top);
	
	load_small_process((void*)&prog1_start);
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
	return 0;
}