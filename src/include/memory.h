#ifndef __memory_h
#define __memory_h

#define PAGE_SIZE 4096
#define PAGE_NUM 512 //fix for MORERAM
#define MEMSIZE PAGE_SIZE*PAGE_NUM
#define MEMSTART 0x40000000
#define IOSTART 0x80000000

#include "mydefs.h"


uint16_t init_memory();//fix for MORERAM
uint16_t get_page();
void return_page(uint16_t ptr);
uint16_t get_page_link(uint16_t p);
void return_list(uint16_t p);
void* page_addr(uint16_t p);

#endif