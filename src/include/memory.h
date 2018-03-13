#ifndef __memory_h
#define __memory_h

#include "mydefs.h"
#include "cdefs.h"


#define FRAME_SIZE 4096
#define FRAME_NUM 512 //fix for MORERAM
#define MEMSIZE FRAME_SIZE*PAGE_NUM
#define MEMSTART 0x40000000
#define IOSTART 0x80000000

#define MALLOC_MAX FRAME_SIZE-1
#define MALLOC_MIN 32-1





uint16_t init_memory();//fix for MORERAM
uint16_t get_frame();
void return_frame(uint16_t ptr);
uint16_t get_frame_link(uint16_t p);
void return_list(uint16_t p);
void* frame_addr(uint16_t p);

void* malloc(uint32_t size);
void free(void *ptr);
void malloc_test();

#endif