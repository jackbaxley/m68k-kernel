#include "memory.h"

//fix for MORERAM , replace 16 bits with 32 for more than 64k frames

extern int k_end;

fid_t frame_stack[FRAME_NUM]; //fix for MORERAM
uint16_t frame_stack_ptr;//point above top frame //fix for MORERAM

fid_t frame_pess_ll[FRAME_NUM];// linked list of frames used by process; //fix for MORERAM



fid_t get_frame(){//fix for MORERAM
	fid_t t=frame_stack[--frame_stack_ptr];
	frame_pess_ll[t]=0;
	return t;
}

//NOTE: doesnt remove from linked list
void return_frame(fid_t p){//fix for MORERAM
	
	frame_stack[frame_stack_ptr++]=p;
}

fid_t get_frame_link(pid_t p){//fix for MORERAM
	fid_t t=frame_stack[--frame_stack_ptr];
	frame_pess_ll[t]=p;
	return t;
}

void return_list(uint16_t p){
	while(p){
		frame_stack[frame_stack_ptr++]=p;
		p=frame_pess_ll[p];
	}
}

void* frame_addr(uint16_t p){
	return (void*)(MEMSTART+FRAME_SIZE*p);
}

uint16_t init_memory(){
	frame_stack_ptr=0;

	
	//fill with availible frames
	for(uint16_t i=((((uint32_t)&k_end)-MEMSTART)/FRAME_SIZE)+1;i<FRAME_NUM;i++){
		frame_stack[frame_stack_ptr++]=i;

	}
	
	//make supervisor frame table/directories
	uint16_t super_pd_id = get_frame();
	uint16_t super_pt_ram_id = get_frame();
	uint16_t super_pt_io_id = get_frame();
	
	uint32_t* super_pd = (uint32_t*)frame_addr(super_pd_id);
	uint32_t* super_pt_ram = (uint32_t*)frame_addr(super_pt_ram_id);
	uint32_t* super_pt_io = (uint32_t*)frame_addr(super_pt_io_id);
	
	short i;
	
	for(i=0; i<FRAME_SIZE/4 ;i++){
		super_pd[i]=0;
		super_pt_io[i]=0;
	}
	
	super_pd[0x100]=(uint32_t)(super_pt_ram) + 2;//4 byte entry
	super_pd[0x200]=(uint32_t)(super_pt_io)  + 2;//4 byte entry
	
	for(i=0; i<FRAME_NUM; i++){//make connections for valid frames
		super_pt_ram[i]=MEMSTART+FRAME_SIZE*i + 1;//1 is the enable
	}
	for(;i<FRAME_SIZE/4;i++){//fill in rest of frame table
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
	
	
	
	return frame_stack_ptr;
}