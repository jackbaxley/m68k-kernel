#include "process.h"
#include "stdio.h"

#define NUM_PROCESSES 1

typedef struct {
	uint16_t pd_id; //page directory //fix for MORERAM
	uint32_t reg_pc;// program counter
	uint16_t reg_cr; //status register?
	uint32_t reg_d[8];
	uint32_t reg_a[7];
	uint32_t reg_sp;
	uint16_t page_ll;// linked list of pages used by this process //fix for MORERAM
	
} process;


process proceses[NUM_PROCESSES];

uint16_t pid_stack[NUM_PROCESSES];
uint16_t pid_stackptr;
uint16_t pid_queue[NUM_PROCESSES];

uint16_t cur_process; //pid of current process

void init_processes(){
	cur_process=0;
}

uint16_t get_free_pid(){
	return 0;
}

uint16_t load_small_process(uint32_t* loadaddr){// starts new process
	uint16_t pid=get_free_pid();
	uint16_t pd_id=get_page();
	uint16_t pt_id=get_page();
	uint16_t prog_id=get_page();
	uint16_t stack_id=get_page();
	printf("pd: %d\n",pd_id);
	uint32_t* pd=(uint32_t*)page_addr(pd_id);
	uint32_t* pt=(uint32_t*)page_addr(pt_id);
	uint32_t* prog=(uint32_t*)page_addr(prog_id);
	uint32_t* stack=(uint32_t*)page_addr(stack_id);
	printf("pd: 0x%X\n",pd);
	int i;
	for(i=0;i<1024;i++){
		pd[i]=0;
		pt[i]=0;
	}
	printf("3\n");
	
	pd[0]=(uint32_t)(pt)+2;
	pt[0]=(uint32_t)(prog)+1;
	pt[1]=(uint32_t)(stack)+1;
	
	proceses[pid].pd_id=pd_id;
	proceses[pid].reg_pc=0;
	proceses[pid].reg_cr=0;
	proceses[pid].reg_sp=0x2000;
	
	for(i=0;i<64;i++){
		prog[i]=loadaddr[i];
	}
	
	return pid;
	
}

void kill_pid(uint16_t pid){
	
	
}

//port to assembly with movem instruction, would be better
void save_process(uint32_t pc, uint32_t sr, uint32_t sp, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3){
	proceses[cur_process].reg_pc=pc;
	proceses[cur_process].reg_cr=sr;
	proceses[cur_process].reg_sp=sp;
	proceses[cur_process].reg_a[6]=a6;
	proceses[cur_process].reg_a[5]=a5;
	proceses[cur_process].reg_a[4]=a4;
	proceses[cur_process].reg_a[3]=a3;
	proceses[cur_process].reg_a[2]=a2;
	proceses[cur_process].reg_a[1]=a1;
	proceses[cur_process].reg_a[0]=a0;
	proceses[cur_process].reg_d[7]=d7;
	proceses[cur_process].reg_d[6]=d6;
	proceses[cur_process].reg_d[5]=d5;
	proceses[cur_process].reg_d[4]=d4;
	proceses[cur_process].reg_d[3]=d3;
	proceses[cur_process].reg_d[2]=d2;
	proceses[cur_process].reg_d[1]=d1;
	proceses[cur_process].reg_d[0]=d0;
	
}