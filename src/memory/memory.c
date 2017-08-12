#include "memory.h"

//fix for MORERAM , replace 16 bits with 32 for more than 64k pages

extern int k_end;

uint16_t page_stack[PAGE_NUM]; //fix for MORERAM
uint16_t page_stack_ptr;//point above top page //fix for MORERAM

uint16_t page_process_ll[PAGE_NUM];// linked list of pages used by process; //fix for MORERAM



uint16_t get_page(){//fix for MORERAM
	uint16_t t=page_stack[--page_stack_ptr];
	page_process_ll[t]=0;
	return t;
}

//NOTE: doesnt remove from linked list
void return_page(uint16_t p){//fix for MORERAM
	
	page_stack[page_stack_ptr++]=p;
}

uint16_t get_page_link(uint16_t p){//fix for MORERAM
	uint16_t t=page_stack[--page_stack_ptr];
	page_process_ll[t]=p;
	return t;
}

void return_list(uint16_t p){
	while(p){
		page_stack[page_stack_ptr++]=p;
		p=page_process_ll[p];
	}
}

void* page_addr(uint16_t p){
	return (void*)(MEMSTART+PAGE_SIZE*p);
}

uint16_t init_memory(){
	page_stack_ptr=0;

	
	//fill with availible pages
	for(uint16_t i=((((uint32_t)&k_end)-MEMSTART)/PAGE_SIZE)+1;i<PAGE_NUM;i++){
		page_stack[page_stack_ptr++]=i;

	}
	
	//make supervisor page table/directories
	uint16_t super_pd_id = get_page();
	uint16_t super_pt_ram_id = get_page();
	uint16_t super_pt_io_id = get_page();
	
	uint32_t* super_pd = (uint32_t*)page_addr(super_pd_id);
	uint32_t* super_pt_ram = (uint32_t*)page_addr(super_pt_ram_id);
	uint32_t* super_pt_io = (uint32_t*)page_addr(super_pt_io_id);
	
	short i;
	
	for(i=0; i<PAGE_SIZE/4 ;i++){
		super_pd[i]=0;
		super_pt_io[i]=0;
	}
	
	super_pd[0x100]=(uint32_t)(super_pt_ram) + 2;//4 byte entry
	super_pd[0x200]=(uint32_t)(super_pt_io)  + 2;//4 byte entry
	
	for(i=0; i<PAGE_NUM; i++){//make connections for valid pages
		super_pt_ram[i]=MEMSTART+PAGE_SIZE*i + 1;//1 is the enable
	}
	for(;i<PAGE_SIZE/4;i++){//fill in rest of page table
		super_pt_ram[i]=0;
	}
	
	super_pt_io[0]=IOSTART + 1;//1 is enable
	
	//init mmu
	uint64_t srp=0x8000000200000000 | (uint32_t)super_pd;
	
	asm volatile (	"pmove %0, %%srp"
			:
			:"m"(srp)
			);
	uint32_t tcr = 0x82C0AA00;
	
	asm volatile (	"pmove %0, %%tcr"
			:
			:"m"(tcr)
			);
	
	
	
	return page_stack_ptr;
}