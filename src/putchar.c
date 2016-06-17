#include "stdio.h"
#include "serial.h"
 

 
int putchar(int ic)
{
	
	char c = (char) ic;
	serial_write_c(c);

	return ic;
}