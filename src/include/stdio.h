#ifndef _STDIO_H
#define _STDIO_H 1
 
#include "cdefs.h"
#include "serial.h"
 
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
void set_std_si(serial_interface* si);

 
#endif