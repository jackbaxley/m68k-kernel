#include "memory.h"

//fix for MORERAM , replace 16 bits with 32 for more than 64k frames

extern int k_end;

fid_t frame_stack[FRAME_NUM]; //fix for MORERAM
uint16_t frame_stack_ptr;//point above top frame //fix for MORERAM

fid_t frame_pess_ll[FRAME_NUM];// linked list of frames used by process; //fix for MORERAM

fid_t malloc_frames[FRAME_NUM]; //list of frames used by malloc
uint8_t malloc_frames_free_size[FRAME_NUM]; //bitmask of free_sizes availibale in each frame
fid_t malloc_n_frames;// number of malloc frames


void init_malloc();

//gets a free frame from the stack
fid_t get_frame(){//fix for MORERAM
	fid_t t=frame_stack[--frame_stack_ptr];
	frame_pess_ll[t]=0;
	return t;
}

//Returns a frame to the free-frame stack
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

//gets the address of a frame from its frame_id
void* frame_addr(uint16_t p){
	return (void*)(MEMSTART+FRAME_SIZE*p);
}

uint16_t init_memory(){
	init_malloc();
	
	
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

uint8_t malloc_log_size(uint32_t size){
	if(size>=2048)return 7;
	if(size>=1024)return 6;
	if(size>= 512)return 5;
	if(size>= 256)return 4;
	if(size>= 128)return 3;
	if(size>=  64)return 2;
	if(size>=  32)return 1;
	return 0;
}



void init_malloc(){
	for(int i=0;i<FRAME_NUM;i++){
		malloc_frames[i]=0;
		malloc_frames_free_size[i]=0;
	}
}

// regenerates bit-mask of free sizes
void malloc_regen_free_size(fid_t frame){
	uint8_t* f_ptr=frame_addr(frame);
	
	uint8_t mask=0;
	//local pointer
	uint16_t l_ptr=0;
	int search=1;
	
	while(search){
		uint8_t cur_chunk_size=((f_ptr[l_ptr])&0x07);
		
		mask|=(1<<cur_chunk_size);
		
		//increment local pointer to next chunk
		l_ptr+=(32<<cur_chunk_size);
		
		if(l_ptr>=4096){
			//error this shouldnt happen
			return;
		}		
	}
	
	malloc_frames_free_size[frame]=mask;	
}

/*
for all mallocs there is 1 byte overhead in the byte
preceding the pointer returned by malloc. the msb
represents if the chunk is allocated or not, and the
lower 3 bits indicate the chunk size, with 7=full frame,
6=half-frame, etc.

*/
void* malloc(uint32_t size){
	if(size==0)
		return NULL;
	if(size>MALLOC_MAX)
		return NULL;
	uint8_t log_size=malloc_log_size(size);
	int match_frame;
	int match_log_size;
	int match_found=0;
	for(int j=log_size;j<8;j++){
		for(int i=0;i<malloc_n_frames;i++){
		
			if(malloc_frames_free_size[i]&(1<<j)){
				match_found=1;
				match_frame=i;
				match_log_size=j;
				break;
			}
		}
		if(match_found)
			break;
	}
	
	
	//frame pointer
	uint8_t* f_ptr;
	
	
	//if theres no existing page that this allocation will fit in,
	//allocate a new page
	if(!match_found){
		match_frame=malloc_n_frames++;
		match_log_size=7;
		malloc_frames[match_frame]=get_frame();
		malloc_frames_free_size[match_frame]=(1<<7);
		f_ptr=frame_addr(match_frame);
		*f_ptr=0x07;//unallocated full page chunk
		
	}else{
		f_ptr=frame_addr(match_frame);
	}

	//local pointer
	uint16_t l_ptr=0;
	
	int search=1;
	
	while(search){
		uint8_t cur_chunk_size=((f_ptr[l_ptr])&0x07);
		//if the current chunk is the desired chunk
		if( !((f_ptr[l_ptr])&0x80) && cur_chunk_size==match_log_size){
			search=0;
			break;
		}
		//increment local pointer to next chunk
		l_ptr+=(32<<cur_chunk_size);
		
		if(l_ptr>=4096){
			//error this shouldnt happen
			return NULL;
		}		
	}
	
	//now that we have the match chunk, if the match chunk is too big
	//we have to divide the chunk to the correct size
	
	for(uint8_t i=match_log_size;i>log_size;i--){
		
		//split the chunk in half by creating the higher chunk
		f_ptr[l_ptr+(32<<(i-1))]=i; 
			
	}
	f_ptr[l_ptr]=(uint8_t)log_size;
	
	
	malloc_regen_free_size(match_frame);
	return &(f_ptr[l_ptr+1]);
	
}

void free(void* ptr){
	if(ptr==NULL)
		return;
}



void malloc_test(){
	
}
