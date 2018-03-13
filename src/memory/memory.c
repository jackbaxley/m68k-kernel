#include "memory.h"
#include "stdio.h"
#include "error.h"

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

//gets the id of a frame from its pointer
fid_t frame_id(void* p){
	return ((uint32_t)p-MEMSTART)/FRAME_SIZE;
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
	malloc_n_frames=0;
}

// regenerates bit-mask of free sizes
void malloc_regen_free_size(fid_t frame){
	uint8_t* f_ptr=frame_addr(malloc_frames[frame]);
	
	uint8_t mask=0;
	//local pointer
	uint16_t l_ptr=0;
	int search=1;
	
	while(search){
		uint8_t cur_chunk_size=((f_ptr[l_ptr])&0x07);
		
		//printf("offset %d\n",l_ptr);
		//printf("cur_chunk_size=%d, indicating size of %d\n",cur_chunk_size,(32<<cur_chunk_size));
		if( !( (f_ptr[l_ptr])&0x80 ) ){//if chunk is free
			mask|=(1<<cur_chunk_size);
		}
		
		//increment local pointer to next chunk
		l_ptr+=(32<<cur_chunk_size);
		
		if(l_ptr>4096){
			//error this shouldnt happen
			PR_ERROR
			return;
		}
		if(l_ptr==4096){
			search=0;
			break;
		}
	}
	
	malloc_frames_free_size[frame]=mask;
	//printf("Free Sizes Mask: %X\n",mask);
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
	
	//searches for a "match frame", a frame that has space for
	//the new allocation
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
		//printf("No Match, allocated new frame\n");
		match_frame=malloc_n_frames++;
		match_log_size=7;
		malloc_frames[match_frame]=get_frame();
		malloc_frames_free_size[match_frame]=(1<<7);
		f_ptr=frame_addr(malloc_frames[match_frame]);
		*f_ptr=0x07;//unallocated full page chunk
		
	}else{
		f_ptr=frame_addr(malloc_frames[match_frame]);
	}

	//local pointer
	uint16_t l_ptr=0;
	
	int search=1;
	
	while(search){
		uint8_t cur_chunk_size=((f_ptr[l_ptr])&0x07);
		
		//printf("search chunk size %d, match size %d, cur meta byte %X\n",cur_chunk_size,match_log_size,f_ptr[l_ptr]);
		//if the current chunk is the desired chunk
		if( !((f_ptr[l_ptr])&0x80) && cur_chunk_size==match_log_size){
			search=0;
			break;
		}
		//increment local pointer to next chunk
		l_ptr+=(32<<cur_chunk_size);
		
		if(l_ptr>=4096){
			//error this shouldnt happen
			PR_ERROR
			return NULL;
		}		
	}
	
	//printf("log_size%d\n",log_size);
	
	//now that we have the match chunk, if the match chunk is too big
	//we have to divide the chunk to the correct size
	
	for(uint8_t i=match_log_size;i>log_size;i--){
		
		//split the chunk in half by creating the higher chunk
		f_ptr[l_ptr+(32<<(i-1))]=i-1; 
			
	}
	//0x80 marks that this chunk is not free
	f_ptr[l_ptr]=0x80|(uint8_t)log_size;
	
	
	malloc_regen_free_size(match_frame);
	return &(f_ptr[l_ptr+1]);
	
}

void free(void* ptr){
	if(ptr==NULL)
		return;
	
	uint8_t *f_ptr = (uint8_t*)((uint32_t)ptr-((uint32_t)ptr%4096));
	uint16_t l_ptr= (uint8_t*)ptr-f_ptr-1;
	if(l_ptr&31){
		PR_ERROR
		//error if the pointer isnt alligned
		return;
	}
	
	//the frame_id
	fid_t f_id=frame_id(f_ptr);
	
	int match_frame;
	int match=0;
	for(int i=0;i<malloc_n_frames;i++){
		if(malloc_frames[i]==f_id){
			match=1;
			match_frame=i;
			break;
		}
	}
	if(!match){
		PR_ERROR
		return;
	}
	
	uint8_t log_size=f_ptr[l_ptr]&0x07;
	
	int ret_frame=0;
	
	if(log_size==7){
		ret_frame=1;
	}else{
		//recombine pairs
		uint8_t pl_ptr; // pair to the local pointer
		for(int i=log_size;i<7;i++){
			//flip a bit to get the pair chunk pointer
			pl_ptr=l_ptr^(32<<i);
			if(!(f_ptr[pl_ptr]&0x80)){// if the pair chunk is free
				if(i==6){// if the combining two 2048 chunks, give the whole frame back
					ret_frame=1;
					break;
				}
				l_ptr=l_ptr&pl_ptr;//make l_ptr the smaller of the two;
				f_ptr[l_ptr]=i+1;//the new bigger chunk
				//printf("recombining two %d into a %d\n",i,i+1);
			}else{
				break;
			}
		}
	}
	if(ret_frame){
		//printf("Returning a frame\n");
		//shift the array down
		for(int i=match_frame;i<malloc_n_frames-1;i++){
			malloc_frames[i]=malloc_frames[i+1];
		}
		malloc_n_frames--;
		return_frame(f_id);
		
	}else{
		malloc_regen_free_size(match_frame);
	}
}



void malloc_test(){
	printf("Starting Malloc Test.\n");
	void *ptr1,*ptr2,*ptr3,*ptr4;
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	
	printf("allocating %d bytes\n",4000);
	ptr1=malloc(4000);
	printf("retuned ptr: %X\n",(uint32_t)ptr1);
	printf("malloc_n_frames: %d\n",malloc_n_frames);

	printf("allocating %d bytes\n",1000);
	ptr2=malloc(1000);
	printf("retuned ptr: %X\n",(uint32_t)ptr2);
	printf("malloc_n_frames: %d\n",malloc_n_frames);

	printf("allocating %d bytes\n",32);
	ptr3=malloc(32);
	printf("retuned ptr: %X\n",(uint32_t)ptr3);
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	
	printf("allocating %d bytes\n",3000);
	ptr4=malloc(3000);
	printf("retuned ptr: %X\n",(uint32_t)ptr4);
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	
	printf("Freeing pt2\n");
	free(ptr2);
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	printf("Freeing pt3\n");
	free(ptr3);
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	printf("Freeing pt1\n");
	free(ptr1);
	printf("malloc_n_frames: %d\n",malloc_n_frames);
	
	
	
}
