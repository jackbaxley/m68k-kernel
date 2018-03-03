#ifndef _STDLIB_H
#define _STDLIB_H 1
 
#include "cdefs.h"
#include "mydefs.h"
 
__attribute__((__noreturn__))
void abort(void);

uint32_t readlendian32(uint8_t* in);
uint16_t readlendian16(uint8_t* in);
void writelendian32(uint32_t in, uint8_t* out);
void writelendian16(uint16_t in, uint8_t* out);
 
#endif