#ifndef __process_h
#define __process_h

#include "mydefs.h"
#include "memory.h"

void run_process();//not real function, will never return
void init_processes();
uint16_t load_small_process(uint32_t* loadaddr);

#endif