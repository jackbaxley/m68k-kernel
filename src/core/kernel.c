#include "serial.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include "process.h"
#include "buildtime.h"
#include "fs.h"
#include "ext2.h"
#include "fileserver.h"

extern char k_end;
extern char s_stack_top;
extern char i_stack_top;

extern char prog1_start;





void init_fileserver(){// hardware dependent
	volatile serial_interface* serial_interface_B = serial_get_interface('B');
	if(serial_interface_B!=0){
		printf("Got serial interface id: %c\n",serial_interface_B->id);
	}else{
		printf("Could not get serial interface B\n");
	}
	static fs_node_t serial_node, fileserver_node;
	serial_create_node(serial_interface_B,&serial_node);
	fileserver_create_node(&serial_node,&fileserver_node);
	static ext2_system_t filesystem;

	ext2_make_system(&filesystem,&fileserver_node);
	ext2_read_superblock(&filesystem);
	
	
}

void kmain(){
	serial_init();
	volatile serial_interface* serial_interface_A = serial_get_interface('A');
	serial_clear(serial_interface_A);
	set_std_si(serial_interface_A);
	if(serial_interface_A!=0){
		printf("Got serial interface id: %c\n",serial_interface_A->id);
	}else{
		printf("Could not get serial interface A\n");
	}
	
	init_fileserver();
	
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