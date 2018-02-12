#ifndef __serial_h
#define __serial_h

#ifdef NO_IRQ

#define TX_BUFFER_SIZE 0
#define RX_BUFFER_SIZE 0

#else

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

#endif

typedef struct{
	
	
} serial_interface;

void serial_clear();
char serial_get_c();
char serial_check_c();
void serial_write_c(char c);
void serial_write_s(char *s);
void serial_init();
int get_time();


#endif