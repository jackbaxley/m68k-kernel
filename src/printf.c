#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
#include "string.h"


char* hextbl="0123456789ABCDEF";
 
static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}
 
int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;
 
	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if ( *(++format) == '%' )
			goto print_c;
 
		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
 
		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'X' )
		{
			format++;
			int i = (int) va_arg(parameters, int /* char promotes to int */);
			bool w=false;
			if((i>>28)&0x0F || w){print(&hextbl[(i>>28)&0x0F],sizeof(char));w=true;}
			if((i>>24)&0x0F || w){print(&hextbl[(i>>24)&0x0F],sizeof(char));w=true;}
			if((i>>20)&0x0F || w){print(&hextbl[(i>>20)&0x0F],sizeof(char));w=true;}
			if((i>>16)&0x0F || w){print(&hextbl[(i>>16)&0x0F],sizeof(char));w=true;}
			if((i>>12)&0x0F || w){print(&hextbl[(i>>12)&0x0F],sizeof(char));w=true;}
			if((i>> 8)&0x0F || w){print(&hextbl[(i>> 8)&0x0F],sizeof(char));w=true;}
			if((i>> 4)&0x0F || w){print(&hextbl[(i>> 4)&0x0F],sizeof(char));w=true;}
								  print(&hextbl[(i>> 0)&0x0F],sizeof(char));
		}
		else if ( *format == 'd' )
		{
			format++;
			int i = (int) va_arg(parameters, int /* char promotes to int */);
			if(i<0){print("-",sizeof(char));i=-i;}

			int total=0;
			int t;
			if(i>=1000000000){
				t=(i-total)/1000000000;
				print(&hextbl[t],sizeof(char));
				total+=t*1000000000;
			}
			if(i>=100000000){
				t=(i-total)/100000000;
				print(&hextbl[t],sizeof(char));
				total+=t*100000000;
			}
			if(i>=10000000){
				t=(i-total)/10000000;
				print(&hextbl[t],sizeof(char));
				total+=t*10000000;
			}
			if(i>=1000000){
				t=(i-total)/1000000;
				print(&hextbl[t],sizeof(char));
				total+=t*1000000;
			}
			if(i>=100000){
				t=(i-total)/100000;
				print(&hextbl[t],sizeof(char));
				total+=t*100000;
			}
			if(i>=10000){
				t=(i-total)/10000;
				print(&hextbl[t],sizeof(char));
				total+=t*10000;
			}
			if(i>=1000){
				t=(i-total)/1000;
				print(&hextbl[t],sizeof(char));
				total+=t*1000;
			}
			if(i>=100){
				t=(i-total)/100;
				print(&hextbl[t],sizeof(char));
				total+=t*100;
			}
			if(i>=10){
				t=(i-total)/10;
				print(&hextbl[t],sizeof(char));
				total+=t*10;
			}
			if(i>=0){
				t=(i-total)/1;
				print(&hextbl[t],sizeof(char));
				total+=t*1;
			}
			
			
			
			
		}
		else
		{
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
 
	return written;
}