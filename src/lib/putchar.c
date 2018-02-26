#include "stdio.h"
#include "serial.h"
 
serial_interface* std_si;
 
int putchar(int ic)
{
	
	char c = (char) ic;
	serial_write_c(std_si,c);

	return ic;
}

void set_std_si(serial_interface* si){ // set the serial interface for putchar to use

	std_si=si;
	
}