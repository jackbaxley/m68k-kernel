#ifndef __process_h
#define __process_h

#include "mydefs.h"
#include "memory.h"

typedef struct {
	uint16_t pd_id; //page directory //fix for MORERAM
	uint32_t reg_pc;// program counter
	uint16_t reg_cr; //status register?
	uint32_t reg_d[8];
	uint32_t reg_a[7];
	uint32_t reg_sp;
	uint16_t page_ll;// linked list of frames used by this process //fix for MORERAM
	
} process;



void run_process();//not real function, will never return
void init_processes();
uint16_t load_small_process(void* loadaddr);

#endif